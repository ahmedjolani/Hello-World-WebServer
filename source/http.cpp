#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include "html.h"

using namespace std;

#define PORT "80"

class Program {

public:
	static void * GetIPAddr(struct sockaddr * sa) {
		if (sa->sa_family == AF_INET) {
			return &(((struct sockaddr_in *)sa)->sin_addr);
		} else {
			return &(((struct sockaddr_in6 *)sa)->sin6_addr);
		}
	}
	static int Scan(char * buff) {
		int len = 0;
		char c = '\n';
		while (c == '\n') {
			scanf("%c", &c);
		}
		while (c != '\n' && len <= 1024) {
			len++;
			*buff = c;
			buff++;
			scanf("%c", &c);
		}
		buff[len] = 0;

		return len;
	}

};

int main() {
	int rv, sockfd, newsockfd, yes = 1;
	struct addrinfo hints, * res, * p;
	struct sockaddr_storage other;
	socklen_t otherlen;
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	rv = getaddrinfo(NULL, PORT, &hints, &res);
	if (rv != 0) {
		cout << "Error[getaddrinfo]: " << gai_strerror(rv) << endl;
		exit(0);
	}

	for (p = res; p != NULL; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
			cout << "Error[socket]: " << strerror(errno) << endl;
			continue;
		}

		if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes) == -1) {
			cout << "Error[setsockopt]: " << strerror(errno) << endl;
			close(sockfd);
			continue;
		}

		if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			cout << "Error[bind]: " << strerror(errno) << endl;
			close(sockfd);
			continue;
		}
		
		break;
	}

	if (p == NULL) {
		cout << "Error: can't bind server." << endl;
		exit(1);
	}

	if (listen(sockfd, 100) == -1) {
		cout << "Error[listen]: " << strerror(errno) << endl;
		exit(1);
	}

	char buff[1024], client[INET_ADDRSTRLEN];
	int rbuff;
	while (1) {
		otherlen = sizeof other;
		if ((newsockfd = accept(sockfd, (struct sockaddr *)&other, &otherlen)) == -1) {
			cout << "Error[accept]: " << strerror(errno) << endl;
			continue;
		}
		inet_ntop(other.ss_family, Program::GetIPAddr((struct sockaddr *)&other), client, INET6_ADDRSTRLEN);
		cout << "Connected to [" << client << "]." << endl;
		if (!fork()) {
			const char *buffer = "First webserver by Ahmed Jolani :-)";
			if (send(newsockfd, HEADERS, strlen(HEADERS), 0) == -1) {
				cout << "Error[headers sending]: " << strerror(errno) << endl;
				exit(2);
			}
			if (send(newsockfd, CONTENT, strlen(CONTENT), 0) == -1) {
				cout << "Error[content sending]: " << strerror(errno) << endl;
				exit(2);
			}
			
			int ret;
			if ((ret = send(newsockfd, buffer, strlen(buffer), 0)) == -1) {
				cout << "Error[send]: " << strerror(errno) << endl;
			} else {
				cout << buffer << " was sent." << endl;
			}
			if (send(newsockfd, ECONTENT, strlen(ECONTENT), 0) == -1) {
				cout << "Error[headers sending]: " << strerror(errno) << endl;
			}
			exit(2);
		}
		close(newsockfd);
	}
	return 0;
}
