Command to start http server
/------------------------------------/
cd /Program_1/httpserver
make

Valid webpages
/------------------------------------/
index.html
greenscreen.html
wd.txt
signup.html
signup.lua


Instructions
/-----------------------------------------/
1. Start the httpserver
2. Go to a valid webpage using the port number that the server successfully binded on


Notes
/-----------------------------------------/
-In order to trigger the signup.lua script you must do one of the following
	1)Go to the signup.html page, enter a firstname and lastname, and hit the submit button.
 	2)Send a request for the signup.lua and manually enter a firstname and lastname
		-Example http://localhost:10067/singup.lua?firstname=bob&lastname=bill
-By httpserver default page is index.html
