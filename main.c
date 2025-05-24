#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void clear_stdin() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int main() {
    char cwd[1024];
    char command[256];

    while (1) {
        GetCurrentDirectoryA(sizeof(cwd), cwd);
        printf("%s: ", cwd);

        if (fgets(command, sizeof(command), stdin) == NULL) {
            printf("Error reading input\n");
            continue;
        }

        command[strcspn(command, "\n")] = 0;

        if (strlen(command) == sizeof(command) - 1) {
            clear_stdin();
        } else if (strcmp(command, "exit") == 0) {
            return 0;
        } else if(strncmp(command, "echo", 5) == 0) {
            printf("%s\n", command + 5);
            continue;
        }
        else if (strncmp(command, "curl ", 5) == 0) {
            char *cmd = command + 5;

            char fullCommand[512];
            snprintf(fullCommand, sizeof(fullCommand), "curl %s", cmd);

             STARTUPINFOA si;
            PROCESS_INFORMATION pi;
            ZeroMemory(&si, sizeof(si));
            si.cb = sizeof(si);
            ZeroMemory(&pi, sizeof(pi));

            if (!CreateProcessA(
                NULL,             
                fullCommand,      
                NULL,             
                NULL,             
                FALSE,            
                0,                
                NULL,             
                NULL,             
                &si,               
                &pi)             
            ) {
                printf("CreateProcess failed (%lu).\n", GetLastError());
            } else {
                // Чекаємо завершення процесу
                WaitForSingleObject(pi.hProcess, INFINITE);
                CloseHandle(pi.hProcess);
                CloseHandle(pi.hThread);
            }
        }
    }

    return 0;
}
