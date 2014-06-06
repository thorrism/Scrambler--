#!/bin/bash
echo
echo Hi there!
echo Let\'s unscramble your colleague\'s file. Please enter the file name
    read FILE_TO_DECRYPT
    
    # if the file was minified, it will start with min_ . 
    minFlag=${FILE_TO_DECRYPT:0:4}

echo
echo Please select the security level used to encrypt this file!
   OPTIONS="Low Medium High"
   select opt in $OPTIONS; do
       if [ "$opt" = "Low" ]; then
        echo
        echo You\'ve chosen low security. Only variable names will be changed.
        	./deobfuscate.exe $FILE_TO_DECRYPT '1'
        	rose_enc_name="rose_"
    		rose_name+=$FILE_TO_DECRYPT
    		rose_enc_name+=$FILE_TO_DECRYPT
    		originalFileName=${rose_name#*rose_enc_}
    		echo
    		echo All done! The file is your\'s for the viewing. The name is $rose_enc_name
        exit

       elif [ "$opt" = "Medium" ]; then
        echo
        echo You\'ve chosen medium security. Variable and function names will be scrambled.
        	./deobfuscate.exe $FILE_TO_DECRYPT '2'
        	rose_enc_name="rose_"
    		rose_name+=$FILE_TO_DECRYPT
    		rose_enc_name+=$FILE_TO_DECRYPT
    		originalFileName=${rose_name#*rose_enc_}
    		echo
    		echo All done! The file is your\'s for the viewing. The name is $rose_enc_name
        exit

       elif [ "$opt" = "High" ]; then
        echo
	 	echo This file has been minified. Enter the name of the key file your colleague gave you to unminify:
	    read KEY_FILE
	    echo $KEY_FILE
	        ./unminify.exe $FILE_TO_DECRYPT $KEY_FILE
	        echo ========= Done unminify\'ing your file. Now decrypting. =============
	        ./deobfuscate.exe $FILE_TO_DECRYPT '3'
	        rose_enc_name="rose_"
	        rose_name+=$FILE_TO_DECRYPT
		rose_enc_name+=$FILE_TO_DECRYPT 
	        originalFileName=${rose_name#*rose_min_enc_}
	        echo
	        echo All done! The file is your\'s for the viewing. The name is $rose_enc_name
        exit

        else
        	clear
        	echo Please enter 1, 2 or 3 as an option
       echo
       fi
      
   done
