#include "IpcSockClient.h"

#include <string>
#include <iostream>
#include <iomanip>

#include "sock_config.h"

int main(void)
{
    std::cout << "start client" << std::endl;

    while(1)
    {
        std::string str;
        std::cout << "input message : ";
        std::getline(std::cin, str);

        if(str.empty())
            continue;

        IpcSockClient sock(SOCK_PATH);

        sock.init();

        if(sock.start()) {
            sock.send(str.c_str(), str.size());
            sock.stop();
        }
        else {
            break;
        }
    }
    return 0;
}
