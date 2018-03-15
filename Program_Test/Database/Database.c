#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include <errno.h>
#include "Database.h"

typedef struct{
	int bank_location;
	int remaining_balance;
	int id;
}db_data;

void closeDB(sqlite3* db){
	sqlite3_close(db1);
}

static int lookUpQueryCallback(void *Used, int argc, char **argv, char **azColName) {
   db_data *theData = (db_data *)Used;
   theData->bank_location = atoi(argv[0]);
   /*int i;

   for(i = 0; i<argc; i++) {
      printf("I'm here %s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
  }*/
   return 0;
}

static int addQueryCallback(void *NotUsed, int argc, char **argv, char **azColName) {
   return 0;
}

static int fundsQueryCallback(void *Used, int argc, char **argv, char **azColName){
	db_data *theData = (db_data *)Used;
   	int i;

   for(i = 0; i<argc; i++) {
      //printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
      //if(azColName[0] == "ID"){
      	theData->id = atoi(argv[0]);
     // }
      //if(azColName[1] == "CURRENT_BALANCE"){
      	 theData->remaining_balance = atoi(argv[4]);
      	 //printf("Current bal: %i\n", theData->remaining_balance);
      //}
      //printf("Current col:%s i: %i\n", azColName[i], i);

   }


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
     // fprintf(stdout, "Created successfully\n");
   	}

   	return result;

}

int lookUpQuery(sqlite3 *db, char* sql, db_data theData){
	int result;
	char *zErrMsg = 0;

	result = sqlite3_exec(db, sql, lookUpQueryCallback, &theData, &zErrMsg);

	//printf("The result: %i\n", result);
	/*if(result != SQLITE_OK){
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
      	sqlite3_free(zErrMsg);

   	} else {
      fprintf(stdout, "Table created successfully\n");
   	/}*/
    //printf("Rowcount: %i\n", theData.row_count);
   	return theData.bank_location;

}

db_data fundsQuery(sqlite3 *db, char* sql, db_data theData){
	int result;
	char *zErrMsg = 0;

	result = sqlite3_exec(db, sql, fundsQueryCallback, &theData, &zErrMsg);

	/*if(result != SQLITE_OK){
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
      	sqlite3_free(zErrMsg);

   	} else {
      fprintf(stdout, "Table created successfully\n");
   	/}*/
    //printf("Balance: %i\n", theData.remaining_balance);
    //printf("ID: %i\n", theData.id);
	return theData;

}

void initializeDB(sqlite3 *db, char* filename, char* bank){
	char sql[300];
	int result;
	FILE *file;
	char line[300];
	int id = 0;
	char* accountNum;
	char* transactionType;
	int amount = 0;
	int balance = 0;
	int bank_location = 0;


	if(strcmp(bank,"VIRTUALBANK") == 0){
		sprintf(sql, "CREATE TABLE ACCOUNT_MAPPING (ID INTEGER PRIMARY KEY NOT NULL, ACCOUNT_NUMBER VARCHAR(50) NOT NULL, BANK_LOCATION INTEGER NOT NULL)");
		//printf("SQL: %s\n",sql);
		addQuery(db, sql);

	  file = fopen(filename, "r");
		if(!file){
			//printf("oh mu: %s\n", strerror(errno));
		}
		else{
			while(fgets(line, 300, file)!= NULL){
				if(line[0] != '\n'){
						id = atoi(strtok(line, " "));
						accountNum = strtok(NULL, " ");
						bank_location = atoi(strtok(NULL, " "));

		   			sprintf(sql,"INSERT INTO ACCOUNT_MAPPING (ID, ACCOUNT_NUMBER, BANK_LOCATION) VALUES (%i, '%s', %i)", id, accountNum , bank_location);
		   			addQuery(db, sql);
		   		}
		   	}
		   	fclose(file);
		}
	}else{
		sprintf(sql, "CREATE TABLE TRANSACTIONS (ID INTEGER PRIMARY KEY NOT NULL, ACCOUNT_NUMBER VARCHAR(50) NOT NULL, TRANSACTION_TYPE VARCHAR(10) NOT NULL, TRANSACTION_AMOUNT DECIMAL(10,2) NOT NULL, CURRENT_BALANCE DECIMAL(10,2) NOT NULL)");
		addQuery(db, sql);

	  file = fopen(filename, "r");
		if(!file){
			//printf("oh mu: %s\n", strerror(errno));
		}
		else{
			while(fgets(line, 300, file)!= NULL){
				if(line[0] != '\n'){
					id = atoi(strtok(line, " "));
					accountNum = strtok(NULL, " ");
					transactionType = strtok(NULL, " ");
					amount = atoi(strtok(NULL, " "));
					balance = atoi(strtok(NULL, " "));
		   			sprintf(sql,"INSERT INTO TRANSACTIONS (ID, ACCOUNT_NUMBER, TRANSACTION_TYPE, TRANSACTION_AMOUNT, CURRENT_BALANCE) VALUES (%i, '%s', '%s', %i, %i)", id, accountNum ,transactionType, amount, balance);
		   			addQuery(db, sql);
		   		}
		   	}
		   	fclose(file);
		}
	}
}

sqlite3* openDBConnection(int bank, sqlite3* db){

	int result = 0;

	if(bank == 1){
			result = sqlite3_open("bank1.db", &db);
			if(result){
					fprintf(stderr, "There was a problem with the bank1 db: %s\n", sqlite3_errmsg(db));
			}else{
					//fprintf(stderr, "Bank 1 db was successfully opened\n");
				}
	}else if(bank == 2){
			result = sqlite3_open("bank2.db", &db);

			if(result){
				fprintf(stderr, "There was a problem with the bank2 db: %s\n", sqlite3_errmsg(db));
			}else{
				//fprintf(stderr, "Bank 2 db was successfully opened\n");
			}
	}else{
		result = sqlite3_open("virtualbank.db", &db);

		if(result){
			fprintf(stderr, "There was a problem with the bank2 db: %s\n", sqlite3_errmsg(db));
		}else{
			//fprintf(stderr, "Bank 2 db was successfully opened\n");
		}
	}

	return db;
}

void setupDB(int bank){
	sqlite3* db;

	if(bank == 1){
		db = openDBConnection(1, db);
		initializeDB(db, "/home/coursework/DistributedAndCloudComputing/Program_2/Bank1/Bank1.txt", "BANK1");
	}else if(bank == 2){
		db = openDBConnection(2, db);
		initializeDB(db, "/home/coursework/DistributedAndCloudComputing/Program_2/Bank2/Bank2.txt", "BANK2");
	}else{
		db = openDBConnection(3, db);
		initializeDB(db, "/home/coursework/DistributedAndCloudComputing/Program_2/VirtualBank/AccountMapping.txt", "VIRTUALBANK");
	}

	closeDB(db);
}

int accountLookUP(char* accountNum){
	//int result = 0;
	//int location = 0;
	char sql[300];
	db_data theData;
	//theData.row_count = 0;

	//sqlite3* bank1_db;
	//sqlite3* bank2_db;
	sqlite3* virtual_db;

	virtual_db = openDBConnection(3, virtual_db);
	//bank2_db = openDBConnection(2, bank2_db);



	sprintf(sql, "SELECT BANK_LOCATION FROM ACCOUNT_MAPPING where ACCOUNT_NUMBER = '%s' LIMIT 1", accountNum);
	theData.bank_location = lookUpQuery(virtual_db, sql, theData);
	//printf("Location: %i\n", theData.bank_location);
	//printf("Row Count for Bank 1: %i\n", result);
	/*if(result == 1){
		location = 1;
	}else{
		sprintf(sql, "SELECT * FROM TRANSACTIONS where ACCOUNT_NUMBER = '%s' LIMIT 1", accountNum);
		result = lookUpQuery(bank2_db, sql, theData);
		//printf("Row Count for Bank 2: %i\n", result);
		if(result == 1){
			location = 2;
		}
	}*/
	if(theData.bank_location == 0){
		printf("Whoops there was a problem\n");
	}

	closeDB(virtual_db);
	return theData.bank_location;
}

int credit(int bank, char*accountNum, int amount){
	sqlite3* db;
	char sql[300];
	db_data theData;

	db = openDBConnection(bank, db);
	sprintf(sql, "SELECT * FROM TRANSACTIONS WHERE ACCOUNT_NUMBER = '%s' ORDER BY ID DESC LIMIT 1", accountNum);
	//printf("SQL: %s\n", sql);
	theData = fundsQuery(db,sql, theData);
	printf("Current Balance for Account: %s is: %i\n", accountNum, theData.remaining_balance);

	theData.remaining_balance += amount;
	theData.id += 1;
	//printf("New calculated balance: %i\n",theData.remaining_balance);
	sprintf(sql, "INSERT INTO TRANSACTIONS (ID, ACCOUNT_NUMBER, TRANSACTION_TYPE, TRANSACTION_AMOUNT, CURRENT_BALANCE) VALUES (%i, '%s', 'credit', %i, %i)", theData.id , accountNum, amount, theData.remaining_balance);
	addQuery(db, sql);

	sprintf(sql, "SELECT * FROM TRANSACTIONS WHERE ACCOUNT_NUMBER = '%s' ORDER BY ID DESC LIMIT 1", accountNum);
	theData = fundsQuery(db,sql,theData);
	printf("New Balance for Account: %s is: %i\n", accountNum, theData.remaining_balance);

	closeDB(db);
	return 1;
}

int debit(int bank, char* accountNum, int amount){
	int transactionProcessed = 0;
	sqlite3* db;
	char sql[300];
	db_data theData;


	db = openDBConnection(bank, db);
	sprintf(sql, "SELECT * FROM TRANSACTIONS WHERE ACCOUNT_NUMBER = '%s' ORDER BY ID DESC LIMIT 1", accountNum);
	//printf("SQL: %s\n", sql);
	theData = fundsQuery(db,sql, theData);
	printf("Current Balance for Account: %s is: %i\n", accountNum, theData.remaining_balance);

	if((theData.remaining_balance - amount) >= 0){
		theData.remaining_balance -= amount;
		theData.id += 1;
		sprintf(sql, "INSERT INTO TRANSACTIONS (ID, ACCOUNT_NUMBER, TRANSACTION_TYPE, TRANSACTION_AMOUNT, CURRENT_BALANCE) VALUES (%i, '%s', 'credit', %i, %i)", theData.id , accountNum, amount, theData.remaining_balance);
		addQuery(db, sql);
		sprintf(sql, "SELECT * FROM TRANSACTIONS WHERE ACCOUNT_NUMBER = '%s' ORDER BY ID DESC LIMIT 1", accountNum);
		theData = fundsQuery(db,sql,theData);
		printf("New Balance for Account: %s is: %i\n",accountNum, theData.remaining_balance);
		transactionProcessed = 1;

	}

	closeDB(db);

	return transactionProcessed;
}
