// Client side C/C++ program to demonstrate Socket programming
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>

void Cipher(char *message, char *keyPath);

int main(int argc, char *argv[])
{
    if(argc < 3)
    {
        printf("Not enough parameters.\n");
    }
    
    int port = atoi(argv[1]);
    //char *domainName = argv[2];
    char *keyPath = argv[2];
    char message[80] = {0};

    struct hostent* hostent = gethostbyname("crystalcove.eecs.uci.edu");
    struct in_addr addr;
    addr.s_addr = *(unsigned long *)hostent->h_addr;
    printf("IPV4 : %s \n" , inet_ntoa(addr) );
    
    //I get the address of the server, so what should I do next?

    struct sockaddr_in address;
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }
  
    memset(&serv_addr, '0', sizeof(serv_addr));
  
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
      
    // Convert IPv4 and IPv6 addresses from text to binary form
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) 
    //if(inet_pton(AF_INET, inet_ntoa(addr), &serv_addr.sin_addr)<=0) 
    {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }
  
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\nConnection Failed \n");
        return -1;
    }
    
    while(1)
    {
        printf("Enter the email address: ");
        //scanf("%[^\n]", message);
        scanf("%s", message);
        //printf("The email you entered is: %s\n", message);
        
        Cipher(message, keyPath);
        //printf("After encryption: %s, %ld\n", result, strlen(result));

        send(sock, message, strlen(message), 0);
        //printf("Hello message sent\n");
        memset(message,0,sizeof(message));
        memset(buffer,0,sizeof(buffer));
        valread = read(sock , buffer, 1024);
        printf("%s\n",buffer);
    }
    return 0;
}

void Cipher(char *message, char *keyPath)
{
    //Read the key file
    FILE *fp = NULL;
    char buff[25];
    fp = fopen(keyPath, "r");
    fscanf(fp, "%s", buff);
    fclose(fp);
    //printf("Content of keyfile is: %s\n", buff );
    
    //encryption
    int i = 0;
    while(message[i] != '\0')
    {
        message[i] = message[i] ^ buff[i];
        i++;
    }
    //printf("After encryption: %s\n", message);
}
