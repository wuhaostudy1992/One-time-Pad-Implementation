// Server side C/C++ program to demonstrate Socket programming
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>

#define PORT 8080

void Decipher(char *message, char *keyPath);
char* Name(char* message);
int Compare(char* left, char*right);

int main(int argc, char const *argv[])
{
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
      
    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
      
    // Forcefully attaching socket to the port
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );
      
    // Forcefully attaching socket to the port
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    
    while(1)
    {
        memset(buffer,0,sizeof(buffer));
        valread = read(new_socket , buffer, 1024);
        //printf("%s, %ld\n",buffer, strlen(buffer));
        Decipher(buffer, "mykey.txt");
        //printf("%s, %ld\n",buffer, strlen(buffer));
        char *name = Name(buffer);
        send(new_socket , name , strlen(name) , 0);
    }
    return 0;
}

void Decipher(char *message, char *keyPath)
{
    //Read the key file
    FILE *fp = NULL;
    char buff[25];
    fp = fopen(keyPath, "r");
    fscanf(fp, "%s", buff);
    fclose(fp);
    //printf("Content of keyfile is: %s\n", buff );
    
    //dencryption
    int i = 0;
    while(message[i] != '\0')
    {
        message[i] = message[i] ^ buff[i];
        i++;
    }
    //printf("After encryption: %s\n", message);
}

char* Name(char* message)
{
    if(Compare(message, "harris@ics.uci.edu"))
        return "Lan G. Harris";
    else if(Compare(message, "joe@cnn.com"))
        return "Joe Smith";
    else if(Compare(message, "jane@slashdot.org"))
        return "Jane Smith";
    else if(Compare(message, "test"))
        return "This is for testing";
    else if(Compare(message, "t"))
        return "Th";
    else
        return "Unknown";
}

int Compare(char* left, char*right)
{
    int length = strlen(left);
    if(length != strlen(right))
    {
        return 0;
    }
    int i = 0;
    while(i < length)
    {
        if(left[i] != right[i])
            return 0;
        i++;
    }
    return 1;
}
