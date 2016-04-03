---------------------------------------------------------------------------------------
Author:  Christian Trull 

Package: ctrull-hw3.tar.gz

Known Problems:
   TCPEchoClient.c:
      When printing to terminal there is a chance that, on the last packet of data,
      the print statement segfaults. When printing to file this problem doesnt occur.
      When receving the last packet of data the connection usually hangs from a few
      seconds to minutes depending on the site. If an incorrect url or IP address is
      passed in, the program will segfault after the gethostbyname() call.
   
   TCPEchoServer.c:
      
   
   HandleTCPClient.c:
      a
   
Design:
   TCPEchoClient.c:
      This file compiles to simget and takes in input parameters of the target url/IP
      address, an optional file name, and an optional port number. The client uses 
      gethostbyname() to determine how to connect from the input address. If inputs 
      are passed in correctly, the client will create an HTTP get request and send it 
      to the target server. If the request is valid, the server will respond with an 
      appropriate HTTP response. If a file name was passed in the response is written 
      into the file, else it is printed to stdout.
   
   TCPEchoServer.c:
      The HTTP server takes in an optional filepath and an optional port number. The
      server opens a connection on the specified or default port and waits indefinitely
      for a client to connect and send an HTTP request.
   
   HandleTCPClient.c:
      This file deals with listening to sent HTTP requests, interpretting the request,
      and responding with a proper HTTP response, based on if the request was valid, 
      the host header is supported, and other parameters. The parsing checks for HTTP
      response numbers 200, 400, 403, 404, and 405. 
   
   
---------------------------------------------------------------------------------------
