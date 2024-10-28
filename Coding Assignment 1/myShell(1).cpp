// myShell.cpp : Simple Command Line Interpreter

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <windows.h>

// Function prototypes
void executeCommand(const std::vector<std::string>& args);
DWORD WINAPI commandThread(LPVOID lpParam);

int main() {
    std::cout << "Welcome to myShell\n";

    std::string input;
    // Supported commands
    std::vector<std::string> supportedCommands = {
        "dir", "help", "vol", "path", "tasklist", "notepad", "echo", "color", "ping"
    };

    // Main loop
    while (true) {
        std::cout << "==> ";
        std::getline(std::cin, input);

        // Parse input
        std::istringstream iss(input);
        std::vector<std::string> args;
        std::string arg;
        while (iss >> arg) {
            args.push_back(arg);
        }

        // Check if no command was entered
        if (args.empty()) {
            continue;
        }

        // Check for exit command
        if (args[0] == "exit" || args[0] == "quit") {
            std::cout << "Thanks for using myShell!\n";
            break;
        }

        // Check if command is supported
        if (std::find(supportedCommands.begin(), supportedCommands.end(), args[0]) != supportedCommands.end()) {
            HANDLE hThread = CreateThread(NULL, 0, commandThread, &args, 0, NULL);
            if (hThread) {
                WaitForSingleObject(hThread, INFINITE);
                CloseHandle(hThread);
            } else {
                std::cerr << "Error creating thread\n";
            }
        } else {
            std::cout << "Command not recognized. Type 'help' for a list of commands.\n";
        }
    }

    return 0;
}

// Thread function to execute command
DWORD WINAPI commandThread(LPVOID lpParam) {
    std::vector<std::string>* args = static_cast<std::vector<std::string>*>(lpParam);
    executeCommand(*args);
    return 0;
}

// Function to execute command
void executeCommand(const std::vector<std::string>& args) {
    std::string command = args[0];
    
    // Excute system command
    if (command == "dir" || command == "vol" || command == "path" || command == "tasklist") {
        std::string fullCommand = "cmd /c " + command;
        system(fullCommand.c_str());

    // display help information
    } else if (command == "help") {
        std::cout << "Supported commands: dir, help, vol, path, tasklist, notepad, echo, color, ping\n";
    } 
    // notepad launch
    else if (command == "notepad") {
        system("start notepad");
    } 
    // echo command
    else if (command == "echo") {
        for (size_t i = 1; i < args.size(); ++i) {
            std::cout << args[i] << " ";
        }
        std::cout << "\n";
    } 
    // console color change
    else if (command == "color") {
        if (args.size() > 1) {
            std::string fullCommand = "cmd /c color " + args[1];
            system(fullCommand.c_str());
        } else {
            std::cout << "Usage: color [color_code]\n";
        }
    } 
    // ping command
    else if (command == "ping") {
        if (args.size() > 1) {
            std::string fullCommand = "ping " + args[1];
            system(fullCommand.c_str());
        } else {
            std::cout << "Usage: ping [ip_address]\n";
        }
    }
}