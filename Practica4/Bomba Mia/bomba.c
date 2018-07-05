#include <stdio.h>	// para printf()
#include <stdlib.h>	// para exit()
#include <string.h>	// para strncmp()/strlen()
#include <sys/time.h>	// para gettimeofday(), struct timeval

int password = 1186;

void boom(){
	printf("***************\n");
	printf("*** BOOM!!! ***\n");
	printf("***************\n");
	exit(-1);
}

void defused(){
	printf("*************************\n");
	printf("*** bomba desactivada ***\n");
	printf("*************************\n");
	exit(0);
}


int main(){
#define SIZE 100
	char pass[SIZE];
	int  pasv;
#define TLIM 60
	struct timeval tv1,tv2;	// gettimeofday() secs-usecs

	gettimeofday(&tv1,NULL);
	int ia=0;
	
	printf("Introduce la contraseña: ");
	fgets(pass,SIZE,stdin);

	for(int i=0;i<strlen(pass);i++){
		ia+=int(pass[i]);
	}

	//printf("%i\n",ia );
	if(ia!=password)
		boom();

	gettimeofday(&tv2,NULL);
	if (tv2.tv_sec - tv1.tv_sec > TLIM)
	    boom();
	printf("Introduce el código: ");
	scanf("%i",&pasv);
	/*printf("%i\n",pasv );
	printf("%i\n",password );
	int resultado=pasv-password;
	printf("%i\n",resultado );*/
	if(pasv-password>0) boom();

	gettimeofday(&tv1,NULL);
	if (tv1.tv_sec - tv2.tv_sec > TLIM)
	    boom();

	defused();
}
