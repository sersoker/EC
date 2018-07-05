//  según la versión de gcc y opciones de optimización usadas, tal vez haga falta
//  usar gcc –fno-omit-frame-pointer si gcc quitara el marco pila (%ebp)
// Inicializar los numeros a rand()*UNIN_MAX/RAND_MAX
#include <stdio.h>	// para printf()
#include <stdlib.h>	// para exit()
#include <sys/time.h>   // para gettimeofday(), struct timeval
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

int parity1(int* array, int len){
    int  i,j,   res=0;
    for (i=0; i<len; i++){
        unsigned int x=array[i];
        unsigned int var=0;
        for(j=0;j<8*sizeof(x);j++){
            var^=(x&0x1)!=0;
            x>>=1;
        }
        res+=var;
    }
    return res;
}

int parity2(int* array, int len){
    int  i,j,   res=0;
    for (i=0; i<len; i++){
        unsigned int x=array[i]; //¿Porque necesito el unsigned?
        unsigned int var=0;
        do{
            var^=(x&0x1)!=0;
            x>>=1;
        }while(x);
        res+=var;        
    }
    return res;
}


int parity3(int* array, int len){
    unsigned  i,   res=0;
    for (i=0; i<len; i++){
        unsigned int x=array[i]; //¿Porque necesito el unsigned?
        unsigned int val=0;
        while(x){
            val^= x;
            x>>=1;
        }
    res+=val&0x1;
    }
    return res;
}


int parity4(int* array, int len){
    unsigned  i,   res=0;
    for (i=0; i<len; i++){
        unsigned int x=array[i]; //¿Porque necesito el unsigned?
        unsigned int val=0;
        asm("\n"
            "ini3:              \n\t"
            "xor %[x],%[r]      \n\t"
            "shr $1,%[x]      \n\t"
            "test %[x],%[x]     \n\t"
            "jne ini3"
            :[r]"+r"(val)       //e/:   añadir a lo acumulado
            :[x]"r"(x)          //entrada: valor elemento
        );
    res+=val&0x1;
    }
    return res;
}

int parity5(int* array, int len){
    unsigned  i,j,   res=0;
    for (i=0; i<len; i++){
        unsigned int val=array[i];
        for(j=16;j>=1;j=j/2)
            val^=val>>j;

        res+=val&0x1;
    }
    return res;
}


int parity6(int* array, int len){
    unsigned  i,j,   res=0;
    for (i=0; i<len; i++){
        unsigned int val=array[i];
        asm(
	    "mov %[x],%%edx	\n\t"	
	    "shr $16,%%edx	\n\t"
	    "xor %[x],%%edx	\n\t"	
	    "xor %%dh,%%dl	\n\t"	
	    "setpo %%dl		\n\t"
            "movzbl   %%dl,%[x] \n\t"
            :[x]"+r"(val)
            :
            : "edx"
        );

        res+=val;
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
    printf("resultado = %d\t", resultado);
    printf("%s:%9ld us\n", msg, tv_usecs);
    //printf("%9ld \t", tv_usecs);
}

int main(){

    int i;          // inicializar array
    for (i=0; i<SIZE; i++){
     lista[i]=rand()*(UINT_MAX/RAND_MAX);
    }


    crono(parity1, "parity1 (for en lenguaje C      )");
    crono(parity2, "parity2 (while en lenguaje C    )");
    crono(parity3, "parity3 (problema del libro     )");
    crono(parity4, "parity4 (while en asm           )");
    crono(parity5, "parity5 (modificación de popcount4)");
    crono(parity6, "parity6 (traduccion a asm del bucle for)");
    //printf("N*(N+1)/2 = %d\n", (SIZE-1)*(SIZE/2)); /*OF*/
    printf("\n"); /*OF*/

    exit(0);
}
