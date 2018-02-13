io.write("Lua : The value of first name in the table Info is: ", Info.firstname,"\n" , "Lua : The value of the last name in the table info is: ", Info.lastname,  "\n");
io.write("Lua : Now calling html_out...\n")
string = [[WECLOME TO THE MATRIX ]] .. Info.firstname .. [[ ]] .. Info.lastname
html_out(string)

