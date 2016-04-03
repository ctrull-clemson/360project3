#include <stdio.h>      /* for printf() and fprintf() */
#include <sys/socket.h> /* for recv() and send() */
#include <string.h>
#include <stdlib.h>
#include <unistd.h>     /* for close() */

#define RCVBUFSIZE 4096   /* Size of receive buffer */

void DieWithError(char *errorMessage);  /* Error handling function */

void HandleTCPClient(int clntSocket, char directory[])
{
   char echoBuffer[RCVBUFSIZE];        /* Buffer for echo string */
   int recvMsgSize;                    /* Size of received message */
   char path[128];                     /* Path of wanted file from request */
   
   /* Receive message from client */
   if((recvMsgSize = recv(clntSocket, echoBuffer, RCVBUFSIZE, 0)) < 0)
      DieWithError("recv() failed");
   
   // Determine if GET or HEAD request
   // GET request
   if((strncmp(echoBuffer, "GET", 3) == 0) || (strncmp(echoBuffer, "HEAD", 4) == 0))
   {
      // Get file path
      char *slash = strstr(echoBuffer, "/") + 1;
      char *space = strstr(slash, " ");      
      memcpy(path, slash, (space-slash));
      path[(space-slash) + 1] = '\0';
      strcat(directory, path);
      printf("Filepath: %s\n", directory);
      
      printf("Buffer below:%s\n", echoBuffer);
      close(clntSocket);    /* Close client socket */
      exit(0);
      
      // Determine if file exists at path
      if(access(directory, F_OK) == 0)
      {
         // File exists
         
         
         
         // Check for read permission
         if(access(directory, W_OK) == 0)
         {
            // File has read permission
            
         }
         // No read permission
         else
         {
            // Send 403 message
            
         }
         
      }
      
      
      
   }
   
   // Not supported request
   else
   {
      // 405 request response
      
   }
   
   
   close(clntSocket);    /* Close client socket */
}




