// #include <iostream>
// #include <socket.h>

// int	main()
// {
// 	int	ipAddress;

// 	ipAddress = system("ipconfig getifaddr en0");
// 	std::cout << "ip:>" << ipAddress << "<" << std::flush;

// 	// std::cout << system("ipconfig getifaddr en0") << "<" << std::endl << std::flush;
// 	return (0);
// }


// ************  IKINCI KISIM ************************************************************************
// #include <iostream>
// #include <cstdio>
// #include <cstdlib>
// #include <cstring>

// int main() {
//     char buffer[128];
//     std::string localIP;
//     std::string publicIP;

// #ifdef _WIN32
//     // Windows için ipconfig komutunu kullanarak local IP al
//     std::system("ipconfig | findstr /R \"Ethernet adapter.*IPv4\"");
// #else
//     // Unix/Linux için ifconfig komutunu kullanarak local IP al
//     std::system("ifconfig | grep 'inet ' | awk '{print $2}'");
// #endif

//     // httpbin.org servisini kullanarak public IP al
//     std::system("curl ifconfig.me");

//     return 0;
// }

// *************   UCUNCU KISIM    	******************************************************************

// #include <iostream>
// #include <cstdio>
// #include <cstdlib>
// #include <cstring>

// int main() {
//     char buffer[128];
//     std::string localIP;
//     std::string publicIP;

// #ifdef _WIN32
//     // Windows için ipconfig komutunu kullanarak local IP al
//     FILE* pipe = _popen("ipconfig | findstr /R \"Ethernet adapter.*IPv4\"", "r");
// #else
//     // Unix/Linux için ifconfig komutunu kullanarak local IP al
//     FILE* pipe = popen("ifconfig | grep 'inet ' | awk '{print $2}'", "r");
// #endif

//     if (!pipe) {
//         std::cerr << "Hata: Komut çalıştırılamadı!" << std::endl;
//         return -1;
//     }

//     while (fgets(buffer, sizeof(buffer), pipe) != NULL) {
//         localIP += buffer;
//     }

// #ifdef _WIN32
//     _pclose(pipe);
// #else
//     pclose(pipe);
// #endif

//     // httpbin.org servisini kullanarak public IP al
//     pipe = popen("curl ifconfig.me", "r");

//     if (!pipe) {
//         std::cerr << "Hata: Komut çalıştırılamadı!" << std::endl;
//         return -1;
//     }

//     while (fgets(buffer, sizeof(buffer), pipe) != NULL) {
//         publicIP += buffer;
//     }

// #ifdef _WIN32
//     _pclose(pipe);
// #else
//     pclose(pipe);
// #endif

//     std::cout << "Local IP: " << localIP << std::endl;
//     std::cout << "Public IP: " << publicIP << std::endl;

//     return 0;
// }

// ******************    DORDUNCU KISIM ***********************************************
#include <iostream>
#include <cstdio> // For popen
#include <cstring> // For fgets
#include <unistd.h>

std::string exec(const char* cmd)
{
    char buffer[128];
    std::string result = "";
    FILE* pipe = popen(cmd, "r");
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }
    while (fgets(buffer, sizeof(buffer), pipe) != NULL) {
        result += buffer;
    }
    pclose(pipe);
    return result;
}

int main() {
    // const char* cmd = "ipconfig getifaddr en0"; // "en0" interface for Wi-Fi, "en1" for Ethernet
    // std::string localIp = exec("ipconfig getifaddr en0");
    // std::string publicIp = exec("curl ifconfig.me");

    char command[100] = "ipconfig getifaddr en0";
    execl("/bin/sh", "sh", "-c", command, (char *)0);

    // std::cout << "Local IP Address: " << localIp;
    // std::cout << "Public IP Address: " << publicIp;
    
    return 0;
}
