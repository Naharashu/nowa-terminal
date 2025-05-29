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

void get_cpu_name(char *cpu_name, DWORD size) {
    HKEY hKey;
    if (RegOpenKeyExA(
        HKEY_LOCAL_MACHINE,
        "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0",
        0,
        KEY_READ,
        &hKey) == ERROR_SUCCESS)
    {
        RegQueryValueExA(
            hKey,
            "ProcessorNameString",
            NULL,
            NULL,
            (LPBYTE)cpu_name,
            &size);

        RegCloseKey(hKey);
    } else {
        snprintf(cpu_name, size, "Unknown CPU");
    }
}


int main() {
    char cwd[1024];
    char command[512];
    SetConsoleTitle("Nowa Terminal 1.4");
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    printf("Nowa-Terminal v1.4, type \"help\" to see list of commands and \"version\" to see versions of moduls and app.\n");
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
            printf("echo - print text(echo [text])\nls - show info about directory and files in it\ncd - go to directory(cd [dirname])\ncdir - create directory(cdir [dirname])\nrdir - remove directory if it empty(rdir [dirname])\ncurl - use curl(curl [params])\ncfile - create file(cfile [filename.file_extension])\nrfile - remove file(rfile [filename.file_extension])\nread - read file content(read [filename.file_extension])\nclear - clear terminal\ntitle - rename terminal title(title [name])\nsysinfo - info about system\nexit - end session\n");
            SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
        } else if (strncmp(command, "cfile ", 6) == 0) {
            create_file(command + 6);
            continue;
        } else if (strcmp(command, "sysinfo") == 0) {
            char username[256];
            DWORD username_len = sizeof(username);
            GetUserNameA(username, &username_len);

            char computerName[256];
            DWORD computer_len = sizeof(computerName);
            GetComputerNameA(computerName, &computer_len);

            OSVERSIONINFOEXA osvi;
            ZeroMemory(&osvi, sizeof(OSVERSIONINFOEXA));
            osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEXA);
            GetVersionExA((LPOSVERSIONINFOA)&osvi);

            SYSTEM_INFO siSysInfo;
            GetNativeSystemInfo(&siSysInfo);
            SYSTEM_INFO si;
            GetNativeSystemInfo(&si);

            const char* arch;
            switch (si.wProcessorArchitecture) {
                case PROCESSOR_ARCHITECTURE_AMD64:
                    arch = "x64";
                    break;
                case PROCESSOR_ARCHITECTURE_INTEL:
                    arch = "x86";
                    break;
                case PROCESSOR_ARCHITECTURE_ARM:
                    arch = "ARM";
                    break;
                case PROCESSOR_ARCHITECTURE_ARM64:
                    arch = "ARM64";
                    break;
                default:
                    arch = "Unknown";
                    break;
            }
            SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_INTENSITY);
            if(osvi.dwMajorVersion == 10) {
                printf("                    .oodMMMMMMMMMMMM\n");
                printf("       ..oodMMM  MMMMMMMMMMMMMMMMMMM\n");
                printf(" oodMMMMMMMMMMM  MMMMMMMMMMMMMMMMMMM\n");
                printf(" MMMMMMMMMMMMMM  MMMMMMMMMMMMMMMMMMM\n");
                printf(" MMMMMMMMMMMMMM  MMMMMMMMMMMMMMMMMMM\n");
                printf(" MMMMMMMMMMMMMM  MMMMMMMMMMMMMMMMMMM\n");
                printf(" MMMMMMMMMMMMMM  MMMMMMMMMMMMMMMMMMM\n");
                printf(" MMMMMMMMMMMMMM  MMMMMMMMMMMMMMMMMMM\n");
                printf("\n");
                printf(" MMMMMMMMMMMMMM  MMMMMMMMMMMMMMMMMMM\n");
                printf(" MMMMMMMMMMMMMM  MMMMMMMMMMMMMMMMMMM\n");
                printf(" MMMMMMMMMMMMMM  MMMMMMMMMMMMMMMMMMM\n");
                printf(" MMMMMMMMMMMMMM  MMMMMMMMMMMMMMMMMMM\n");
                printf(" MMMMMMMMMMMMMM  MMMMMMMMMMMMMMMMMMM\n");
                printf(" `^^^^^^MMMMMMM  MMMMMMMMMMMMMMMMMMM\n");
                printf("       ````^^^^  ^^MMMMMMMMMMMMMMMMM\n");
                printf("                       ```^^^^^^MMMM\n");
            } else {
                printf("                                      +xXxXXXxXX+                                                   \n");
                printf("                                   XXXXXXXxxxxxxxxxx                                                \n");
                printf("                                  xXXXxxxxxxxxxxxxxx+$                                              \n");
                printf("                                  XXxxxxx+++++++++++$$;+;+             xX;                          \n");
                printf("                                 xxxxxx+++++;;;;;;;xX ;;;;++++xxxxxXXXXX+&&                         \n");
                printf("                                 xxxx++++;;;;;;;;;;+ :;;;;;;++++xxxxXXXXx&                          \n");
                printf("                                xxxx+++;;;;;::::::++ :::;;;;;++++xxxxXX+&&                          \n");
                printf("                               xxxx+++;;;;:::::::;; .:::::;;;;;+++xxxxXX&                           \n");
                printf("                               xxx+++;;;;::::::::::;:::::::;;;;;+++xxx+&&                           \n");
                printf("                              xx+xXxx+++;;;:x+:::. ...::::::;;;;+++xxxX&                            \n");
                printf("                               $X +++;;;::.   .   ;....:::::;;;;+++xxx&&                            \n");
                printf("                             xxxx+++;;:::.....    X....::::::;;;;+++xx$                             \n");
                printf("                             xxxx+++;;;::......      X:::::::;;;+;XXX$$                             \n");
                printf("                            +xxxx+++;;;:::....:    .     ..::;;+++x;                                \n");
                printf("                            xxxxxx+++;;;::::..  ;........::::::::::+X                               \n");
                printf("                           +Xxxxxx++++;;;::::;  ............:..:::;;X                               \n");
                printf("                           XXxxxxxx++++;;;;;:. :.............::::;+X$                               \n");
                printf("                          +XXXxxxxxxx++++;;;+. .............::::;;XX                                \n");
                printf("                          XXXXXXxxxxxxx+++++::............:::::;;+X                                 \n");
                printf("                         +X+XX&&&$$XXxXxxx+;; :.....:..::::::;;;;X$                                 \n");
                printf("                          &&&           xxx+ .:::::::::::::;;;;;+$                                 \n");
                printf("                                             ;:::::::::;;;;;;;;+$$                                 \n");
                printf("                                              xx+:;;;;;;;;;;;+x$&                                  \n");
                printf("                                                  XXXXX$$$$$$&                                     \n");
            }
            SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

            printf("Sysinfo: v1.2.0\n");
            printf("-------------------------------\n");
            printf("User:       %s\n", username);
            printf("Hostname:   %s\n", computerName);
            printf("OS:         Windows %lu.%lu (Build %lu)\n", osvi.dwMajorVersion, osvi.dwMinorVersion, osvi.dwBuildNumber);
            printf("Arch:       %s\n", arch);
            printf("Shell:      Nowa-Terminal\n");
            char cpu_name[256];
            get_cpu_name(cpu_name, sizeof(cpu_name));
            printf("CPU:        %s\n", cpu_name);
            print_command_output("GPU", "wmic path win32_VideoController get name");
            MEMORYSTATUSEX statex;
            statex.dwLength = sizeof(statex);
            if (GlobalMemoryStatusEx(&statex)) {
                printf("RAM:        %llu MB\n", statex.ullTotalPhys / (1024 * 1024));
            }
            print_command_output("BIOS", "wmic bios get smbiosbiosversion");
            continue;
        }
        else if (strncmp(command, "read ", 5) == 0) {
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
