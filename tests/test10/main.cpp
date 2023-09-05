#include "server.hpp"

int main()
{
    int sockFd = createSocket(8080);
    sockaddr_in cliAddr;
    socklen_t cliSize;
    int cliFd;
    int readed;
    char buffer[1024];
    char clientIP[INET_ADDRSTRLEN];
    int cliPort;
    client tmp;
    server mysw;
    mysw.serverFd = sockFd;
    cliSize = sizeof(sockaddr_in);
    cliFd = -1;
    int state = 0;


    fd_set readFds;
    fd_set writeFds;
    fd_set readFdsSup;
    fd_set writeFdsSup;

    FD_ZERO(&readFds);
    FD_ZERO(&writeFds);
    FD_ZERO(&readFdsSup);
    FD_ZERO(&writeFdsSup);



    FD_SET(sockFd, &readFds);
    while (1)
    {
        while (state == 0)
        {
            readFdsSup = readFds;
            writeFdsSup = writeFds;
            state = select(mysw.findMaxFd() + 1, &readFdsSup, &writeFdsSup, NULL, 0);
        }

        if (FD_ISSET(sockFd, &readFdsSup))
        {
            cliFd = accept(sockFd, (sockaddr *)&cliAddr, &cliSize);
            cliPort = ntohs(cliAddr.sin_port);
            inet_ntop(AF_INET, &(cliAddr.sin_addr), clientIP, INET_ADDRSTRLEN);
            tmp.cliFd = cliFd;
            tmp.ipAddr = clientIP;
            tmp.port = cliPort;
            mysw.clients.push_back(tmp);
            FD_SET(cliFd, &readFds);

            std::cout << "New Client Connected!" << std::endl;
            state = 0;
        }

        //read event
        for(std::vector<client>::iterator begin = mysw.clients.begin(); begin != mysw.clients.end() && state;++begin)
        {
            if (FD_ISSET((*begin).cliFd, &readFdsSup))
            {
                readed = read((*begin).cliFd, buffer, 1024);
                if (readed <= 0)
                {
                    FD_CLR((*begin).cliFd, &readFds);
                    FD_CLR((*begin).cliFd, &writeFds);
                    close((*begin).cliFd);
                    mysw.clients.erase(begin);
                    std::cout << "A client disconnected!" << std::endl;
                }
                else
                {
                    buffer[readed] = 0;
                    for(std::vector<client>::iterator it = mysw.clients.begin(); it != mysw.clients.end();++it)
                    {
                        if (it != begin)
                        {
                            (*it).messageBox.push_back(buffer);
                            FD_SET((*it).cliFd, &writeFds);
                        }

                    }
                }
                state = 0;
                break;
            }
        }



        //write event
        for(std::vector<client>::iterator begin = mysw.clients.begin(); begin != mysw.clients.end() && state;++begin)
        {
            if (FD_ISSET((*begin).cliFd, &writeFdsSup))
            {
                readed = write((*begin).cliFd, (char *)(*begin).messageBox[0].c_str(), (*begin).messageBox[0].length());
                (*begin).messageBox.erase((*begin).messageBox.begin());

                if ((*begin).messageBox.empty())
                    FD_CLR((*begin).cliFd, &writeFds);

                if (readed <= 0)
                {
                    FD_CLR((*begin).cliFd, &readFds);
                    FD_CLR((*begin).cliFd, &writeFds);
                    close((*begin).cliFd);
                    mysw.clients.erase(begin);
                    std::cout << "A client disconnected!" << std::endl;
                }
                state = 0;
                break;
            }
        }
    }


}