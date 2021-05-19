#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<string.h>
void make_file(char msg[]); // ���ϸ� �Է¹޾� csv ���� ����
void read_file(); // test.csv ���� ���� �б�
void append_file(); // test.csv ���Ͽ� ���� �߰� (����) 
int main()
{




	return 0;
}
void make_file(char msg[]) // ���ϸ� �Է¹޾� csv ���� ����
{
    char extension[50] = {".csv"};  // Ȯ���ڸ�
    char filename[50] = { "" };  // ����� ���� �̸�
    strcpy(filename, msg);
    strcat(filename, extension);
    FILE* fp;
    fp = fopen(filename, "w"); // ���� ����
    fclose(fp);
}
void read_file() // test.csv ���� ���� �б�
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

