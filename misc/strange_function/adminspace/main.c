#include <stdio.h>

int sum(int a,int b,int c,int d, int e,int f,int g,int h){
    return a + b + c + d + e + f + g + h;
}

int main(){
    int a,b,c,d,e,f,g,h;
    scanf("%d %d %d %d %d %d %d %d" , &a , &b , &c ,&d,&e,&f,&g,&h);

    if(sum(a,b,c,d,e,f,g,h) == 1337){
        printf("[FLAG] %s" , "pudge{sum_funct1ons_1s_strang3}");
    }else{
        printf("%s", "NOPE ^-^");
    }

    return 0;
}