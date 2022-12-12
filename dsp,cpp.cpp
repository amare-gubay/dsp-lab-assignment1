/* Author: Pete Broadwell, Grinnell College

   This program uses streaming Internet sockets to simulate the readers-writers
   problem. This is the server/writer portion of the simulation. */
#include<iosteream>
using namespace std;
#include <sys/types.h>
#include <unistd.h>          
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>     /* socket command libraries needed by some compilers */
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>

#define SIM_LENGTH 10 /* number of integers to be written and read */
#define PORT 1227 /* local port on which connection will be established */

/* procedure to remove socket and exit */
void clean_up(int cond, int *sock)
{ cout<<"Exiting now.\n";
  close(*sock); /* close socket connection */
  exit(cond);
} /* end of clean_up */

int main(void)
{ /* processing for server */
  int sock; /* listening socket descriptor */
  int connect_sock; /* connected socket descriptor */
  struct sockaddr_in serv_name; /* socket address structure */
  size_t len; /* variable for the size of the socket address structure */
  int count;
  
  /* set socket descriptor */ 
  sock = socket(AF_INET, SOCK_STREAM, 0);  
  if (sock < 0)
    { perror ("Error opening channel");
      clean_up(1, &sock);
    }

  /* set name as the physical address of the socket descriptor (sock) */
  bzero(&serv_name, sizeof(serv_name)); /* initialize the address structure */
  serv_name.sin_family = AF_INET; /* socket family will be AF_INET
                                     (Internet sockets) */
  serv_name.sin_port = htons(PORT); /* sets the port on which the server will
				       establish the socket connection */
  
  /* bind the socket address to the socket descriptor */
  if (bind(sock, (struct sockaddr *)&serv_name, sizeof(serv_name)) < 0)
    { perror ("Error naming channel");
      clean_up(1, &sock);
    }
      
  cout<<"Server is alive and waiting for socket connection from client.\n";
  listen(sock, 1); /* listen for connections on the socket */
      
  /* accept connection request from client */
  len = sizeof(serv_name);
  connect_sock = accept(sock, (struct sockaddr *)&serv_name, &len);

  /* send integers to the client's connected socket descriptor */
  for (count = 1; count <= SIM_LENGTH; count++)
    { write(connect_sock, &count, 4);
      cout<<"Server has written %d to socket.\n"<< count;
    }

  close(connect_sock); /* close connected socket */
  clean_up(0, &sock); /* exit with no errors */

} /* end of main */
