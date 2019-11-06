CFLAGS = -Wall -O2 -static
CC=gcc
STRIP=strip

CCWIN=i686-w64-mingw32-gcc
CCWINSTRIP=i686-w64-mingw32-strip
WINDRES=i686-w64-mingw32-windres

default:trimarea_tool trimarea_tool.exe

trimarea_tool: trimarea_tool.o

	${CC} ${CFLAGS} trimarea_tool.o -o trimarea_tool
	${STRIP} trimarea_tool

trimarea_tool.exe:

	${CCWIN} -c trimarea_tool.c -o trimarea_tool.o
	${WINDRES} trimarea_tool.rc -O coff -o trimarea_tool.res
	${CCWIN} trimarea_tool.o trimarea_tool.res -o trimarea_tool.exe
	${CCWINSTRIP} trimarea_tool.exe

clean:
	rm -f *.o *.res

distclean:
	rm -f *.o *.res trimarea_tool trimarea_tool.exe

