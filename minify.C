#include "rose.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
using namespace std;

int main (int argc, char** argv){
    // SgProject* proj = frontend (argc, argv);
    if ( argc != 2 ) { /* argc should be 2 for correct execution */
        printf( "usage: %s filename", argv[0] );
    }
    else {
        // assume argv[1] is a filename to open
        FILE *file = fopen( argv[1], "r" );
        fseek(file, 0, SEEK_END);
        size_t  size = ftell(file);      // This is the size of the file.
        
        fseek(file, 0, SEEK_SET);        // Move the position back to the start.
        int numberOfChars = size/sizeof(char);
        /* fopen returns 0, the NULL pointer, on failure */
        if ( file == 0 ) {
            printf( "Could not open file\n" );
        }
        else  {
            char x;
            /* read one character at a time from file, stopping at EOF, which
             indicates the end of the file.  */
            int counter = 0;
            vector<int> specialChars (numberOfChars, 0);
            vector<char> minFile;
            while  ( ( x = getc(file)) != EOF ) {
                if ((char)x == ' ') {
                    specialChars.at(counter) = 1;
                } else if (x == '\n') {
                    specialChars.at(counter) = 2;
                } else if (x == '\t') {
                    specialChars.at(counter) = 3;
                }
                else if (x == '\r') {
                    specialChars.at(counter) = 4;
                } else  {
                    minFile.push_back(x);
                }
                counter++;
            }
            
//            cout << "----MINIFIED FILE---- \n \n";
//            for (std::vector<char>::iterator it = minFile.begin(); it != minFile.end(); ++it) {
//                std::cout << *it;
//            }
            vector<string> keyStringArray;
            ostringstream convert;
            string result;
            cout << endl;
            fclose( file );
            for (std::vector<int>::iterator it = specialChars.begin(); it != specialChars.end(); ++it) {
                convert << (*it);
                result = convert.str();
            }
        string fileName = argv[1];
        //remove file extension from file name
        std::size_t found = fileName.find(".");
        if (found!=std::string::npos) {
            fileName.erase(fileName.begin()+found, fileName.end());
        }
        string keyFileSuffix = "keyFile.txt";
        //keyFile is named filename_keyFile.txt without the extension of the original filename
        fileName += keyFileSuffix; 

        cout << result;
        ofstream myfile;
        myfile.open (keyFileSuffix.c_str());
        myfile << result;
        myfile.close();
        
        FILE *minFileOutput;
        fileName = argv[1];
        fileName.replace(0,5, "");
        string encName = "min_";
        encName += fileName;
        // names the file min_filename
        minFileOutput=fopen(encName.c_str(), "wb");
        fwrite(&minFile[0], sizeof(char), minFile.size(), minFileOutput);
        fclose(minFileOutput);
    }
}
return 0;
}
