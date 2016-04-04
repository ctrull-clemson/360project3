#include <stdio.h>      /* for printf() and fprintf() */
#include <sys/socket.h> /* for recv() and send() */
#include <string.h>
#include <stdlib.h>
#include <unistd.h>     /* for close() */
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>

#define RCVBUFSIZE 4096   /* Size of receive buffer */
void serverPrintOut(char queryType[], char path[], char timing[], int responseType);
void DieWithError(char *errorMessage);  /* Error handling function */

void HandleTCPClient(int clntSocket, char directory[])
{
   char echoBuffer[RCVBUFSIZE];        /* Buffer for echo string */
   int recvMsgSize;                    /* Size of received message */
   char path[128];                     /* Path of wanted file from request */
   char returnBuffer[RCVBUFSIZE] = ""; /* Buffer to store return information */
   int index = 0;
   char timing[128];
   
   strcat(returnBuffer, "HTTP/1.1 ");
   index = strlen("HTTP/1.1 ");
      
   
   /* Receive message from client */
   if((recvMsgSize = recv(clntSocket, echoBuffer, RCVBUFSIZE, 0)) < 0)
      DieWithError("recv() failed");
   
   
   // Determine if GET or HEAD request
   if((strncmp(echoBuffer, "GET", 3) == 0) || (strncmp(echoBuffer, "HEAD", 4) == 0))
   {
      // Get file path
      char *slash = strstr(echoBuffer, "/") + 1;
      
      if(slash == NULL)
      {
         strcpy(&returnBuffer[index], "400 Bad Request\r\n\r\n");
         index += strlen("400 Bad Request\r\n\r\n");   
         returnBuffer[index] = '\0';
         
         if (send(clntSocket, returnBuffer, strlen(returnBuffer), 0) <= 0)
            DieWithError("send() failed to send");
         
         close(clntSocket);    /* Close client socket */
         exit(0);
      }
      char *space = strstr(slash, " "); 
      
      if(space == NULL)
      {
         strcpy(&returnBuffer[index], "400 Bad Request\r\n\r\n");
         index += strlen("400 Bad Request\r\n\r\n");   
         returnBuffer[index] = '\0';
         
         if (send(clntSocket, returnBuffer, strlen(returnBuffer), 0) <= 0)
            DieWithError("send() failed to send");
         
         close(clntSocket);    /* Close client socket */
         exit(0);
      }
      
      memcpy(path, slash, (space-slash));
      path[(space-slash) + 1] = '\0';
      
      if(strcmp(path, "") == 0)
         { strcpy(path, "index.html"); }      
      strcat(directory, path);    
      
      // Determine if file exists at path
      if(access(directory, F_OK) == 0)
      {
         // File exists
         struct stat attrib;
         stat(directory, &attrib);
         char fileDate[100];
         strftime(fileDate, 100, "Last-Modified: %a, %d %b %Y %H:%M:%S", localtime(&(attrib.st_ctime)));
         strftime(timing, 100, "%d %b %Y %H:%M", localtime(&(attrib.st_ctime)));
         
         // Check for read permission
         if(access(directory, W_OK) == 0)
         {
            // File has read permission
            // Send 200 message
            strcpy(&returnBuffer[index], "200 OK\r\n");
            index += strlen("200 OK\r\n");  
            strcpy(&returnBuffer[index], fileDate);
            index += strlen(fileDate);
            strcpy(&returnBuffer[index], "\r\n");
            index += strlen("\r\n"); 
            
            // Server printout
            if(strncmp(echoBuffer, "GET", 3) == 0)
            { 
               serverPrintOut("GET", directory, timing, 200); 
               
               strcpy(&returnBuffer[index], "Content -Type:\t");
               index += strlen("Content -Type:\t"); 
            
               // Content length stuff
               char *fileType = strstr(path, ".");
               if(strstr(fileType, ".css") != NULL)
               {               
                  // text/css
                  strcpy(&returnBuffer[index], "text/css");
                  index += strlen("text/css");
               }
               else if((strstr(fileType, ".html") != NULL) || (strstr(fileType, ".htm") != NULL))
               {
                  // text/html
                  strcpy(&returnBuffer[index], "text/html");
                  index += strlen("text/html");
               }
               else if(strstr(fileType, ".js") != NULL)
               {
                  // application/javascript
                  strcpy(&returnBuffer[index], "application/javascript");
                  index += strlen("application/javascript");
               }
               else if(strstr(fileType, ".txt") != NULL)
               {
                  // text/plain
                  strcpy(&returnBuffer[index], "text/plain");
                  index += strlen("text/plain");
               }
               else if(strstr(fileType, ".jpg") != NULL)
               {
                  // image/jpeg
                  strcpy(&returnBuffer[index], "image/jpeg");
                  index += strlen("image/jpeg");
               }
               else if(strstr(fileType, ".pdf") != NULL)
               {
                  // application/pdf
                  strcpy(&returnBuffer[index], "application/pdf");
                  index += strlen("application/pdf");
               }
               else
               {
                  // application/octet-stream
                  strcpy(&returnBuffer[index], "application/octet-stream");
                  index += strlen("application/octet-stream");
               }
               
               strcpy(&returnBuffer[index], "application/octet-stream");
               index += strlen("application/octet-stream");
               
               strcpy(&returnBuffer[index], "\r\n");
               index += strlen("\r\n");
            }
            else
               { serverPrintOut("HEAD", directory, timing, 200); }
               
            
            strcpy(&returnBuffer[index], "Server: simhttpServer/1.1\r\n");
            index += strlen("Server: simhttpServer/1.1\r\n");  
            
            strcpy(&returnBuffer[index], "Server: simhttpServer/1.1\r\n");
            index += strlen("Server: simhttpServer/1.1\r\n");  
            
            strcpy(&returnBuffer[index], "\r\n");
            index += strlen("\r\n");
            returnBuffer[index] = '\0';
         }
         
         // No read permission
         else
         {
            // Send 403 message
            strcpy(&returnBuffer[index], "403 Forbidden\r\n\r\n");
            index += strlen("403 Forbidden\r\n\r\n");  
            returnBuffer[index] = '\0';
            
            // Server printout
            if(strncmp(echoBuffer, "GET", 3) == 0)
               { serverPrintOut("GET ", directory, timing, 403); }
            else
               { serverPrintOut("HEAD ", directory, timing, 403); }
         }         
      }
      
      // File does not exist
      else
      {
         // 404 request response
         strcpy(&returnBuffer[index], "404 Not Found\r\n\r\n");
         index += strlen("404 Not Found\r\n\r\n");    
         returnBuffer[index] = '\0';
      
         // Server printout
         if(strncmp(echoBuffer, "GET", 3) == 0)
            { serverPrintOut("GET ", directory, "", 404); }
         else
            { serverPrintOut("HEAD ", directory, "", 404); }
        
      }
   }   
   
   // Not supported request
   else
   {
      // 405 request response
      strcpy(&returnBuffer[index], "405 Method Not Allowed\r\n\r\n");
      index += strlen("405 Method Not Allowed\r\n\r\n");   
      returnBuffer[index] = '\0';
   }
   
   //printf("Sending response:\n%s", returnBuffer);
   
   // Send the string to the client 
   if (send(clntSocket, returnBuffer, strlen(returnBuffer), 0) <= 0)
      DieWithError("send() failed to send");
   
   close(clntSocket);    /* Close client socket */
   exit(0);
}

void serverPrintOut(char queryType[], char path[], char timing[], int responseType)
{
   printf("%s\t%s\t%s\t%d\n", queryType, path + 2, timing, responseType);
   
   return;
}


