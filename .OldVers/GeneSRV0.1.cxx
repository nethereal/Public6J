#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cstring>
#include <iostream>
#include <ctime>

#define BUFLEN 8
//#define NPACK 1
#define PORT 9930

void diep(char const *s) {
	perror(s);
}

void UnCharToChar(unsigned char ar1[], char ar2[], int hm) { for(int i=0; i<hm; i++) { ar2[i]=static_cast<char>(ar1[i]); } }

void CharToUnChar(char ar1[], unsigned char ar2[], int hm) { for(int i=0; i<hm; i++) { ar2[i]=static_cast<unsigned char>(ar1[i]); } }

int main(void)
{
	std::clock_t start;
	double duration;
	
	struct sockaddr_in si_me, si_other;
	
	int s, i, slen=sizeof(si_other);
	
	char buf[BUFLEN];
	unsigned char ubuf[BUFLEN];
	
	bool quitlisten = false;
	
	if ((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))==-1)
		diep("Error building socket");
		
	std::memset((char *) &si_me, 0, sizeof(si_me));
	
	si_me.sin_family = AF_INET;
	si_me.sin_port = htons(PORT);
	si_me.sin_addr.s_addr = htonl(INADDR_ANY);
	
	if (bind(s, (sockaddr*)&si_me, sizeof(si_me))==-1)
		diep("Error during bind");
	start = std::clock();	
	do 
	{
		if (recvfrom(s, buf, BUFLEN, 0, (sockaddr*)&si_other, (socklen_t*)&slen)==-1) { 
			diep("Error when calling recvfrom()"); 
			quitlisten = true;
		}	
		else {
			CharToUnChar(buf, ubuf, BUFLEN);
			std::cout << "Received packet from " << inet_ntoa(si_other.sin_addr) << ":" << ntohs(si_other.sin_port) << std::endl;
			std::cout << "Data: ";
			for (i=0;i<BUFLEN;i++) { std::cout << (uint64_t) ubuf[i] << " "; } 
			std::cout << std::endl;

			//printf("Received packet from %s:%d\nData: %s , \n\n", inet_ntoa(si_other.sin_addr), ntohs(si_other.sin_port), buf);
			duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
			if (duration > 10) { quitlisten = true; } 
		}
	} 
	while (!quitlisten);
	close(s);
	return 0;
}	
