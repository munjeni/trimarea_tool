CFLAGS = -Wall -O2 -static
CC=gcc
STRIP=strip

CCWIN=i686-w64-mingw32-gcc
CCWINSTRIP=i686-w64-mingw32-strip
WINDRES=i686-w64-mingw32-windres

default:trimarea_tool trimarea_tool.exe

trimarea_tool: trimarea_tool.o trimarea_reconstruct.o

	${CC} ${CFLAGS} trimarea_tool.o -o trimarea_tool
	${CC} ${CFLAGS} trimarea_reconstruct.o -o trimarea_reconstruct
	${STRIP} trimarea_tool
	${STRIP} trimarea_reconstruct

trimarea_tool.exe:

	${CCWIN} -c trimarea_tool.c -o trimarea_tool.o
	${CCWIN} -c trimarea_reconstruct.c -o trimarea_reconstruct.o
	${WINDRES} trimarea_tool.rc -O coff -o trimarea_tool.res
	${WINDRES} trimarea_reconstruct.rc -O coff -o trimarea_reconstruct.res
	${CCWIN} trimarea_tool.o trimarea_tool.res -o trimarea_tool.exe
	${CCWIN} trimarea_reconstruct.o trimarea_reconstruct.res -o trimarea_reconstruct.exe
	${CCWINSTRIP} trimarea_tool.exe
	${CCWINSTRIP} trimarea_reconstruct.exe

clean:
	rm -f *.o *.res

distclean:
	rm -f *.o *.res trimarea_tool trimarea_tool.exe trimarea_reconstruct trimarea_reconstruct.exe

