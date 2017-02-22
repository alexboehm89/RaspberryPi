/***** TCP SERVER *****/

#include <cstdio>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

using namespace std;

void sigchld_handler(int s)
{
    // waitpid() might overwrite errno, so we save and restor it:
    int saved_errno = errno;

    while(waitpid(-1, NULL, WNOHANG) > 0);

    errno = saved_errno;
}

void *get_in_addr( struct sockaddr *sa) 
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int main() 
{

    // Set listening port of server
    const int port = 2420;
    
    // Prepare TCP connection
    int status;
    struct addrinfo server_specs;
    struct addrinfo *result; // points to structure that holds server information

    memset(&server_specs, 0, sizeof(server_specs)); // make sure the struct is empty
    server_specs.ai_family      = AF_INET; // IPv4
    server_specs.ai_socktype    = SOCK_STREAM; // TCP stream socket
    server_specs.ai_flags       = AI_PASSIVE; // automatically fill in IP

    // Call getaddrinfo() and throw error if not succesful
    if ((status = getaddrinfo(NULL, port, &server_specs, &result)) != 0) {
        cerr << "Error calling getaddrinf(): " << gai_strerror(status) << endl;
        return(1);
    }

    // Loop through server info and bind to first valid entry 
    struct addrinfo *server_info;
    for (server_info = result, server_info != NULL, server_info->server_info.ai_next) {

        // Get socket descriptor
        int sockfd;
        if ((sockfd = socket(server_info->ai_family, 
                             server_info->ai_socktype, 
                             server_info->ai_protocol)) != 0) {
            cerr << "server: socket" << endl;
            continue;
        }

        // Make socket reusable if socket was just closed
        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
            cerr << "setsockopt" << endl;
            exit(EXIT_FAILURE);
        }

        // Bind socket
        if (bind(sockfd, server_info->ai_addr, server_info->ai_addrlen) == -1) {
            close(sockfd);
            cerr << "server: bind" << endl;
            continue;
        }

        break;
    }

    // Free linked list to server information
    freeaddrinfo(result);

    if (server_info == NULL) {
        cerr << "Server: failed to bind" << endl;
        exit(EXIT_FAILURE);
    }

    // Listen on port
    const int backlog = 10; // number of pending connections queue will hold
    if (listen(sockfd, backlog) == -1) {
        cerr << "listen" << endl;
        exit(EXIT_FAILURE);
    }

    // Reap all dead processes
    sa.sa_handler = sigchld_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        cerr << "sigaction" << endl;
        exit(EXIT_FAILURE);
    }

    cout << " server: waiting for connections... " << endl;

    // Main loop for accept()ing clients
    while(1) {
        sin_size = sizeof(their_addr);
        new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
        if (new_fd == -1) {
            cerr << "accept" << endl;
            continue;
        }

        inet_ntop(their_addr.ss_family, get_in_addr((struct sockaddr *)&their_addr), s, sizeof(s));
        cout << "server: got connection for " << s << endl;

        if (!fork()) {
            close(sockfd);
            if (send(new_fd, "Hello, World!", 13,0) == -1) {
                cerr << "send" << endl;
            }
            close(new_fd);
            exit(0);
        }
        close(new_fd);
    }

    
    return 0;
}

