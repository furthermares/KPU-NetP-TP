<<<<<<< HEAD
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdbool.h>    // bool, true, false가 정의된 헤더 파일
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
bool check_id(char input_id[],int pwd);
int exists(const char *fname);
const char* read_file(char input_id[]); // test.csv 파일 내용 읽기
const char* monthly_read_file(char input_id[],int input_month); // 월별 조회
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
	char msg4[1023];
	char data[1024];
	char msg_date[1023];
	char msg_type[1023];
	char msg_contents[1023];
	char msg_price[1023];
	char login_select[10];
	char Date[50];
	char Type[20];
	char data_month[1024];
	char Contents[50];
	int price;
	FILE* fp;
	char check_file[50];
	char check_file2[50];
	char msg_month[10];
	int month;
	
	// 로그인 기능 구현
	while((write(fd_index,"1. 로그인\n2. 회원등록\n번호 입력 : \n",strlen("1. 로그인\n2. 회원등록\n번호 입력 : \n"))) && (str_len=read(fd_index, login_select, sizeof(login_select)))!=0)
	{
		if(atoi(login_select)==1) // 로그인 선택시
		{
				 while((write(fd_index,"아이디를 입력하세요 : \n",strlen("아이디를 입력하세요 : \n"))) && (str_len=read(fd_index, msg1, sizeof(msg1)))!=0)
                       		 {
					 	strcpy(username,msg1);
						strcat(check_file,username);
                                        	strcat(check_file,".csv");
						break;
				 }
				 while((write(fd_index,"비밀번호를 입력하세요 : \n",strlen("비밀번호를 입력하세요 : \n"))) && (str_len=read(fd_index, msg2, sizeof(msg2)))!=0)
                                 {
                                                pwd=atoi(msg2);
                                                break;
                                 }

				 
				 
				 
				 if(check_id(username,pwd)==true)
				 {
				 		write(fd_index,"[알림] 로그인 성공\n",strlen("[알림] 로그인 성공\n"));
				 		break;
				 }
				 else if(check_id(username,pwd)==false)
				 {
			       			write(fd_index,"[알림] 로그인 실패\n",strlen("[알림] 로그인 실패\n"));
						
				 }
				 
		}
		else // 회원 등록 선택시 
		{
			 while((write(fd_index,"아이디를 입력하세요 : \n",strlen("아이디를 입력하세요 : \n"))) && (str_len=read(fd_index, msg1, sizeof(msg1)))!=0)
                                 {
                                        strcpy(username,msg1);
                                        
					strcat(check_file2,username);
                          		strcat(check_file2,".csv");
                         		if(access(check_file2,0)==0)
                         	 	{
                                 		 write(fd_index,"[알림] 이미 존재하는 아이디 입니다\n",strlen("[알림] 이미 존재하는 아이디 입니다\n"));
						 strcpy(check_file2,"");
						 break;
                          	 	}
	  				else
					{	
					        while((write(fd_index,"비밀번호를 입력하세요 : \n",strlen("비밀번호를 입력하세요 : \n"))) && (str_len=read(fd_index, msg2, sizeof(msg2)))!=0)
                                 		 {		
                                       			pwd=atoi(msg2);
                                       	 		make_file(username,pwd);
                                			write(fd_index,"[알림] 가계부 생성이 완료되었습니다. \n",strlen("[알림] 가계부 생성이 완료되었습니다. \n"));

                                       			break;
				  		}

			  			break;
					}
				}
		}
	}
	
//	CODE STARTS
//	printf("[DEBUG] ==CODE STARTS==\n");

	while((write(clnt_sock,"1. 가계부 조회\n2. 가계부 내용 추가\n3. 가계부 삭제\n4. 월별 조회\n0. 종료\n번호 입력 :  \n",
			strlen("1. 가계부 조회\n2. 가계부 내용 추가\n3. 가계부 삭제\n4. 월별 조회\n0. 종료\n번호 입력 :  \n"))) && (str_len=read(clnt_sock, msg, sizeof(msg)))!=0) {
	
		printf("[DEBUG] ==LOOP STARTS==\n");
		
		char* option = (char*)msg;
		//printf("[SYS] option=[%s]\n",option);		//Marked For Deletion

		switch(atoi(option)){

			case 1:

			if(access(check_file,0)==0)
                        {
                                strcpy(data,read_file(username));
                                write(fd_index,data,strlen(data));

                        }
			else{
				write(fd_index,"[알림] 가계부가 존재하지 않습니다.\n",strlen("[알림] 가계부가 존재하지 않습니다.\n"));
                                strcpy(check_file,"");
                                break;
			}
			// Driver Code
			// Substitute the file_path string
			// with full path of CSV file
			
			break;

			case 2:
			 while((write(fd_index,"날짜를 입력하세요 ex)2021.06.10 : \n",strlen("날짜를 입력하세요 ex)2021.06.10 : \n"))) && (str_len=read(fd_index, msg_date, sizeof(msg_date)))!=0)
                        {
                                strcpy(Date,msg_date);

                                break;

                        }
			  while((write(fd_index,"유형을 입력하세요 (수익 or 지출) : \n",strlen("유형을 입력하세요 (수익 or 지출) : \n"))) && (str_len=read(fd_index, msg_type, sizeof(msg_type)))!=0)
                        {
                                strcpy(Type,msg_type);

                                break;

                        }


			 while((write(fd_index,"내용을 입력하세요 : \n",strlen("내용을 입력하세요 : \n"))) && (str_len=read(fd_index, msg_contents, sizeof(msg_contents)))!=0)
                        {
                                strcpy(Contents,msg_contents);
                              

                                break;

                        }
			 while((write(fd_index,"금액을 입력하세요 : \n",strlen("금액을 입력하세요 : \n"))) && (str_len=read(fd_index, msg_price, sizeof(msg_price)))!=0)
                        {
                                price=atoi(msg_price);
                                

                                break;

                        }

			append_file(username,Date,Type,Contents,price);
			write(fd_index,"[알림] 데이터 추가완료\n\n",strlen("[알림] 데이터 추가완료\n\n"));
			break;

			case 3:
			delete_file(username);	
			write(fd_index,"[알림] 가계부 삭제완료\n\n",strlen("[알림] 가계부 삭제완료\n\n"));
			break;

			case 4:
			 while((write(fd_index,"조회할 달을 입력하세요 : \n",strlen("조회할 달을 입력하세요 : \n"))) && (str_len=read(fd_index, msg_month, sizeof(msg_month)))!=0)
                        {
                                month=atoi(msg_month);
				strcpy(data_month,monthly_read_file(username,month));
                                write(fd_index,data_month,strlen(data_month));


                                break;

                        }

			
			break; 

			default:
			printf("[SYS]");
		}

		
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
	pthread_mutex_lock(&mutx);
	FILE* fp;
	fp = fopen(filename, "w"); // 파일 생성
	fprintf(fp, "%s, %d\n", user_id, user_pwd);
	pthread_mutex_unlock(&mutx);
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
	pthread_mutex_lock(&mutx);
	// Saving data in file
	fprintf(fp, "%s, %s, %s, %d\n", Date,
		Type, Contents, price);
	pthread_mutex_unlock(&mutx);

	printf("\n 데이터 추가 완료 \n");

	fclose(fp);
	
}
bool check_id(char input_id[],int pwd)
{
	char output_buffer[1024];
	char extension[50] = { ".csv" };  // 확장자명
	char filename[50] = { "" };  // 사용자 파일 이름
	char user_id[50] = { "" };
	strcpy(filename, input_id);
	strcat(filename, extension);
	strcpy(user_id, input_id);
	FILE* fp = fopen(filename, "r");
	if (!fp)
	{
		printf("Can't open file\n");
		return false;
	}
	else {
		// Here we have taken size of
		// array 1024 you can modify it
		char buffer[1024] = { "" };

		int row = 0;
		int column = 0;
		while (fgets(buffer, 1024, fp)) {
			char* value = strtok(buffer, ", ");
			value = strtok(NULL, ", ");
			if (atoi(value) == pwd)
			{
				return true;
			}
			else
			{
				return false;
			}

		}
	}
}
int exists(const char *fname)
{
    FILE *file;
    if ((file = fopen(fname, "r")))
    {
        fclose(file);
        return 1;
    }
    return 0;
}
const char* monthly_read_file(char input_id[],int input_month) // 월별 조회
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
		char buffer[1024] = { "" };
		char month_buffer[1024] = { "" };

		int row = 0;
		int column = 0;
		int month_data=0;
		while (fgets(buffer, 1024, fp)) {
			column = 0;
			row++;

			// To avoid printing of column
			// names in file can be changed
			// according to need


			// Splitting the data
			char* value = strtok(buffer, ", ");
			char* month_value = malloc(sizeof(char) * 100);

			if (row == 1) // 첫행은 아이디, 비밀번호 출력
			{
				strcpy(output_buffer, "아이디 : ");
				strcat(output_buffer, value);

				value = strtok(NULL, ", ");
				strcat(output_buffer, "\t\t비밀번호 : ");
				strcat(output_buffer, value);
				strcat(output_buffer, "\n\n");

				continue;
			}

			while (value)
			{
				if ((int)value[5] == 48)
				{
					month_data = (int)value[6] - 48;
				}
				else
				{
					month_data = ((int)value[5] - 48) * 10 + (int)value[6] - 48;
				}
				if(month_data==input_month)
				{
					for (int i = 0; i < 4; i++)
					{
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
				else
				{
					value = strtok(NULL, ", ");
					value = strtok(NULL, ", ");
					value = strtok(NULL, ", ");
					value = strtok(NULL, ", ");
				}

			}


		}

		// Close the file

		fclose(fp);
	}

	return output_buffer;
}




=======
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
>>>>>>> 8f02be8ef3f39fc46749e920c6b0099e83762f41
