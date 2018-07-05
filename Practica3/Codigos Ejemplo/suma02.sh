as --32 -g suma_02_S_libC.s -o suma_02_S_libC.o
ld -m elf_i386 suma_02_S_libC.o -o suma_02_S_libC \-lc -dynamic-linker /lib/ld-linux.so.2
./suma_02_S_libC
