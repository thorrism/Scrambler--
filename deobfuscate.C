#include <rose.h>
#include <string>
#include <vector>
#include <cmath>
#include <boost/algorithm/string.hpp>
using namespace std;

/* This is an internal class used to DECRYPT a vectors of names. Supports the decryption of
 * variable names, function declarations, and function references.
 *
 * The use of this class is intended to be on a file that has already been encrypted. User A
 * encrypts their file with obfuscate.C and User B decrypts using this file, deobfuscate.C. In
 * order to decrypt, the user needs the same keyword used by the encryption, or else the result
 * is not correct. Code still functions the same once decrypted.
 *
 * Bugs: Spacing is not preserved due to the obfuscation step. For some reason, the compiler used
 * ROSE decides to not preserve spacing on the file. Also, their is reduced encryption for class
 * definitions and methods names cannot be encrypted. Will fix when I have time in the future.
 */
class Cipher {
	public:
		Cipher();
		virtual ~Cipher();
		void decrypt(vector<string> names);
		//getters and setters for decrytped vectors
		void addClassName(string name);
		void addDecryptedVarName(string name);
		void addDecryptedFuncName(string name);
		void addDecryptedFuncRefName(string name);
		//add encrypted names to vectors
		void addVariableName(string name);
		void addFunctionName(string name);
		void setSecurityLevel(int level);
		int getSecurityLevel();
		void addFunctionRefName(string name);
		string getName(int index, vector<string> names);
		void setKey(string key);
		string getKey();

		//deal with extending the key to length of word we want to encipher
		string extendKey(string name);

		//data fields
		vector<string> variableNames;
		vector<string> functionNames;
		vector<string> functionRefNames;
		vector<string> decryptedVariables;
		vector<string> decryptedFunctions;
		vector<string> decryptedFunctionRefs;
		string fileName;
		vector<string> classNames;
		string key;
		bool complete;
		bool anyVars, anyFuncRefs;
		int securityLevel;
};

Cipher::Cipher(){
	setKey("testing");
	securityLevel = 1;
	complete = false;
	anyVars = false;
	anyFuncRefs = false;
}

Cipher::~Cipher(){
}

/*Extend the key to be as long as the string it is encrypting. This is called upon, and used when we want to encrypt a new name*/
string
Cipher::extendKey(string name){
	string result;
	string tmpKey = key;
	int keyIndex = 0;
	for(int i=0; i<name.size(); ++i){
		if(keyIndex >= tmpKey.size())
			keyIndex = 0;
		//modify the temp key to uppercase letters if arg "name" has an uppercase at current index
		if(name[i] > 64 && name[i] < 91)
			tmpKey[keyIndex] = tmpKey[keyIndex] - 32;
		result += tmpKey[keyIndex];
		tmpKey = key;
		++keyIndex;
	}
	return result;
}

/*Set the key for the current cipher*/
void
Cipher::setKey(string key){
	boost::algorithm::to_lower(key);
	this->key = key;
}

/*Get the current key for the cipher*/
string
Cipher::getKey(){
	return this->key;
}

/*Set the security level for the cipher class*/
void
Cipher::setSecurityLevel(int level){
	this->securityLevel = level;
}

/*Get the security level for the cipher class*/
int
Cipher::getSecurityLevel(){
	return this->securityLevel;
}

/*Add a class name when we find it to the vector of class names*/
void
Cipher::addClassName(string name){
	this->classNames.push_back(name);
}

/*Add a variable name to the vector of variable names */
void
Cipher::addVariableName(string name){
	this->variableNames.push_back(name);
}

/*Add a function name to the vector of non-decrypted function names */
void
Cipher::addFunctionName(string name){
	bool found = false;
	string temp;
	for(int i=0; i<name.length(); ++i){
		if(name[i] == ':'){
			found = true;
			i+=1;
			if(name[i+1] == '~')
				i+=1;
		}
		else{
			if(found)
				temp += name[i];
		}
	}
	if(!found)
		this->functionNames.push_back(name);
	if(found){
		if(find(classNames.begin(), classNames.end(), temp) !=classNames.end())
			return;
		else
			this->functionNames.push_back(temp);
	}
}

/*Add a function reference name to the vector of non-decrypted reference names */
void
Cipher::addFunctionRefName(string name){
	this->functionRefNames.push_back(name);
}

/*Add an decrypted variable name to the vector of decrypted variable names */
void
Cipher::addDecryptedVarName(string name){
	this->decryptedVariables.push_back(name);
}

/*Add an decrypted function name to the vector of decrypted function names */
void
Cipher::addDecryptedFuncName(string name){
	this->decryptedFunctions.push_back(name);
}

/*Add an decrypted function reference name to the vector of decrypted reference names */
void
Cipher::addDecryptedFuncRefName(string name){
	this->decryptedFunctionRefs.push_back(name);
}

/*Grab the name at a specific index in the input vector*/
string
Cipher::getName(int index, vector<string> names){
	return names.at(index);
}

void
Cipher::decrypt(vector<string> names){
	bool varNames = true;
	bool funcNames = true;
	string name;
	string modKey;
	int index;
	int indexMod;
	int keyIndex;

	if(decryptedVariables.size() > 0 || !anyVars)			//if we have already added to the decrypted variables vector, now add to the encrypted functions vector
		varNames = false;
	if(decryptedFunctions.size() > 0)			            //if we have already added to the decrypted variables vector, now add to the encrypted functions vector
		funcNames = false;
	for(int x=0; x<names.size(); ++x){
		name = names[x];
		modKey = extendKey(name);
		for(int i=0; i<name.length(); ++i){
			if(name[i] > 64 && name[i] < 91){    		     //upper case letters case
				index = name[i] - 64;
				keyIndex = modKey[i] - 64;
				if( (index - keyIndex) < 1){
					indexMod = 26 - abs(index - keyIndex);
					name[i] = indexMod + 64;
				}
				else
					name[i] = (index - keyIndex) + 64;
			}
			if(name[i] > 96 && name[i] < 123){				 //lower case letters case
				index = name[i] - 96;
				keyIndex = modKey[i] - 96;
				if( (index - keyIndex) < 1){     			 //if the index goes out of bounds
					if( (index - keyIndex ) == 0)
						cout << "Is this ever a problem????" << endl;
					indexMod = 26 - abs(index - keyIndex);   //off set by the absolute value of the difference from z-a
					name[i] = indexMod + 96;
				}
				else
					name[i] = (index - keyIndex) + 96;
			}
		}
		if(varNames)
			addDecryptedVarName(name);  					//add the encrypted var name to the vector
		else{
			if(!varNames && funcNames)
				addDecryptedFuncName(name);     			//add the encrypted func name to the vector
			else
				addDecryptedFuncRefName(name);				//add the encrypted func ref name to the vector
		}
	}
	varNames = false;
	complete = true;
}
//******END OF CIPHER CLASS******//

/*global to hold our cipher.. for now. Intended to change*/
Cipher cipher;

/*Tests the argument node name to see if it is the header file for our current class file we are encrypting. Returns true if it is, false otherwise*/
static bool isClassName(string name){
	bool found = false;
	string temp;
	for(int i=0; i<name.length(); ++i){
		if(name[i] == ':'){
			found = true;
			i+=1;
			if(name[i+1] == '~')
				i+=1;
		}
		else{
			if(found)
				temp += name[i];
		}
	}
	if(find(cipher.classNames.begin(), cipher.classNames.end(), temp) !=cipher.classNames.end())
		return true;
	else
		return false;
}

/*Tests the argument node name to see if it is the header file for our current class file we are encrypting. Returns true if it is, false otherwise*/
static bool isHeader(string nodeName){
	string tmpName;
	string className;
	bool classFound = false;
	for(int i=0; i<nodeName.length(); ++i){
		if(nodeName[i] == ':'){
			classFound = true;
		}
		else{
			if(!classFound)
				tmpName+= nodeName[i];
			else
				className+= nodeName[i];
		}
	}
	/*if the two names match, must be a class constructor*/
	if(tmpName == className)
		return true;
	else
		return false;
}

/*Tests to see if a function reference expression is derived from the input false. Returns true if it is, and false if defined from an external header file*/
static bool isLocalRef(string nodeName){
	if(find(cipher.functionNames.begin(), cipher.functionNames.end(), nodeName) !=cipher.functionNames.end())
		return true;
	else
		return false;
}

/*Find all the variable names in a file first, then a second sweep of the file encrypts each name after the first sweep found them*/
static void printClassName (SgNode* node){
	static int i = 0;
	static string tmpName, fileName;
	fileName = cipher.fileName;
	tmpName = node->get_file_info()->get_filenameString();
	/*see if the current header file is the same as the filename we are looking at. If it is we want to modify it*/
	fileName.replace((fileName.length()-4), (fileName.length()-1), "");
	tmpName.replace((tmpName.length()-4), (tmpName.length()-1), "");

	/*If the file we are looking at isn't apart of our input file from the user, we skip it */
	if(((node->get_file_info()->get_filenameString() == cipher.fileName) && node->get_file_info()->isCompilerGenerated() == false) || (tmpName == fileName)){
		SgClassDeclaration *name = isSgClassDeclaration(node);
		cipher.addClassName(name->get_name().str());
	}
}

/*Find all the variable names in a file first, then a second sweep of the file encrypts each name after the first sweep found them*/
static void printInitializedName (SgNode* node){
	static int i = 0;
	static string tmpName, fileName;
	fileName = cipher.fileName;
	tmpName = node->get_file_info()->get_filenameString();
	/*see if the current header file is the same as the filename we are looking at. If it is we want to modify it*/
	fileName.replace((fileName.length()-4), (fileName.length()-1), "");
	tmpName.replace((tmpName.length()-4), (tmpName.length()-1), "");

	/*If the file we are looking at isn't apart of our input file from the user, we skip it */
	if(((node->get_file_info()->get_filenameString() == cipher.fileName) && node->get_file_info()->isCompilerGenerated() == false) || (tmpName == fileName)){
		SgInitializedName* name = isSgInitializedName (node);
		string nodeName = name->get_name().str();
		if(name->get_file_info()->isCompilerGenerated() == false){
			if(!cipher.complete){
				cipher.addVariableName(nodeName);
				cipher.anyVars = true;
			}
			else{
				SageInterface::set_name(name, cipher.getName(i, cipher.decryptedVariables));
				++i;
			}
		}
	}
}

/*Find all the function name declarations in a file first, then a second sweep of the file encrypts each name after the first sweep found them*/
static void printFunctionDeclaration (SgNode* node){
    static int j = 0;
    static int skips = 0;
    static SgName* newName;
    static string tmpName, fileName;
   	fileName = cipher.fileName;
   	tmpName = node->get_file_info()->get_filenameString();
   	//see if the current header file is the same as the filename we are looking at
   	fileName.replace((fileName.length()-4), (fileName.length()-1), "");
   	tmpName.replace((tmpName.length()-4), (tmpName.length()-1), "");

   	/*If the file we are looking at isn't apart of our input file from the user, we skip it */
	if(((node->get_file_info()->get_filenameString() == cipher.fileName) && node->get_file_info()->isCompilerGenerated() == false) || (tmpName == fileName)){
		SgFunctionDeclaration* decl = isSgFunctionDeclaration (node);
		string nodeName = decl->get_qualified_name().str ();
		for(int i=0; i<nodeName.length(); ++i){
			if(nodeName[i] == (char) 58){
				//remove the two double colons for function names
				nodeName.erase(nodeName.begin()-1);
				nodeName.erase(nodeName.begin());
				i = nodeName.length();
			}
		}
		if(!cipher.complete){								   //only had the name if the cipher hasn't completed
			cipher.addFunctionName(nodeName);
		}
		else{													//if the cipher is complete, we want to apply the new name
			if(isClassName(nodeName)){
				return;
			}
			string tmp = cipher.getName(j, cipher.decryptedFunctions);
			newName = new SgName(tmp);
			decl->set_name(*newName);
			j++;
		}
	}
			delete(newName);
}

/*Find all the function references in a file first, then a second sweep of the file encrypts each name after the first sweep found them*/
static void printFunctionReference (SgNode* node){
	static int k = 0;
	static SgName* newName;
	static SgFunctionSymbol* newSymbol;
	static string tmpName, fileName;
	fileName = cipher.fileName;
	tmpName = node->get_file_info()->get_filenameString();
	/*see if the current header file is the same as the filename we are looking at. If it is we want to modify it*/
	fileName.replace((fileName.length()-4), (fileName.length()-1), "");
	tmpName.replace((tmpName.length()-4), (tmpName.length()-1), "");

	/*If the file we are looking at isn't apart of our input file from the user, we skip it */
	if(((node->get_file_info()->get_filenameString() == cipher.fileName) && node->get_file_info()->isCompilerGenerated() == false) || (tmpName == fileName)){
		SgFunctionRefExp* ref = isSgFunctionRefExp(node);
		string nodeName = SageInterface::get_name(ref);
		nodeName.replace(0, 13, "");
		if(!cipher.complete){
			if(isLocalRef(nodeName)){
				cipher.addFunctionRefName(nodeName);
				cipher.anyFuncRefs = true;
			}
		}
		else{
			nodeName = cipher.getName(k, cipher.decryptedFunctionRefs);
			newName = new SgName(nodeName);
			SgFunctionDeclaration* newDec = ref->getAssociatedFunctionDeclaration();
			newDec->set_name(*newName);
			newSymbol = new SgFunctionSymbol(newDec);
			ref->set_symbol(newSymbol);
			k++;
		}
	}
	delete(newName);
//	delete(newSymbol);
}


/*Prompt the user for the keyword in order to encrypt the input file*/
string promptKeyword(){
     string tmpKey;
     cout << "Enter the same keyword used to decrypt this file: ";
     cin >> tmpKey;
	 return tmpKey;
}

/*Prompts the user for a security level desired. No longer used though, we capture security in the bash script*/
int promptSecurity(){
	int securityLevel;
	string input;
	cout << endl << "Hi there! It seems like you want to scramble some text. \n";
	cout << "Please select your desired security level: \n";
	cout << "1) Low security" << endl;
	cout << "2) Medium security" << endl;
	cout << "3) High security" << endl;
	getline(cin, input);
	securityLevel = atoi(input.c_str());
	if( ((securityLevel < 1) || (securityLevel > 3))){
		cout << "Please enter a valid security level..." << endl;
	    promptSecurity();
	}
	return securityLevel;
}

int main (int argc, char** argv){
	/*building a new argv to only contain the things we want*/
	int argOneLen = strlen(argv[0]);
	int argTwoLen = strlen(argv[1]);
	char newArgs[2][100];
	char* argsPtr = *newArgs;
	char** newArgv = &argsPtr;
	newArgv[0] = argv[0];
	newArgv[1] = argv[1];

	/*length is 2 in order to capture only input file and not the security level, which is the third arg*/
	SgProject* proj = frontend (2, newArgv);
    //capture the file name in order to skip header files
    Rose_STL_Container<string> fileNames = proj->getAbsolutePathFileNames();
    cipher.fileName = fileNames.at(0);
    Rose_STL_Container<SgNode *> class_names = NodeQuery::querySubTree (proj, V_SgClassDeclaration);
    for_each (class_names.begin(), class_names.end(), printClassName);

    //capture all the variable names
    Rose_STL_Container<SgNode *> init_names = NodeQuery::querySubTree (proj, V_SgInitializedName);
    for_each (init_names.begin(), init_names.end(), printInitializedName);

    //capture all the function names
    Rose_STL_Container<SgNode *> func_decls = NodeQuery::querySubTree (proj, V_SgFunctionDeclaration);
    for_each (func_decls.begin(), func_decls.end(), printFunctionDeclaration);

    //capture all function reference names
    Rose_STL_Container<SgNode *> func_refs = NodeQuery::querySubTree (proj, V_SgFunctionRefExp);
    for_each (func_refs.begin(), func_refs.end(), printFunctionReference);

    /*set the security level*/
    cipher.setSecurityLevel(atoi(argv[2]));
    /*prompt user for the key to encrypt with*/
    cipher.setKey(promptKeyword());

    /*encrypt all the words captured from the input file*/
    cout << "Decrypting using the key word: " << cipher.getKey() << endl;

    if(cipher.getSecurityLevel() >= 1)
    	cipher.decrypt(cipher.variableNames);   	//first encrypt the variable names
    if(cipher.getSecurityLevel() >= 2){
    	cipher.decrypt(cipher.functionNames);   	//next encrypt the function names
       	cipher.decrypt(cipher.functionRefNames);   //next encrypt the function references
    }

    /*now, traverse again and apply the encrypted names with the originals*/
    if(cipher.anyVars && (cipher.getSecurityLevel() >= 1))
    	for_each (init_names.begin(), init_names.end(), printInitializedName);
    	if(cipher.getSecurityLevel() >= 2){
    		if(cipher.anyFuncRefs)
    			for_each (func_refs.begin(), func_refs.end(), printFunctionReference);
    		for_each (func_decls.begin(), (func_decls.end()), printFunctionDeclaration);
        }

    /*output the input file encrypted*/
    return backend(proj);
}
