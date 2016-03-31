include Make.defines

PROGS =	 TCPclient TCPserver

OPTIONS = -DUNIX  -DUSE_SIGACTION -DANSI


COBJECTS =	DieWithError.o HandleTCPClient.o 
CSOURCES =	DieWithError.c HandleTCPClient.c

CPLUSOBJECTS = 

COMMONSOURCES =

CPLUSSOURCES =

all:	${PROGS}


TCPclient:	TCPEchoClient.o $(CPLUSOBJECTS) $(COBJECTS) $(LIBS) $(COMMONSOURCES) $(SOURCES)
		${CC} $(INCLUDES) ${LINKOPTIONS}  $@ TCPEchoClient.o $(CPLUSOBJECTS) $(COBJECTS) $(LIBS) $(LINKFLAGS)


TCPserver:	TCPEchoServer.o $(CPLUSOBJECTS) $(COBJECTS)
		${CC} $(INCLUDES) ${LINKOPTIONS} $@ TCPEchoServer.o $(CPLUSOBJECTS) $(COBJECTS) $(LIBS) $(LINKFLAGS)


.cc.o:	$(HEADERS)
	$(CPLUS) $(CPLUSFLAGS) $(OPTIONS) $<

.c.o:	$(HEADERS)
	$(CC) $(CFLAGS) $(OPTIONS) $<



backup:
	rm -f tcpecho.tar.gz
	rm -f tcpecho.tar
	tar -cf tcpecho.tar *
	gzip -f tcpecho.tar

clean:
		rm -f ${PROGS} ${CLEANFILES}

depend:
		makedepend TCPEchoClient.c TCPEchoServer.c $(INCLUDES) $(CFLAGS) $(HEADERS) $(SOURCES) $(COMMONSOURCES) $(CSOURCES)
#		mkdep $(CFLAGS) $(HEADERS) $(SOURCES) $(COMMONSOURCES) $(CSOURCES)

# DO NOT DELETE

TCPEchoClient.o: /usr/include/stdio.h /usr/include/features.h
TCPEchoClient.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
TCPEchoClient.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-32.h
TCPEchoClient.o: /usr/include/linux/stddef.h /usr/include/bits/types.h
TCPEchoClient.o: /usr/include/bits/typesizes.h /usr/include/libio.h
TCPEchoClient.o: /usr/include/_G_config.h /usr/include/wchar.h
TCPEchoClient.o: /usr/include/bits/wchar.h /usr/include/gconv.h
TCPEchoClient.o: /usr/lib/gcc/i386-redhat-linux/4.1.1/include/stdarg.h
TCPEchoClient.o: /usr/include/bits/stdio_lim.h
TCPEchoClient.o: /usr/include/bits/sys_errlist.h /usr/include/sys/socket.h
TCPEchoClient.o: /usr/include/sys/uio.h /usr/include/sys/types.h
TCPEchoClient.o: /usr/include/linux/time.h /usr/include/linux/types.h
TCPEchoClient.o: /usr/include/linux/posix_types.h /usr/include/linux/stddef.h
TCPEchoClient.o: /usr/include/asm/posix_types.h /usr/include/asm/types.h
TCPEchoClient.o: /usr/include/endian.h /usr/include/bits/endian.h
TCPEchoClient.o: /usr/include/sys/select.h /usr/include/bits/select.h
TCPEchoClient.o: /usr/include/bits/sigset.h /usr/include/bits/time.h
TCPEchoClient.o: /usr/include/sys/sysmacros.h
TCPEchoClient.o: /usr/include/bits/pthreadtypes.h /usr/include/bits/uio.h
TCPEchoClient.o: /usr/include/bits/socket.h /usr/include/linux/limits.h
TCPEchoClient.o: /usr/include/bits/sockaddr.h /usr/include/asm/socket.h
TCPEchoClient.o: /usr/include/asm/sockios.h /usr/include/arpa/inet.h
TCPEchoClient.o: /usr/include/netinet/in.h /usr/include/stdint.h
TCPEchoClient.o: /usr/include/bits/in.h /usr/include/bits/byteswap.h
TCPEchoClient.o: /usr/include/netdb.h /usr/include/rpc/netdb.h
TCPEchoClient.o: /usr/include/bits/netdb.h /usr/include/stdlib.h
TCPEchoClient.o: /usr/include/alloca.h /usr/include/string.h
TCPEchoClient.o: /usr/include/linux/unistd.h /usr/include/asm/unistd.h
TCPEchoServer.o: /usr/include/stdio.h /usr/include/features.h
TCPEchoServer.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
TCPEchoServer.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-32.h
TCPEchoServer.o: /usr/include/linux/stddef.h /usr/include/bits/types.h
TCPEchoServer.o: /usr/include/bits/typesizes.h /usr/include/libio.h
TCPEchoServer.o: /usr/include/_G_config.h /usr/include/wchar.h
TCPEchoServer.o: /usr/include/bits/wchar.h /usr/include/gconv.h
TCPEchoServer.o: /usr/lib/gcc/i386-redhat-linux/4.1.1/include/stdarg.h
TCPEchoServer.o: /usr/include/bits/stdio_lim.h
TCPEchoServer.o: /usr/include/bits/sys_errlist.h /usr/include/sys/socket.h
TCPEchoServer.o: /usr/include/sys/uio.h /usr/include/sys/types.h
TCPEchoServer.o: /usr/include/linux/time.h /usr/include/linux/types.h
TCPEchoServer.o: /usr/include/linux/posix_types.h /usr/include/linux/stddef.h
TCPEchoServer.o: /usr/include/asm/posix_types.h /usr/include/asm/types.h
TCPEchoServer.o: /usr/include/endian.h /usr/include/bits/endian.h
TCPEchoServer.o: /usr/include/sys/select.h /usr/include/bits/select.h
TCPEchoServer.o: /usr/include/bits/sigset.h /usr/include/bits/time.h
TCPEchoServer.o: /usr/include/sys/sysmacros.h
TCPEchoServer.o: /usr/include/bits/pthreadtypes.h /usr/include/bits/uio.h
TCPEchoServer.o: /usr/include/bits/socket.h /usr/include/linux/limits.h
TCPEchoServer.o: /usr/include/bits/sockaddr.h /usr/include/asm/socket.h
TCPEchoServer.o: /usr/include/asm/sockios.h /usr/include/arpa/inet.h
TCPEchoServer.o: /usr/include/netinet/in.h /usr/include/stdint.h
TCPEchoServer.o: /usr/include/bits/in.h /usr/include/bits/byteswap.h
TCPEchoServer.o: /usr/include/stdlib.h /usr/include/alloca.h
TCPEchoServer.o: /usr/include/string.h /usr/include/linux/unistd.h
TCPEchoServer.o: /usr/include/asm/unistd.h
DieWithError.o: /usr/include/stdio.h /usr/include/features.h
DieWithError.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
DieWithError.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-32.h
DieWithError.o: /usr/include/linux/stddef.h /usr/include/bits/types.h
DieWithError.o: /usr/include/bits/typesizes.h /usr/include/libio.h
DieWithError.o: /usr/include/_G_config.h /usr/include/wchar.h
DieWithError.o: /usr/include/bits/wchar.h /usr/include/gconv.h
DieWithError.o: /usr/lib/gcc/i386-redhat-linux/4.1.1/include/stdarg.h
DieWithError.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
DieWithError.o: /usr/include/stdlib.h /usr/include/sys/types.h
DieWithError.o: /usr/include/linux/time.h /usr/include/linux/types.h
DieWithError.o: /usr/include/linux/posix_types.h /usr/include/linux/stddef.h
DieWithError.o: /usr/include/asm/posix_types.h /usr/include/asm/types.h
DieWithError.o: /usr/include/endian.h /usr/include/bits/endian.h
DieWithError.o: /usr/include/sys/select.h /usr/include/bits/select.h
DieWithError.o: /usr/include/bits/sigset.h /usr/include/bits/time.h
DieWithError.o: /usr/include/sys/sysmacros.h /usr/include/bits/pthreadtypes.h
DieWithError.o: /usr/include/alloca.h
HandleTCPClient.o: /usr/include/stdio.h /usr/include/features.h
HandleTCPClient.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
HandleTCPClient.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-32.h
HandleTCPClient.o: /usr/include/linux/stddef.h /usr/include/bits/types.h
HandleTCPClient.o: /usr/include/bits/typesizes.h /usr/include/libio.h
HandleTCPClient.o: /usr/include/_G_config.h /usr/include/wchar.h
HandleTCPClient.o: /usr/include/bits/wchar.h /usr/include/gconv.h
HandleTCPClient.o: /usr/lib/gcc/i386-redhat-linux/4.1.1/include/stdarg.h
HandleTCPClient.o: /usr/include/bits/stdio_lim.h
HandleTCPClient.o: /usr/include/bits/sys_errlist.h /usr/include/sys/socket.h
HandleTCPClient.o: /usr/include/sys/uio.h /usr/include/sys/types.h
HandleTCPClient.o: /usr/include/linux/time.h /usr/include/linux/types.h
HandleTCPClient.o: /usr/include/linux/posix_types.h
HandleTCPClient.o: /usr/include/linux/stddef.h /usr/include/asm/posix_types.h
HandleTCPClient.o: /usr/include/asm/types.h /usr/include/endian.h
HandleTCPClient.o: /usr/include/bits/endian.h /usr/include/sys/select.h
HandleTCPClient.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
HandleTCPClient.o: /usr/include/bits/time.h /usr/include/sys/sysmacros.h
HandleTCPClient.o: /usr/include/bits/pthreadtypes.h /usr/include/bits/uio.h
HandleTCPClient.o: /usr/include/bits/socket.h /usr/include/linux/limits.h
HandleTCPClient.o: /usr/include/bits/sockaddr.h /usr/include/asm/socket.h
HandleTCPClient.o: /usr/include/asm/sockios.h /usr/include/linux/unistd.h
HandleTCPClient.o: /usr/include/asm/unistd.h
