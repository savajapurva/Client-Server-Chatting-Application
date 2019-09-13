#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>


int main(int argc, char *argv[]){
  char message[255];
  int server, portNumber;
  struct sockaddr_in servAdd;     // server socket address
  
 if(argc != 3){
    printf("Call syntax: %s <IP Address> <Port Number>\n", argv[0]);
    exit(0);
  }

  if ((server = socket(AF_INET, SOCK_STREAM, 0)) < 0){ //create socket
     fprintf(stderr, "Cannot create socket\n");
     exit(1);
  }

  servAdd.sin_family = AF_INET;
  sscanf(argv[2], "%d", &portNumber);
  servAdd.sin_port = htons((uint16_t)portNumber);

  if(inet_pton(AF_INET, argv[1], &servAdd.sin_addr) < 0){ //connect with port
  fprintf(stderr, " inet_pton() has failed\n");
  exit(2);
}

  
 if(connect(server, (struct sockaddr *) &servAdd, sizeof(servAdd))<0){ //connect server with Ip
  fprintf(stderr, "connect() has failed, exiting\n");
  exit(3);
 }

 while(1){
  

 
  fprintf(stderr, "Please enter command(s) or enter quit sign to exit from a server \n");
  fgets(message, 254, stdin);
  
  
  if(strcmp(message,"quit\n")==0)
	{
		close(server);
		// fprintf(stderr, "All data received\n");
		 exit(0);
	}

  write(server, message, strlen(message)+1);
  char ch;
  char str[1000];
  int i=0;
  fprintf(stderr, "reading from Server\n");
  while(1)
  {
	  if (!read(server, &ch, 1)){
        fprintf(stderr, "Server is not available\n");
       exit(3); 
	}
	fprintf(stderr, "%c",ch);
	
	str[i]=ch; 
	i++;
	if(ch=='\n')
	{
		str[i-1]='\0';
		i=0;
	}
	if(strcmp(str,"######")==0)
	{
		 fprintf(stderr, "All data received\n");
		 break;
	}
 
 }
  
  
}
}

