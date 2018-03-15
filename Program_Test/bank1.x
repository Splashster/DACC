struct bank1AccountInfo{
		char* accountNum1;
		char* accountNum2;
		int amount;
};

program	BANK1{
	version VER2 {
		int b1_credit(bank1AccountInfo) = 3;
		int b1_debit(bank1AccountInfo) = 4;

	} = 2;
} = 101;
