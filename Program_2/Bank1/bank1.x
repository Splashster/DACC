struct accountInfo{
		char* accountNum1;
		char* accountNum2;
		int amount;
};

program	BANK1{
	version VER2 {
		int b1_credit(accountInfo) = 3;
		int b1_debit(accountInfo) = 4;

	} = 2;
} = 101;