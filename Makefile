CFLAGS=-Wall -I/usr/include/curl/
LIBS=-L/usr/local/lib -lcurl

all: libCurl

test: testconnect teststream
	valgrind --leak-check=full ./testconnect
	valgrind --leak-check=full ./teststream

testconnect: Makefile curl.c curl.h addr.h testconnect.c
	gcc ${CFLAGS} -o testconnect testconnect.c -L/usr/local/lib -lCurl

teststream: Makefile curl.c curl.h addr.h teststream.c
	gcc ${CFLAGS} -o teststream teststream.c -L/usr/local/lib -lCurl

libCurl: Makefile curl.o curl.h addr.h
	gcc -shared -W1,-soname,libCurl.so.1 -o libCurl.so.1.0 curl.o ${LIBS}

curl.o: Makefile curl.h curl.c addr.h
	gcc ${CFLAGS} -fPIC -c curl.c -o curl.o

install: all
	cp libCurl.so.1.0 /usr/local/lib
	ln -sf /usr/local/lib/libCurl.so.1.0 /usr/local/lib/libCurl.so.1
	ln -sf /usr/local/lib/libCurl.so.1.0 /usr/local/lib/libCurl.so
	ldconfig /usr/local/lib
	cp curl.h /usr/local/include/Curl.h
	cp cacert.pem /etc

clean:
	rm *.o; rm *.so*; rm testconnect; rm teststream 
