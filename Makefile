#You may wish to add -DNO_UNISTD and -DNO_SYSTEM to CFLAGS if
#your system lacks the /usr/include/unistd.h header file
#or the system() function, respectively. Non-unix systems
#will also definitely have different linker syntax.

CFLAGS=
CC=gcc
AR=ar
LIBS=-L./ -lcgic

all: libcgic.a pref out tradecnt.cgi index

libcgic.a: cgic.o cgic.h
	rm -f libcgic.a
	$(AR) rc libcgic.a cgic.o

pref: pref.o libcgic.a
	gcc pref.o -o pref.cgi ${LIBS}

out: out.o libcgic.a
	gcc out.o -o out.cgi ${LIBS}

index: index.o
	gcc index.o -o index.cgi

tradecnt.cgi: tradecnt.cgi.o
	gcc tradecnt.cgi.o -o tradecnt.cgi
