#include <stdio.h>      // for printf() and fprintf() 
#include <sys/socket.h> // for socket(), connect(), send(), and recv() 
#include <arpa/inet.h>  // for sockaddr_in and inet_addr() 
#include <netdb.h>      // for getHostByName() 
#include <stdlib.h>     // for atoi() and exit() 
#include <string.h>     // for memset() 
#include <unistd.h>     // for close() 

#define RCVBUFSIZE 1024  // Size of receive buffer 

void DieWithError(char *errorMessage);  // Error handling function 

int main(int argc, char *argv[])
{
   int sock;                        // Socket descriptor 
   struct sockaddr_in echoServAddr; // Echo server address 
   struct hostent *thehost;         // Hostent from gethostbyname() 
   unsigned short echoServPort;     // Echo server port 
   char *fileName = NULL;           // Name of file the user wants to save results to 
   char *url;                       // URL passed in by parameters 
   char host[64];                   // Host determined from URL passed in by parameters 
   char path[64] = "/";             // Path determined from URL passed in by parameters 
   char getHeader[RCVBUFSIZE];      // Buffer for echo string 
   char httpResponse[RCVBUFSIZE];   // Buffer for echo string 
   unsigned int echoStringLen;      // Length of string to echo 
   int bytesRcvd;                   // Bytes read in single recv() 
   int hostStart = 0, hostEnd = 1;
   FILE* file = NULL;
   int i; 
   
   echoServPort = 8080;
      
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
     
   
   // Move host and path string into their own variables
   if(hostEnd != strlen(url))
   {
      memcpy(path, &url[hostEnd], (strlen(url) - hostEnd));
      path[(strlen(url) - hostEnd)] = '\0';
   }
   memcpy(host, &url[hostStart], (hostEnd - hostStart));
   host[(hostEnd - hostStart)] = '\0';
   
   // Create get header
   int index = 0;
   
   // First row
   strcpy(&getHeader[index], "GET ");
   index += strlen("GET ");
   strcpy(&getHeader[index], path);
   index += (strlen(path));
   strcpy(&getHeader[index], " ");
   index += (strlen(" "));
   strcpy(&getHeader[index], "HTTP/1.1\r\n");
   index += strlen("HTTP/1.1\r\n");
   
   // Second row
   strcpy(&getHeader[index], "User-Agent: Wget/1.14 (darwin 12.2.1) \r\n");
   index += strlen("User-Agent: Wget/1.14 (darwin 12.2.1) \r\n");
   
   // Third row
   strcpy(&getHeader[index], "Accept: */*\r\n");
   index += strlen("Accept: */*\r\n");
   
   // Fourth row
   strcpy(&getHeader[index], "Host: ");
   index += strlen("Host: ");
   strcpy(&getHeader[index], host);
   index += strlen(host);
   strcpy(&getHeader[index], "\r\n");
   index += strlen("\r\n");
   
   // Fifth row
   strcpy(&getHeader[index], "Connection: Keep-Alive\r\n");
   index += strlen("Connection: Keep-Alive\r\n");
   
   // Sixth row
   strcpy(&getHeader[index], "\r\n");
   index += strlen("\r\n");
   getHeader[index] = '\0';
   
   printf("%s", getHeader);
   
   ///////////////// HERE AFTER IS OLD CODE /////////////////

   // Create a reliable, stream socket using TCP 
   if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
      DieWithError("socket() failed");

   // Construct the server address structure 
   memset(&echoServAddr, 0, sizeof(echoServAddr));     // Zero out structure 
   echoServAddr.sin_family      = AF_INET;             // Internet address family 
   echoServAddr.sin_addr.s_addr = inet_addr(host);     // Host name
   echoServAddr.sin_port        = htons(echoServPort); // Server port 

   // If user gave a dotted decimal address, we need to resolve it  
   thehost = gethostbyname(host);
   echoServAddr.sin_addr.s_addr = *((unsigned long *) thehost->h_addr_list[0]);

   // Establish the connection to the echo server 
   if (connect(sock, (struct sockaddr *) &echoServAddr, sizeof(echoServAddr)) < 0)
      DieWithError("connect() failed");

   echoStringLen = strlen(getHeader);          // Determine input length 

   // Send the string to the server 
   if (send(sock, getHeader, echoStringLen, 0) != echoStringLen)
      DieWithError("send() sent a different number of bytes than expected");
   
   if(fileName != NULL)
   {
      printf("Filename: %s\n", fileName);
      file = fopen(fileName,  "w");
      fprintf(file, "%d", 1);
   }
   
   bytesRcvd = 1;   
   while(bytesRcvd != 0)
   {
      bytesRcvd = recv(sock, httpResponse, RCVBUFSIZE - 1, 0);
      //printf("%d\n", bytesRcvd);
      if(fileName != NULL)
      {
         fprintf(file, "%s", httpResponse);
      }
      else
      {
         printf("%s", httpResponse);
         fflush(stdout);      
      }
   }
   
   close(sock);
   exit(0);
}






