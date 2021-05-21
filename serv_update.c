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

#define BUF_SIZE 1024
#define MAX_CLNT 256

void * handle_clnt(void * arg);
void send_msg(char * msg, int len);		//original code. just here for comparison.
void error_handling(char * msg);
void send_msg2(char * msg, int len);
void  make_file ( char input_id [], int user_pwd); // id 입력 csv 파일 생성
void delete_file(char input_id[]);
void append_file(char input_id[],char Date[],char Type[],char Contents[],int price);
const char* read_file(char input_id[]); // test.csv 파일 내용 읽기

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
	int fd_index = clnt_sock; // 데이터 전송할 클라이언트 소켓 번호
	char username[1024];
	int pwd;
	char msg1[1023];
	char msg2[1023];
	char msg3[1023];
	char data[1024];
	char msg_date[1023];
	char msg_type[1023];
	char msg_contents[1023];
	char msg_price[1023];

	char Date[50];
	char Type[20];
	char Contents[50];
	int price;
	FILE* fp;
	while((write(fd_index,"아이디를 입력하세요 : \n",strlen("아이디를 입력하세요 : \n"))) && (str_len=read(fd_index, msg1, sizeof(msg1)))!=0)
                        {
                                strcpy(username,msg1);
                                printf("입력받은 아이디 %s ",username);

                                break;

                        }
                         while((write(fd_index,"비밀번호를 입력하세요 : \n",strlen("비밀번호를 입력하세요 : \n"))) && (str_len=read(fd_index, msg2, sizeof(msg2)))!=0)
                        {
                                pwd=atoi(msg2);
                                printf("입력받은 비밀번호  %d ",pwd);

                                break;

                        }
	
	
	
	
//	CODE STARTS
	printf("[DEBUG] ==CODE STARTS==\n");

	while((write(clnt_sock,"1. 가계부 생성\n2. 가계부 조회\n3. 가계부 내용 추가\n4. 가계부 삭제\n0. 종료\n번호 입력 :  \n",
			strlen("1. 가계부 생성\n2. 가계부 조회\n3. 가계부 내용 추가\n4. 가계부 삭제\n0. 종료\n번호 입력 :  \n"))) && (str_len=read(clnt_sock, msg, sizeof(msg)))!=0) {
	
		printf("[DEBUG] ==LOOP STARTS==\n");
		
		char* option = (char*)msg;
		//printf("[SYS] option=[%s]\n",option);		//Marked For Deletion

		switch(atoi(option)){
			case 1:
			
			
			make_file(username,pwd);
			write(fd_index,"가계부 생성이 완료되었습니다. \n",strlen("가계부 생성이 완료되었습니다. \n"));

											//MFD
			break;

			case 2:
				
			strcpy(data,read_file(username));

			write(fd_index,data,strlen(data));

			// Driver Code
			// Substitute the file_path string
			// with full path of CSV file
			
			break;

			case 3:
			 while((write(fd_index,"날짜를 입력하세요 : \n",strlen("날짜를 입력하세요 : \n"))) && (str_len=read(fd_index, msg_date, sizeof(msg_date)))!=0)
                        {
                                strcpy(Date,msg_date);
                                printf("입력받은 아이디 %s ",Date);

                                break;

                        }
			  while((write(fd_index,"유형을 입력하세요 : \n",strlen("유형을 입력하세요 : \n"))) && (str_len=read(fd_index, msg_type, sizeof(msg_type)))!=0)
                        {
                                strcpy(Type,msg_type);
                                printf("입력받은 아이디 %s ",Type);

                                break;

                        }


			 while((write(fd_index,"내용을 입력하세요 : \n",strlen("내용을 입력하세요 : \n"))) && (str_len=read(fd_index, msg_contents, sizeof(msg_contents)))!=0)
                        {
                                strcpy(Contents,msg_contents);
                                printf("입력받은 비밀번호  %s ",Contents);

                                break;

                        }
			 while((write(fd_index,"금액을 입력하세요 : \n",strlen("금액을 입력하세요 : \n"))) && (str_len=read(fd_index, msg_price, sizeof(msg_price)))!=0)
                        {
                                price=atoi(msg_price);
                                printf("입력받은 비밀번호  %d ",price);

                                break;

                        }

			append_file(username,Date,Type,Contents,price);
			write(fd_index,"데이터 추가완료\n\n",strlen("데이터 추가완료\n\n"));
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
void send_msg2(char * msg, int len)
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
void make_file(char input_id[], int user_pwd) // id 입력받아 csv 파일 생성
{
	char extension[50] = { ".csv" };  // 확장자명
	char filename[50] = { "" };  // 사용자 파일 이름
	char user_id[50] = { "" };
	strcpy(filename, input_id);
	strcat(filename, extension);
	strcpy(user_id, input_id);
	FILE* fp;
	fp = fopen(filename, "w"); // 파일 생성
	fprintf(fp, "%s, %d\n", user_id, user_pwd);
	fclose(fp);
}

void delete_file(char input_id[]) // 파일명 입력받아 csv 파일 생성
{
	char extension[50] = { ".csv" };  // 확장자명
	char filename[50] = { "" };  // 사용자 파일 이름
	strcpy(filename, input_id);
	strcat(filename, extension);

	remove(filename);
}
const char* read_file(char input_id[]) // test.csv 파일 내용 읽기
{
	static char output_buffer[1024];
	char extension[50] = { ".csv" };  // 확장자명
	char filename[50] = { "" };  // 사용자 파일 이름
	char user_id[50] = { "" };
	strcpy(filename, input_id);
	strcat(filename, extension);
	strcpy(user_id, input_id);
	FILE* fp = fopen(filename, "r");
	if (!fp)
		printf("Can't open file\n");
	else {
		// Here we have taken size of
		// array 1024 you can modify it
		char buffer[1024] = {""};

		int row = 0;
		int column = 0;

		while (fgets(buffer,1024, fp)) {
			column = 0;
			row++;

			// To avoid printing of column
			// names in file can be changed
			// according to need


			// Splitting the data
			char* value = strtok(buffer, ", ");

			if (row == 1) // 첫행은 아이디, 비밀번호 출력
			{
				strcpy(output_buffer,"아이디 : ");
				strcat(output_buffer, value);

				value = strtok(NULL, ", ");
				strcat(output_buffer, "\t\t비밀번호 : ");
				strcat(output_buffer, value);
				strcat(output_buffer, "\n\n");
				continue;
			}

			while (value) {
				// Column 1
				if (column == 0) {
					strcat(output_buffer, "날짜 : ");
				}

				// Column 2
				if (column == 1) {
					strcat(output_buffer, "\t유형 : ");
				}

				// Column 3
				if (column == 2) {
					strcat(output_buffer, "\t내용 : ");
				}

				// Column 4
				if (column == 3) {
					strcat(output_buffer, "\t\t금액 : ");
				}

				strcat(output_buffer, value);
				value = strtok(NULL, ", ");
				column++;
			}
			strcat(output_buffer, "\n");
			
		}

		// Close the file

		fclose(fp);
	}
	
	return output_buffer;
}
void append_file(char input_id[],char Date[],char Type[],char Contents[],int price)
{
	char extension[50] = { ".csv" };  // 확장자명
	char filename[50] = { "" };  // 사용자 파일 이름
	char user_id[50] = { "" };
	strcpy(filename, input_id);
	strcat(filename, extension);
	strcpy(user_id, input_id);
	FILE* fp = fopen(filename, "a+");

	if (!fp) {
		// Error in file opening
		printf("Can't open file\n");
		
	}

	

	// Saving data in file
	fprintf(fp, "%s, %s, %s, %d\n", Date,
		Type, Contents, price);

	printf("\n 데이터 추가 완료 \n");

	fclose(fp);
	
}
