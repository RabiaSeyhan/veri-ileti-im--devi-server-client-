#include <stdio.h> 
#include <netdb.h> 
#include <netinet/in.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <sys/types.h> 
#define MAX 30 
#define PORT 300 
#define SA struct sockaddr 

// İstemci ile sunucu arasindaki sohbet icin tasarlanmıis fonksiyon
void func(int sockfd) 
{ 
	char buff[MAX]; 
	int n; 
	// sohbet icin sonsuz dongü
	for (;;) { 
		bzero(buff, MAX); 

		// mesaji musteriden oku ve ara bellege kopyala
		read(sockfd, buff, sizeof(buff)); 
		// istemci iceriğini iceren yazdırma buffer
		printf("Müşteriden: %s\t müşteriye : ", buff); 
		bzero(buff, MAX); 
		n = 0; 
		// sunucu iletisini arabellekte kopyala 
		while ((buff[n++] = getchar()) != '\n') 
			; 

		// ve bu buffer musteriye gönder 
		write(sockfd, buff, sizeof(buff)); 

		// mesaj "Exit" içeriyorsa,sunucudan çıkılır ve sohbet biter
		if (strncmp("exit", buff, 4) == 0) { 
			printf("sunucudan çıkış:)...\n"); 
			break; 
		} 
	} 
} 

// Sürücü işlevi
int main() 
{ 
	int sockfd, connfd, len; 
	struct sockaddr_in servaddr, cli; 

	// soket oluşturma ve doğrulama
	sockfd = socket(AF_INET, SOCK_STREAM, 0); 
	if (sockfd == -1) { 
		printf("Soket oluşturma başarısız oldu...\n"); 
		exit(0); 
	} 
	else
		printf("Soket oluşturma başarıyla gerçekleşti..\n"); 
	bzero(&servaddr, sizeof(servaddr)); 

	// IP ata,PORT(yukarıda belirttin portunu)
	servaddr.sin_family = AF_INET; 
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY); 
	servaddr.sin_port = htons(PORT); 

	// Yeni oluşturulan soketin verilen IP'ye bağlanması ve doğrulanması
	if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) { 
		printf("soket bağlanma başarısız oldu...\n"); 
		exit(0); 
	} 
	else
		printf("Soket başarıyla bağlandı..\n"); 

	// Artık sunucu dinlemeye ve doğrulamaya hazır
	if ((listen(sockfd, 5)) != 0) { 
		printf("dinleme (dinle) başarısız oldu...\n"); 
		exit(0); 
	} 
	else
		printf("sunucu dinleniyor...\n"); 
	len = sizeof(cli); 

	// Veri paketini müşteriden kabul et ve onayla
	connfd = accept(sockfd, (SA*)&cli, &len); 
	if (connfd < 0) { 
		printf("sunucu kabul edilmedi...\n"); 
		exit(0); 
	} 
	else
		printf("sunucu istemciyi kabul eder...\n"); 

	// İstemci ve sunucu arasında sohbet etme işlevi
	func(connfd); 

	// Sohbet ettikten sonra soketi kapat
	close(sockfd); 
} 

