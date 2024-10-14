// myShell.cpp : Interactive Command Line Shell

#include <iostream>
#include <windows.h>
#include <string.h>
#include <stdio.h>

#define MAX_COMMAND_LENGTH 100

// Supported commands
const char* commands[] = { "dir", "help", "vol", "path", "tasklist", "notepad", "echo", "color", "ping", nullptr };

// Function to check if command is supported
bool is_supported_command(const char* cmd) {
    for (int i = 0; commands[i] != nullptr; i++) {
        if (strcmp(cmd, commands[i]) == 0) {
            return true;
        }
    }
    return false;
}

// Thread function to run the command
DWORD WINAPI run_command(LPVOID lpParam) {
    char* command = (char*)lpParam;
    system(command);  // Execute the command in the system shell
    return 0;
}

int main() {
    char input[MAX_COMMAND_LENGTH];
    char* token;
    char* args[4];  // Assuming a maximum of 4 arguments
    DWORD threadId;
    HANDLE threadHandle;

    std::cout << "Welcome to myShell\n";

    while (true) {
        std::cout << "==> ";
        fgets(input, MAX_COMMAND_LENGTH, stdin);

        // Remove the newline character from the input
        input[strcspn(input, "\n")] = 0;

        // Exit condition
        if (strcmp(input, "exit") == 0 || strcmp(input, "quit") == 0) {
            std::cout << "Thanks for using myShell!\n";
            break;
        }

        // Parse input into command and arguments
        token = strtok(input, " ");
        int i = 0;
        while (token != nullptr && i < 4) {
            args[i++] = token;
            token = strtok(nullptr, " ");
        }

        // Check if the command is supported
        if (is_supported_command(args[0])) {
            // Create a new thread to execute the command
            threadHandle = CreateThread(nullptr, 0, run_command, (LPVOID)input, 0, &threadId);
            
            // Wait for the thread to finish
            WaitForSingleObject(threadHandle, INFINITE);
            CloseHandle(threadHandle);
        } else {
            std::cout << "Command not supported!\n";
        }
    }

    return 0;
}