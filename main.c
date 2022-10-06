#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>


int esDirectorio(char * path);
int search(char*dir, char*pattern);

/**
 * @brief Inicializa el programa con 3 parametros 
 * 
 * @param argc 3 parametros para poder inicializar
 * @param argv argv[0]->ejecutable, argv[1]->ruta de directorio, argv[2]->patron a buscar
 * @return int 
 */
int main(int argc, char*argv[]){
    if (argc !=3){
        fprintf(stderr, "uso: %s DIR PATRON\n", argv[0]);
        exit(EXIT_SUCCESS);
    }

	int resp = search(argv[1], argv[2]);
	printf("TOTAL=[%d]", resp);	

    exit(EXIT_SUCCESS);
}

/**
 * @brief Define si una ruta es un directorio,  0 FALSO || 1 VERDADERO
 * 
 * @param path ruta a determinar si es directorio
 * @return int 0 FALSO || 1 VERDADERO
 */
int esDirectorio(char * path){
    struct stat sb;
    int resp = stat(path, &sb);

    if(resp < 0){
    	perror("stat");
        return 0;
    }
    if(S_ISDIR(sb.st_mode)){
        return 1;
    }
    return 0;
}

/**
 * @brief Busca de de forma recursiva dentro de un directorio todos los archivos que contengan un patron dado
 * 
 * @param dir ruta del directorio
 * @param pattern patron a buscar en el nombre de los archivos dentro del directorio y su subdirectorios
 * @return int total de archivos encontrados con el patrón
 */
int search(char*dir, char*pattern){
	int total = 0;
	DIR*streamDir = opendir(dir);
	
	//**Verificar que el directorio sea valido
	if(streamDir == NULL){
		//perror("opendir");
		return 0;
	}
	
	struct dirent *entry;
	char*absPath;
	
	//**Leemos los archivos hasta que encontremos un null indicando que se han leido todos
	while( (entry = readdir(streamDir) ) ){
		//reservamos memoria dinamica para almacenar la ruta absoluta del archivo en lectura
	    absPath = (char*)malloc(strlen(dir) + strlen (entry->d_name)+2);
	  
	    strcpy(absPath, dir);
	    strcat(absPath, "/");
	    strcat(absPath, entry->d_name);	  
	    //printf("%s",absPath);
		//verificamos si el archivo cumple el patron
	  if(strstr(entry->d_name, pattern) ){
			printf("%s\n",absPath);
			total += 1;
		}
		
		//buscamos recursivamente en los subdirectorios
		if(esDirectorio(dir) == 1 
			&& strcasecmp(entry->d_name, ".") != 0
			&& strcasecmp(entry->d_name, "..") != 0	
		){
			total = total + search(absPath, pattern);
		}	
		
	}
	free(absPath);
	//closedir(streamDir);
	return total;
}
