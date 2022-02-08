#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define N_MAX 16
#define MAX_TAMPON 1024

typedef struct querry_header_t {
	uint16_t id;
	uint16_t options;
	uint16_t qdcount;
	uint16_t ancount;
	uint16_t nscount;
	uint16_t arcount;
} querry_header_t;

typedef struct question_t {
	uint8_t bytes[N_MAX];
	int len;
} question_t;

querry_header_t createRFCQuerryHeader() {
	querry_header_t querry;
	querry.id = 0x4200;
	//querry.options = (qr << 15) | (opcode << 10) | (aa << 9) | (tc << 8) | (rd << 7) | (ra << 6) | rcode;
	querry.options = 0x0001;
	querry.qdcount = 0x0100;
	querry.ancount = 0x0000;
	querry.nscount = 0x0000;
	querry.arcount = 0x0000;

	return querry;
}



void messageUDP(char *hote,char *service,unsigned char *message,int taille){
struct addrinfo precisions,*resultat,*origine;
int statut;
int s;

/* Creation de l'adresse de socket */
memset(&precisions,0,sizeof precisions);
precisions.ai_family=AF_UNSPEC;
precisions.ai_socktype=SOCK_DGRAM;
statut=getaddrinfo(hote,service,&precisions,&origine);
if(statut<0){ perror("messageUDPgenerique.getaddrinfo"); exit(EXIT_FAILURE); }
struct addrinfo *p;
for(p=origine,resultat=origine;p!=NULL;p=p->ai_next)
  if(p->ai_family==AF_INET6){ resultat=p; break; }

/* Creation d'une socket */
s=socket(resultat->ai_family,resultat->ai_socktype,resultat->ai_protocol);
if(s<0){ perror("messageUDPgenerique.socket"); exit(EXIT_FAILURE); }

/* Option sur la socket */
int vrai=1;
if(setsockopt(s,SOL_SOCKET,SO_BROADCAST,&vrai,sizeof(vrai))<0){
  perror("initialisationServeurUDPgenerique.setsockopt (BROADCAST)");
  exit(-1);
  }

/* Envoi du message */
int nboctets=sendto(s,message,taille,0,resultat->ai_addr,resultat->ai_addrlen);
if(nboctets<0){ perror("messageUDPgenerique.sento"); exit(EXIT_FAILURE); }

char tampon[MAX_TAMPON];
int taille2;

taille2=recvfrom(s,tampon,MAX_TAMPON,0,NULL,NULL);
fputs(tampon,stdout);

/* Liberation de la structure d'informations */
freeaddrinfo(origine);

/* Fermeture de la socket d'envoi */
close(s);
}

int main(int argc,char *argv[]){
if(argc!=3){
  fprintf(stderr,"Syntaxe : %s <serveur> <message>\n",argv[0]);
  exit(EXIT_FAILURE);
  }
char *hote=argv[1];
//char *message=argv[2];
char *service="53";

unsigned char message_converted[] = {0x02, 'n', 's', 0x04, 'p', 'l', 'i', 'l', 0x02, 'f', 'r', 0x00};
uint16_t qtype = 0x0100, qclass = 0x0100;

int size_querry = (sizeof(querry_header_t) + sizeof(message_converted) + 2*sizeof(uint16_t));

unsigned char * querry = malloc(sizeof(querry_header_t) + sizeof(message_converted) + 2*sizeof(uint16_t));

querry_header_t header = createRFCQuerryHeader();

memcpy(querry, &header, sizeof(header));
//for (int i = 0; i < 13; i++)
//{
memcpy(querry+sizeof(header), message_converted, sizeof(message_converted));
memcpy(querry+sizeof(header)+sizeof(message_converted), &qtype, sizeof(uint16_t));
memcpy(querry+sizeof(header)+sizeof(message_converted)+sizeof(uint16_t), &qclass, sizeof(uint16_t));
//}

//printf("%s\n", querry);

messageUDP(hote,service,(unsigned char *)querry,size_querry);
return 0;
}
