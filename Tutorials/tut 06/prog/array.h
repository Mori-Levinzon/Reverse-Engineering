#if !defined(__ARRAY_H)
#define __ARRAY_H

#include <stdio.h>
#include <string.h>
// #include <windows.h>
#include "funcs.h"
#include "stdlib.h"
#include <stdbool.h>

#define MAX_SIZE 10000

enum
{
    SUCCESS,
    FAILED,
    FAILED_HORRIBLY
};

enum
{
    WRITE_ARRAY = 1,
    ACTIVE_FUNC = 2,
    REPLACE_FUNC = 3,
    INVALID_COMMAND = 4
};

typedef struct _array
{
    int (*func_ptr)(int *, int);
    int size;
    int *data;
} Array;

Array *create_array()
{
    unsigned int index;
    printf("Enter function number from:\n");
    print_funcs();
    if (scanf("%u", &index) <= 0 || index >= NUM_FUNCS)
    {
        return NULL;
    }
    printf("chosen func is: %s\n", Functions[index].name);
    Array *new_arr = calloc(1, sizeof(Array));
    new_arr->func_ptr = Functions[index].func;
    new_arr->size = 0;
    new_arr->data = NULL;
    return new_arr;
}

int replace_func(Array *arr)
{
    unsigned int index;
    printf("Enter new function number from :\n");
    print_funcs();
    if (scanf("%u", &index) <= 0 || index >= NUM_FUNCS)
    {
        free(arr);
        return FAILED_HORRIBLY;
    }
    printf("chosen func is: %s\n", Functions[index].name);
    arr->func_ptr = Functions[index].func;
    return SUCCESS;
}

int write_array(Array *arr)
{
    unsigned int new_size;
    printf("Enter new size:\n");
    if (scanf("%u", &new_size) <= 0 || new_size > MAX_SIZE)
    {
        return FAILED;
    }
    if (arr->data != NULL)
        free(arr->data);
    int *new_data = calloc(new_size, sizeof(int));
    int tmp = 1;
    printf("Enter numbers:\n");
    for (int i = 0; i < new_size && tmp != 0; i++)
    {
        scanf("%d", &tmp);
        new_data[i] = tmp;
    }
    arr->size = new_size;
    arr->data = new_data;
    return SUCCESS;
}

int activte_func(Array *arr)
{
    int res;
    if (arr->data == NULL)
    {
        return FAILED;
    }
    res = arr->func_ptr(arr->data, arr->size);
    printf("function result: %d\n", res);
    return SUCCESS;
}

#endif // __ARRAY_H

