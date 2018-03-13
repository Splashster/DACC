#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include <errno.h>

void closeDB(sqlite3* bank1_db, sqlite3* bank2_db){
	sqlite3_close(bank1_db);
	sqlite3_close(bank2_db);

}

static int callback(void *NotUsed, int argc, char **argv, char **azColName) {
   int i;
   for(i = 0; i<argc; i++) {
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
   }
   printf("\n");
   return 0;
}

int sendQuery(sqlite3 *db, char* sql){
	int result;
	char *zErrMsg = 0;

	result = sqlite3_exec(db, sql, callback, 0, &zErrMsg);

	if(result != SQLITE_OK){
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
      	sqlite3_free(zErrMsg);

   	} else {
      fprintf(stdout, "Table created successfully\n");
   	}

   	return result;

}

void initilizeDB(sqlite3 *db, char* filename, char* bank){
	char sql[100];
	int result;
	FILE *file;
	char* line;
	int id;
	char* accountNum;
	char* transactionType;
	int amount;
	int balance;
	size_t len = 0;

	//printf("bank: %s file: %s\n", bank, filename);

	sprintf(sql, "CREATE TABLE %s (ID INT PRIMARY KEY NOT NULL, ACCOUNT_NUMBER INT NOT NULL, TRANSACTION_TYPE VARCHAR(10) NOT NULL, TRANSACTION_AMOUNT DECIMAL(10,2) NOT NULL, CURRENT_BALANCE DECIMAL(10,2) NOT NULL)", bank);
	//puts(sql);
	printf("%s\n",sql);

	sendQuery(db, sql);

	printf("Failing here\n");
	//printf("%s\n", filename);
   	file = fopen(filename, "r");
   //	printf("Failing here3\n");
	if(!file){
		printf("oh mu: %s\n", strerror(errno));
	}
	else{
		//printf("Failing here5\n");
		//printf("Working on fileshttps://www.youtube.com/watch?v=rcDizlmjNQY");
		while(getline(&line, &len, file) != -1)
		{
			printf("Next\n");
			if(line[0] != '\n'){
				//printf("In\n");
				id = atoi(strtok(line, " "));
				accountNum = strtok(NULL, " ");
				transactionType = strtok(NULL, " ");
				amount = atoi(strtok(NULL, " "));
				balance = atoi(strtok(NULL, " "));
				//printf("broke\n");
	   			sprintf(sql,"INSERT INTO %s (ID, ACCOUNT_NUMBER, TRANSACTION_TYPE, TRANSACTION_AMOUNT, CURRENT_BALANCE) VALUES (%i, '%s', '%s', %i, %i)",bank, id, accountNum ,transactionType, amount, balance);

	   			printf("%s\n",sql);
	   			sendQuery(db, sql);
	   		}
	   	}
	   	fclose(file);
	   	printf("done\n");
	}
	printf("complete\n");
}

void startDB(sqlite3* bank1_db, sqlite3* bank2_db){

	char *zErrMsg = 0;
	int result;

	result = sqlite3_open("bank1.db", &bank1_db);

	if(result){
		fprintf(stderr, "There was a problem with the bank1 db: %s\n", sqlite3_errmsg(bank1_db));
	}else{
		fprintf(stderr, "Bank 1 db was successfully opened\n");
		initilizeDB(bank1_db, "/home/coursework/DistributedAndCloudComputing/Program_2/Bank1/Bank1.txt", "BANK1");
		printf("BROKEN\n");
	}

	result = sqlite3_open("bank2.db", &bank2_db);

	printf("br\n");
	if(result){
		fprintf(stderr, "There was a problem with the bank2 db: %s\n", sqlite3_errmsg(bank2_db));
	}else{
		fprintf(stderr, "Bank 2 db was successfully opened\n");
		initilizeDB(bank2_db, "/home/coursework/DistributedAndCloudComputing/Program_2/Bank2/Bank2.txt", "BANK2");
	}


}
