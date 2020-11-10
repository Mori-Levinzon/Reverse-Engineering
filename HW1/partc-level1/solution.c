#include <stdio.h>
#include <string.h>

int main() {
    char input[12];
    int wood = 0, brick = 0, wool = 0, grain = 0, ore = 0;
	
	char str1[]="%s";
    while (scanf(str1, input) != EOF) {
        if (input[0] == 'r') {
            wood++;
            brick++;
        }
        if (input[0] == 's') {
            wood++;
            brick++;
            wool++;
            grain++;
        }
        if (input[0] == 'c') {
            grain += 2;
            ore += 3;
        }
        if (input[0] == 'd') {
            wool++;
            grain++;
            ore++;
        }
    }
	char str[] = "%02d %02d %02d %02d %02d";

    printf(str,wood, brick, wool, grain, ore);

    return 0;
}
