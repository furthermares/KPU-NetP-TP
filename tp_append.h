#include <stdio.h>
#include <string.h>

// Driver Code
int tp_append()
{
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
//		CODE STARTS
	while((str_len=read(clnt_sock, msg, sizeof(msg)))!=0)
		printf("tp-append read: [%s]",msg);
		
	//pthread_mutex_lock(&mutx);
	
	printf("\nEnter Account Holder Name\n");
	//scanf("%s", &name);
	printf("\nEnter Account Number\n");
	//scanf("%d", &accountno);
	printf("\nEnter Available Amount\n");
	//scanf("%d", &amount);

	// Saving data in file
	fprintf(fp, "%s, %d, %d\n", name,
			accountno, amount);

	printf("\nNew Account added to record\n");

	fclose(fp);
	return 0;
}

