#include <err.h>
#include <errno.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <regex.h>
#include <assert.h>
#include <stdbool.h>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/stat.h>

#define MAXBUFFLEN 4096 //used to be 4096
#define HEADERLEN  2048 //used to be 2048

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
#define RE_REQUEST                                                                                 \
    ("^"                                                                                           \
     "(" RE_METHOD ")"                                                                             \
     " "                                                                                           \
     "(" RE_URI ")"                                                                                \
     " "                                                                                           \
     "(" RE_HTTPVERSION ")"                                                                        \
     "\r\n")

void pack_response(int, int);

struct header_field {
    char method[8];
    char uri[19];
    char httpVersion[9];
    int contentLength;
};

/**
   Converts a string to an 16 bits unsigned integer.
   Returns 0 if the string is malformed or out of the range.
 */
uint16_t strtouint16(char number[]) {
    char *last;
    long num = strtol(number, &last, 10);
    if (num <= 0 || num > UINT16_MAX || *last != '\0') {
        return 0;
    }
    return num;
}

/**
   Creates a socket for listening for connections.
   Closes the program and prints an error message on error.
 */
int create_listen_socket(uint16_t port) {
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

    if (listen(listenfd, 500) < 0) {
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
                pack_response(connfd, 500);
                return;
            }
            bytes_written = write(fd, body, bodySize);
            if (bytes_written == -1) {
                pack_response(connfd, 500);
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
        } else {
            //file is not write accessible
            pack_response(connfd, 403);
        }
        pack_response(connfd, 200);
        close(fd);
    } else {
        //file doesnt exist
        fd = open(header->uri, O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU);
        if (fd == -1) {
            pack_response(connfd, 500);
            return;
        }
        bytes_written = write(fd, body, bodySize);
        if (bytes_written == -1) {
            pack_response(connfd, 500);
            return;
        }
        while (bytes_written < header->contentLength) {
            memset(buffer, 0, MAXBUFFLEN);
            bytes_read = recv(connfd, buffer, MAXBUFFLEN, 0);
            bytes_written += write(fd, buffer, bytes_read);
        }
        pack_response(connfd, 201);
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
                pack_response(connfd, 500);
                return;
            }
            struct stat st;
            if (!fstat(fd, &st)) {
                fileSize = st.st_size;
                strcat(responseBuffer, statusGETreturn);
                sprintf(append, "%d\r\n\r\n", fileSize);
                strcat(responseBuffer, append);
                send(connfd, responseBuffer, strlen(responseBuffer), 0);
                char buffer[MAXBUFFLEN] = { 0 };
                int inByteCtr = 0;
                while (inByteCtr < fileSize) {
                    int readSize = read(fd, buffer, MAXBUFFLEN);
                    inByteCtr += readSize;
                    send(connfd, buffer, readSize, 0);
                }
                close(fd);
                return;
            } else {
                //fstat failed for some reason
                pack_response(connfd, 500);
                return;
            }
        } else {
            //file has no access
            pack_response(connfd, 403);
            return;
        }
    } else {
        //file not found
        pack_response(connfd, 404);
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
                pack_response(connfd, 500);
                return;
            }
            bytes_written = write(fd, body, bodySize);
            if (bytes_written == -1) {
                pack_response(connfd, 500);
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
        } else {
            //file is not write accessible
            pack_response(connfd, 403);
        }
        pack_response(connfd, 200);
        close(fd);
    } else {
        //file doesnt exist
        pack_response(connfd, 404);
    }
    return;
}

bool parse_header(struct header_field *header, char *buffer, int connfd) {
    memset(header->method, 0, 8);
    memset(header->uri, 0, 19);
    memset(header->httpVersion, 0, 9);
    header->contentLength = -1;

    char contentLenStr[10]; //can handle content length up to 10 digits long
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
        regfree(&re);
        return 1;
    } else {
        memcpy(contentLenStr, buffer + 16 + clGroup[0].rm_so,
            clGroup[0].rm_eo - clGroup[0].rm_so - 16);
        header->contentLength = atoi(contentLenStr);
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

void handle_connection(int connfd) {
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
    if (parse_header(&header, buffer, connfd) == 0) {
        return;
    }
    free(buffer);
    if (strcmp(header.httpVersion, "HTTP/1.1")) {
        //wrong http version
        pack_response(connfd, 400);
    } else {
        if (!strcmp(header.method, "GET")) {
            get_cmd(connfd, &header);
        } else if (!strcmp(header.method, "PUT")) {
            if (header.contentLength == -1) {
                pack_response(connfd, 400);
                return;
            }
            put_cmd(connfd, &header, body, bodySize);
        } else if (!strcmp(header.method, "APPEND")) {
            if (header.contentLength == -1) {
                pack_response(connfd, 400);
                return;
            }
            append_cmd(connfd, &header, body, bodySize);
        } else {
            pack_response(connfd, 501);
        }
    }
}

int main(int argc, char *argv[]) {
    int listenfd;
    uint16_t port;

    if (argc != 2) {
        errx(EXIT_FAILURE, "wrong arguments: %s port_num", argv[0]);
    }

    port = strtouint16(argv[1]);
    if (port == 0) {
        errx(EXIT_FAILURE, "invalid port number: %s", argv[1]);
    }
    listenfd = create_listen_socket(port);

    signal(SIGPIPE, SIG_IGN);

    while (1) {

        int connfd = accept(listenfd, NULL, NULL);
        if (connfd < 0) {
            warn("accept error");
            continue;
        }
        handle_connection(connfd);

        // good code opens and closes objects in the same context. *sigh*
        close(connfd);
    }

    return EXIT_SUCCESS;
}
