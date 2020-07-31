#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <openssl/evp.h>
#include <pthread.h>

//#include <iostream>
//#include <thread>

#define CONN_NUM 8

int create_socket(){
    int sock;
    struct sockaddr_in addr_info;
    addr_info.sin_family = AF_INET;
    addr_info.sin_port = htons(20000);
    inet_pton(AF_INET, "127.0.0.1", &(addr_info.sin_addr));
    sock = socket(PF_INET, SOCK_STREAM, 0);
    bind(sock, (struct sockaddr*)&addr_info, sizeof(addr_info));
    listen(sock, 100);
    return sock;
}

int accept_connection(int sock){
    int recv_sock;
    struct sockaddr_in client_addr_info;
    socklen_t client_addr_size;

    recv_sock = accept(sock, (struct sockaddr*)&client_addr_info, &client_addr_size);
    //cout << "Accepted" << endl;
    //printf("Accepted\n");
    return recv_sock;
}

int receive_data(int sock, char* data){
    return recv(sock, data, sizeof(data), 0);
}

int send_data(int sock, char* data){
    return send(sock, data, strlen(data), 0);
}

void sha256(const char* str, char* hash){
    EVP_MD_CTX *mdctx;
    const EVP_MD *md;
    unsigned char md_value[EVP_MAX_MD_SIZE];
    unsigned int md_len, i;

    mdctx = EVP_MD_CTX_new();
    md = EVP_get_digestbyname("sha256");
    EVP_DigestInit_ex(mdctx, md, NULL);
    EVP_DigestUpdate(mdctx, str, strlen(str));
    EVP_DigestFinal_ex(mdctx, md_value, &md_len);
    EVP_MD_CTX_free(mdctx);
    for(i = 0; i < md_len; i++){
        sprintf(hash + (i * 2), "%02x", md_value[i]);
    }
    hash[64] = 0;
    return;
}
void gen_random(char *s, const int len) {
    static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";

    for (int i = 0; i < len; ++i) {
        s[i] = alphanum[rand() % (sizeof(alphanum) - 1)];
    }


    //s[len]='\n';
    s[len] = 0;
}

static void * handle_client(void *arg) {
    int client_fd = * ((int *) arg);
    //pthread_detach(pthread_self());

    printf("thread %ld serving client %d \n", (long) pthread_self(), client_fd);

    while(1){
    int len;
                char data[6];
                len = receive_data(client_fd, data);
                if(len > 0){
                  data[5]=0;
                  printf("length data--%ld\n", strlen(data));
                    /* socket is valid, do something here... */
                    int numZeros=4,numF,workDone=0;
                    char seed[6];
                    char hash[69];
                    while(workDone==0)
                    {
                      numF=0;
                      gen_random(seed,5);
                      char seeddata[11];
                      strcpy(seeddata,seed);
                      strcat(seeddata,data);
                      sha256(seeddata,hash);
                      for(int i=0;i<4;i++)
                      {
                        if(hash[i]=='0')
                        {
                          numF++;

                        }
                        else if(hash[i]!='0')
                        {
                          break;
                        }
                        if(numF==4) workDone=1;
                      }
                    }
                    char complete[76];

                    strcpy(complete,data);
                    strcat(complete,",");
                    strcat(complete,seed);
                    strcat(complete,",");
                    strcat(complete,hash);
                    strcat(complete,"\n");
                    printf("complete---%s",complete );
                    printf("data---%s\n",data );
                    printf("seed---%s\n",seed );
                    printf("hash---%s\n",hash );
                    int rec = send_data(client_fd,complete);

                } else if(len == 0){
                    close(client_fd);
		    //close(client_fd);
		    printf("thread %ld serving client %d \n", (long) pthread_self(), client_fd);
		    return (NULL);
                }
    }
    return (NULL);
}

int main(){
    int sock, i;
    //pthread_t thread_ptr[CONN_NUM];
    pthread_t *thread_ptr;
    thread_ptr = malloc(sizeof(pthread_t) * CONN_NUM);
    sock = create_socket();
    /*The following is done by single-thread, you should modify it!*/
    while(1){
        for(i = 0; i < CONN_NUM; i++){
            int *client_fd;
	        client_fd = malloc(sizeof(int) * 1);
	        *client_fd = accept_connection(sock);
	        pthread_create(thread_ptr + i, NULL, &handle_client, client_fd);
        }
        for(i = 0; i < CONN_NUM; i++){
            pthread_join(*(thread_ptr + i), NULL);
        }
    }
}
