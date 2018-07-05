//  según la versión de gcc y opciones de optimización usadas, tal vez haga falta
//  usar gcc –fno-omit-frame-pointer si gcc quitara el marco pila (%ebp)

#include <stdio.h>	// para printf()
#include <stdlib.h>	// para exit()
#include <sys/time.h>	// para gettimeofday(), struct timeval
#include <limits.h>   // max para random


#define SIZE (1<<24)    // tamaño suficiente para tiempo apreciable
unsigned int lista[SIZE];

/*--------------------------------------* /
    #define SIZE 4  // tamaño suficiente para tiempo apreciable
    unsigned int lista[SIZE]={0x80000000,0x00100000,0x00000800,0x00000001};
    #define RESULT 4
/*--------------------------------------* /
    #define SIZE 8  // tamaño suficiente para tiempo apreciable
    unsigned int lista[SIZE]={0x7FFFFFFF,0xFFEFFFFF,0xFFFFF7FF,0xFFFFFFFE,0x010000024,0x00356700,0x890AC00,0x00BD00EF};
    #define RESULT 156
/*--------------------------------------* /
    #define SIZE 8  // tamaño suficiente para tiempo apreciable
    unsigned int lista[SIZE]={0x0,0x10204080,0x3590AC06,0x70B0D0E0,0xFFFFFFFF,0x12345678,0x9ABCDEF0,0xCAFEBEEF}; 
    #define RESULT 116
/*--------------------------------------*/


int resultado=0;

int popcount1(int* array, int len){
    int  i,j,   res=0;
    for (i=0; i<len; i++){
        unsigned int x=array[i];
        for(j=0;j<8*sizeof(x);j++){
            res+=(x&0x1)!=0;
            x>>=1;
        }
    }
    return res;
}

int popcount2(int* array, int len){
    int  i,j,   res=0;
    for (i=0; i<len; i++){
        unsigned int x=array[i]; //¿Porque necesito el unsigned?
        do{
            res+=(x&0x1);
            x>>=1;
        }while(x);
    }
    return res;
}


int popcount3(int* array, int len){
    int  i,j,   res=0;
    for (i=0; i<len; i++){
        unsigned int x=array[i]; //¿Porque necesito el unsigned?
        asm("\n"
            "ini3:              \n\t"
            "shr $0x1,%[x]      \n\t"
            "adc $0x0,%[r]      \n\t"
            "test %[x],%[x]     \n\t"
            "jne ini3"
            :[r]"+r"(res)       //e/:   añadir a lo acumulado
            :[x]"r"(x)          //entrada: valor elemento
        );
    }
    return res;
}


int popcount4(int* array, int len){
    unsigned  i,j,   res=0,val=0;
    for (i=0; i<len; i++){
        unsigned int x=array[i];
        val=0;
        for(j=0;j<8;j++){
            val+= x & 0x01010101;
            x>>=1;
        }
        val+=(val>>16);
        val+=(val>>8);
        res+=val&0xFF;  
    }

return res;
}

int popcount5(int* array, int len){

    int  i,j,   res=0,val=0;
    int SSE_mask[] = { 0x0f0f0f0f, 0x0f0f0f0f, 0x0f0f0f0f, 0x0f0f0f0f };
    int SSE_LUTb[] = { 0x02010100, 0x03020201, 0x03020201, 0x04030302 };

    for (i = 0; i < len; i += 4) {
            asm("movdqu    %[x], %%xmm0 \n\t"
                "movdqa  %%xmm0, %%xmm1 \n\t" // dos copias de x
                "movdqu    %[m], %%xmm6 \n\t"// máscara
                "psrlw       $4, %%xmm1 \n\t"
                "pand    %%xmm6, %%xmm0 \n\t"//; xmm0 – nibbles inferiores
                "pand    %%xmm6, %%xmm1 \n\t"//; xmm1 – nibbles superiores

                "movdqu    %[l], %%xmm2 \n\t"//; ...como pshufb sobrescribe LUT
                "movdqa  %%xmm2, %%xmm3 \n\t"//; ...queremos 2 copias
                "pshufb  %%xmm0, %%xmm2 \n\t"//; xmm2 = vector popcount inferiores
                "pshufb  %%xmm1, %%xmm3 \n\t"//; xmm3 = vector popcount superiores

                "paddb   %%xmm2, %%xmm3 \n\t"//; xmm3 - vector popcount bytes
                "pxor    %%xmm0, %%xmm0 \n\t"//; xmm0 = 0,0,0,0
                "psadbw  %%xmm0, %%xmm3 \n\t"//;xmm3 = [pcnt bytes0..7|pcnt bytes8..15]
                "movhlps %%xmm3, %%xmm0 \n\t"//;xmm3 = [             0           |pc bytes0..7 ]
                "paddd   %%xmm3, %%xmm0 \n\t"//;xmm0 = [         no usado        |pc bytes0..15]
                "movd    %%xmm0, %[val] \n\t"
                : [val]"=r" (val)
                : [x] "m" (array[i]),
                [m] "m" (SSE_mask[0]),
                [l] "m" (SSE_LUTb[0])
            );
            res += val;
    }
    return res;
}

int popcount6 (unsigned* array, int len){
    unsigned x;
    int i,val, res = 0;

    for (i=0; i<len; i++){
        x = array[i];
        asm(
        "popcnt %[x],%[val]"
        : [val] "=r" (val)
        : [x]  "r" (x)
        );
        res += val;
    }
return res;
}

int popcount7 (unsigned* array, int len){
    unsigned x1, x2;
    int i,val, res = 0;
    
    if(len & 0x1)
        printf("leer 64b y len impar?\n");

    for (i=0; i<len; i+=2){
        x1 = array[i];
        x2 = array[i+1];

        asm(
        "popcnt %[x1], %[val] \n\t"
        "popcnt %[x2], %%edi  \n\t"
        "add    %%edi, %[val] \n\t"
        : [val] "=&r" (val)
        : [x1]    "r" (x1),
          [x2]    "r" (x2)
        : "edi"
        );

        res += val;
    }
return res;
}


void crono(int (*func)(), char* msg){
    struct timeval tv1,tv2;	// gettimeofday() secs-usecs
    long           tv_usecs;	// y sus cuentas

    gettimeofday(&tv1,NULL);
    resultado = func(lista, SIZE);
    gettimeofday(&tv2,NULL);

    tv_usecs=(tv2.tv_sec -tv1.tv_sec )*1E6+
             (tv2.tv_usec-tv1.tv_usec);
    //printf("resultado = %d\t", resultado);
    //printf("%s:%9ld us\n", msg, tv_usecs);
    printf("%9ld \t", tv_usecs);
}

int main(){

    int i;          // inicializar array
    for (i=0; i<SIZE; i++){
     lista[i]=rand()*(UINT_MAX/RAND_MAX);
    }

    crono(popcount1, "popcount1 (for en lenguaje C      )");
    crono(popcount2, "popcount2 (while en lenguaje C    )");
    crono(popcount3, "popcount3 (bucle while en asm     )");
    crono(popcount4, "popcount4 (problema del libro     )");
    crono(popcount5, "popcount5 (SSSE3 peso Hamming     )");
    crono(popcount6, "popcount6 (Versión SSE4. 2 (popcount)");
    crono(popcount7, "popcount7 (popcount 64bit )");
    //printf("N*(N+1)/2 = %d\n", (SIZE-1)*(SIZE/2)); /*OF*/
    printf("\n"); /*OF*/

    exit(0);
}
