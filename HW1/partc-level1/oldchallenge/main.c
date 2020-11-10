#include <stdio.h>
int main() {
    char resource[10];
    int ore=0,brick=0,wool=0,grain=0,lumber=0;
    while (scanf("%s",&resource) !=EOF){
        if (resource[0]=='c'){
            grain+=2;
            ore+=3;
        }
        if (resource[0]=='r') {
            lumber+=1;
            brick+=1;
        }
        if (resource[0]=='s') {
            lumber+=1;
            brick+=1;
            wool+=1;
            grain+=1;
        }
        if (resource[0]=='d') {
            wool+=1;
            grain+=1;
            ore+=1;
        }
    }
    printf("%.2d %.2d %.2d %.2d %.2d\n", lumber, brick, wool, grain, ore);

    return 0;
}