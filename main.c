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
    FILE *file = fopen(filename, "w");  
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

void read_file(const char *filename) {
    FILE *file = fopen(filename, "r");  
    if (file == NULL) {
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
        printf("read: Cannot read file '%s', maybe is not exist\n", filename);
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
        return;
    }
    char stringF[sizeof(filename)];
    while(fgets(stringF, sizeof(filename), file)) {
        printf("%s", stringF);
    }
    fclose(file);
}



int main() {
    char cwd[1024];
    char command[512];
    SetConsoleTitle("Nowa Terminal 1.3");
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    printf("Nowa-Terminal v1.3, type \"help\" to see list of commands and \"version\" to see versions of moduls and app.\n");
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
            printf("echo - print text(echo [text])\nls - show info about directory and files in it\ncd - go to directory(cd [dirname])\ncdir - create directory(cdir [dirname])\nrdir - remove directory if it empty(rdir [dirname])\ncurl - use curl(curl [params])\ncfile - create file(cfile [filename.file_extension])\nrfile - remove file(rfile [filename.file_extension])\nread - read file content(read [filename.file_extension])\nclear - clear terminal\ntitle - rename terminal title(title [name])\nexit - end session\n");
            SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
        } else if (strncmp(command, "cfile ", 6) == 0) {
            create_file(command + 6);
            continue;
        } else if (strncmp(command, "read ", 5) == 0) {
            read_file(command + 5);
            continue;
        }
        else if(strcmp(command, "version") == 0) {
            SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_INTENSITY);
            printf("Nowa-Terminal: 1.3\n\n");
            print_command_output("Bash", "bash --version");
            printf("\n");
            print_command_output("Curl", "curl --version");
            printf("\n");
            SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
        } else if(strncmp(command, "title ", 6) == 0) {
            char* titlename = command + 6;
            if(strcmp(titlename, "") == 0) {
                SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
                printf("title: cannot rename app title, invalid name\n");
                SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
            } else {
                SetConsoleTitle(titlename);
            }
            continue;
        } else if(strcmp(command, "clear") == 0) {
            system("cls");
            continue;
        }
        else if (strncmp(command, "rfile ", 6) == 0) {
            delete_file(command + 6);
            continue;
        }
        else if(strcmp(command, "ls") == 0) {
            system("dir");
            continue;
        } else if(strncmp(command, "cd ", 3) == 0) {
            char* dirname = command + 3;
            if(strcmp(dirname, "") == 0) {
                SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
                printf("cd: cannot go directory, maybe is not exist\n");
                SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
            } else {
                chdir(dirname);
            }
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
