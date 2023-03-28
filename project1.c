#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
 
int main(int argc,char* argv[]){
   
    if(argc<1){
        printf("error");
        exit(EXIT_FAILURE);
    }
    else{
        for(int i=1;i<argc;i++){
             struct stat buff;
             lstat(argv[i],&buff);
        if(S_ISREG(buff.st_mode)){
            printf("regular file\n");

            printf("file name:%s \n");
            printf("size %s\n");
            pritnf("nr hard links %s \n");
            printf("time of last modify %s \n");
            printf("acces right: %s \n");
            pritnf("creat syn link %s \n");
            printf("give link name %s \n");
       }
        else if(S_ISDIR(buff.st_mode)){
            printf("directory\n");
        }
        else if(S_ISLNK(buff.st_mode)){
            printf("simbolik link\n");
        }
        else{
            printf("not regular,directory or simbolik link file\n");
        }

        }
    }

   

    return 0;
}