
Command to build all files
/------------------------------------/
make

Command to start RPC Services
/------------------------------------/
make run_services -j3

Command to start client
/------------------------------------/
make run_client (in order to run client and have data entered in automatically)
./client (in order run client and enter data manually)

Command to delete files created by make
/------------------------------------/
make clean

Database Initialization Files
/------------------------------------/
AccountMapping.txt
Bank1.txt
Bank2.txt

Format for valid transaction
/------------------------------------/
Credit <Account Number> <Credit Amount>
Debit <Account Number> <Debit Amount>
Transfer <Account Number1> <Account Number2> <Transfer Amount>
Quit


Instructions
/-----------------------------------------/
1. Open two terminals
2. Run make in the first terminal
3. Start the RPC services in the first terminal
4. Start client in the second terminal



Notes
/-----------------------------------------/
-Format for the database initialization files are as follows
    -ALL FILES MUST BE DELIMITED BY SPACES
    -AccountMapping.txt (ID, Account Number, Bank Number)
    -Bank1.txt (ID, TransactionType, AccountNumber, Transaction Amount, Current Balance)
    -Bank2.txt (ID, TransactionType, AccountNumber, Transaction Amount, Current Balance)
-In order add new accounts do the following
    -Add a new entry in the AccountMapping.txt file
        -Make sure the ID is incremented and doesn't match an ID already in the file
        -Also, make sure that each item in the entry is delimited by a space
    -Add a new entry in either Bank1 or Bank2.txt (Place into the bank that you specified in the AccountMapping.txt file)
        -Make sure the ID is incremented and doesn't match an ID already in the file
        -Also, make sure that each item in the entry is delimited by a space
-Assumptions
    -The current balance of the account is the current balance of the last entry
        -This means that this program does not try to calculate what it should be based on the initialization data
    -If an account number is located in two banks, the program will choose bank1 as the bank to work with
        -It will not try to update both banks
    -QUIT COMMAND WILL ALWAYS BE PROVIDED WHEN PASSING COMMANDS THROUGH MAKEFILE
        -If quit command is not provided then the program segfaults once the entered transactions are completed

