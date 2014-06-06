#include "rose.h"
#include <string>
#include <ostream>
#include <vector>
#include <cmath>
using namespace std;
int main (int argc, char* argv[]){
    // SgProject* proj = frontend (argc, argv);
    if ( argc != 3 ) { /* argc should be 3 for correct execution
                        1) File to unimify
                        2) .txt key file to unminify file */
        printf( "usage: %s filename", argv[0] );
    }
    else {
        vector<int> specialChars;
        vector<char> minVector;
        vector<char> unMinVector;
        // argv[2] is the keyfile to open
        FILE *file = fopen( argv[2], "r" );
        /* fopen returns 0, the NULL pointer, on failure */
        if ( file == 0 ) {
            printf( "Could not open file\n" );
        }
        else  {
            char x;
            /* read one character at a time from file, stopping at EOF, which
             indicates the end of the file.  */

            while  ((x = getc(file)) != EOF) {
            	int intX = x - '0';
            	specialChars.push_back(intX);
            }
        }
        fclose(file);
        // argv[1] is the minified file to open
        file = fopen( argv[1], "r" );
        fseek(file, 0, SEEK_END);
		size_t  size = ftell(file);      // This is the size of the file.
		fseek(file, 0, SEEK_SET);        // Move the position back to the start.
		int numberOfChars = size/sizeof(char);
		if ( file == 0 ) {
            printf( "Could not open file\n" );
        }
        else  {
            char x;
            /* read one character at a time from file, stopping at EOF, which
             indicates the end of the file.  */
            while  ((x = getc(file)) != EOF ) {
            	minVector.push_back(x);
            }
            int i = 0;
            //unminify the file by pushing special characters into unMinVector
            for (std::vector<int>::iterator it = specialChars.begin(); it != specialChars.end(); ++it) {
                if ((*it) == 1) {
    				unMinVector.push_back(' ');
    				i--;
    			} else if ((*it) == 2) {
    				unMinVector.push_back('\n');
    				i--;
    			} else if ((*it) == 3) {
    				unMinVector.push_back('\t');
    				i--;
    			}
    			else if ((*it) == 4) {
    				unMinVector.push_back('\r');
    				i--;
    			} else {
    				unMinVector.push_back(minVector.at(i));
    			}
  				i++;
    		}
            std::ostringstream convert;
            string result;
            for (std::vector<char>::iterator it = unMinVector.begin(); it != unMinVector.end(); ++it) {
                convert << (*it);
                result = convert.str();
            }
            string fileName = argv[1];
            ofstream myfile;
            myfile.open (fileName.c_str());
            myfile << result;
            myfile.close();
            cout << endl;
        }
    }
    return 0;
}
