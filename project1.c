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
            printf("Menu:\n");
            printf("-n (file name)\n");
            printf("-d (file dim/size)\n");
            printf("-h (number of hard links)\n");
            printf("-m (time of last modification)\n");
            printf("-a (access rights)\n");
            printf("-l (create a sym link give: link name)\n");

            char option[2];
            scanf("%s",&option);
            if(strcmp(option,"-n")==0){
                printf("file name:%s \n",argv[i]);
            }
            if(strcmp(option,"-d")==0){
                printf("size %s\n",buff.st_size);
            }
            if(strcmp(option,"-h")==0){
                printf("nr hard links %s \n",buff.st_nlink);
            }
            if(strcmp(option,"-m")==0){
                 printf("time of last modify %s \n",buff.st_mtime);
            }
            if(strcmp(option,"-a")==0){
                printf("acces right: %s");
                if(S_IRUSR & st_mode) printf("-r");
                if(S_IWUSR & st_mode) printf("-w");
                if(S_IXUSR & st_mode) printf("-e");
                if(S_IRGRP & st_mode) printf("-r");
                if(S_IWGRP & st_mode) printf("-w");
                if(S_IXGRP & st_mode) printf("-e");
                if(S_IROTH & st_mode) printf("-r");
                if(S_IWOTH & st_mode) printf("-w");
                if(S_IROTH & st_mode) printf("-e");
                else printf("-");
            }
            if(strcmp(option,"-l")==0){
                printf("give link name %s \n");
                char name[26];
                scanf("%s",linkName);
                pritnf("creat syn link %s \n",sym(argv[i],linkName));
            }
 
           
           
            
           
           
           
            //
           
            
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