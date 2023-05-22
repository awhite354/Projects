#include <err.h>
#include <errno.h>
#include <fcntl.h>
#include <stdint.h>
#include <inttypes.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>
#include <assert.h>
#include <stdbool.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <pthread.h>
#include <sys/file.h>
#include "my_queue.h"

#define OPTIONS              "t:l:"
#define BUF_SIZE             4096
#define DEFAULT_THREAD_COUNT 4
#define MAXBUFFLEN           4096 //used to be 4096
#define HEADERLEN            2048 //used to be 2048

#define statusGETreturn "HTTP/1.1 200 OK\r\nContent-Length: "
#define status200       "200 OK\r\nContent_Length: 3\r\n\r\nOK\n"
#define status201       "201 Created\r\nContent_Length: 8\r\n\r\nCreated\n"
#define status400       "400 Bad Request\r\nContent-Length: 12\r\n\r\nBad Request\n"
#define status403       "403 Forbidden\r\nContent-Length: 10\r\n\r\nForbidden\n"
#define status404       "404 Not Found\r\nContent_Length: 10\r\n\r\nNot Found\n"
#define status500       "500 Internal Server Error\r\nContent_Length: 22\r\n\r\nInternal Server Error\n"
#define status501       "501 Not Implemented\r\nContent_Length: 16\r\n\r\nNot Implemented\n"

#define RE_METHOD      "[a-zA-Z]{0,7}"
#define RE_URI         "[/][a-zA-Z0-9._/]{0,18}"
#define RE_HTTPVERSION "HTTP/[1-9.]{0,8}"
#define RE_CONTENTLEN  ("Content-Length: [0-9]+")
#define RE_ID          ("Request-Id: [0-9]+")
#define RE_REQUEST                                                                                 \
    ("^"                                                                                           \
     "(" RE_METHOD ")"                                                                             \
     " "                                                                                           \
     "(" RE_URI ")"                                                                                \
     " "                                                                                           \
     "(" RE_HTTPVERSION ")"                                                                        \
     "\r\n")

void pack_response(int, int);

pthread_mutex_t queueMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t log_resp_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t queueFullCond = PTHREAD_COND_INITIALIZER;
pthread_cond_t queueEmptyCond = PTHREAD_COND_INITIALIZER;

struct header_field {
    char method[8];
    char uri[19];
    char httpVersion[9];
    int contentLength;
    int requestID;
};

struct Queue *q = NULL;

void create_log(struct header_field *header, int);

static FILE *logfile;
#define LOG(...) fprintf(logfile, __VA_ARGS__);
// Converts a string to an 16 bits unsigned integer.
// Returns 0 if the string is malformed or out of the range.
static size_t strtouint16(char number[]) {
    char *last;
    long num = strtol(number, &last, 10);
    if (num <= 0 || num > UINT16_MAX || *last != '\0') {
        return 0;
    }
    return num;
}
// Creates a socket for listening for connections.
// Closes the program and prints an error message on error.
static int create_listen_socket(uint16_t port) {
    struct sockaddr_in addr;
    int listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if (listenfd < 0) {
        err(EXIT_FAILURE, "socket error");
    }
    memset(&addr, 0, sizeof addr);
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htons(INADDR_ANY);
    addr.sin_port = htons(port);
    if (bind(listenfd, (struct sockaddr *) &addr, sizeof addr) < 0) {
        err(EXIT_FAILURE, "bind error");
    }
    if (listen(listenfd, 128) < 0) {
        err(EXIT_FAILURE, "listen error");
    }
    return listenfd;
}

void put_cmd(int connfd, struct header_field *header, char *body, int bodySize) {
    int fd = 0;
    int bytes_written = 0;
    int bytes_read = 0;
    char buffer[MAXBUFFLEN] = { 0 };
    if (access(header->uri, F_OK) == 0) {
        //file found
        if (access(header->uri, W_OK) == 0) {
            //file is write accessable
            fd = open(header->uri, O_WRONLY | O_TRUNC);
            if (fd == -1) {
                pthread_mutex_lock(&log_resp_mutex);
                create_log(header, 500);
                pack_response(connfd, 500);
                pthread_mutex_unlock(&log_resp_mutex);
                return;
            }
            flock(fd, LOCK_EX);
            bytes_written = write(fd, body, bodySize);
            if (bytes_written == -1) {
                flock(fd, LOCK_UN);
                pthread_mutex_lock(&log_resp_mutex);
                create_log(header, 500);
                pack_response(connfd, 500);
                pthread_mutex_unlock(&log_resp_mutex);
                return;
            }
            while (bytes_written < header->contentLength) {
                memset(buffer, 0, MAXBUFFLEN);
                bytes_read = recv(connfd, buffer, MAXBUFFLEN, 0);
                if (bytes_read == 0) {
                    break;
                }
                bytes_written += write(fd, buffer, bytes_read);
            }
            flock(fd, LOCK_UN);
        } else {
            //file is not write accessible
            pthread_mutex_lock(&log_resp_mutex);
            create_log(header, 403);
            pack_response(connfd, 403);
            pthread_mutex_unlock(&log_resp_mutex);
        }
        pthread_mutex_lock(&log_resp_mutex);
        create_log(header, 200);
        pack_response(connfd, 200);
        pthread_mutex_unlock(&log_resp_mutex);
        close(fd);
    } else {
        //file doesnt exist
        fd = open(header->uri, O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU);
        if (fd == -1) {
            pthread_mutex_lock(&log_resp_mutex);
            create_log(header, 500);
            pack_response(connfd, 500);
            pthread_mutex_unlock(&log_resp_mutex);
            return;
        }
        flock(fd, LOCK_EX);
        bytes_written = write(fd, body, bodySize);
        if (bytes_written == -1) {
            flock(fd, LOCK_UN);
            pthread_mutex_lock(&log_resp_mutex);
            create_log(header, 500);
            pack_response(connfd, 500);
            pthread_mutex_unlock(&log_resp_mutex);
            return;
        }
        while (bytes_written < header->contentLength) {
            memset(buffer, 0, MAXBUFFLEN);
            bytes_read = recv(connfd, buffer, MAXBUFFLEN, 0);
            bytes_written += write(fd, buffer, bytes_read);
        }
        flock(fd, LOCK_UN);
        pthread_mutex_lock(&log_resp_mutex);
        create_log(header, 201);
        pack_response(connfd, 201);
        pthread_mutex_unlock(&log_resp_mutex);
        close(fd);
    }
    return;
}

void get_cmd(int connfd, struct header_field *header) {
    char append[14] = { 0 };
    int fd = 0;
    int fileSize = 0;
    char responseBuffer[HEADERLEN] = { 0 };
    if (access(header->uri, F_OK) == 0) {
        //file found
        if (access(header->uri, R_OK) == 0) {
            //file has access
            fd = open(header->uri, O_RDONLY);
            if (fd == -1) {
                pthread_mutex_lock(&log_resp_mutex);
                create_log(header, 500);
                pack_response(connfd, 500);
                pthread_mutex_unlock(&log_resp_mutex);
                return;
            }
            struct stat st;
            flock(fd, LOCK_SH);
            if (!fstat(fd, &st)) {
                fileSize = st.st_size;
                strcat(responseBuffer, statusGETreturn);
                sprintf(append, "%d\r\n\r\n", fileSize);
                strcat(responseBuffer, append);
                pthread_mutex_lock(&log_resp_mutex);
                send(connfd, responseBuffer, strlen(responseBuffer), 0);
                char buffer[MAXBUFFLEN] = { 0 };
                int inByteCtr = 0;
                while (inByteCtr < fileSize) {
                    int readSize = read(fd, buffer, MAXBUFFLEN);
                    inByteCtr += readSize;
                    send(connfd, buffer, readSize, 0);
                }
                flock(fd, LOCK_UN);
                create_log(header, 200);
                pthread_mutex_unlock(&log_resp_mutex);
                close(fd);
                return;
            } else {
                //fstat failed for some reason
                pthread_mutex_lock(&log_resp_mutex);
                create_log(header, 500);
                pack_response(connfd, 500);
                pthread_mutex_unlock(&log_resp_mutex);
                return;
            }
        } else {
            //file has no access
            pthread_mutex_lock(&log_resp_mutex);
            create_log(header, 403);
            pack_response(connfd, 403);
            pthread_mutex_unlock(&log_resp_mutex);
            return;
        }
    } else {
        //file not found
        pthread_mutex_lock(&log_resp_mutex);
        create_log(header, 404);
        pack_response(connfd, 404);
        pthread_mutex_unlock(&log_resp_mutex);
        return;
    }
    return;
}

void append_cmd(int connfd, struct header_field *header, char *body, int bodySize) {
    int fd = 0;
    int bytes_written = 0;
    int bytes_read = 0;
    char buffer[MAXBUFFLEN] = { 0 };
    if (access(header->uri, F_OK) == 0) {
        //file found
        if (access(header->uri, W_OK) == 0) {
            //file is write accessable
            fd = open(header->uri, O_WRONLY | O_APPEND);
            if (fd == -1) {
                pthread_mutex_lock(&log_resp_mutex);
                create_log(header, 500);
                pack_response(connfd, 500);
                pthread_mutex_unlock(&log_resp_mutex);
                return;
            }
            flock(fd, LOCK_EX);
            bytes_written = write(fd, body, bodySize);
            if (bytes_written == -1) {
                flock(fd, LOCK_UN);
                pthread_mutex_lock(&log_resp_mutex);
                create_log(header, 500);
                pack_response(connfd, 500);
                pthread_mutex_unlock(&log_resp_mutex);
                return;
            }
            while (bytes_written < header->contentLength) {
                memset(buffer, 0, MAXBUFFLEN);
                bytes_read = recv(connfd, buffer, MAXBUFFLEN, 0);
                if (bytes_read == 0) {
                    break;
                }
                bytes_written += write(fd, buffer, bytes_read);
            }
            flock(fd, LOCK_UN);
        } else {
            //file is not write accessible
            pthread_mutex_lock(&log_resp_mutex);
            create_log(header, 403);
            pack_response(connfd, 403);
            pthread_mutex_unlock(&log_resp_mutex);
        }
        pthread_mutex_lock(&log_resp_mutex);
        create_log(header, 200);
        pack_response(connfd, 200);
        pthread_mutex_unlock(&log_resp_mutex);
        close(fd);
    } else {
        //file doesnt exist
        pthread_mutex_lock(&log_resp_mutex);
        create_log(header, 404);
        pack_response(connfd, 404);
        pthread_mutex_unlock(&log_resp_mutex);
    }
    return;
}

bool parse_header(struct header_field *header, char *buffer, int connfd) {
    memset(header->method, 0, 8);
    memset(header->uri, 0, 19);
    memset(header->httpVersion, 0, 9);
    header->requestID = 0;
    header->contentLength = -1;

    char contentLenStr[10]; //can handle content length up to 10 digits long
    char requestIdStr[10]; //can handle content length up to 10 digits long
    regex_t re;

    if (regcomp(&re, RE_REQUEST, REG_EXTENDED)) {
        //bad compilation: internal server error
        pack_response(connfd, 500);
        regfree(&re);
        return 0;
    }
    regmatch_t groups[4];
    if (regexec(&re, buffer, 4, groups, 0)) {
        //error reading header file
        pack_response(connfd, 400);
        regfree(&re);
        return 0;
    } else {
        memcpy(header->method, buffer + groups[1].rm_so, groups[1].rm_eo - groups[1].rm_so);
        memcpy(header->uri, buffer + 1 + groups[2].rm_so, groups[2].rm_eo - 1 - groups[2].rm_so);
        memcpy(header->httpVersion, buffer + groups[3].rm_so, groups[3].rm_eo - groups[3].rm_so);
    }
    regfree(&re);

    if (regcomp(&re, RE_CONTENTLEN, REG_EXTENDED)) {
        //bad compilation
        pack_response(connfd, 500);
        regfree(&re);
        return 0;
    }
    regmatch_t clGroup[1];
    if (regexec(&re, buffer, 2, clGroup, 0)) {
        //error getting content length
        //regfree(&re);
        //return 1;

    } else {
        memcpy(contentLenStr, buffer + 16 + clGroup[0].rm_so,
            clGroup[0].rm_eo - clGroup[0].rm_so - 16);
        header->contentLength = atoi(contentLenStr);
    }

    regfree(&re);

    if (regcomp(&re, RE_ID, REG_EXTENDED)) {
        //bad compilation
        pack_response(connfd, 500);
        regfree(&re);
        return 0;
    }
    regmatch_t idGroup[1];
    if (regexec(&re, buffer, 2, idGroup, 0)) {
        //error getting content length
        regfree(&re);
        return 1;
    } else {
        memcpy(
            requestIdStr, buffer + 12 + idGroup[0].rm_so, idGroup[0].rm_eo - idGroup[0].rm_so - 12);
        header->requestID = atoi(requestIdStr);
    }
    regfree(&re);
    return 1;
}

void pack_response(int connfd, int status) {
    char responseBuffer[HEADERLEN] = { 0 };

    strcat(responseBuffer, "HTTP/1.1 ");
    switch (status) {
    case 200: strcat(responseBuffer, status200); break;
    case 201: strcat(responseBuffer, status201); break;
    case 400: strcat(responseBuffer, status400); break;
    case 403: strcat(responseBuffer, status403); break;
    case 404: strcat(responseBuffer, status404); break;
    case 500: strcat(responseBuffer, status500); break;
    case 501: strcat(responseBuffer, status501); break;
    }
    send(connfd, responseBuffer, strlen(responseBuffer), 0);
    return;
}

void create_log(struct header_field *header, int statusCode) {
    LOG("%s,", header->method);
    fflush(logfile);
    LOG("/%s,", header->uri);
    fflush(logfile);
    LOG("%d,", statusCode);
    fflush(logfile);
    LOG("%d\n", header->requestID);
    fflush(logfile);
    return;
}

static void handle_connection(int connfd) {
    char *buffer;
    buffer = (char *) malloc(HEADERLEN);
    char *body = NULL;
    int bodySize = 0;
    char headerEnd[] = "\r\n\r\n";
    struct header_field header;
    ssize_t bytes_read = recv(connfd, buffer, HEADERLEN, 0);
    body = strstr(buffer, headerEnd) + 4;
    bodySize = bytes_read + (buffer - body);
    if (body == NULL) {
        pack_response(connfd, 400);
        return;
    }
    //printf("%s",buffer);
    if (parse_header(&header, buffer, connfd) == 0) {
        return;
    }
    //printf("Content-Length: %d\n", header.contentLength);
    //printf("ID: %d\n", header.requestID);
    free(buffer);
    if (strcmp(header.httpVersion, "HTTP/1.1")) {
        //wrong http version
        pthread_mutex_lock(&log_resp_mutex);
        create_log(&header, 400);
        pack_response(connfd, 400);
        pthread_mutex_unlock(&log_resp_mutex);
    } else {
        if (!strcmp(header.method, "GET")) {
            get_cmd(connfd, &header);
        } else if (!strcmp(header.method, "PUT")) {
            if (header.contentLength == -1) {
                pthread_mutex_lock(&log_resp_mutex);
                create_log(&header, 400);
                pack_response(connfd, 400);
                pthread_mutex_unlock(&log_resp_mutex);
                return;
            }
            put_cmd(connfd, &header, body, bodySize);
        } else if (!strcmp(header.method, "APPEND")) {
            if (header.contentLength < 0) {
                pthread_mutex_lock(&log_resp_mutex);
                create_log(&header, 400);
                pack_response(connfd, 400);
                pthread_mutex_unlock(&log_resp_mutex);
                return;
            }
            append_cmd(connfd, &header, body, bodySize);
        } else {
            pthread_mutex_lock(&log_resp_mutex);
            create_log(&header, 501);
            pack_response(connfd, 501);
            pthread_mutex_unlock(&log_resp_mutex);
        }
    }

    /*
    //Prof Code
    char buf[BUF_SIZE];
    ssize_t bytes_read, bytes_written, bytes;
    do {
        // Read from connfd until EOF or error.
        bytes_read = read(connfd, buf, sizeof(buf));
        if (bytes_read < 0) {
            return;
        }
        // Write to stdout.
        bytes = 0;
        do {
            bytes_written = write(STDOUT_FILENO, buf + bytes, bytes_read - bytes);
            if (bytes_written < 0) {
                return;
            }
            bytes += bytes_written;
        } while (bytes_written > 0 && bytes < bytes_read);
        // Write to connfd.
        bytes = 0;
        do {
            bytes_written = write(connfd, buf + bytes, bytes_read - bytes);
            if (bytes_written < 0) {
                return;
            }
            bytes += bytes_written;
        } while (bytes_written > 0 && bytes < bytes_read);
    } while (bytes_read > 0);
    */
}

static void sigterm_handler(int sig) {
    if (sig == SIGTERM) {
        warnx("received SIGTERM");
        fclose(logfile);
        queue_free(q);
        exit(EXIT_SUCCESS);
    }
}

static void sigint_handler(int sig) {
    if (sig == SIGINT) {
        warnx("received SIGINT");
        fclose(logfile);
        queue_free(q);
        exit(EXIT_SUCCESS);
    }
}

static void usage(char *exec) {
    fprintf(stderr, "usage: %s [-t threads] [-l logfile] <port>\n", exec);
}

void *handle_threads(void *arg) {
    for (;;) {
        //printf("IN HANDLE THREADS\n");
        (void) arg;
        int connfd;
        pthread_mutex_lock(&queueMutex);
        while (queue_empty(q)) {
            pthread_cond_wait(&queueEmptyCond, &queueMutex);
        }
        connfd = dequeue(q);
        pthread_cond_signal(&queueFullCond);
        pthread_mutex_unlock(&queueMutex);
        handle_connection(connfd);
        close(connfd);
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    //struct Queue* q = queue_new(5);
    q = queue_new(5);
    // queue_print(q);
    // enqueue(q, 1);
    // enqueue(q, 2);
    // queue_print(q);
    // enqueue(q, 3);
    // enqueue(q, 4);
    // queue_print(q);
    // //int temp;
    // //temp = dequeue(q);
    // //temp = dequeue(q);
    // queue_print(q);
    // //temp = dequeue(q);
    // //temp = dequeue(q);
    // queue_print(q);
    // enqueue(q, 5);
    // printf("%d", enqueue(q, 6));
    // queue_print(q);
    //return 0;
    int opt = 0;
    int threads = DEFAULT_THREAD_COUNT;
    logfile = stderr;
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 't':
            threads = strtol(optarg, NULL, 10);
            if (threads <= 0) {
                errx(EXIT_FAILURE, "bad number of threads");
            }
            break;
        case 'l':
            logfile = fopen(optarg, "w");
            if (!logfile) {
                errx(EXIT_FAILURE, "bad logfile");
            }
            break;
        default: usage(argv[0]); return EXIT_FAILURE;
        }
    }
    if (optind >= argc) {
        warnx("wrong number of arguments");
        usage(argv[0]);
        return EXIT_FAILURE;
    }
    uint16_t port = strtouint16(argv[optind]);
    if (port == 0) {
        errx(EXIT_FAILURE, "bad port number: %s", argv[1]);
    }
    signal(SIGPIPE, SIG_IGN);
    signal(SIGTERM, sigterm_handler);
    signal(SIGINT, sigint_handler);
    int listenfd = create_listen_socket(port);
    //LOG("port=%" PRIu16 ", threads=%d\n", port, threads);

    pthread_t thread_pool[threads];
    for (int i = 0; i < threads; i++) {
        int s = pthread_create(&thread_pool[i], NULL, handle_threads, NULL);
        if (s != 0) {
            //error
            return 0;
        }
    }

    for (;;) {
        int connfd = accept(listenfd, NULL, NULL);
        if (connfd < 0) {
            warn("accept error");
            continue;
        }
        pthread_mutex_lock(&queueMutex);
        while (!enqueue(q, connfd)) {
            pthread_cond_wait(&queueFullCond, &queueMutex);
        }
        pthread_cond_signal(&queueEmptyCond);
        //queue_print(q);
        pthread_mutex_unlock(&queueMutex);
        //handle_connection(connfd);
        //close(connfd);
    }
    return EXIT_SUCCESS;
}
