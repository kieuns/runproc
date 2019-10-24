// runproc.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <iostream>
#include <string.h>
#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include "resource.h"

int main(int argc, char* argv[], char* envp[])
{
    if(argc < 1) {
        printf("Usage: %s [cmdline]", argv[0]);
        return 0;
    }

    std::cout << "argc: " << argc << std::endl;
    for(int i = 0; i < argc; i++) {
        std::cout << "argv[" << i << "]: " << argv[i] << std::endl;
    }

    int iNumberLines = 0;    // Default is no line numbers.
    // If /n is passed to the .exe, display numbered listing
    // of environment variables.
    if((argc == 2) && _stricmp(argv[1], "/n") == 0) { iNumberLines = 1; }
    // Walk through list of strings until a NULL is encountered.
    for(int i = 0; envp[i] != NULL; ++i) {
        if(iNumberLines) { std::cout << i << ": " << envp[i] << "\n"; }
    }

    // CreateProcess() 실행 해보자.

    std::string _cmdline("");
    if(argc >= 1) {
        for(int i = 1; i < argc; i++) {
            _cmdline += argv[i];
            if(i != (argc - 1)) _cmdline += " ";
        }
    }

    std::cout << "RunCmd: " << _cmdline << std::endl;

    STARTUPINFOA si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));
    // Start the child process. 
    if(!CreateProcessA(
        NULL,                                   // No module name (use command line)
        (LPSTR)_cmdline.c_str(),                // Command line
        NULL,                                   // Process handle not inheritable
        NULL,                                   // Thread handle not inheritable
        FALSE,                                  // Set handle inheritance to FALSE
        CREATE_NO_WINDOW | DETACHED_PROCESS,      // creation flags
        NULL,                                   // Use parent's environment block
        NULL,                                   // Use parent's starting directory 
        &si,                                    // Pointer to STARTUPINFO structure
        &pi)                                    // Pointer to PROCESS_INFORMATION structure
        )
    {
        printf("CreateProcess failed (%d).\n", GetLastError());
        return 0;
    }

    //// Wait until child process exits.
    //WaitForSingleObject(pi.hProcess, INFINITE);

    //// Close process and thread handles. 
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    return 1;
}

