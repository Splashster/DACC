#include <sqlite3.h>

void setupDB();
void closeDB(sqlite3* bank1_db, sqlite3* bank2_db);
int accountLookUp(char* accountNum);
int credit(int bank, char*accountNum, int amount);