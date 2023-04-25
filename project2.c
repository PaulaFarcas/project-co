#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <dirent.h>
#include<sys/wait.h>

int main(int argc,char* argv[]){
   
    if(argc<1){
        printf("error");
        exit(EXIT_FAILURE);
    }
    else{
       for(int i=1;i<argc;i++){
            pid_t   pid=fork();
            if(pid==-1){
                perror("failed");
            }
            else if(pid==0){
                printf("working ");
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
                    scanf("%s",option);
                   // printf("%s",option);
                    //fflush(stdout);
                    //gets(option);
                    if(strcmp(option,"-n")==0){
                        printf("file name:%s \n",argv[i]);
                    }
                    if(strcmp(option,"-d")==0){
                        printf("size %ld\n",buff.st_size);
                    }
                    if(strcmp(option,"-h")==0){
                    printf("nr hard links %ld \n",buff.st_nlink);
                    }
                    if(strcmp(option,"-m")==0){
                    printf("time of last modify %ld \n",buff.st_mtime);
                    }
                    if(strcmp(option,"-a")==0){
                        printf("acces right: ");
                        if(S_IRUSR & buff.st_mode) printf("-r\n");
                        if(S_IWUSR & buff.st_mode) printf("-w\n");
                        if(S_IXUSR & buff.st_mode) printf("-e\n");
                        if(S_IRGRP & buff.st_mode) printf("-r\n");
                        if(S_IWGRP & buff.st_mode) printf("-w\n");
                        if(S_IXGRP & buff.st_mode) printf("-e\n");
                        if(S_IROTH & buff.st_mode) printf("-r\n");
                        if(S_IWOTH & buff.st_mode) printf("-w\n");
                        if(S_IROTH & buff.st_mode) printf("-e\n");
                        else printf("-");
                    }
                    if(strcmp(option,"-l")==0){
                        printf("give link name:\n");
                        char linkName[26];
                        scanf("%s",linkName);
                        symlink(argv[i],linkName);
                    }        
                    if(strstr(argv[i],".c")){
                        pid=fork();
                        if(pid<0){
                            perror("error");
                        }
                        else if(pid==0){
                            execlp("./script.sh","./script.sh",argv[i],NULL);
                            perror("error");
                        }
                        else{
                            int status;
                            int w = waitpid(pid, &status, WUNTRACED | WCONTINUED);
                            if (w == -1) {
                                perror("waitpid");
                                exit(EXIT_FAILURE);
                            }
                            if (WIFEXITED(status)) {
                                printf("\nexited, status=%d\n%d", WEXITSTATUS(status),w);
                            }
                        }
                    }
                }
                else if(S_ISDIR(buff.st_mode)){
                    printf("directory\n");
                    printf("Menu: \n");
                    printf("-n name\n");
                    printf("-d size \n");
                    printf("-a acces right\n");
                    printf("-c total number of c files\n");
                    char option[2];
                    scanf("%s",option);
                    if(strcmp(option,"-n")==0){
                        printf("directorry name is %s\n",argv[i]);
                    }
                    if(strcmp(option,"-d")==0){
                        printf("size of directory is %ld \n",buff.st_size);
                    }
                    if(strcmp(option,"-a")==0){
                        printf("acces right: ");
                        if(S_IRUSR & buff.st_mode) printf("-r\n");
                        if(S_IWUSR & buff.st_mode) printf("-w\n");
                        if(S_IXUSR & buff.st_mode) printf("-e\n");
                        if(S_IRGRP & buff.st_mode) printf("-r\n");
                        if(S_IWGRP & buff.st_mode) printf("-w\n");
                        if(S_IXGRP & buff.st_mode) printf("-e\n");
                        if(S_IROTH & buff.st_mode) printf("-r\n");
                        if(S_IWOTH & buff.st_mode) printf("-w\n");
                        if(S_IROTH & buff.st_mode) printf("-e\n");
                        else printf("-");
                    }
                    if(strcmp(option,"-c")==0){
                        struct dirent *directory;
                        DIR *dir;
                        dir=opendir(argv[i]);
                        while((directory=readdir(dir))!=NULL){
                            char names[1024];
                            snprintf(names,sizeof(names),"%s/%s",argv[i],directory->d_name);
                        }
                    }
                    pid=fork();
                    if(pid<0){
                        perror("eror");
                    }
                    else if(pid==0){
                        char filename[30];
                        snprintf(filename,sizeof(filename),"%s/%s",argv[i],"file.txt");
                        FILE *f=fopen(filename,"w");
                        if(f==NULL){
                            perror("could not create file");
                        }
                        fclose(f);
                        printf("%s",filename);
                        exit(0);
                        
                    }
                    else{
                        int status;
                        int w = waitpid(pid, &status, WUNTRACED | WCONTINUED);
                        if (w == -1) {
                            perror("waitpid");
                            exit(EXIT_FAILURE);
                        }

                        if (WIFEXITED(status)) {
                            printf("\nexited, status=%d\n%d", WEXITSTATUS(status),w);
                        }
                    }
                    

                }
                else if(S_ISLNK(buff.st_mode)){
                    printf("simbolik link\n");
                    printf("Menu:\n");
                    printf("-n link name\n");
                    printf("-l delete link\n");
                    printf("-d size of a link\n");
                    printf("-t size of target\n");
                    printf("-a acces right for symbolic link\n");
                    char option[2];
                    scanf("%s",option);
                    if(strcmp(option,"-n")==0){
                        printf("link name is %s\n", argv[i]);
                    }
                    if(strcmp(option,"-l")==0){
                        unlink(argv[i]);
                        printf("link deleted\n");
                    
                    }
                    if(strcmp(option,"-d")==0){
                        printf("size of link is %ld\n",buff.st_size);
                    }
                    if(strcmp(option,"-t")==0){
                        struct stat target;
                        stat(argv[i],&target);
                        printf("size of target is: %ld\n",target.st_size);
                    }
                    if(strcmp(option,"-a")==0){
                        printf("acces right: ");
                        if(S_IRUSR & buff.st_mode) printf("-r\n");
                        if(S_IWUSR & buff.st_mode) printf("-w\n");
                        if(S_IXUSR & buff.st_mode) printf("-e\n");
                        if(S_IRGRP & buff.st_mode) printf("-r\n");
                        if(S_IWGRP & buff.st_mode) printf("-w\n");
                        if(S_IXGRP & buff.st_mode) printf("-e\n");
                        if(S_IROTH & buff.st_mode) printf("-r\n");
                        if(S_IWOTH & buff.st_mode) printf("-w\n");
                        if(S_IROTH & buff.st_mode) printf("-e\n");
                        else printf("-");
                    }
                }
                else{
                    printf("not regular,directory or simbolik link file\n");
                }
                exit(0);
            }
            else{
                int status;
                int w = waitpid(pid, &status, WUNTRACED | WCONTINUED);
                if (w == -1) {
                    perror("waitpid");
                    exit(EXIT_FAILURE);
                }

                if (WIFEXITED(status)) {
                    printf("\nexited, status=%d\n%d", WEXITSTATUS(status),w);
                }
            }  
        }
    }
    return 0;
}