echo "O0 popcount" >>Salida.txt
gcc -m32 popcount.c -o popcount -fno-omit-frame-pointer -O0
for ((i=0;i<11;i++)) do ./popcount;done >>Salida.txt
echo "O1 popcount">>Salida.txt
gcc -m32 popcount.c -o popcount -fno-omit-frame-pointer -O1
for ((i=0;i<11;i++)) do ./popcount;done >>Salida.txt
echo "O2 popcount">>Salida.txt
gcc -m32 popcount.c -o popcount -fno-omit-frame-pointer -O2
for ((i=0;i<11;i++)) do ./popcount;done >>Salida.txt
echo "O3 popcount">>Salida.txt
gcc -m32 popcount.c -o popcount -fno-omit-frame-pointer -O3
for ((i=0;i<11;i++)) do ./popcount;done >>Salida.txt
#gcc -m32 popcount.c -o popcount -fno-omit-frame-pointer -O3
#./popcount
