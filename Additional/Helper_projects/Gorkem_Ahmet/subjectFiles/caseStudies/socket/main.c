# include <netinet/in.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/socket.h>
# include <unistd.h>
# define PORT 8080
// server'a bağlantı kurmak için: $> telnet 127.0.0.1 8080
// bir server oluşturmak için gerekli olan fonksiyonlar: socket(), bind(), listen() ve accept()
int main()
{
	int server_fd, new_socket, valread; // dosya tanımlayıcıları ve veri uzunluğunu saklamak için kullanılacak değişkenlerdir.
	struct sockaddr_in address; // IPv4 adresini ve port numarasını depolamak için kullanılan yapıdır.
	int opt = 1; // soket seçeneklerini yapılandırmak için kullanılan bir değişkendir.
	int addrlen = sizeof(address); // soket adresinin boyutunu tutar.
	char buffer[1024] = { 0 }; // veri alışverişi için kullanılacak bir tampon dizisidir.
	char* hello = "Hello from server\n"; // istemcilere gönderilecek mesajını içeren bir karakter dizisidir.

	// socket(); fonksiyonu bir socket oluşturur.
	// 'AF_INET' adres ailesi olarak IPv4'ü belirtir.
	// 'SOCK_STREAM' soket türü olarak TCP'yi belirtir.
	// Oluşturulan soketin tanımlayıcısı server_fd değişkenine atanır.
	// Eğer soket oluşturulamazsa, hata mesajı yazdırılır ve program EXIT_FAILURE ile sonlandırılır.
	// socket()'in 3.parametresi fonksiyonun default olarak kullanıldığını belirtir.
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket failed");
		exit(EXIT_FAILURE);
	}
	else
		printf("%s fd:%d\n", "socket fd succesfuly created", server_fd);

	// Bu bölüm, oluşturulan soketin yapılandırılmasını ve bağlantı için gerekli adres bilgilerini ayarlamayı içerir.
	// "SO_REUSEADDR" ve "SO_REUSEPORT" seçenekleri, sunucu soketinin zaten kullanımda olsa bile yeniden kullanılmasını sağlar.
	//	Bu, bağlantıları kaybetmeden yeniden başlatılabilen sunucular için kullanışlıdır.
	// "SO_REUSEADDR" seçeneği, bir soketin zaten kullanımda olsa bile yeniden kullanılmasını sağlar.
	//	Bu, bağlantıları kaybetmeden yeniden başlatılabilen sunucular için kullanışlıdır.
	// "SO_REUSEPORT" seçeneği, birden fazla soketin aynı bağlantı noktasında dinlemesini sağlar.
	//	Bu, yük dengeleyiciler ve diğer, trafiği birden çok sunucu arasında dağıtmak zorunda olan uygulamalar için kullanışlıdır.
	// SOL_SOCKET seçeneği, soketlerle ilgili genel ayarları değiştirmek için kullanılır.
	// &opt ve sizeof(opt) parametreleri, soket seçeneği değerini belirtmek için kullanılır.
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}
	else
		printf("%s\n", "socket settings changed");

	address.sin_family = AF_INET; // adres yapısının aile türünü (AF_INET, yani IPv4) belirtir.
	address.sin_addr.s_addr = INADDR_ANY; // sunucunun herhangi bir ağ arabirimine bağlanabileceğini belirtir. Bu, sunucunun tüm ağ arabirimlerini dinlemesini sağlar.
	address.sin_port = htons(PORT); // port numarasını yapılandırır. htons işlevi, "host byte order"ı (endianness) "network byte order"a dönüştürmek için kullanılır. Bu, port numarasının doğru şekilde iletilmesini sağlar.

	// Bu bölüm, oluşturulan soketi belirli bir adres ve port numarasına bağlamayı amaçlar.
	//	bind() işlemi, sunucunun belirli bir adresi ve portu dinlemeye başlamasını sağlar.
	// İlk parametre, bağlamak istediğimiz soketin dosya tanımlayıcısı olan server_fd'yi içerir.
	//	İkinci parametre, bir struct sockaddr türüne dönüştürülen adres yapısını içerir.
	//	Üçüncü parametre, adres yapısının boyutunu belirtir.
	// Eğer bu işlem başarılı bir şekilde gerçekleşmezse, sunucu gelen bağlantıları kabul edemeyeceği için çalışmaz.
	if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
		perror("bind failed");
		exit(EXIT_FAILURE);
	}
	else
		printf("%s\n", "server is connecting");

	// Bu bölüm, sunucunun gelen bağlantıları dinlemeye başlamasını sağlar.
	//	listen işlemi, sunucunun belirli bir soketi kullanarak gelen bağlantıları kabul edebilmesi için gerekli olan altyapıyı oluşturur.
	// İlk parametre, dinlemek istediğimiz soketin dosya tanımlayıcısı olan server_fd'yi içerir.
	//	İkinci parametre, aynı anda kaç bağlantının sıraya girebileceğini belirten bir tam sayıdır.
	//	Bu değer genellikle backlog olarak adlandırılır ve sistem tarafından desteklenen maksimum değere sınırlanabilir.
	if (listen(server_fd, 3) < 0) {
		perror("listen");
		exit(EXIT_FAILURE);
	}
	else
		printf("%s\n", "server is listening");

	// Bu bölüm, sunucunun gelen bağlantıları kabul etmeyi ve yeni bir soket tanımlayıcısı oluşturarak bu bağlantıyı yönetmeyi amaçlar.
	//	accept işlemi, sunucunun bekleyen bağlantıları alarak bu bağlantılara yeni bir soket atar ve bu soketi kullanarak iletişim kurmasını sağlar.
	// İlk parametre, dinlemekte olan soketin dosya tanımlayıcısı olan server_fd'yi içerir.
	//	İkinci parametre, bağlantıyı sağlayan istemcinin adres bilgisini almak için kullanılır.
	//	Üçüncü parametre, struct sockaddr türünde bir adres yapısının boyutunu tutar.
	// Eğer accept işlemi başarılı bir şekilde gerçekleşirse, yeni bir soket tanımlayıcısı olan new_socket döner.
	//	Bu yeni soket, sunucunun gelen bağlantıyı yönetmesi için kullanılır.
	//	Eğer bir hata oluşursa, -1 döner.
	if ((new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen)) < 0) {
		perror("accept");
		exit(EXIT_FAILURE);
	}
	else
		printf("%s\n", "server is accepting");

	// Kabul edilen new_socket soketi üzerinden istemciden gelen veriyi buffer adlı karakter dizisine okur.
	//	1024 burada okunacak maksimum bayt sayısını belirtir.
	//	Okunan bayt sayısı valread değişkenine atanır.
	valread = read(new_socket, buffer, 1024);

	// buffer karakter dizisindeki okunan veriyi ekrana yazdırır.
	//	Yani, istemciden gelen veriyi sunucu ekranına basar.
	printf("%s\n", buffer);

	// new_socket soketi üzerinden istemciye hello adlı karakter dizisini gönderir.
	//	strlen(hello) ifadesi, "Merhaba" mesajının uzunluğunu belirtir.
	//	0 ise iletim sırasında kullanılan ek seçeneklerdir.
	send(new_socket, hello, strlen(hello), 0);

	// Bu satır, "Merhaba" mesajının istemciye başarıyla gönderildiğini ekrana yazdırır.
	printf("Hello message sent\n");

	// Kabul edilen istemci soketini kapatır.
	close(new_socket);

	// Sunucu soketinin hızlı ve düzgün bir şekilde kapatılmasını sağlar.
	// server_fd soketi, dinleme yapısından çıkmak ve sunucuyu kapatmak için shutdown fonksiyonuyla işaretlenmelidir.
	//	SHUT_RDWR parametresi, hem okuma hem de yazma yollarının kapatılmasını belirtir.
	shutdown(server_fd, SHUT_RDWR);
	return 0;
}
