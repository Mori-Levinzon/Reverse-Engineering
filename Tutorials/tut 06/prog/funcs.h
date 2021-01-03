#if !defined(__FUNCS_H)
#define __FUNCS_H

int XOR(int *data, int size)
{
    int res = 0;
    for (int i = 0; i < size; i++)
    {
        res ^= data[i];
    }
    return res;
}

int SUM(int *data, int size)
{
    int res = 0;
    for (int i = 0; i < size; i++)
    {
        res += data[i];
    }
    return res;
}

int MUL(int *data, int size)
{
    int res = 1;
    for (int i = 0; i < size; i++)
    {
        res *= data[i];
    }
    return res;
}

#define MAX_NAME_SIZE 10

struct function
{
    int index;
    int (*func)(int *, int);
    const char name[MAX_NAME_SIZE];
};

static struct function Functions[] = {
    {0, XOR, "XOR"},
    {1, SUM, "SUM"},
    {2, MUL, "MUL"}};

#define NUM_FUNCS (sizeof(Functions) / sizeof(Functions[0]))

void print_funcs()
{
    for (int i = 0; i < NUM_FUNCS; i++)
    {
        printf("%d - %s\n", i, Functions[i].name);
    }
}

#endif // __FUNCS_H



