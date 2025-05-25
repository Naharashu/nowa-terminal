#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <direct.h>

HANDLE hConsole;

void clear_stdin() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void print_command_output(const char* label, const char* command) {
    FILE* fp = _popen(command, "r");
    if (fp == NULL) {
        printf("%s: failed to execute\n", label);
        return;
    }

    char buffer[256];
    printf("%s: ", label);
    while (fgets(buffer, sizeof(buffer), fp)) {
        printf("%s", buffer);
    }
    _pclose(fp);
}

void create_file(const char *filename) {
    FILE *file = fopen(filename, "w");  // "w" — запис, створює файл
    if (file == NULL) {
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
        printf("cfile: Cannot create file '%s'\n", filename);
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
        return;
    }
    fprintf(file, "");
    fclose(file);
}

#include <stdio.h>

void delete_file(const char *filename) {
    if (remove(filename) == 0) {
        
    } else {
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
        printf("rfile: Cannot delete file '%s'\n", filename);
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    }
}



int main() {
    char cwd[1024];
    char command[256];
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    printf("Nowa-Terminal v1.1, type \"help\" to see list of commands and \"version\" to see versions of moduls and app.\n");
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
        } else if(strncmp(command, "echo ", 5) == 0) {
            printf("%s\n", command + 5);
            continue;
        } else if(strcmp(command, "help") == 0) {
            SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
            printf("echo - print text(echo [text])\nls - show info about directory and files in it\ncd - go to directory(cd [dirname])\ncdir - create directory(cdir [dirname])\nrdir - remove directory if it empty(rdir [dirname])\ncurl - use curl(curl [params])\ncfile - create file(cfile [filename.file_extension])\nrfile - remove file(rfile [filename.file_extension])\nexit - end session\n");
            SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
        } else if (strncmp(command, "cfile ", 6) == 0) {
            create_file(command + 6);
            continue;
        } else if(strcmp(command, "version") == 0) {
            SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_INTENSITY);
            printf("Nowa-Terminal: 1.1\n\n");
            print_command_output("Bash", "bash --version");
            printf("\n");
            print_command_output("Curl", "curl --version");
            printf("\n");
            SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
        }
        else if (strncmp(command, "rfile ", 6) == 0) {
            delete_file(command + 6);
            continue;
        }
        else if(strcmp(command, "ls") == 0) {
            system("dir");
            continue;
        } else if(strncmp(command, "cd ", 3) == 0) {
            chdir(command + 3);
            continue;
        } else if(strncmp(command, "cdir ", 5) == 0) {
            char* dirname = command + 5;
            if(strcmp(dirname, "") == 0) {
                SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
                printf("cdir: cannot create directory\n");
                SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
            } else {
                _mkdir(dirname);
            }
            continue;
        } else if(strncmp(command, "rdir ", 5) == 0) {
            char* dirname = command + 5;
            if(strcmp(dirname, "") == 0) {
                SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
                printf("rdir: cannot remove directory\n");
                SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
            } else {
                _rmdir(dirname);
            }
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
                WaitForSingleObject(pi.hProcess, INFINITE);
                CloseHandle(pi.hProcess);
                CloseHandle(pi.hThread);
            }
        }
    }

    return 0;
}
