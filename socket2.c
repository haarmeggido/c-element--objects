#include<stdio.h>
#include<arpa/inet.h>
#include<string.h>
#include<unistd.h>
int main(void){

int status,gniazdo,gniazdo2,dlugosc,nr = 0, end = 1;
struct sockaddr_in ser, cli;
char buf[200];

gniazdo = socket(AF_INET, SOCK_STREAM, 0);
if(gniazdo==-1) {printf("socket error\n"); return 0;}

ser.sin_family = AF_INET;
ser.sin_port = htons(9128);
ser.sin_addr.s_addr = inet_addr("149.156.207.22");


status= bind(gniazdo,(struct sockaddr*)&ser, sizeof ser);
if(status == -1){printf("connect error\n"); return 0;}

status = listen(gniazdo,10);
if(status == -1) { printf("listen error"); return 0;}

while(end){
	dlugosc = sizeof cli;
	gniazdo2 = accept(gniazdo,(struct sockaddr*)&cli, (socklen_t*)&dlugosc);
	if(gniazdo2 == -1) { printf("accept error"); return 0;}
	status = recv(gniazdo2, buf, sizeof buf, 0);
	if(buf[0] == 'Q') {sprintf(buf,"ok, klient kończy prace"); end = 0;}
	else if(buf[0] =='N') {sprintf(buf,"you are client nr %d", nr+=1);}
	else if(buf[0] =='s') {sprintf(buf,"AMOGUS");}
	else sprintf(buf,"nie rozumiem pytania");
	status = send(gniazdo2, buf, strlen(buf),0);
	close(gniazdo2);
}	



}
