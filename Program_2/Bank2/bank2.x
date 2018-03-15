struct accountInfo{
		char* accountNum1;
		char* accountNum2;
		int amount;
};

program	BANK2{
	version VER3 {
		int b2_credit(accountInfo) = 5;
		int b2_debit(accountInfo) = 6;

	} = 3;
} = 102;