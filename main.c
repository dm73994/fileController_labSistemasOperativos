#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>


int esDirectorio(char * path);
int search(char*dir, char*pattern);

int main(int argc, char*argv[]){
    
	int resp = search(argv[1], argv[2]);
	printf("%d", resp);	

    exit(EXIT_SUCCESS);
}

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

int search(char*dir, char*pattern){
	int total = 0;
	DIR*streamDir = opendir(dir);
	
	if(streamDir == NULL){
		perror("opendir");
		return 0;
	}
	
	struct dirent *entry;
	int dirLenght = strlen(dir);
	
	char*absPath;
	while( (entry = readdir(streamDir) ) ){
		int fileLenght = strlen(entry->d_name)+1;		
	    absPath = (char*)malloc(dirLenght+fileLenght);
	  
	    strcpy(absPath, dir);
	    strcat(absPath, "/");
	    strcat(absPath, entry->d_name);	  
	    
	    if(strstr(entry->d_name, pattern) ){
	    	total += 1;
		}
		
	    //printf("%s\n", absPath);	    	
		if(esDirectorio(dir) == 1 
			&& strcasecmp(entry->d_name, ".") != 0
			&& strcasecmp(entry->d_name, "..") != 0	
		){
	    	//printf("%s\n", absPath);	    	
			total = total + search(absPath, pattern);
		}	
		
	}
	free(absPath);
	//closedir(streamDir);
	return total;
}