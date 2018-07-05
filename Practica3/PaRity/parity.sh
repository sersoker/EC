echo "O0 Parity" >>Salida.txt
gcc -m32 parity.c -o parity -fno-omit-frame-pointer -O0
for ((i=0;i<11;i++)) do ./parity;done >>Salida.txt
echo "O1 Parity">>Salida.txt
gcc -m32 parity.c -o parity -fno-omit-frame-pointer -O1
for ((i=0;i<11;i++)) do ./parity;done >>Salida.txt
echo "O2 Parity">>Salida.txt
gcc -m32 parity.c -o parity -fno-omit-frame-pointer -O2
for ((i=0;i<11;i++)) do ./parity;done >>Salida.txt
echo "O3 Parity">>Salida.txt
gcc -m32 parity.c -o parity -fno-omit-frame-pointer -O3
for ((i=0;i<11;i++)) do ./parity;done >>Salida.txt
#gcc -m32 parity.c -o parity -fno-omit-frame-pointer -O3
#./parity
