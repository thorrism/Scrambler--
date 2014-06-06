#!/bin/bash
echo
echo Hi there! 
echo It seems like you want to scramble a file. Please enter the file name:
	read NAME
	rose_name="rose_"
	rose_name+=$NAME

echo
echo Please select your desired security level
   OPTIONS="Low Medium High"
   select opt in $OPTIONS; do
       if [ "$opt" = "Low" ]; then
        echo You\'ve chosen low security. Only variable names will be changed.
        echo
        	./obfuscate.exe $NAME '1'
        	# rename rose_fileName to enc_fileName
        	newFileName="enc_"
        	newFileName+=${rose_name#*rose_}
        	echo 
        	echo
        	echo Your file is now encrypted! To decrypt the file, send with the $newFileName file. 
        	echo
        	echo Your colleague will also need to know the passphrase you entered while decrypting the file.
        	echo
        exit

       elif [ "$opt" = "Medium" ]; then
        echo You\'ve chosen medium security. Variable and function names will be scrambled.
        echo
        	./obfuscate.exe $NAME '2'
        	# rename rose_fileName to enc_fileName
        	newFileName="enc_"
        	newFileName+=${rose_name#*rose_}
        	echo 
        	echo
        	echo Your file is now encrypted! To decrypt the file, send with the $newFileName file. 
        	echo
        	echo Your colleague will also need to know the passphrase you entered while decrypting the file.
        	echo
        exit

       elif [ "$opt" = "High" ]; then
       	echo You\'ve chosen High security. Not only will Variable and function names be scrambled, the text will also be minified.
       	echo
        	echo $NAME
        	./obfuscate.exe $NAME '3'
        	newFileName="rose_"
        	newFileName+=$NAME
        	./minify.exe $newFileName

        	minifiedFileName="min_"
        	minifiedFileName+=$NAME
        	
        	keyFile=*_keyFile.txt

        	echo 
        	echo
        	echo Your file is now encrypted! To decrypt the file, send with the $minifiedFileName file along with the keyFile.txt file. 
        	echo
        	echo Your colleague will also need to know the passphrase you entered while decrypting the file.
        	echo
        exit

        else
        	clear
        	echo Please enter 1, 2 or 3 as an option
       fi
   done

