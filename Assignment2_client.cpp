#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <iostream>
#include <iterator>
#include <queue>
#include <string>
#include <vector>
#include <sstream>
#include <functional>

using namespace std;

int main(int argc, char *argv[])
{
    int sockfd, portno, n;
	string hostName;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer[512];
	cout << "Enter the server's host name: ";
	cin >> hostName;
	cout << "Enter the server's port number: ";
	cin >> portno;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        return (-1);
    server = gethostbyname(hostName.c_str());
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0) 
        return (-3);
	do {
		printf("Please enter the major you wish to view the pay data of: ");
		bzero(buffer,512);
		fgets(buffer,511,stdin);
		n = write(sockfd,buffer,strlen(buffer));
		if (n < 0) 
			return(-1);
		bzero(buffer,512);
		n = read(sockfd,buffer,511);
		if (n < 0) 
			return (-1);
		printf("%s\n",buffer);
	} while (buffer[0] != '\0');
    return 0;
}