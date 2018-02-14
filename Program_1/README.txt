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

Valid lua script
/-------------------------------------/
signup.lua


Instructions
/-----------------------------------------/
1. Start the httpserver
2. Go to a valid webpage using the port number that the server successfully binded on


Notes
/-----------------------------------------/
-In order to trigger the signup.lua script you must do one of the following
	1)Go to the signup.html page, enter a firstname and lastname, and hit the submit button.
 	2)Send a request for the signup.lua and manually enter a firstname and lastname into the URL
		-Example http://localhost:10067/signup.lua?firstname=bob&lastname=bill
	3)Send a request for just the signup.lua script
		-Example http://localhost:10067/signup.lua
-The lua script will only print out the firstname and lastname if the request is in the following format
	-Example http://localhost:10067/signup.lua?firstname=bob&lastname=bill
	-If this format is not followed, the lua script will print another message out instead.

-Httpserver's default page is index.html
