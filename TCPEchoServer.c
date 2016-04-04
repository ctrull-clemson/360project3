/****************************************************************************************************************
*
* Author name: Christian Trull
*
* Module:      simhttp source code
*
* File Name:   TCPEchoServer.c
*
* Summary:
*  This file contains the code that functions as a server that HTTP GET requests can be sent to, and will then
*     respond with a correct HTTP response. This code manages the creation of the socket, and listening for 
*     connections. All interpretation and writing of inputs and resposnes is located in the HandleTCPClient.c
*     file. This code is a simple TCP Server code, similar to the one provided in our textbook.
*
****************************************************************************************************************/

#include <stdio.h>      /* for printf() and fprintf() */
#include <sys/socket.h> /* for socket(), bind(), and connect() */
#include <arpa/inet.h>  /* for sockaddr_in and inet_ntoa() */
#include <stdlib.h>     /* for atoi() and exit() */
#include <string.h>     /* for memset() */
#include <unistd.h>     /* for close() */

#define MAXPENDING 5    /* Maximum outstanding connection requests */

void DieWithError(char *errorMessage);  /* Error handling function */
void HandleTCPClient(int clntSocket, char directory[]);   /* TCP client handling function */

int main(int argc, char *argv[])
{
   int servSock;                       /* Socket descriptor for server */
   int clntSock;                       /* Socket descriptor for client */
   //int i;                            /* Loop controllers */
   char directory[64];                 /* Server's storage directory */
   struct sockaddr_in echoServAddr;    /* Local address */
   struct sockaddr_in echoClntAddr;    /* Client address */
   unsigned short echoServPort;        /* Server port */
   unsigned int clntLen;               /* Length of client address data structure */
   echoServPort = 8080;

   if ((argc < 1) || (argc > 4))       /* Test for correct number of arguments */
   {
      fprintf(stderr, "Incorrect number of arguments entered!\n");
      exit(1);
   }

   // Only directory given
   if(argc == 2)
   {
      echoServPort = 8080;
      strcpy(&directory[0], argv[1]);
   }
   // Only port given, with flag
   else if(argc == 3)
   {
      echoServPort = atoi(argv[2]);
      strcpy(directory, ".");
   }
   // Both port and directory given
   else if(argc == 4)
   {
      echoServPort = atoi(argv[2]);
      strcpy(&directory[0], argv[3]);
   }
      
   int dirLen = strlen(directory);
   if(directory[dirLen-1] != '/')
   {
      directory[dirLen] = '/';
      directory[dirLen + 1] = '\0';
   }
   
   /* Create socket for incoming connections */
   if ((servSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
      DieWithError("socket() failed");

   /* Construct local address structure */
   memset(&echoServAddr, 0, sizeof(echoServAddr));   /* Zero out structure */
   echoServAddr.sin_family = AF_INET;                /* Internet address family */
   echoServAddr.sin_addr.s_addr = htonl(INADDR_ANY); /* Any incoming interface */
   echoServAddr.sin_port = htons(echoServPort);      /* Local port */

   /* Bind to the local address */
   if (bind(servSock, (struct sockaddr *) &echoServAddr, sizeof(echoServAddr)) < 0)
      DieWithError("bind() failed");

   /* Mark the socket so it will listen for incoming connections */
   if (listen(servSock, MAXPENDING) < 0)
      DieWithError("listen() failed");

   for (;;) /* Run forever */
   {
      /* Set the size of the in-out parameter */
      clntLen = sizeof(echoClntAddr);

      /* Wait for a client to connect */
      if ((clntSock = accept(servSock, (struct sockaddr *) &echoClntAddr, 
                      &clntLen)) < 0)
      DieWithError("accept() failed");

      HandleTCPClient(clntSock, directory);
   }
   /* NOT REACHED */
}





