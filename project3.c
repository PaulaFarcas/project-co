#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <stdbool.h>
#include <sys/wait.h>
#include <regex.h>

void menuForRegularFile(){
    printf("\nMenu for a regular file:\n\n");
    printf("Option\t|\tDescription\n\n");
    printf("-n\t|\tFile Name\n-d\t|\tFile Dimension/Size\n-h\t|\tNumber of Hard Links\n-m\t|\tTime of Last Modification\n-a\t|\tAccess Rights\n-l\t|\tCreate a Symbolic Link & Give the Link Name\n");
}
void menuForDirectory(){
    printf("\nMenu for a Directory:\n\n");
    printf("Option\t|\tDescription\t \n\n");
    printf("-n\t|\tDirectory Name\n-d\t|\tDirectory Dimension/Size\n-a\t|\tAccess Rights\n-c\t|\tTotal number of .c files in the directory\n");

}
void menuForSimbolicLink(){
    printf("\nMenu for a symbolic link:\n\n");
    printf("Option\t|\tDescription\n\n");
    printf("-n\t|\tLink Name\n-l\t|\tDelete link\n-d\t|\tSize of the link\n-t\t|\tSize of the target\n-a\t|\tAccess Rights\n");
}

void printAccessRights(struct stat info){
    printf("\nAccess rights: \n");
    printf((S_ISDIR(info.st_mode)) ? "d" : ((S_ISLNK(info.st_mode)) ? "l" : "-"));
    printf((info.st_mode & S_IRUSR) ? "r" : "-");
    printf((info.st_mode & S_IWUSR) ? "w" : "-");
    printf((info.st_mode & S_IXUSR) ? "x" : "-");
    printf((info.st_mode & S_IRGRP) ? "r" : "-");
    printf((info.st_mode & S_IWGRP) ? "w" : "-");
    printf((info.st_mode & S_IXGRP) ? "x" : "-");
    printf((info.st_mode & S_IROTH) ? "r" : "-");
    printf((info.st_mode & S_IWOTH) ? "w" : "-");
    printf((info.st_mode & S_IXOTH) ? "x" : "-");
    printf("\n\nUSER:\n-read: %s\n-write: %s\n-execute: %s\n", 
       (info.st_mode & S_IRUSR) ? "YES" : "NO",
       (info.st_mode & S_IWUSR) ? "YES" : "NO",
       (info.st_mode & S_IXUSR) ? "YES" : "NO");
    printf("\nGROUP:\n-read: %s\n-write: %s\n-execute: %s\n", 
       (info.st_mode & S_IRGRP) ? "YES" : "NO",
       (info.st_mode & S_IWGRP) ? "YES" : "NO",
       (info.st_mode & S_IXGRP) ? "YES" : "NO");
    printf("\nOTHERS:\n-read: %s\n-write: %s\n-execute: %s\n", 
       (info.st_mode & S_IROTH) ? "YES" : "NO",
       (info.st_mode & S_IWOTH) ? "YES" : "NO",
       (info.st_mode & S_IXOTH) ? "YES" : "NO");
}
void checkIfInputIsValid(char *path, char *option,char *pattern,struct stat info){
    int validInput;
    do {
        if(S_ISREG(info.st_mode)){
            menuForRegularFile();
        }
        else if(S_ISDIR(info.st_mode)){
            menuForDirectory();
        }
        else if(S_ISLNK(info.st_mode)){
            menuForSimbolicLink();
        }
        printf("\nEnter option: ");
        scanf("%s", option);
        // Compile the regular expression pattern and check the input against it
        regex_t regex;
        int ret = regcomp(&regex, pattern, REG_EXTENDED);
        if (ret != 0) {
            perror("\nError compiling regular expression.\n");
            exit(1);
        }
        ret = regexec(&regex, option, 0, NULL, 0);
        regfree(&regex);
        // If the input is valid, exit the loop
        if (ret == 0) {
            validInput = 1;
        } else {
            printf("Invalid input. Please use a syntax that corresponds with the options from the menu.\n");
            validInput = 0;
        }
    } while (!validInput);
} 
void sizeofTargetLink(char* path){
    char target_path[256];
        ssize_t len = readlink(path, target_path, sizeof(target_path)-1);
        struct stat targetstat;
        if (len != -1) {
            target_path[len] = '\0';
            if (lstat(target_path, &targetstat) == 0) {
                if (S_ISREG(targetstat.st_mode)) {
                    printf("\nThe size of the target file is: %lld bytes\n", (long long) targetstat.st_size);
                } else {
                    printf("\nTarget is not a regular file.\n");
                }
            } else {
                perror("lstat");
                return;
            }
        }
        else {
            perror("readlink");
            return;
        }
}
int nrOfCFiles(char *path) {
    DIR *dir_ptr = NULL;
    struct dirent *direntp;
    char *npath;
    if (!path) return 0;
    if( (dir_ptr = opendir(path)) == NULL ) return 0;

    int count=0;
    while( (direntp = readdir(dir_ptr)))
    {
        if (strcmp(direntp->d_name,".")==0 || strcmp(direntp->d_name,"..")==0) continue;
        switch (direntp->d_type) {
            case DT_REG:
                if(strstr(direntp->d_name, ".c")){
                    ++count;
                }
                break;
            case DT_DIR:            
                npath=malloc(strlen(path)+strlen(direntp->d_name)+2);
                sprintf(npath,"%s/%s",path, direntp->d_name);
                count += nrOfCFiles(npath);
                free(npath);
                break;
        }
    }
    closedir(dir_ptr);
    return count;
}

void processRegularFile(char *path,struct stat info){
    char *pattern = "^-[ndhmal]+$";
    char option[32];
    checkIfInputIsValid(path,option,pattern,info);
    // File Name
    if(strstr(option, "n") || strstr(option, "-n")){
        printf("\nFile name is: %s\n", path);
    }
    // File Dimension/Size
    if(strstr(option, "d") || strstr(option, "-d")){
        printf("\nFile dimension/size is: %ld bytes\n", info.st_size);
    }
    // Number of Hard Links
    if(strstr(option, "h") || strstr(option, "-h")){
        printf("\nNumber of hard links is: %ld \n", info.st_nlink);
    }
    // Time of Last Modification
    if(strstr(option, "m") || strstr(option, "-m")){
        printf("\nTime of last modification is: %s", ctime(&info.st_mtime));
    }
    // Access Rights            
    if(strstr(option, "a") || strstr(option, "-a")){
        printAccessRights(info);   
    }
    // Create a Symbolic Link & Give the Link Name
    if(strstr(option, "l") || strstr(option, "-l")){
        char linkName[52];
        printf("\nIntroduce the link name: ");
        scanf("%s", linkName);
        if (symlink(path, linkName) == -1) {
            perror("\nError creating symbolic link!\n");
        } else {
            printf("\nSymbolic link created: %s -> %s\n", linkName, path);
        }
    }
}
void processDirectory(char* path,struct stat info){
    char option[32];
    char *pattern = "^-[ndac]+$";
    checkIfInputIsValid(path,option,pattern,info);
    // Use a do-while loop to ensure valid input

    // Directory Name
    if(strstr(option, "n") || strstr(option, "-n")){
        printf("\nDirectory name is:%s\n",path);        
    }
    // Directory Dimension/Size
    if(strstr(option, "d") || strstr(option, "-d")){
        printf("\nDirectory dimension/size is: %ld bytes\n", info.st_size);
    }
    // Access Rights
    if(strstr(option, "a") || strstr(option, "-a")){
        printAccessRights(info);   
    }
    // Total number of .c files in the directory
    if(strstr(option, "c") || strstr(option, "-c")){                      
        printf("\nTotal number of .c files in the directory is: %d\n", nrOfCFiles(path));
    }
}
void processLinks(char* path,struct stat info){
    char *pattern = "^-[nldta]+$";
    printf("\nChoose one or more options from the menu.\n");
    printf("If the -l or l option is given, the other following options won't be performed!!\n");
    char option[32];
    checkIfInputIsValid(path,option,pattern,info);
    // Link Name
    if(strstr(option, "n") || strstr(option, "-n")){
        printf("\nThe link name is: %s\n", path);
    }
    // Delete link
    if(strstr(option, "l") || strstr(option, "-l")){
        if(unlink(path) != 0){
            perror("unlink() error\n");
        }else{
            printf("\nThe link %s was deleted.\n", path);
        }                    
    }
    // Size of the link
    if(strstr(option, "d") || strstr(option, "-d")){
        printf("\nThe size of the link is: %ld bytes\n", info.st_size);
    }
    // Size of the target
    if(strstr(option, "t") || strstr(option, "-t")){
        sizeofTargetLink(path);
    }
    // Access Rights            
    if(strstr(option, "a") || strstr(option, "-a")){
        printAccessRights(info);
    }
}

int computeNrLinesOfRegularFile(char *path){
    char c;
    FILE *file=fopen(path,"r");
    int nr_lines=0;
    if(file==NULL){
        printf("not opend file");
        exit(1);
    }
    while((c=getc(file))!=EOF){
        if(c=='\n'){
            nr_lines++;
        }
    }
    return nr_lines;
}
void executeScript(char *path){
    execlp("./script.sh", "./script.sh", path, NULL);
    perror("\nExec failed.\n");
    exit(1);
}
void ChildPipe(int * pfd){
    close(pfd[0]);
    dup2(pfd[1],1);
    
}
double ComputeScore(int nr_errors,int nr_warnings){
    int score;
    if(nr_errors==0 && nr_warnings==0){
        score=10;
    }
    else if(nr_errors>=1){
        score=1;
    }
    else if(nr_warnings>10){
        score=2;
    }
    if(nr_errors==0 && nr_warnings>10){
        score=2+8*(10-nr_warnings)/10;
    }
    return score;

}
void ComputeScoreParent(int *pfd,char* path){
    int nr_errors=0,nr_warnings=0;
    double score;
    FILE *file;
    file=fdopen(pfd[0],"r");
    if(file==NULL){
        printf("could not open file\n");
    }
    fscanf(file,"%d,%d",&nr_errors,&nr_warnings);

    //printf("Nr warnings:%d\nNr error:%d",nr_warnings,nr_errors);

    score=ComputeScore(nr_errors,nr_warnings);
    //write score int grades.txt file
    FILE *fp=fopen("grades.txt","w");
    if(fp==NULL){
        perror("\nError when opening");
        exit(1);
    }
    fprintf(fp,"%s: %.2f\n",path,score);
    fclose(fp);
  
}

void secondProcessForRegularFile(char *path,pid_t getID,int *pfd){
    if(strstr(path, ".c")){
        printf("\nSecond PID for .C files: %d\n", getID);
        ChildPipe(pfd);
        executeScript(path);
    }   
    else{                     
        //regular file txt file-compute nr of lines;           
        printf("\nSecond PID for .txt files: %d\n", getID);
        printf("\nNr of lines is: %d\n",computeNrLinesOfRegularFile(path));

    }
}
void secondProcessForDirectory(char *path,pid_t getID){ 
    printf("\nSecond PID for directories: %d\n", getID);
    printf("\nArgument is a directory. A corresponding txt file will be created:\n");
    char fileName[100];
    snprintf(fileName, sizeof(fileName), "%s/%s", path, "file.txt");
    FILE *f = fopen(fileName, "w");
    if(f == NULL){
        perror("\nError creating file.\n");
    }
    fclose(f);
    printf("\nFile created: %s\n", fileName);
}
void secondProcessForLinks(char *path,pid_t getID){
    //char actualpath [_POSIX_PATH_MAX];
    //realpath(path,actualpath);
    //printf("%s",actualpath);
    printf("\nSecond PID for links: %d\n",getID);
    execlp("chmod","chmod","u=rwx,g=rw,o=",path,NULL);
    perror("\nError executing chmod\n");
    exit(1);                    
}

int main(int argc, char* argv[]) {
  
   if (argc == 1) {
        printf("Please put at least another argument");
        return EXIT_FAILURE;
    } else {
        int status;
        struct stat info;
        mode_t m;
        for (int i = 1; i < argc; i++) {
            lstat(argv[i], &info);
            m = info.st_mode;
            pid_t pid = fork();
            if(pid<0){
                perror("Forking error... Exiting ...\n");
                exit(EXIT_FAILURE);
            }else if(pid==0){
                pid_t getID = getpid(); 
                if(S_ISREG(m)){
                    printf("\nPID for regular files: %d\n", getID);
                    printf("\nArgument %d, with the name: %s is a regular file\n",i,argv[i]);
                    processRegularFile(argv[i],info);
                }
                if(S_ISDIR(m)){
                    printf("\nPID for directories: %d\n", getID);
                    printf("\nArgument %d,with the name: %s is a Directory\n", i,argv[i]);
                    processDirectory(argv[i],info);
                }
                if(S_ISLNK(m)){
                    printf("\nPID for simbolik links: %d\n", getID);
                    printf("\nArgument %d, with the name: %s is a link\n", i,argv[i]);
                    processLinks(argv[i],info);
                }
                exit(EXIT_SUCCESS);
            }else{
                
                int w = waitpid(pid, &status, WUNTRACED | WCONTINUED);
                if(w == -1){
                    perror("\nWaitpid\n");
                    exit(EXIT_FAILURE);
                }
                if(!WIFEXITED(status)){
                    perror("\nError at exiting.\n");
                    exit(-1);
                }  //here the output is in order but they run sequencially;
                int pfd[2]; 
                if(pipe(pfd)<0){
                    perror("pipe creation failed");
                    exit(1);
                }
                pid_t pid2 = fork();
                if (pid2 < 0) {
                    perror("fork");
                    exit(EXIT_FAILURE);
                }
                else if(pid2==0){
                    pid_t getID = getpid();
                    if(S_ISREG(m)){
                        secondProcessForRegularFile(argv[i],getID,pfd);
                    }
                    if(S_ISDIR(m)){
                        secondProcessForDirectory(argv[i],getID); 
                    }
                    if(S_ISLNK(m)){
                        secondProcessForLinks(argv[i],getID);
                    }   
                    exit(EXIT_SUCCESS);
                    
                }else{
                    if(strstr(argv[i], ".c")){
                        close(pfd[1]);//close writing end of pipe
                        ComputeScoreParent(pfd,argv[i]);
                        printf("\nWe have a .C file. Parent process computes score and writes it in grades.txt file\n");
                        
                    }
                    close(pfd[0]);//close read end for parent

                   /* int w = waitpid(pid, &status, WUNTRACED | WCONTINUED);
                    if(w == -1){
                        perror("\nWaitpid\n");
                        exit(EXIT_FAILURE);
                    }
                    if(!WIFEXITED(status)){
                        perror("\nError at exiting.\n");
                        exit(-1);
                    }*/  //if i put the waitpid here the output of the process gets mixed but they run in parallel;
                
                    int w2 = waitpid(pid2, &status, WUNTRACED | WCONTINUED);
                    if(w2 == -1){
                        perror("\nWaitpid\n");
                        exit(EXIT_FAILURE);
                    }
                    if(WIFEXITED(status)){
                         if(WEXITSTATUS(status)==1){
                           
                            printf("Error at changing permmissions");
                        }
                        else  if(S_ISLNK(m)){
                            printf("\nSymbolic link permissions changed succesfully\n");
                        }
                        printf("\nThe process with PID: %d has ended, with the exit code  %d  \n ",w2, WEXITSTATUS(status));
                        printf("\nThe process with PID: %d has ended, with the exit code  %d  \n ",w, WEXITSTATUS(status));
                        
                    }
                    else{
                        perror("\nError at exiting.\n");
                        exit(-1);
                    }
                }
            }
        }
    }
    return EXIT_SUCCESS;
}