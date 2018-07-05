gcc suma_04_SC_c.c -m32 -g -c -O1 -fno-omit-frame-pointer
as --32 -g suma_04_SC_s.s -o suma_04_SC_s.o
ld -m elf_i386 suma_04_SC_c.o suma_04_SC_s.o -o suma_04_SC \-lc -dynamic-linker /lib/ld-linux.so.2
./suma_04_SC
