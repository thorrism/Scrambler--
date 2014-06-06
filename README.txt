 In order to run the file:
 >>make clean
 >>make
 Now, you can run the bash scripts to test the program.	
 Within this folder contains all the files necessary to run our program. You just need to enter the command line or terminal and navigate
 to the location where this file exist that you download, or git pulled. Included, is a test file in order for the program to be tested.
 Next, do the following:
 
To ENCRYPT a file you type:
 ./theScrambler.sh
Follow the steps that the console asks including a file to input and encrypt, and a security level.
What is output is a file either called "min_youfile.cpp" along with "keyFile.txt" for level 3 or "enc_youfile.cpp" for level 1/2.

To DECRYPT a file you type:
 ./theUnscrambler.sh
Follow the steps that the console asks including an encrypted file to input and decrypt, a security level used to encrypt that file, and a key file if you chose level 3.
What is output is a file either called "rose_min_yourfile.cpp" for level 3 or "rose_enc_yourfile.cpp" for level 1/2.

FOR EXAMPLE:
./theScrambler.sh
"Hi there!"
"It seems you want to scramble a file. Please enter a file name:"
>>test.cpp

"Please select your desired security level"
"1) Low"
"2) Medium"
"3) High"
>>#? 3

"Awesome. Enter your passphrase: "
>>testing

And what is output is a file called min_test.cpp, and keyFile.txt

./theUnscrambler.sh
"Hi there!"
"It seems you want to unscramble a file. Please enter a file name:"
>>min_test.cpp

"Please select your desired security level"
"1) Low"
"2) Medium"
"3) High"
>>3

"This file has been minified. Please enter the key file you were provided"
>>keyFile.txt

"Enter the same keyword used to decrypt this file: "
>>testing

And what is output is a file called rose_min_test.cpp this is the same functioning and looking (somewhat) file
as test.cpp from before the scrambler!

Scrambler++ is an obfuscation tool created in C++ that can encrypt and minify code for distribution. The program allows User A to use a scrambler script 
through the terminal or command line, that takes an input file and secret keyword, and creates a new program with the name "enc_yourfile.cpp" or 
"min_yourfile.cpp". A keyFile.txt file is output if you choose our highest security level, so that you can unminify it adding back all the original spacings.
User A can now safely send the encrypted file along with the keyFile.txt file, if necessary, to User B. Then, User B can use the unscrambler script along 
with the keyword and the keyFile.txt to reverse engineer the file back to it's original state.</p>

Our tool offers three levels of security:
 >>Variable names only
 >>Function declarations/references and Variable names
 >>All of the above and minifed (all spacings removed)

