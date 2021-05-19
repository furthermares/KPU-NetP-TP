#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<string.h>
void make_file(char msg[]); // 파일명 입력받아 csv 파일 생성
void read_file(); // test.csv 파일 내용 읽기
void append_file(); // test.csv 파일에 내용 추가 (한줄) 
int main()
{




	return 0;
}
void make_file(char msg[]) // 파일명 입력받아 csv 파일 생성
{
    char extension[50] = {".csv"};  // 확장자명
    char filename[50] = { "" };  // 사용자 파일 이름
    strcpy(filename, msg);
    strcat(filename, extension);
    FILE* fp;
    fp = fopen(filename, "w"); // 파일 생성
    fclose(fp);
}
void read_file() // test.csv 파일 내용 읽기
{
    FILE* fp = fopen("test.csv", "r");
    if (!fp)
        printf("Can't open file\n");
    else {
        // Here we have taken size of
        // array 1024 you can modify it
		char buffer[1024];

		int row = 0;
		int column = 0;

		while (fgets(buffer,
			1024, fp)) {
			column = 0;
			row++;

			// To avoid printing of column
			// names in file can be changed
			// according to need
			if (row == 1)
				continue;

			// Splitting the data
			char* value = strtok(buffer, ", ");

			while (value) {
				// Column 1
				if (column == 0) {
					printf("날짜 : ");
				}

				// Column 2
				if (column == 1) {
					printf("\t유형 : ");
				}

				// Column 3
				if (column == 2) {
					printf("\t내용 : ");
				}

				// Column 4
				if (column == 3) {
					printf("\t\t금액 : ");
				}

				printf("%s", value);
				value = strtok(NULL, ", ");
				column++;
			}

			printf("\n");
		}

		// Close the file
		fclose(fp);
	}
	return 0;
}
void append_file()
{
	FILE* fp = fopen("test.csv", "a+");

	char Date[50],Type[50],Contents[50];
	int price;

	if (!fp) {
		// Error in file opening
		printf("Can't open file\n");
		return 0;
	}

	// Asking user input for the
	// new record to be added
	printf("\n날짜 입력 : ");
	scanf("%s", &Date);
	printf("\n유형 입력 : ");
	scanf("%s", &Type);
	printf("\n내용 입력 : ");
	scanf("%s", &Contents);
	printf("\n금액 입력 : ");
	scanf("%d", &price);

	// Saving data in file
	fprintf(fp, "%s, %s, %s, %d\n", Date,
		Type, Contents, price);

	printf("\n 데이터 추가 완료 \n");

	fclose(fp);
	return 0;
}

