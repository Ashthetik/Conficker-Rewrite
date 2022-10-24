#ifndef KBCheck_hpp
#define KBCheck_hpp

#include <iostream>
#include <string>
#include <filesystem>
// Check if it's Windows    
#ifdef _WIN32
#include <windows.h>
#include <winuser.h>
#endif
// check if it's linux
#ifdef __linux__
#include <X11/Xlib.h>
#endif
// Check if it's Mac
#ifdef __APPLE__
#include <ApplicationServices/ApplicationServices.h>
#endif

namespace ConFicker 
{
    class KBCheck
    {
        public:
            KBCheck() = default;
            ~KBCheck() = default;
            bool isKeyBoardHooked();
            static int findKeyBoardLayout()
            {
                // For windows:
                #ifdef _WIN32
                HKL hkl = GetKeyboardLayout(0);
                if (hkl == 0x04090409)
                {
                    return 1;
                }
                else if (hkl == 0x4190419)
                {
                    return 0;
                }
                else 
                {
                    return 1;
                }
                #endif
                // For Linux:
                #ifdef __linux__
                Display *display = XOpenDisplay(NULL);
                if (display == NULL)
                {
                    return 1;
                }
                char *xmodmap = XSetLocaleModifiers("");
                if (xmodmap == NULL)
                {
                    return 1;
                }
                XModifierKeymap *modmap = XGetModifierMapping(display);
                if (modmap == NULL)
                {
                    return 1;
                }
                #endif

                // For Mac:
                #ifdef __APPLE__
                TISInputSourceRef currentKeyboard = TISCopyCurrentKeyboardInputSource();
                CFDataRef uchr = (CFDataRef)TISGetInputSourceProperty(currentKeyboard, kTISPropertyUnicodeKeyLayoutData);
                const UCKeyboardLayout *keyboardLayout = (const UCKeyboardLayout*)CFDataGetBytePtr(uchr);
                if (keyboardLayout == NULL)
                {
                    return 1;
                }
                else {
                    return 0;
                }
                #endif
            }
    };
};

#endif // KBCheck_hpp