#include<stdio.h>

int fun(char *str1){
    char* str2=str1;
   

    while(*++str1){
        printf("%s ",str1);
        printf("%d\n",(str1-str2));
    };
    printf("%s",str1);
    return (str1-str2);
}
int main(){
    char* str="SDA";
    
    printf("%d",fun(str));
    return 0;
}