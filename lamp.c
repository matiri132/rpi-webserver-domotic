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
char nuevo_estado[4];

int main(){
	
	if(argV[0] != NULL){
		fp = fopen ( "/proc/lampctrl", "r+" );        
		if (fp==NULL) {
			fputs ("File error",stderr);
			return 0;
		}

		if(strcmp(argV[0], "status")){
			while (feof(archivo) == 0){
 				fgets(estado_actual,4,fp);
 				printf("Estado actual:  (0: Apagado , 1: Encendido)\n");
 				printf("L0: %c - L1: %c - L2: %c \n",estado_actual[0] , estado_actual[1] , estado_actual[2]);
 	    	}
		}

		if(strcmp(argV[0], "lamp")){
			if(strcmp(argV[1], "L0"){
			     if(strcmp(argV[2], "0")){
			     	estado_actual[0] = '0';
			     }else if(strcmp(argV[2], "1"){
			     	estado_actual[0] = '1';
			     }else{
			     	printf("Valor erroneo\n");
			     }
			}else if(strcmp(argV[1], "L1"){
			     if(strcmp(argV[2], "0")){
			     	estado_actual[1] = '0';
			     }else if(strcmp(argV[2], "1"){
			     	estado_actual[1] = '1';
			     }else{
			     	printf("Valor erroneo\n");
			     }
			}else if(strcmp(argV[1], "L2"){
			     if(strcmp(argV[2], "0")){
			     	estado_actual[2] = '0';
			     }else if(strcmp(argV[2], "1"){
			     	estado_actual[2] = '1';
			     }else{
			     	printf("Valor erroneo\n");
			     }
			}else{
				printf("Valor de lampara invalido\n");
			}

			fputs(estado_actual , fp);
		}


    		
	    
	}else{
		printf("No hay opciones validas\n");
	}

	fclose ( fp );
	return 0;
    
}