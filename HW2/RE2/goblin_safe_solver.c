#include <stdio.h>
#include <windows.h>


#define SIZE 25


int main(void)
{
//    system("start safe.exe");
//    Sleep(20);
    static char files[] ="dictionary.txt";
    char buffer[SIZE];
    DWORD dwWritten,dwRead;


    HANDLE superSecretPipe = CreateFile(TEXT("\\\\.\\pipe\\SuperSecretPipe"), GENERIC_READ | GENERIC_WRITE,
                                        0,
                                        NULL,
                                        OPEN_EXISTING,
                                        0,
                                        NULL);
    while (superSecretPipe==INVALID_HANDLE_VALUE){
        superSecretPipe = CreateFile(TEXT("\\\\.\\pipe\\SuperSecretPipe"), GENERIC_READ | GENERIC_WRITE,
                                     0,
                                     NULL,
                                     OPEN_EXISTING,
                                     0,
                                     NULL);    }
    HANDLE  secretAnswerPipe = CreateNamedPipe("\\\\.\\pipe\\SecretAnswerPipe",
                                               3,
                                               0,   // FILE_FLAG_FIRST_PIPE_INSTANCE is not needed but forces CreateNamedPipe(..) to fail if the pipe already exists...
                                               1,
                                               1024 * 16,
                                               1024 * 16,
                                               120000,
                                               0);
    if (ConnectNamedPipe(secretAnswerPipe,0) !=0) {printf("Connected\n");}


//    CreateFile(superSecretPipe);
//    CreateNamedPipe(secretAnswerPipe);

    if (superSecretPipe != INVALID_HANDLE_VALUE)
    {
        printf("here");
        FILE *file;
        if ((file = fopen("dictionary.txt", "r")) == NULL) {
            printf("Error! opening file");
            // Program exits if file pointer returns NULL.
            exit(1);
        }
        /* Get each line until there are none left */
        char line[SIZE] = {0};
        int line_count=0;
        while (fgets(line, SIZE, file))
        {
            /* Print each line */
            printf("line[%06d]: %s", ++line_count, line);

            /* Add a trailing newline to lines that don't already have one */
            if (line[strlen(line) - 1] != '\n')
                printf("\n");

            WriteFile(superSecretPipe,line,strlen(line)+1,   // = length of string + terminating '\0' !!!
                      &dwWritten,
                      NULL);
            if (superSecretPipe != INVALID_HANDLE_VALUE) {
                if (ConnectNamedPipe(secretAnswerPipe, NULL) != FALSE)   // wait for someone to connect to the pipe
                {
                    printf("ConnectNamedPipe reached");
                    if (ReadFile(secretAnswerPipe, buffer, sizeof(buffer) - 1, &dwRead, NULL) != FALSE) {
                        printf("ReadFile reached");

                        /* add terminating zero */
                        buffer[dwRead] = '\0';

                        /* do something with data in buffer */
                        printf("%s", buffer);
                    }
                }
            }
        }

        CloseHandle(superSecretPipe);
        DisconnectNamedPipe(secretAnswerPipe);

    }

    return 0;
}