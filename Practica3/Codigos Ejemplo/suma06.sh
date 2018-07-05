gcc -m32 -O1 -g -c suma_06_CS_c.c 
as --32 -g suma_06_CS_s.s -o suma_06_CS_s.o
gcc -m32 suma_06_CS_c.o suma_06_CS_s.o -o suma_06_CS

