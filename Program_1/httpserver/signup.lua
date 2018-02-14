if Info.firstname == "" or Info.lastname == "" then
	string = [[ YOU CAN'T RUN FROM THE MATRIX! ]]
else 
	string = [[WECLOME TO THE MATRIX ]] .. Info.firstname .. [[ ]] .. Info.lastname
end
io.write("Lua : Now calling html_out...\n")
html_out(string)

