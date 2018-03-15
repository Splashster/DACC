struct bank2AccountInfo{
		char accountNum1[50];
		char accountNum2[50];
		int amount;
};

program	BANK2{
	version VER3 {
		int b2_credit(bank2AccountInfo) = 5;
		int b2_debit(bank2AccountInfo) = 6;

	} = 3;
} = 102;
