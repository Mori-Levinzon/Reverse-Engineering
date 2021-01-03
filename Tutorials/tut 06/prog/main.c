
#include <stdio.h>
#include <string.h>
#include <windows.h>
#include "funcs.h"
#include "stdlib.h"
#include <stdbool.h>
#include "array.h"



int main(){
    int command;
    Array* arr = create_array();
    if (arr == NULL){
        fprintf(stderr, "Error creating Array\n");
        return FAILED;
    }
    while(true){
        printf("Enter command:\n 1 - write array\n 2 - activate function\n 3 - replace function\n"
        );
        if (scanf("%d", &command) <= 0)
        {
            fprintf(stderr, "Error reciving command\n");
            return FAILED;
        }

        switch (command)
        {
        case WRITE_ARRAY:
            if (FAILED == write_array(arr)){
                fprintf(stderr, "failed writing array\n");
                return FAILED;
            }
            break;

        case ACTIVE_FUNC:
            if (FAILED == activte_func(arr))
            {
                fprintf(stderr, "failed activating func\n");
                return FAILED;
            }
            break;

        case REPLACE_FUNC:
            if (FAILED == replace_func(arr))
            {
                fprintf(stderr, "failed replacing func\n");
                return FAILED;
            }
            break;

        default:
            fprintf(stderr, "Invalid command.\n");
            return FAILED;
        }
    }

    return SUCCESS;
}

void win_exercise()
{
    MessageBox(NULL, "Congratulations!!!\nYou solved this exercise! ", "You did it!", 0);
    ExitProcess(0);
}

// bad sequence: 2 3 4 1 3 1 2 3 2 1 3 0 2 