/****************************************************************************************************************
*
* Author name: Christian Trull
*
* Module:      HTTP request interpretation source code
*
* File Name:   HandleTCPClient.c
*
* Summary:
*  This file contains the code that listens to client requests and then interprets and responds to them, following
*     the HTTP 1.1 specifications. The code first checks to see if the request is  GET or HEAD request, else 
*     sends either a 400 or 405 message. When a GET or HEAD request is sent, the code pulls out the file that
*     is requested, check to see if the file 1) exists, and 2) is readable. If anything goes wrong here, an
*     HTTP message is sent back with the proper code. If the file does exist and is readable, then the code 
*     adds the necessary information to the Host Header. 
*
*     Due to problems, this code does not add the data section to GET requests, segfaults on some misformed HTTP
*     requests, and sometimes mistakes 405 and 400 errors. 
*
****************************************************************************************************************/

#include <stdio.h>      /* for printf() and fprintf() */
#include <sys/socket.h> /* for recv() and send() */
#include <string.h>
#include <stdlib.h>
#include <unistd.h>     /* for close() */
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>

#define RCVBUFSIZE 500000   /* Size of receive buffer */
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
   
   // Not supported, valid headers. 405 message response
   if((strncmp(echoBuffer, "OPTIONS", 7) == 0) || (strncmp(echoBuffer, "POST", 4) == 0)
         || (strncmp(echoBuffer, "PUT", 3) == 0) || (strncmp(echoBuffer, "DELETE", 6) == 0)
         || (strncmp(echoBuffer, "TRACE", 5) == 0) || (strncmp(echoBuffer, "CONNECT", 7) == 0))
   {
      // 405 request response
      strcpy(&returnBuffer[index], "405 Method Not Allowed\r\n\r\n");
      index += strlen("405 Method Not Allowed\r\n\r\n");   
      returnBuffer[index] = '\0';      
   }
   
   // Determine if GET or HEAD request
   else if((strncmp(echoBuffer, "GET", 3) == 0) || (strncmp(echoBuffer, "HEAD", 4) == 0))
   {
      char *httpCheck = strstr(echoBuffer, "HTTP/1.1");
      if(httpCheck == NULL)
      {
         strcpy(&returnBuffer[index], "400 Bad Request\r\n\r\n");
         index += strlen("400 Bad Request\r\n\r\n");   
         returnBuffer[index] = '\0';
         
         if (send(clntSocket, returnBuffer, strlen(returnBuffer), 0) <= 0)
            DieWithError("send() failed to send");
         
         close(clntSocket);    /* Close client socket */
         return;      
      }
      
      // Get file path
      char *slash = strstr(echoBuffer, "/");      
      if(slash == NULL)
      {
         strcpy(&returnBuffer[index], "400 Bad Request\r\n\r\n");
         index += strlen("400 Bad Request\r\n\r\n");   
         returnBuffer[index] = '\0';
         
         if (send(clntSocket, returnBuffer, strlen(returnBuffer), 0) <= 0)
            DieWithError("send() failed to send");
         
         close(clntSocket);    /* Close client socket */
         return;
      }
      slash++;
      
      char *space = strstr(slash, " ");       
      if(space == NULL)
      {
         strcpy(&returnBuffer[index], "400 Bad Request\r\n\r\n");
         index += strlen("400 Bad Request\r\n\r\n");   
         returnBuffer[index] = '\0';
         
         if (send(clntSocket, returnBuffer, strlen(returnBuffer), 0) <= 0)
            DieWithError("send() failed to send");
         
         close(clntSocket);    /* Close client socket */
         return;
      }
      
      char *changeDirectory = strstr(echoBuffer, "../");
      if(changeDirectory != NULL)
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
      
      // Determine the relative path for the file that was requested by the client
      memcpy(path, slash, (space-slash));
      path[(space-slash) + 1] = '\0';
      
      // If the client did not explicitly state the file, we default to index.html
      if(strcmp(path, "") == 0)
         { strcpy(path, "index.html"); }      
      strcat(directory, path);    
      
      // Determine if file exists at the specified path
      if(access(directory, F_OK) == 0)
      {
         // File exists         
         // Get information about the file. Last Modified date and time
         struct stat attrib;
         stat(directory, &attrib);
         char fileDate[100];
         strftime(fileDate, 100, "Last-Modified: %a, %d %b %Y %H:%M:%S", localtime(&(attrib.st_ctime)));
         strftime(timing, 100, "%d %b %Y %H:%M", localtime(&(attrib.st_ctime)));
         
         // Check for read permission
         if(access(directory, W_OK) == 0)
         {
            // File has read permission
            // Create the header information for a 200 response
            strcpy(&returnBuffer[index], "200 OK\r\n");
            index += strlen("200 OK\r\n");  
            strcpy(&returnBuffer[index], fileDate);
            index += strlen(fileDate);
            strcpy(&returnBuffer[index], "\r\n");
            index += strlen("\r\n"); 
            
            // For GET requests, print out the proper message to server, and determine content type
            if(strncmp(echoBuffer, "GET", 3) == 0)
            { 
               serverPrintOut("GET", directory, timing, 200); 
               
               strcpy(&returnBuffer[index], "Content -Type:\t");
               index += strlen("Content -Type:\t"); 
            
               // Determine content type
               char *fileType = strstr(path, ".");
               if(strstr(fileType, ".css") != NULL)
               {               
                  strcpy(&returnBuffer[index], "text/css");
                  index += strlen("text/css");
               }
               else if((strstr(fileType, ".html") != NULL) || (strstr(fileType, ".htm") != NULL))
               {
                  strcpy(&returnBuffer[index], "text/html");
                  index += strlen("text/html");
               }
               else if(strstr(fileType, ".js") != NULL)
               {
                  strcpy(&returnBuffer[index], "application/javascript");
                  index += strlen("application/javascript");
               }
               else if(strstr(fileType, ".txt") != NULL)
               {
                  strcpy(&returnBuffer[index], "text/plain");
                  index += strlen("text/plain");
               }
               else if(strstr(fileType, ".jpg") != NULL)
               {
                  strcpy(&returnBuffer[index], "image/jpeg");
                  index += strlen("image/jpeg");
               }
               else if(strstr(fileType, ".pdf") != NULL)
               {
                  strcpy(&returnBuffer[index], "application/pdf");
                  index += strlen("application/pdf");
               }
               else
               {
                  strcpy(&returnBuffer[index], "application/octet-stream");
                  index += strlen("application/octet-stream");
               }
               
               strcpy(&returnBuffer[index], "application/octet-stream");
               index += strlen("application/octet-stream");
               
               strcpy(&returnBuffer[index], "\r\n");
               index += strlen("\r\n");
            }
            
            // Was a HEAD request, so just print out to stdout
            else
               { serverPrintOut("HEAD", directory, timing, 200); }
               
            // Add more information to header for the response.            
            strcpy(&returnBuffer[index], "Server: simhttpServer/1.1\r\n");
            index += strlen("Server: simhttpServer/1.1\r\n");  
            
            int character;
            FILE *targetFile = fopen(directory, "r");
            
            fseek(targetFile, 0, SEEK_END);  // seek to end of file
            int contentLength = ftell(targetFile);        // get current file pointer
            fseek(targetFile, 0, SEEK_SET);  // seek back to beginning of file
            char length[15];
            sprintf(length, "%d", contentLength);
            
            // Add content length to resposne
            strcpy(&returnBuffer[index], "Content -Length:\t");
            index += strlen("Content -Length:\t");            
            strcpy(&returnBuffer[index], length);
            index += strlen(length);
            strcpy(&returnBuffer[index], "\r\n");
            index += strlen("\r\n");
            strcpy(&returnBuffer[index], "\r\n");
            index += strlen("\r\n");
            
            int dex = 0;
            char * fileInput = malloc(contentLength * 2);
            while ((character = getc(targetFile)) != EOF)
            {
               fileInput[dex++] = character;
            }
            
            strcpy(&returnBuffer[index], fileInput);
            index += strlen(fileInput);
            strcpy(&returnBuffer[index], "\r\n");
            index += strlen("\r\n");
            
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
}

void serverPrintOut(char queryType[], char path[], char timing[], int responseType)
{
   printf("%s\t%s\t%s\t%d\n", queryType, path + 2, timing, responseType);
   
   return;
}


