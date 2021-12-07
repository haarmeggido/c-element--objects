#include<stdio.h>
#include<arpa/inet.h>
#include<string.h>
#include<unistd.h>
int main(void){

int status,gniazdo;
struct sockaddr_in ser;
char buf[200];

gniazdo = socket(AF_INET, SOCK_STREAM, 0);
if(gniazdo==-1) {printf("socket error\n"); return 0;}

ser.sin_family = AF_INET;
ser.sin_port = htons(9128);
ser.sin_addr.s_addr = inet_addr("149.156.207.22");

printf("Podaj tekst\n");
fgets(buf,sizeof buf,stdin);

status = connect(gniazdo,(struct sockaddr*)&ser, sizeof ser);
if(status == -1){printf("connect error\n"); return 0;}

status = send(gniazdo, buf, strlen(buf), 0);
status = recv(gniazdo, buf, sizeof(buf) - 1, 0);
buf[status]='\0';
printf("Otrzymałem %s\n", buf);


close(gniazdo);




}
