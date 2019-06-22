#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>
#include <wait.h>
#include <sys/stat.h>
#include <fcntl.h>

FILE *fp;
char estado_actual[4];

void status();

int main(int argc, char **argv){

	if(argv[1] != NULL){
		fp = fopen ( "/proc/lampctrl", "rw" );        
		if (fp==NULL) {
			fputs ("File error\n",stderr);
			return 0;
		}
		
		if(strcmp(argv[1], "status") == 0){
			status();
			}else if(strcmp(argv[1], "lamp") == 0){
				fgets(estado_actual, 4 , fp);
				if(strcmp(argv[2], "L0") == 0){
			     if(strcmp(argv[3], "0")== 0){
			     	estado_actual[0] = '0';
			     }else if(strcmp(argv[3], "1")== 0){
			     	estado_actual[0] = '1';
			     }else{
			     	printf("Valor erroneo\n");
			     }
			}else if(strcmp(argv[2], "L1")== 0){
			     if(strcmp(argv[3], "0")==0){
			     	estado_actual[1] = '0';
			     }else if(strcmp(argv[3], "1")==0){
			     	estado_actual[1] = '1';
			     }else{
			     	printf("Valor erroneo\n");
			     }
			}else if(strcmp(argv[2], "L2")==0){
			     if(strcmp(argv[3], "0")==0){
			     	estado_actual[2] = '0';
			     }else if(strcmp(argv[3], "1")==0){
			     	estado_actual[2] = '1';
			     }else{
			     	printf("Valor erroneo\n");
			     }
			}else{
				printf("Valor de lampara invalido\n");
			}
			printf("Actualizado estado de las lamparas \n");
			status();
			fputs(estado_actual , fp);
			}else{
			printf("%s : No es un comando valido \n" , argv[1]);
		}
	}else{
		printf("Debe ingresar parametros\n");
	}
	fclose ( fp );
	return 0;
}

void status(){
	while (feof(fp) == 0){
 				fgets(estado_actual,4,fp);
 				printf("Estado actual:  (0: Apagado , 1: Encendido)\n");
 				printf("L0: %c - L1: %c - L2: %c \n",estado_actual[0] , estado_actual[1] , estado_actual[2]);
 	}
}
