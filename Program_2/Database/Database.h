void startDB();
void closeDB(sqlite3* bank1_db, sqlite3* bank2_db);
int accountLookUp(sqlite3* bank1_db, sqlite3* bank2_db, char* accountNum);