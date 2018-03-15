#include <sqlite3.h>

void setupDB(int bank);
int accountLookUp(char* accountNum);
int credit(int bank, char*accountNum, int amount);
int debit(int bank, char* accountNum, int amount);