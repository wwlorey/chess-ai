#### CS 5400 Artificial Intelligence | Bitboard-based chess AI

#### Provided README:

#################################
#	Coding Standards	#
#################################

You are free to use any of the following programming languages for your submission : 
	- C++
	- C#
	- Java
	- Python3 (NOT Python2)
Your code must be well formatted and styled according to good coding standards, such as the MST coding standard outlined here : 
http://web.mst.edu/~cpp/cpp_coding_standard_v1_1.pdf
It is required that your code is well documented.

NOTE : Sloppy, undocumented, or otherwise unreadable code will be penalized for not following good coding standards (as laid out in the grading rubric on the course website) : 
http://web.mst.edu/~tauritzd/courses/cs5400/sp2019/ 

#################################
#          !IMPORTANT!          #
#################################

Read the README in the sub-directory corresponding to your chosen language.  

Read the chess framework's documentation for your chosen language here : http://docs.siggame.io/chess/ 

#################################
#	Submission Rules	#
#################################

Included in your repository is a script named ”finalize.sh”, which you will use to indicate which version of your code is the one to be graded. When you are ready to submit your final version, run the either the command ./finalize.sh or ./finalize.sh -language_flag from your local Git directory, then commit and push your code. Running the finalize script without a language flag will cause the script to run an interactive prompt where you may enter your programming language. Alternatively, you can pass a -j flag when running the finalize script to indicate that you are submitting in Java (i.e. ./finalize.sh -j). The flag -j indicates Java, -cpp indicates C++, -cs indicates C#, and -p indicates Python 3. This will create a text file, ”readyToSubmit.txt,” that is pre-populated with a known text message for grading purposes. You may commit and push as much as you want, but your submission will be confirmed as ”final” if ”readyToSubmit.txt” exists and is populated with the text generated by ”finalize.sh” at 11:59pm on the due date. If you do not plan to submit before the deadline, then you should NOT run the ”finalize.sh” script until your final submission is ready. If you accidentally run ”finalize.sh” before you are ready to submit, do not commit or push your repo and delete ”readyToSubmit.txt.” Once your final submission is ready, run ”finalize.sh”, commit and push your code, and do not make any further changes to it

Late submissions will be penalized 5% for the first 24 hour period and an additional 10% for every 24 hour period thereafter. Any submission turned in more than 5 days late will not be graded, and will result in a score of 0 for that assignment. 

#################################
#       Compiling & Running	#
#################################

You have been provided a bash script called "play.sh", which compiles and runs your code; it also starts a game session between your AI and itself. DO NOT MODIFY THIS SCRIPT.
You can run "play.sh" using the following command format :

	./play.sh Joueur.<lang> Session_ID testRunArg1 ... testRunArgN

Where Joueur.<lang> is the directory for the language you are coding in and where testRunArg# is an optional argument that is accepted by the provided testRun script. An example of the above command for c++ would be :

	./play.sh Joueur.cpp AIisAwesome

	./play.sh Joueur.cpp AIisAwesome --gameSettings fen=b6k%2F8%2F8%2F8%2F5p2%2F6q1%2F4P3%2F7K%20w%20K%20-%200%201

You can find some helpful FEN strings in the file FEN.txt. There are FENs for testing castling, en passant and check.

You can find a more advanced play.sh script in the adv_tools sub-directory. It can play multiple games, redirect game output to files, check output files for win/draw/stalemate/error in bulk, run a local server, run against a different client on the same computer, run on the forge, run on a specified server address, and supports all arguments you could provide to testRun. Run ./play.sh --help to see usage.

#################################
#      	    OPTIONALLY		#
#################################

You have also been provided a bash script called "compress.sh", which compresses the designated directory in a way that is compatible with the Sig-Game Arena interface.
You can run "compress.sh" using the following command format :
	./compress.sh Joueur.<lang>
Where Joueur.<lang> is the directory for the language you are coding in. An example of the above command for c++ would be :
        ./compress.sh Joueur.cpp

You can register for an account and upload your Joueur.<lang>.zip to the Sig-Game page here : https://chess.siggame.io

Don't forget you are limited to 150MB compressed filesize.

