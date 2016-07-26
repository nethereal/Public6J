#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cstring>
#include <iostream>

#define BUFLEN 8
//#define NPACK 1
#define PORT 9930
#define SRV_IP "127.0.0.1"

void diep(char const *s) {
	perror(s);
}

void UnCharToChar(unsigned char ar1[], char ar2[], int hm) { for(int i=0; i<hm; i++) { ar2[i]=static_cast<char>(ar1[i]); } }

void CharToUnChar(char ar1[], unsigned char ar2[], int hm) { for(int i=0; i<hm; i++) { ar2[i]=static_cast<unsigned char>(ar1[i]); } }

int main(void)
{
	struct sockaddr_in si_other;
	int s, i, slen=sizeof(si_other);
	char buf[BUFLEN];
	unsigned char ubuf[BUFLEN];

	if ((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))==-1) { 
		diep("Error building socket");
	}
	
	for (int randcnt=0; randcnt<BUFLEN ; randcnt++) {
		ubuf[randcnt] = (unsigned char)(rand() % 255);
	}
	UnCharToChar(ubuf, buf, BUFLEN);
	
	std::memset((char *) &si_other, 0, sizeof(si_other));
	si_other.sin_family = AF_INET;
	si_other.sin_port = htons(PORT);
	
	if (inet_aton(SRV_IP, &si_other.sin_addr)==0) {
		fprintf(stderr, "Failure while calling inet_aton()\n");
		return 1;
	}

	std::cout << "Sending packet" << std::endl;
	for (int xy=0;xy<BUFLEN;xy++) { std::cout << (uint64_t) ubuf[xy] << " " ; } 
	std::cout << " :values for packet " << i << std::endl;

	if (sendto(s, buf, BUFLEN, 0, (sockaddr*)&si_other, slen)==-1) { 
		diep("Error while calling sendto()"); 
	}
	else {
		std::cout << "Packet sent successfully!" << std::endl;
	}

	close(s);
	return 0;
}
