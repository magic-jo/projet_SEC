#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
extern int h_errno;

#define MAXCAR  80
       
int main(int argc, char * argv[])
{

struct hostent * hp; //Variable recuperant le resltat de gesthostbyname

char servname[MAXCAR+1];

struct in_addr adrip ; 

//Definir le nom de la machine
if (argc==1) 
{ 
    printf("Entrer le nom d'un machine d'INTERNET :");
    fgets(servname, MAXCAR,stdin);
    servname[strlen(servname)-1]='\0';
    printf("Nom serveur : '%s' \n",servname);
    getchar();
} else strcpy(servname,argv[1]);
    
printf("Serveur '%s' \n", servname);

hp=gethostbyname(servname);

if (hp)
{
    bcopy(hp->h_addr, &adrip, 4);
    printf("Adresse IP du serveur : '%s' \n", inet_ntoa(adrip) );
}
else printf("Serveur n'existant pas dans le DNS !!! \n");

return EXIT_SUCCESS;
}




/*
 struct hostent {
               char  *h_name;            
               char **h_aliases;         
               int    h_addrtype;       
               int    h_length;          
               char **h_addr_list;      
           } */
 /*          #define h_addr h_addr_list[0] 
*/


