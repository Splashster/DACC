#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h> 

void closeDB(sqlite3 *db){
	sqlite3_close(db);
}

void startDB(){
	sqlite3 *bank1_db;
	sqlite3 *bank2_db;
	char *zErrMsg = 0;
	int result;

	result = sqlite3_open("bank1.db", &bank1_db);

	if(result){
		fprintf(stderr, "There was a problem with the bank1 db: %s\n", sqlite3_errmsg(bank1_db));
		return(0);
	}else{
		fprintf(stderr, "Bank 1 db was successfully opened\n");
		initilizeDB(bank1_db);
	}

	result = sqlite3_open("bank2.db", &bank2_db);

	if(result){
		fprintf(stderr, "There was a problem with the bank1 db: %s\n", sqlite3_errmsg(bank2_db));
		return(0);
	}else{
		fprintf(stderr, "Bank 2 db was successfully opened\n");
		initilizeDB(bank2_db);
	}


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

	result = sqlite3_exec(db, sql, callback, 0, zErrMsg);

	if(result != SQLITE_OK){
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
      	sqlite3_free(zErrMsg);
   	} else {
      fprintf(stdout, "Table created successfully\n");
   	}

   	return result;
}

void initilizeDB(sqlite3 *db, char* filename){
	char* sql;
	int result;
	FILE *file;
	char* line;
	char* id;
	char* accountNum;
	char* transactionType;
	int amount;
	int balance;
	size_t len = 0;

	sql = "CREATE TABLE BANK1 (ID INT PRIMARY KEY NOT NULL, ACCOUNT_NUMBER INT NOT NULL, TRANSACTION_TYPE VARCHAR(10) NOT NULL, TRANSACTION_AMOUNT DECIMAL(10,2) NOT NULL, CURRENT_BALANCE DECIMAL(10,2) NOT NULL)";

	sendQuery(db, sql);

   	file = fopen(filename, "r");
	if(file){
		while(getline(&line, &len, file) != -1)
		{
			if(line[0] != '\n'){
				id = strtok(line, " ");
				accountNum = strtok(NULL, " ");
				transactionType = strtok(NULL, " ");
				
	   			sql = "INSERT INTO BANK1 "
}