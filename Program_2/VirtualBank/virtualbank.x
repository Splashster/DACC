struct accountInfo{
		char* accountNum1;
		char* accountNum2;
		int amount;
};

program	VIRTUALBANK{
	version VER1 {
		int vb_credit(accountInfo) = 1;
		int vb_debit(accountInfo) = 2;
		int vb_transfer(accountInfo) = 3;

	} = 1;
} = 99;