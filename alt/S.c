#include "server.h"
#include "resp.h"
#define BUF_SIZE 1024


int server_init(int port){
    int socket_fd;
    struct sockaddr_in server_addr;
    int opt = 1;

    if((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0){
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR , &opt, sizeof(opt))){
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    if(bind(socket_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0){
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    printf("Server listening on port %d\n", port);



    return socket_fd;
}



// function that handles the response to the client
char * response( char * request){

    if (strncmp(request ,"PING",3) == 0){
        return "+PONG";

    }else if(strncmp(request ,"ECHO",3) == 0){
        return "+ECHO\r\n";
    
    }else if(strncmp(request, "TIME",3) == 0){
        return "+TIME\r\n";
    
    }else if(strncmp(request, "QUIT",3) == 0){
        return "+OK\r\n";
    
    }else if(strncmp(request, "SET",2) == 0){
        return "+OK\r\n";
    
    }else return "-ERR\r\n";

}


//function that handles the request from the client 
void request_handler(char * request, hashtable *ht,char* buf, int rnum, RespRequest *req, RespResponse *res){

		if (decode_request(req, buf, rnum) == 0 && 
			(req->state == OK || req->state == PART_OK) ){
			if (req->argc > 1 && strncmp(request_argv(req,0), "add", 3) == 0) {
				int total = 0;
				for (int i = 1; i < req->argc; i++)
					total += atoi(request_argv(req,i));
				encode_response_integer(res, total);
			}else if (req->argc > 1 && strncmp(request_argv(req,0), "set", 3) == 0) {
                hashtable_set(ht, request_argv(req,1), request_argv(req,2));
                encode_response_status(res,1,"OK");
            }else if (req->argc ==1 && strncmp(request_argv(req,0), "PING", 3) == 0) {
                encode_response_status(res,1,"PONG");
            }else if (req->argc =2 && strncmp(request_argv(req,0), "get", 3) == 0) {
                 // get the value from the hashtable of the key
                 // key is the second argument
                char* key = request_argv(req,1);
               printf("Key: %s\n", key);
               char *value = hashtable_get(ht, key);
                //
               printf("Value: %s\n", value);
                // 
                encode_response_string(res, hashtable_get(ht, key), strlen(hashtable_get(ht, key)));

                // print all the keys and values in the hashtable
                 //hashtable_print(ht);

               // encode_response_string(res, value, strlen(value));
            }else if (req->argc ==1 && strncmp(request_argv(req,0), "QUIT", 3) == 0) {
                encode_response_status(res,1,"OK");
               
            
			}else if (req->argc =2 && strncmp(request_argv(req,0), "DEL", 3) == 0) {
                hashtable_delete(ht, request_argv(req,1));
                encode_response_status(res,1,"OK");
            
            } else {
				encode_response_status(res,0,"ERR unknown command");
			}
		}
		else {
			encode_response_status(res,0,"ERR decode command fail");
		}
   
}

int main(int argc , char **argv){

    int PORT = atoi(argv[1]);
    int socket_fd; 
    int CLIENTS[MAXCLIENTS];
    int client_count = 0;
    int newfd; 
    int max_fd;
    fd_set master;    // master file descriptor list
    fd_set read_fds;  // temp file descriptor list for select()
    char buf[256];    // buffer for client data
    int nbytes;


    struct timeval tv;
    //initialize server socket
    socket_fd = server_init(PORT);

    //listen for connections
    if(listen(socket_fd, 10) < 0){
        perror("listen");
        exit(EXIT_FAILURE);
    }

    //accept connections
    int addrlen = sizeof(struct sockaddr_in);
    struct sockaddr_in client_addr;
   // int new_socket;
    // create a hashtable
    hashtable *ht = hashtable_new(65536);
    RespRequest *req = create_request(BUF_SIZE);
	RespResponse *res = create_response(BUF_SIZE);
    //// Set up file descriptors for select()
    FD_ZERO(&master);    // clear the master and temp sets
    FD_ZERO(&read_fds);
    FD_SET(socket_fd, &master);

    // keep track of the biggest file descriptor
    max_fd = socket_fd; // so far, it's this one


     for(;;) {
        read_fds = master; // copy it
        if (select(max_fd+1, &read_fds, NULL, NULL, NULL) == -1) {
            perror("select");
            exit(4);
        }

        // run through the existing connections looking for data to read
        for(int i = 0; i <= max_fd; i++) {
            if (FD_ISSET(i, &read_fds)) { // we got one!!
                if (i == socket_fd) {
                    // handle new connections
                   
                    newfd = accept(socket_fd,
                        (struct sockaddr *)&client_addr,
                        &addrlen);

                    if (newfd == -1) {
                        perror("accept");
                    } else {
                        FD_SET(newfd, &master); // add to master set
                        if (newfd > max_fd) {    // keep track of the max
                            max_fd = newfd;
                        }
                       
                    }
                } else {
                    // handle data from a client
                    if ((nbytes = recv(i, buf, sizeof buf, 0)) <= 0) {
                        // got error or connection closed by client
                        if (nbytes == 0) {
                            // connection closed
                            printf("selectserver: socket %d hung up\n", i);
                        } else {
                            perror("recv");
                        }
                        close(i); // bye!
                        FD_CLR(i, &master); // remove from master set
                    } else {
                        // we got some data from a client
                        for(int j = 0; j <= max_fd; j++) {
                            reset_request(req);
		                    reset_response(res);
                           //handle the request from the client
                            int rnum = read(j,buf,sizeof(buf));
                           request_handler(buf, ht, buf, rnum, req, res);
                            write(j, res->buf, res->used_size);
                        }
                    }
                } // END handle data from client
            } // END got new incoming connection
        } // END looping through file descriptors
    } // END for(;;)--and you thought it would never end!
	


        return 0;
}

