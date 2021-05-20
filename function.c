#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<string.h>
void make_file(char input_id[], int user_pwd); // id �Է¹޾� csv ���� ����
void read_file(char input_id[]); // test.csv ���� ���� �б�
void append_file(char input_id[]); // test.csv ���Ͽ� ���� �߰� (����) 
void delete_file(char input_id[], int user_pwd); // csv ���� ���� ���

int main()
{
	

}
void make_file(char input_id[], int user_pwd) // id �Է¹޾� csv ���� ����
{
	char extension[50] = { ".csv" };  // Ȯ���ڸ�
	char filename[50] = { "" };  // ����� ���� �̸�
	char user_id[50] = { "" };
	strcpy(filename, input_id);
	strcat(filename, extension);
	strcpy(user_id, input_id);
	FILE* fp;
	fp = fopen(filename, "w"); // ���� ����
	fprintf(fp, "%s, %d\n", user_id, user_pwd);
	fclose(fp);
}
void delete_file(char input_id[], int user_pwd) // ���ϸ� �Է¹޾� csv ���� ����
{
	char extension[50] = { ".csv" };  // Ȯ���ڸ�
	char filename[50] = { "" };  // ����� ���� �̸�
	char user_id[50] = { "" };  // ����� ���̵�
	char buffer[1024];  // �о���� ����
	
	strcpy(filename, input_id);
	strcat(filename, extension);
	strcpy(user_id, input_id);

	FILE* fp = fopen(filename, "r");
	if (!fp) // ���� �߻� �� ����
		return 0;
	fgets(buffer, 1024, fp); // ���� �о����
	fclose(fp);
	char* file_pwd = strtok(buffer, ", ");
	file_pwd = atoi(strtok(NULL, ", ")); // ���Ͽ� ����Ǿ��ִ� ��й�ȣ
	if (file_pwd == user_pwd)
	{
		remove(filename);
	}
	else
	{
		printf("��й�ȣ Ʋ��");
	}
}
void read_file(char input_id[]) // test.csv ���� ���� �б�
{
	char extension[50] = { ".csv" };  // Ȯ���ڸ�
	char filename[50] = { "" };  // ����� ���� �̸�
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
		char buffer[1024];

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

			if (row == 1) // ù���� ���̵�, ��й�ȣ ���
			{
				printf("\t���̵� : ");
				printf("%s", value);

				value = strtok(NULL, ", ");
				printf("\t\t��й�ȣ : ");
				printf("%s", value);
				printf("\n\n");
				continue;
			}

			while (value) {
				// Column 1
				if (column == 0) {
					printf("��¥ : ");
				}

				// Column 2
				if (column == 1) {
					printf("\t���� : ");
				}

				// Column 3
				if (column == 2) {
					printf("\t���� : ");
				}

				// Column 4
				if (column == 3) {
					printf("\t\t�ݾ� : ");
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
void append_file(char input_id[])
{
	char extension[50] = { ".csv" };  // Ȯ���ڸ�
	char filename[50] = { "" };  // ����� ���� �̸�
	char user_id[50] = { "" };
	strcpy(filename, input_id);
	strcat(filename, extension);
	strcpy(user_id, input_id);
	FILE* fp = fopen(filename, "a+");

	char Date[50], Type[50], Contents[50];
	int price;

	if (!fp) {
		// Error in file opening
		printf("Can't open file\n");
		return 0;
	}

	// Asking user input for the
	// new record to be added
	printf("\n��¥ �Է� : ");
	scanf("%s", &Date);
	printf("\n���� �Է� : ");
	scanf("%s", &Type);
	printf("\n���� �Է� : ");
	scanf("%s", &Contents);
	printf("\n�ݾ� �Է� : ");
	scanf("%d", &price);

	// Saving data in file
	fprintf(fp, "%s, %s, %s, %d\n", Date,
		Type, Contents, price);

	printf("\n ������ �߰� �Ϸ� \n");

	fclose(fp);
	return 0;
}
