#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>

#define MAXPENDING 5
#define BUFFSIZE 2048
#define MAX 2048

void ErrExit(char *mess) 
{
    perror(mess);
    exit(1);
}

int getCommandArray(char * str, char *args[]) // handle entered command
{
    const char s[1] = ";";
    char *token;
    token = strtok(str, s);
    int i = 0;
	
    while( token != NULL)
    {
        args[i] = token;
        i++;
        //printf("%s\n", token);
        token = strtok(NULL,s);
    }
   return i;
}

void ServiceClient(int sock){

		char buffer[BUFFSIZE];
		int received = -1;

        dup2(sock,1);//read and write console in the socket

		while(1) { // this will make server wait for another command to run until it receives exit
			fprintf(stderr,"Waiting for client command\n");
			if(!(received = recv(sock, buffer,BUFFSIZE,0))){
					fprintf(stderr,"Client is exited\n");
					break;
					exit(0);
			}

			buffer[received-2] = '\0'; //split \n last char or command

			//puts (buffer);
			printf("COMMANDs:(%s)",buffer);
			
			char *arr[100];
			int noOfcommands=getCommandArray(buffer, arr); //split ; and store from buffer to array
			int i=0;
			int ch;
			for(i=0;i<noOfcommands;i++)
			{
				printf("\n");
				fprintf(stderr,"COMMANDs(%s)\n",arr[i]);
				
				char *token;
				token = strtok(arr[i], " ");
					int i = 0;
				
				char *args[100];
			while( token != NULL)
					{
					args[i] = token;
					i++;
					//printf("%s\n", token);
					token = strtok(NULL," ");
				}
				args[i]=NULL;
				
				if(fork()==0){ // taking child argument ex. ls -F
				ch=execvp(args[0] ,args);
				exit(ch);
			} else {
				int n;
				wait(&n);
				if (WEXITSTATUS(n)==0){
				  fprintf(stderr,"sucess\n");
			} else {
				fprintf(stderr,"ERROR\n");
				}
			}
			}
			
			printf("\n");
			
			printf("######\n"); // used to know end of command/input list
		}

}

int main(int argc, char const *argv[])
{
 	
   int sd, client, portNumber;
   struct sockaddr_in servAdd;     // server socket address
  
  if(argc != 2){
    printf("Call model: %s <Port Number>\n", argv[0]);
    exit(0);
  }

  sd = socket(AF_INET, SOCK_STREAM, 0);
  servAdd.sin_family = AF_INET;
  servAdd.sin_addr.s_addr = htonl(INADDR_ANY);
  sscanf(argv[1], "%d", &portNumber);
  servAdd.sin_port = htons((uint16_t)portNumber);
  
  bind(sd, (struct sockaddr *) &servAdd, sizeof(servAdd));
  listen(sd, 5);

  while(1){
	printf("waiting for client\n");
    client = accept(sd, NULL, NULL);
    printf("Got a client\n");

    if(!fork())
      ServiceClient(client);
      close(client);
    }
    return 0;
}





    