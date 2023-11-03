// /**
//  * @file DCC.cpp
//  * @author Görkem SEVER (gsever), Ahmet KARACA(akaraca)
//  * @brief 
//  * @version 0.1
//  * @date 2023-09-03
//  * 
//  * @link: https://github.com/irssi/irssi/tree/master/src/irc/dcc
//  * @note: DCC (Direct Client-to-Client) mean; Send file with IRC.
//  * 
//  * @link: https://en.wikipedia.org/wiki/Reserved_IP_addresses
//  * @link: http://www.iana.org/numbers
//  * 
//  * @copyright Copyright (c) 2023
//  * 
//  */
// #include "../../includes/Server.hpp"

// /**
//  * @brief 
//  * 
//  * @note;
//  * IRC (Internet Relay Chat) DCC (Direct Client-to-Client) komutları,
// 	kullanıcıların birbirleriyle doğrudan bağlantı kurmasını sağlar.

// 1.	**DCC CHAT {nick}**: Bu komut, belirtilen takma adla bir sohbet oturumu başlatır.
// Örneğin, "DCC CHAT john" komutu, "john" adlı kullanıcıyla bir sohbet oturumu başlatır.
// Bu hizmet, kullanıcıların bir DCC bağlantısı üzerinden birbirleriyle sohbet etmelerini sağlar.
// Trafik doğrudan kullanıcılar arasında gider ve IRC ağı üzerinden gitmez.
// DCC CHAT genellikle bir CTCP (Client-To-Client Protocol) HANSHAKE(el sıkışması) kullanılarak başlatılır.

// 2.	**DCC SEND {nick} {dosya}**: Bu komut, belirtilen takma ada belirtilen dosyayı gönderir.
// Örneğin, "DCC SEND john image.jpg" komutu, "john" adlı kullanıcıya "image.jpg" dosyasını gönderir.

// 3.	**DCC GET {nick} {dosya}**: Bu komut, belirtilen takma addan belirtilen dosyayı alır.
// Örneğin, "DCC GET john image.jpg" komutu, "john" adlı kullanıcıdan "image.jpg" dosyasını alır.

// 4.	**DCC RESUME {nick} {dosya} {port} {position}**: Bu komut, belirtilen takma adla belirtilen 
//  dosyanın belirtilen konumdan indirilmesini sürdürür.
// Örneğin, "DCC RESUME john image.jpg 5000 1024" komutu, "john" adlı kullanıcıdan "image.jpg"
//  dosyasının 1024. bayttan itibaren indirilmesini sürdürür.

// 5.	**DCC ACCEPT {nick} {dosya} {port} {position}**: Bu komut, belirtilen takma adla belirtilen
//  dosyanın belirtilen konumdan indirilmesini kabul eder.
// Örneğin, "DCC ACCEPT john image.jpg 5000 1024" komutu, "john" adlı kullanıcıdan "image.jpg"
//  dosyasının 1024. bayttan itibaren indirilmesini kabul eder.

// Bu komutlar genellikle IRC istemcilerinde bulunur ve doğrudan IRC sunucusu yerine
//  kullanıcılar arasında bağlantı kurarlar.
//  * 
//  */


// /*
// DCC (Direct Client-to-Client) protokolü, IRC (Internet Relay Chat) istemcileri arasında doğrudan bağlantılar kurmak için kullanılır¹. DCC bağlantıları, IRC ağı ve sunucularını atlayarak istemciler arasında çeşitli verilerin aktarılmasına olanak sağlar². İşte DCC protokolünün bazı temel komutları ve nasıl çalıştıkları:

// 1. **DCC CHAT**: Bu hizmet, kullanıcıların bir DCC bağlantısı üzerinden birbirleriyle sohbet etmelerini sağlar¹. Trafik doğrudan kullanıcılar arasında gider ve IRC ağı üzerinden gitmez¹. DCC CHAT genellikle bir CTCP (Client-To-Client Protocol) el sıkışması kullanılarak başlatılır¹.

// 2. **DCC SEND**: Bu hizmet, bir dosyanın bir kullanıcıdan diğerine gönderilmesini sağlar¹. Dosya transferi genellikle bir CTCP el sıkışması kullanılarak başlatılır¹.

// 3. **DCC RESUME**: Bu komut, durdurulmuş bir dosya transferini sürdürmek için kullanılır. Örnek: `/DCC RESUME KullaniciAdi dosya.zip`

// 4. **DCC ACCEPT**: Bu komut, durdurulmuş bir dosya transferinin devam etmesini kabul etmek için kullanılır. Örnek: `/DCC ACCEPT KullaniciAdi dosya.zip`

// 5. **DCC CLOSE**: Bu komut, bir DCC bağlantısını kapatmak için kullanılır. Örnek: `/DCC CLOSE get KullaniciAdi`

// Bu komutların her biri, belirli bir DCC işlemi gerçekleştirmek için kullanılır. Her komutun tam olarak nasıl çalıştığını anlamak için, belirli bir IRC istemcisinin belgelerine veya yardım dosyalarına başvurmanız önerilir.

// Umarım bu bilgiler yardımcı olur! Başka bir sorunuz varsa, lütfen sormaktan çekinmeyin. 😊

// Source: Conversation with Bing, 03.09.2023
// (1) Direct Client-to-Client - Wikipedia. https://en.wikipedia.org/wiki/Direct_Client-to-Client.
// (2) Direct Client-to-Client Protocol (DCC). https://modern.ircdocs.horse/dcc.html.
// (3) List of Internet Relay Chat commands - Wikipedia. https://en.wikipedia.org/wiki/List_of_Internet_Relay_Chat_commands.
// */

// // DCC send <nick_name> <file>
// // DCC get <nick_name> <file>
// void	Server::dcc( Client* it, std::vector<std::string> tokenArr )
// {
// 	std::cout << YELLOW << "DCC" << END << std::endl;

// 	if (it->getIRCstatus() != AUTHENTICATED)
// 	{
// 		it->sendMessageFd(RPL_NOTICE(it->getPrefix(), it->getNickname(), "Client's status is insufficient."));
// 		return ;
// 	}

// 	if (tokenArr.size() < 4)
// 		throw( std::out_of_range("") );

// 	if (Server::getClientFdByNickname(tokenArr[2]) == -1)
// 	{
// 		it->sendMessageFd(ERR_NOSUCHNICK(it->getPrefix(), tokenArr[2]));
// 		return ;
// 	}

// 	this->_dccSubVector = std::vector<std::string>(tokenArr.begin() + 2, tokenArr.end());
// 	this->_dccSubVector.insert(this->_dccSubVector.begin(), it->getNickname());

// 	#if defined(__APPLE__)
// 	pthread_t	dccThreadID = NULL;
// 	#elif defined(linux)
// 	pthread_t	dccThreadID;
// 	#endif


// 	if (tokenArr[1].compare("SEND") == 0)
// 	{
// 		if (pthread_create(&dccThreadID, NULL, &Server::dccSend, this) != 0)
// 			throw (std::runtime_error("pthread_create: Thread 'create' error!"));
// 	}
// 	else if (tokenArr[1].compare("GET") == 0)
// 	{
// 		if (pthread_create(&dccThreadID, NULL, &Server::dccGet, this) != 0)
// 			throw (std::runtime_error("pthread_create: Thread 'create' error!"));
// 	}		
// 	else
// 		it->sendMessageFd(ERR_UNKNOWNCOMMAND(it->getPrefix(), tokenArr[0] + " " + tokenArr[1]));

// 	if (pthread_detach(dccThreadID) != 0) // threadi beklemek istemiyoruz bu yüzden join kullanmıyoruz bunun yerine sadece detach kullanıyoruz.
// 		throw (std::runtime_error("pthread_detach: Thread 'detach' error!"));


// 	// Sender dosyayı göndermek için alıyıcıya bilgi göndersin ve alıcıdan mesaj beklesin
// 	//	Eğer alıcı dosya almak işlemini kabul ederse, alıcıda yeni bir socket oluşturulsun ve accept işlemine geçilsin
// 	//		Alıcı yeni port açıp bu bilgiye sender'a bildirmelidir ve bu mesaj işlenmelidir.
// 	//	Sender geri dönüt yanıtına göre socket oluşturup, alıcıya bağlansın ve dosya gönderme işlemini yerine getirsin.
// }

// // <nick_name> <file>
// void* Server::dccSend( void* arg )
// {
// 	std::cout << YELLOW << "DCC-SEND" << END << std::endl;

// 	Server*	server;
// 	server = static_cast<Server*>(arg);

// 	std::cout << server->_dccSubVector[0] << std::endl; // sender
// 	std::cout << server->_dccSubVector[1] << std::endl; // receiver
// 	std::cout << server->_dccSubVector[2] << std::endl; // file

// 	Client* sender = server->getClientByNickname(server->_dccSubVector[0]);
// 	Client* receiver = server->getClientByNickname(server->_dccSubVector[1]);
// 	std::string filePath = server->_dccSubVector[2];

// 	std::string hostname = receiver->getHostname();
// 	const char* receiverIP = hostname.c_str();

// 	int	receiverPort = receiver->getPort();
// 	// int	receiverPort = 6666;



// // ---------------------------- DOSYA KONTROLU ---------------------------- //
// 	struct stat info;

// 	if (stat(filePath.c_str(), &info) != -1)
// 	{
// 		if (!S_ISREG(info.st_mode))
// 		{
// 			sender->sendMessageFd(ERROR(sender->getPrefix(), "DCC-SEND This is not file!"));
// 			std::cerr << "DCC-SEND This is not file!" << std::endl;
// 			pthread_exit((void *)EXIT_FAILURE);
// 		}
// 	}
// 	else
// 	{
// 		sender->sendMessageFd(ERROR(sender->getPrefix(), "DCC-SEND " + strerror(errno)));
// 		std::cerr << strerror(errno) << std::endl;
// 		pthread_exit((void *)EXIT_FAILURE);
// 	}

// 	std::string _buffer;
// 	char buffer[4096];
// 	FILE *fd = fopen(filePath.c_str(), "rb");

// 	while (!feof(fd)) {
// 		int size = fread(&buffer, 1, 256, fd);
// 		if (size < 0)
// 			break;

// 		_buffer.append(buffer, size);
// 	}

// 	fclose(fd);

// // ---------------------------- HEDEFE BILGI GONDERME ---------------------------- //
// 	std::stringstream Port, Size;
// 	Port << receiverPort;
// 	Size << info.st_size;

// 	std::size_t last_dot_pos = filePath.find_last_of('/');
// 	std::string fileName = filePath.substr(last_dot_pos + 1);

// 	std::string message = GREEN "DCC SEND from " + sender->getNickname() + END " [127.0.0.1 port " + Port.str() + "]: " + fileName + " [" + Size.str() + "B]";
// 	receiver->sendMessageFd(message);

// // ---------------------------- SOCKET OLUŞTURMA ---------------------------- //
// //Eğer server, her iki client'in soketlerini kullanmak için yeni bir soket oluşturmazsa, aşağıdaki sorunlarla karşılaşabilir:
// //Dosya aktarımı, başka client'ler tarafından dinlenebilir.
// //Dosya bilgilerinin şifrelenmemesi durumunda, dosyalar güvenli bir şekilde iletilemez.
// //Dosya transferi sırasında bir hata meydana gelirse, dosya transferi tamamlanamaz.
// //Bu nedenle, IRC server'da file transfer için yeni soket oluşturmak her zaman gereklidir.
//     int dccSocket = socket(AF_INET, SOCK_STREAM, 0);
//     if (dccSocket == -1) {
// 		sender->sendMessageFd(ERROR(sender->getPrefix(), "DCC-SEND " + strerror(errno)));
//         perror("Socket creation failed");
//         pthread_exit((void *)EXIT_FAILURE);
//     }

// 	int val = 1;
// 	if (setsockopt(dccSocket, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val)))
// 		throw std::runtime_error("Error while setting socket options.");

// // ---------------------------- HEDEF BAGLANTIYI DINLEME ---------------------------- //

// 	struct sockaddr_in dccServerAddress = {};
// 	int dcc_address_len = sizeof(dccServerAddress);

// 	bzero((char *) &dccServerAddress, dcc_address_len);

// 	dccServerAddress.sin_family = AF_INET;
// 	dccServerAddress.sin_addr.s_addr = inet_addr(receiverIP);//INADDR_ANY;
// 	dccServerAddress.sin_port = htons(receiverPort);

//     if (bind(dccSocket, (struct sockaddr*)&dccServerAddress, sizeof(dccServerAddress)) == -1) {
//         perror("Bind failed");
//         close(dccSocket);
//         pthread_exit((void*)EXIT_FAILURE);
//     }

// 	std::cout << ".............1.............." << std::endl;
//     if (listen(dccSocket, 1) == -1) { // 1 bağlantıyı kabul etmek için sınırlıyoruz
//         perror("Listen failed");
//         close(dccSocket);
//         pthread_exit((void*)EXIT_FAILURE);
//     }

// 	std::cout << ".............2.............." << std::endl;
// // ---------------------------- HEDEF BAGLANTIYA BAGLANMA ---------------------------- //

// 	int client_fd = accept(dccSocket, (struct sockaddr *) &dccServerAddress, (socklen_t *) &dcc_address_len);
// 	if (client_fd < 0) {
// 		close(dccSocket);
//         pthread_exit((void*)EXIT_FAILURE);
// 	}
// 	std::cout << "............3..............." << std::endl;

// // ---------------------------- HEDEFE DOSYAYI GONDERME ---------------------------- //

// 	send(client_fd, _buffer.c_str(), _buffer.size(), 0);

// 	close(client_fd);
// 	close(dccSocket);
// 	_buffer.clear();

// 	pthread_exit(EXIT_SUCCESS);
// }

// // <nick_name> <file>
// void* Server::dccGet( void* arg )
// {
// 	std::cout << YELLOW << "DCC-GET" << END << std::endl;

// 	Server*	server;
// 	server = static_cast<Server*>(arg);
	
// 	std::cout << server->_dccSubVector[0] << std::endl; // receiver
// 	std::cout << server->_dccSubVector[1] << std::endl; // sender
// 	std::cout << server->_dccSubVector[2] << std::endl; // file

// 	Client* receiver = server->getClientByNickname(server->_dccSubVector[0]);
// 	Client* sender = server->getClientByNickname(server->_dccSubVector[1]);
// 	std::string fileName = server->_dccSubVector[2];

// 	std::string hostname = sender->getHostname();
// 	const char* senderIP = hostname.c_str();

// 	int	receiverPort = receiver->getPort();
// 	// int	receiverPort = 6666;

//     int dccSocket = socket(AF_INET, SOCK_STREAM, 0);
//     if (dccSocket == -1) {
//         perror("Socket creation failed");
// 		pthread_exit((void *)EXIT_FAILURE);
//     }

// 	struct sockaddr_in clientAddress = {};
// 	int client_address_len = sizeof(clientAddress);

// 	bzero((char *) &clientAddress, client_address_len);

// 	clientAddress.sin_family = AF_INET;
// 	clientAddress.sin_addr.s_addr = inet_addr(senderIP);
// 	clientAddress.sin_port = htons(receiverPort);

// 	std::cout << "----------------1------------------" << std::endl;

// 	if (connect(dccSocket, (struct sockaddr*)&clientAddress, sizeof(clientAddress)) == -1) {
// 		perror("Connection failed");
// 		close(dccSocket);
// 		pthread_exit((void *)EXIT_FAILURE);
// 	}

// 	std::cout << "----------------2------------------" << std::endl;

// // ---------------------------- DOSYAYI ALMA ---------------------------- //

//     // Dosyayı oluştur ve verileri yaz
//     std::ofstream outFile(fileName.c_str(), std::ios::binary);
//     if (!outFile) {
//         std::cerr << "Dosya oluşturma hatası." << std::endl;
//         close(dccSocket);
// 		pthread_exit((void *)EXIT_FAILURE);
//     }

//     char buffer[1024];
//     ssize_t bytesRead;
//     while ((bytesRead = recv(dccSocket, buffer, sizeof(buffer), 0)) > 0) {
//         outFile.write(buffer, bytesRead);
//     }

//     if (bytesRead == -1) {
//         std::cerr << "Veri alma hatası." << std::endl;
//         outFile.close();
//         close(dccSocket);
// 		pthread_exit((void *)EXIT_FAILURE);
//     }

//     // Dosya ve soketleri kapat
//     outFile.close();
//     close(dccSocket);
// 	std::cout << "File received successfully." << std::endl;

// 	pthread_exit(EXIT_SUCCESS);
// }
