#include "IpcSockCliant.h"

#include <string>
#include <iostream>
#include <iomanip>

#include "sock_config.h"

int main(void)
{
	std::cout << "start cliant" << std::endl;

 	while(1)
 	{
		std::string str;
		std::cout << "input message : ";
		std::getline(std::cin, str);
		
		if(str.empty())
			continue;

		IpcSockCliant sock(SOCK_PATH);

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
