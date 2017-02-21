/***** TCP SERVER *****/

#include <cstdio>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

using namespace std;

int main() {

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
        exit(1);
    }

    // Iterate through *result to find valid entry
    struct addrinfo *server_info;
    for (server_info = result, server_info != NULL, server_info->server_info.ai_next) {

    }

    // Get socket descriptor
    int socketsd;
    if ((socketsd = socket(server_info->ai_family, 
                           server_info->ai_socktype, 
                           server_info->ai_protocol)) != 0) {
        cerr << "Error calling socket(): " << endl;
        exit(1);
    }



    // Free linked list to server information
    freeaddrinfo(result);

    return 0;
}

