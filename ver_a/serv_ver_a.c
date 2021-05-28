#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>

#include "tp_read.h"
//#include "tp_append.h"

#define BUF_SIZE 100
#define MAX_CLNT 256

void * handle_clnt(void * arg);
void send_msg(char * msg, int len);		//original code. just here for comparison.
void error_handling(char * msg);

int clnt_cnt=0;
int clnt_socks[MAX_CLNT];
pthread_mutex_t mutx;

int main(int argc, char *argv[])
{
	int serv_sock, clnt_sock;
	struct sockaddr_in serv_adr, clnt_adr;
	int clnt_adr_sz;
	pthread_t t_id;
	if(argc!=2) {
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}
  
	pthread_mutex_init(&mutx, NULL);
	serv_sock=socket(PF_INET, SOCK_STREAM, 0);

	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family=AF_INET; 
	serv_adr.sin_addr.s_addr=htonl(INADDR_ANY);
	serv_adr.sin_port=htons(atoi(argv[1]));
	
	if(bind(serv_sock, (struct sockaddr*) &serv_adr, sizeof(serv_adr))==-1)
		error_handling("bind() error");
	if(listen(serv_sock, 5)==-1)
		error_handling("listen() error");
	
	while(1)
	{
		clnt_adr_sz=sizeof(clnt_adr);
		clnt_sock=accept(serv_sock, (struct sockaddr*)&clnt_adr,&clnt_adr_sz);
		
		pthread_mutex_lock(&mutx);
		clnt_socks[clnt_cnt++]=clnt_sock;
		pthread_mutex_unlock(&mutx);
	
		pthread_create(&t_id, NULL, handle_clnt, (void*)&clnt_sock);
		pthread_detach(t_id);
		printf("[SYS] Connected client IP: %s \n", inet_ntoa(clnt_adr.sin_addr));
	}
	close(serv_sock);
	return 0;
}
	
void * handle_clnt(void * arg)
{
	int clnt_sock=*((int*)arg);
	int str_len=0, i;
	char msg[BUF_SIZE];
	
//	CODE STARTS
	printf("[DEBUG] ==CODE STARTS==\n");

	while((write(clnt_sock, "Input (1,2,3,4,0=quit)\n", strlen("Input (1,2,3,4,0=quit)\n"))) &&
	(str_len=read(clnt_sock, msg, sizeof(msg)))!=0) {
	
		printf("[DEBUG] ==LOOP STARTS==\n");
		
		char* option = (char*)msg;
		//printf("[SYS] option=[%s]\n",option);		//Marked For Deletion

		switch(atoi(option)){
			case 1:
			printf("[DEBUG] selected case 1\n");
			//write(clnt_sock, "[SYS] selected case 1\n", strlen("[SYS] selected case 1\n"));
//bring tp_read code to here for write func.
			tp_read();								//MFD
			break;

			case 2:
			printf("[DEBUG] selected case 2\n");
			//write(clnt_sock, "[SYS] selected case 2\n", strlen("[SYS] selected case 2\n"));

			// Driver Code
			// Substitute the file_path string
			// with full path of CSV file
			FILE* fp = fopen("main.csv", "a+");

			char name[50];
			int accountno, amount;
			if (!fp) {
			// Error in file opening
			printf("Can't open file\n");
				return 0;
			}

			// Asking user input for the
			// new record to be added
			//printf("\nEnter Account Holder Name\n");	//MFD
			write(clnt_sock, "\nEnter Account Holder Name\n", strlen("\nEnter Account Holder Name\n"));
			//scanf("%s", &name);
			if(str_len=read(clnt_sock, msg, sizeof(msg))!=0){
				strcpy(name, msg);
			}
			
			//printf("\nEnter Account Number\n");			//MFD
			write(clnt_sock, "\nEnter Account Number\n", strlen("\nEnter Account Number\n"));
			if(str_len=read(clnt_sock, msg, sizeof(msg))!=0){
				accountno = atoi(msg);
			}

			//printf("\nEnter Available Amount\n");		//MFD
			write(clnt_sock, "\nEnter Available Amount\n", strlen("\nEnter Available Amount\n"));
			if(str_len=read(clnt_sock, msg, sizeof(msg))!=0){
				amount = atoi(msg);
			}

			// Saving data in file
			fprintf(fp, "%s, %d, %d\n", name,
					accountno, amount);

			//printf("\nNew Account added to record\n");	//MFD
			write(clnt_sock, "\nNew Account added to record\n", strlen("\nNew Account added to record\n"));
			
			memset(msg, 0, sizeof(msg));

			fclose(fp);
			break;

			case 3:
			printf("[SYS] case 3 selected.\n");
			//write(clnt_sock, "[SYS] selected case 3\n", strlen("[SYS] selected case 3\n"));
			break;

			case 4:
			printf("[SYS] case 4 selected.\n");
			//write(clnt_sock, "[SYS] selected case 4\n", strlen("[SYS] selected case 4\n"));
			break;

			case 9:
			printf("[SYS] quitting server.\n");
			//write(clnt_sock, "[SYS] quitting server.\n", strlen("[SYS] quitting server\n"));
			exit(0);
			break; 

			default:
			printf("[SYS] default\n");
		}

		printf("==LOOPING==\n\n");
//		CODE ENDS
	}
	
	pthread_mutex_lock(&mutx);
	for(i=0; i<clnt_cnt; i++)   // remove disconnected client
	{
		if(clnt_sock==clnt_socks[i])
		{
			while(i++<clnt_cnt-1)
				clnt_socks[i]=clnt_socks[i+1];
			break;
		}
	}
	clnt_cnt--;
	pthread_mutex_unlock(&mutx);
	close(clnt_sock);
	return NULL;
}
void send_msg(char * msg, int len)
{
	printf("[SYS] send_msg called.\n");
	int i;
	pthread_mutex_lock(&mutx);
	for(i=0; i<clnt_cnt; i++) {
		write(clnt_socks[i], msg, len);	
	}	
	pthread_mutex_unlock(&mutx);
}

void error_handling(char * msg)
{
	fputs(msg, stderr);
	fputc('\n', stderr);
	exit(1);
}
