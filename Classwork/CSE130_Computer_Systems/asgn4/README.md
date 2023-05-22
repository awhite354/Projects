# Http Server
Httpserver is a program that implements the PUT, GET, and APPEND functions of a server from assignment 1. The methods in this program can be used to securely interact with the files in the directory that the server is running on. This allows a user to get, put, and append files through a socket with the server. In addition to these functions, the server is multi-threaded by creating a thread pool with n threads (specified by the user) and using these threads to perform tasks for the client. To keep track of tasks, I implemented a circular queue that stores the connfd's that need to be processed. This implementation also includes a log that keeps track of the operations performed by the server, this logfile should represent the order in which the requests are received. Our request also needs to be executed in an atomic manner when necessary, like when accessing the same file.

## Usage

```bash
./httpserver [-t threads] [-l logfile] <port>
```

## Design
I started this program by laying out the different modules I wanted to use for the design. I chose to create modules for each method(PUT, GET, APPEND),  a module to parse the header, and a module to output common responses to the client. All the other logic was handled in the provided handle_connection module. I added a logging module that took in the header and status code to create the log entry. I also added a handle_threads function which keeps the threads in a loop to check if there is work to be done. The queue I added is used to make sure the dispatcher and worker threads can interface to dish out work. Handling interfering requests was also handled by using flock() on files when being accessed. A shared lock is used when reading from a file, and an exclusive lock is used when appending or putting to a file. The proper ordering of the logfile and processes was enforced with a mutex lock around the pack_response function and create_log function.

## My_Queue
My queue is a circular queue that is used to store connfd's. The dispatcher thread enqueues connfd's into the queue, (waiting if there is no room), and the worker threads dequeue connfd's (waiting if it is empty). The queue is set to store ints (which may need to be changed later, due to changing states when implementing non-blocking io). I also included a few helper functions such as print_queue() which prints the queue (for debugging purposes), and free_queue() which frees the allocated memory when SIGINT or SIGTERM is called.
 
## Functions in httpserver

#### handle_threads
This function is called when pthread_create() is used to create n threads. This function loops forever for each thread, and waits on a condition variable if there is nothing in the queue. If the queue gets a connfd, the worker thread can dequeue the connfd, signal that there is room in the queue again, and handle the connection. 
#### handle_connection
The handle connection module was given a connfd and creates a buffer set to the maximum header length to receive data from the socket. It then splits the buffer if needed into a separate body section after "\r\n\r\n". It then parses the header and passes the body, connfd, parsed header, and body size to the appropriate function. If any error occurs this is logged in the log file.
#### parse_header
The parse header function uses the <regex.h> library to parse the header into appropriate variables laid out in a header struct that is passed in by reference. It then returns a boolean value whether it is parsed appropriately or not. I don't have any logging in this function.  
#### pack_response
Pack response gets a status number and the connfd and uses a switch statement to send the appropriate response to the client.
#### create_log
This function creates a log using the provided LOG() function as well as the fflush function to clear the output buffer.

#### get_cmd
The get command is given the connfd, and header struct. This function verifies the existence, open-ability, and readability of a file specified in the header URI. It then gets the file size and creates a buffer to transfer data. It loops while filling the buffer from reading the file, and sending it to the client until the file is completely sent to the client. Whether this operation is successful or not, a log is created with the correct status code.
#### put_cmd
The put command module is given the connfd, header struct, body, and body size. This function checks the existence and writability of a file (specified by the URI) and creates, or overwrites this file accordingly. This is done by first writing the contents of the partial body into the file, then receiving any more data that is part of the body left in the socket. Whether this operation succeeds or not, there is a log entry created with the correct status code.
#### append_cmd
This function operates very similarly to the put function, only it appends to a file, and does not create a file. It uses the same methods to append to a file. Whether this operation is successful or not, a log is created with the correct status code.
## Error Handling
The error handling for this program is all done by sending back status codes, and messages. These errors are also logged, except for errors that occur before parsing. The provided status errors gave enough options to satisfy all errors that a client could encounter when using the server. These errors were all handled to not crash the server when being used by a client. A record of errors and successful processes can be found in the log. 