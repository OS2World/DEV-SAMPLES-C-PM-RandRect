# nmake makefile
#
# Tools used:
#  Compile::Resource Compiler
#  Compile::GNU C
#  Make: nmake or GNU make
all : randrect.exe

randrect.exe : randrect.obj randrect.def
	gcc -Zomf randrect.obj randrect.def -o randrect.exe
	
randrect.obj : randrect.c
	gcc -Wall -Zomf -c -O2 randrect.c -o randrect.obj

clean :
	rm -rf *exe *RES *obj