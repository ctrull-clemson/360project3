#include <stdio.h>      // for printf() and fprintf() 
#include <sys/socket.h> // for socket(), connect(), send(), and recv() 
#include <arpa/inet.h>  // for sockaddr_in and inet_addr() 
#include <netdb.h>      // for getHostByName() 
#include <stdlib.h>     // for atoi() and exit() 
#include <string.h>     // for memset() 
#include <unistd.h>     // for close() 

#define RCVBUFSIZE 32   // Size of receive buffer 

void DieWithError(char *errorMessage);  // Error handling function 

int main(int argc, char *argv[])
{
   int sock;                        // Socket descriptor 
   struct sockaddr_in echoServAddr; // Echo server address 
   struct hostent *thehost;         // Hostent from gethostbyname() 
   unsigned short echoServPort;     // Echo server port 
   char *echoString;                // String to send to echo server 
   char *fileName;                  // Name of file the user wants to save results to 
   char *url;                       // URL passed in by parameters 
   char host[64];                   // Host determined from URL passed in by parameters 
   char path[64];                      // Path determined from URL passed in by parameters 
   char echoBuffer[RCVBUFSIZE];     // Buffer for echo string 
   unsigned int echoStringLen;      // Length of string to echo 
   int bytesRcvd, totalBytesRcvd;   // Bytes read in single recv() 
   int hostStart = 0, hostEnd = 1, pathLength = 0;
   int i;
   
   if (((argc % 2) != 0) || !(argc <= 6 ))    // Test for correct number of arguments 
   {
      fprintf(stderr, "Incorrect number of arguments passed in. Received %d\n", argc);
      exit(1);
   }
   
   // Take in URL from command line arguments
   url = argv[1];
   
   // Argument reader
   for(i = 0; i < argc; i++)
   {
      // Flag for what parameter the next value is.
      if ((i == 2) || (i == 4))
      {
         // First arg:  Port 
         if(strcmp(argv[i], "-p") == 0)
         { 
            echoServPort = atoi(argv[i+1]); 
         }

         // Second arg: File name  
         else if(strcmp(argv[i], "-O") == 0)
         { 
            fileName = argv[i+1]; 
         }

         // Incorrect flag was passed in. 
         else
         {
            fprintf(stderr,"Incorrect parameter; Number %d.\n", i);
            exit(1);        
         }        
      }
   }
   
   // Parse URL string to get host and path
   // If path has http:// then ignore that and continue
   if(strncmp (url,"http://", 7) == 0)
   {
      hostStart = 7;
   }
   
   // Find the end of the host/
   for(i = hostStart; i < (strlen(url) + 1); i++)
   {
      if(url[i] == '/' || url[i] == '\0')
      {
         hostEnd = i;
         i = strlen(url);
      }      
   }
   
   // Move host string into its own variable
   memcpy(host, &url[hostStart], (hostEnd - hostStart));
   host[(hostEnd - hostStart)] = '\0';
   
   // Verify host is correct in terminal
   printf("Host: %s\n", host);
   
   // Move path string into its own variable
   memcpy(path, &url[hostEnd], (strlen(url) - (hostEnd + hostStart)));
   path[(strlen(url) - hostEnd)] = '\0';
   
   // Verify host is correct in terminal
   printf("Path: %s\n", path);
   
   return 0;
   
   
   
   
   
   
   
   
   
   
   ///////////////// HERE AFTER IS OLD CODE /////////////////
/*
   if (argc == 4)
      echoServPort = atoi(argv[3]); // Use given port, if any 
   else
      echoServPort = 7;  // 7 is the well-known port for the echo service 

   // Create a reliable, stream socket using TCP 
   if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
      DieWithError("socket() failed");

   // Construct the server address structure 
   memset(&echoServAddr, 0, sizeof(echoServAddr));     // Zero out structure 
   echoServAddr.sin_family      = AF_INET;             // Internet address family 
   echoServAddr.sin_addr.s_addr = inet_addr(servIP);   // Server IP address 
   echoServAddr.sin_port        = htons(echoServPort); // Server port 


   // If user gave a dotted decimal address, we need to resolve it  
   if (echoServAddr.sin_addr.s_addr == -1) 
   {
      thehost = gethostbyname(servIP);
      echoServAddr.sin_addr.s_addr = *((unsigned long *) thehost->h_addr_list[0]);
   }

   // Establish the connection to the echo server 
   if (connect(sock, (struct sockaddr *) &echoServAddr, sizeof(echoServAddr)) < 0)
      DieWithError("connect() failed");

   echoStringLen = strlen(echoString);          // Determine input length 

   // Send the string to the server 
   if (send(sock, echoString, echoStringLen, 0) != echoStringLen)
      DieWithError("send() sent a different number of bytes than expected");

   // Receive the same string back from the server
   totalBytesRcvd = 0;
   printf("Received: ");                // Setup to print the echoed string 
   while (totalBytesRcvd < echoStringLen)
   {
      // Receive up to the buffer size (minus 1 to leave space for
      a null terminator) bytes from the sender 
      if ((bytesRcvd = recv(sock, echoBuffer, RCVBUFSIZE - 1, 0)) <= 0)
         DieWithError("recv() failed or connection closed prematurely");
         
      totalBytesRcvd += bytesRcvd;   // Keep tally of total bytes 
      echoBuffer[bytesRcvd] = '\0';  // Terminate the string! 
      printf(echoBuffer);            // Print the echo buffer 
   }

   printf("\n");    // Print a final linefeed 

   close(sock);
*/
   exit(0);
}
