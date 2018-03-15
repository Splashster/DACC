#include <sqlite3.h>

void setupDB();
void closeDB(sqlite3* db1, sqlite3* db2, sqlite3* db3);
int accountLookUp(char* accountNum);
int credit(int bank, char*accountNum, int amount);
int debit(int bank, char* accountNum, int amount);