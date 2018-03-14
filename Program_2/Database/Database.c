#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include <errno.h>
#include "Database.h"

typedef struct{
	int row_count;
}db_data;

void closeDB(sqlite3* bank1_db, sqlite3* bank2_db){
	sqlite3_close(bank1_db);
	sqlite3_close(bank2_db);

}

static int lookUpQueryCallback(void *Used, int argc, char **argv, char **azColName) {
   /*int i;
   fprintf(stderr, "Here %s: ", (const char*)data);
   for(i = 0; i<argc; i++) {
      printf("Over here %s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
   }
   printf("\n");

   printf("Num Rows Returned: %s\n", argv[0]);
   row_count = argv[0];*/
   db_data *theData = (db_data *)Used;

   theData->row_count = atoi(argv[0]);
   return 0;
}

static int addQueryCallback(void *NotUsed, int argc, char **argv, char **azColName) {
   int i;
   for(i = 0; i<argc; i++) {
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
   }
   printf("\n");
   return 0;
}

int addQuery(sqlite3 *db, char* sql){
	int result;
	char *zErrMsg = 0;

	result = sqlite3_exec(db, sql, addQueryCallback, 0, &zErrMsg);

	if(result != SQLITE_OK){
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
      	sqlite3_free(zErrMsg);

   	} else {
      fprintf(stdout, "Table created successfully\n");
   	}

   	return result;

}

int lookUpQuery(sqlite3 *db, char* sql){
	int result;
	char *zErrMsg = 0;
	const char* data = "Callback Function";
	db_data theData;

	result = sqlite3_exec(db, sql, lookUpQueryCallback, &theData, &zErrMsg);

	//printf("The result: %i\n", result);
	/*if(result != SQLITE_OK){
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
      	sqlite3_free(zErrMsg);

   	} else {
      fprintf(stdout, "Table created successfully\n");
   	/}*/
     //printf("Rowcount: %i\n", theData.row_count);
   	return theData.row_count;

}

void initializeDB(sqlite3 *db, char* filename, char* bank){
	char sql[300];
	int result;
	FILE *file;
	char line[300];
	int id;
	char* accountNum;
	char* transactionType;
	int amount;
	int balance;
	//size_t len = 0;

	//line = (char*) malloc(len + 1);
	//printf("bank: %s file: %s\n", bank, filename);

	sprintf(sql, "CREATE TABLE %s (ID INT PRIMARY KEY NOT NULL, ACCOUNT_NUMBER INT NOT NULL, TRANSACTION_TYPE VARCHAR(10) NOT NULL, TRANSACTION_AMOUNT DECIMAL(10,2) NOT NULL, CURRENT_BALANCE DECIMAL(10,2) NOT NULL)", bank);
	//puts(sql);
	//printf("%s\n",sql);

	addQuery(db, sql);

	//printf("Failing here\n");
	//printf("%s\n", filename);
   	file = fopen(filename, "r");
   //	printf("Failing here3\n");
	if(!file){
		//printf("oh mu: %s\n", strerror(errno));
	}
	else{
		//printf("Failing here5\n");
		//printf("Working on fileshttps://www.youtube.com/watch?v=rcDizlmjNQY");
		//getline(&line, &len, file) != -1
		while(fgets(line, 300, file)!= NULL){
			//printf("Next\n");
			if(line[0] != '\n'){
				//printf("In\n");
				id = atoi(strtok(line, " "));
				accountNum = strtok(NULL, " ");
				transactionType = strtok(NULL, " ");
				amount = atoi(strtok(NULL, " "));
				balance = atoi(strtok(NULL, " "));
				//printf("broke\n");
	   			sprintf(sql,"INSERT INTO %s (ID, ACCOUNT_NUMBER, TRANSACTION_TYPE, TRANSACTION_AMOUNT, CURRENT_BALANCE) VALUES (%i, '%s', '%s', %i, %i)",bank, id, accountNum ,transactionType, amount, balance);

	   			//printf("%s\n",sql);
	   			addQuery(db, sql);
	   		}
	   	}
	   	fclose(file);
	   //	printf("done\n");
	}
	//printf("complete\n");
}

sqlite3* openDBConnection(int bank, sqlite3* db){

	int result = 0;

	if(bank == 1){
			result = sqlite3_open("bank1.db", &db);
			if(result){
					fprintf(stderr, "There was a problem with the bank1 db: %s\n", sqlite3_errmsg(db));
			}else{
					fprintf(stderr, "Bank 1 db was successfully opened\n");
				}
	}else{
			result = sqlite3_open("bank2.db", &db);
	
			if(result){
				fprintf(stderr, "There was a problem with the bank2 db: %s\n", sqlite3_errmsg(db));
			}else{
				fprintf(stderr, "Bank 2 db was successfully opened\n");
			}
	}

	return db;
}

void setupDB(){
	sqlite3* bank1_db;
	sqlite3* bank2_db;


	bank1_db = openDBConnection(1, bank1_db);
	bank2_db = openDBConnection(2, bank2_db);
	initializeDB(bank1_db, "/home/coursework/DistributedAndCloudComputing/Program_2/Bank1/Bank1.txt", "BANK1");		
	initializeDB(bank2_db, "/home/coursework/DistributedAndCloudComputing/Program_2/Bank2/Bank2.txt", "BANK2");
	
	closeDB(bank1_db, bank2_db);
}

int accountLookUP(char* accountNum){
	int result = 0;
	int location = 0;
	char sql[300];

	sqlite3* bank1_db;
	sqlite3* bank2_db;

	bank1_db = openDBConnection(1, bank1_db);
	bank2_db = openDBConnection(2, bank2_db);


	//printf("I'm working\n");
	sprintf(sql, "SELECT COUNT(*) FROM BANK1 where ACCOUNT_NUMBER = '%s'", accountNum);
	result = lookUpQuery(bank1_db, sql);
	printf("Row Count for Bank 1: %i\n", result);
	if(result == 1){
		location = 1;
	}else{
		sprintf(sql, "SELECT COUNT(*) FROM BANK2 where ACCOUNT_NUMBER = '%s'", accountNum);
		result = lookUpQuery(bank2_db, sql);
		printf("Row Count for Bank 2: %i\n", result);
		if(result == 1){
			location = 2;
		}
	}
	if(location == 0){
		printf("Whoops there was a problem\n");
	}

	closeDB(bank1_db, bank2_db);
	return location;
}

int credit(){
	int result = 0;
	return result;
}

int debit(){
	int result = 0;
	return result;
}

int transfer(){
	int result = 0;
	return result;
}