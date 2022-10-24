#include <stdio.h>
#include <iostream>
#include <filesystem>
#include <string.h>
#include <stdlib.h>
#if defined(_WIN32)
    #define PLATFORM_NAME "windows" // Windows
    #include <windows.h>
#elif defined(_WIN64)
    #define PLATFORM_NAME "windows" // Windows
    #include <windows.h>
#endif

using namespace std;

#pragma comment(lib, "ws2_32.lib")

namespace ConFicker
{
    class PortScanner{
        public:
            PortScanner(int* port, char* ip)
            {
                this->port = port;
                this->ip = ip;
            };
            ~PortScanner() {
                delete this->port;
                delete this->ip;
            };
            #if defined(_WIN32)
                struct sockaddr_in {
                    short   sin_family;
                    u_short sin_port;
                    struct  in_addr sin_addr;
                    char    sin_zero[8];
                };
            #endif

            static bool quickConnect()
            {
                #if defined(_WIN32)
                    sAddr.sin_family = AF_INET;
                    sAddr.sin_port = htons(*this->port);
                    s.Addr.sin_addr.s_addr = inet_addr(this->ip);

                    HANDLE tConnThreadl,
                    DWORD dwThreadId;
                    DWORD dwExitCode = 0;

                    tConnThread = CreateThread(
                        NULL, 0,
                        &quickConnectTTL,
                        &sAddr, 0, &dwThreadId
                    );

                    SYSTEMTIME st;
                    GetSystemTime(&st);
                    int finishtime = (st.wDay * 24 * 60 * 60) + (st.wHour * 60 * 60) + (st.wMinute * 60) + st.wSecond + 5;
                    int currentTime = 0;

                    while (currentTime < finishtime)
                    {
                        GetSystemTime(&st);
                        currentTime = (st.wDay * 24 * 60 * 60) + (st.wHour * 60 * 60) + (st.wMinute * 60) + st.wSecond;
                        GetExitCodeThread(tConnThread, &dwExitCode);
                        if (dwExitCode != STILL_ACTIVE)
                        {
                            break;
                        }
                    }

                    if (dwExitCode == STILL_ACTIVE)
                    {
                        TerminateThread(tConnThread, 0);
                        dwExitCode = 0;
                        return false;
                    }
                    
                    CloseHandle(tConnThread);

                    bool present;
                    if (dwExitCode == 1)
                    {
                        present = true;
                    } else {
                        present = false;
                    }

                    return present;
                #endif
            }

            #if defined(_WIN32)
                DWORD WINAPI quickConnectTTL(LPVOID lpParam)
                {
                    SOCKET s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
                    if (s == INVALID_SOCKET)
                    {
                        return 0;
                    }

                    struct sockaddr_in* sAddr = (struct sockaddr_in*)lpParam;

                    if (connect(s, (struct sockaddr*)sAddr, sizeof(*sAddr)) == SOCKET_ERROR)
                    {
                        closesocket(s);
                        return 0;
                    }

                    closesocket(s);
                    return 1;
                }
            #endif

        private:
            int* port;
            char* ip;
    };
}