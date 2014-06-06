#include <rose.h>
#include <string>
#include <vector>
#include <cmath>
#include <boost/algorithm/string.hpp>
using namespace std;

/* This is an internal class used to ENCRYPT a vector of names. Supports the encryption of
 * variable names, function declarations, and function references.
 *
 * This class is intended to be used on a input C++ file by the user in order to encrypt the names of
 * important features in the program. User A uses this program to encrypt their desired program(s) with
 * a specific keyword, and then it can be successfully sent to User B. User B can then use the deobfuscate.C
 * file, with the same keyword, in order to decrypt the file sent by User A. The input C++ file is mostly
 * still function while encrypted, but it is 100% functional upon decryption. Problems arise when encrypting
 * a class definition file.
 *
 * Bugs: Spacing is not preserved in this step. For some reason, the compiler used, ROSE,
 * decides to not preserve spacing on the file. Also, reduced encryption for class definition files. The method
 * names and name of the class are not encrypted. Otherwise, works exactly as intended
 *  Will fix when I have time in the future.
 */
class Cipher {
	public:
		Cipher();
		virtual ~Cipher();
		void encrypt(vector<string> names);
		//getters and setters
		void addClassName(string name);
		void addEncryptedVarName(string name);
		void addEncryptedFuncName(string name);
		void addEncryptedFuncRefName(string name);
		void addVariableName(string name);
		void addFunctionName(string name);
		void addFunctionRefName(string name);
		string getName(int index, vector<string> names);
		void setKey(string key);
		string getKey();
		void setSecurityLevel(int level);
		int getSecurityLevel();
		//deal with extending the key to length of word we want to encipher
		string extendKey(string name);

		//data fields
		string fileName;
		int securityLevel;
		vector<string> variableNames;
		vector<string> functionNames;
		vector<string> functionRefNames;
		vector<string> encryptedVariables;
		vector<string> encryptedFunctions;
		vector<string> encryptedFunctionRefs;
		string key;
		vector<string> classNames;
		bool anyVars, anyFuncRefs;
		bool complete;
};

Cipher::Cipher(){
	setKey("default");
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
		if(name[i] > 64 && name[i] < 91)	//modify the temp key to uppercase letters if arg "name" has an uppercase at current index
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
	boost::algorithm::to_lower(key); //ensure our key is lowercase
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

/*Add a function name to the vector of non-encrypted function names */
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

/*Add a function reference name to the vector of non-encrypted reference names */
void
Cipher::addFunctionRefName(string name){
	this->functionRefNames.push_back(name);
}

/*Add an encrypted variable name to the vector of encrypted variable names */
void
Cipher::addEncryptedVarName(string name){
	this->encryptedVariables.push_back(name);
}

/*Add an encrypted function name to the vector of encrypted function names */
void
Cipher::addEncryptedFuncName(string name){
	this->encryptedFunctions.push_back(name);
}

/*Add an encrypted function reference name to the vector of encrypted reference names */
void
Cipher::addEncryptedFuncRefName(string name){
	this->encryptedFunctionRefs.push_back(name);
}

/*Grab the name at a specific index in the input vector*/
string
Cipher::getName(int index, vector<string> names){
	return names.at(index);
}

/*Encrypts all the names in the argument vector each letter at a time. Intended to be used in this order: Variable names, function names, function references.
 *Breaks if you call encrypt in any other order than listed above due to bottom conditions.*/
void
Cipher::encrypt(vector<string> names){
	bool varNames = true;                             //bools to control which vector we want to modify of names
	bool funcNames = true;
	string node;
	int indexMod, index, keyIndex;
	string modKey = getKey();
	if(encryptedVariables.size() > 0 || !anyVars)									 //if there are no variable names, we want to then add to the function names
		varNames = false;
	if(encryptedFunctions.size() > 0)
		funcNames = false;
	for(int x=0; x<names.size(); ++x){                //iterate through all the names in our variable names vector
		node = getName(x, names);  			          //get the current name in vector of names to be ciphered
		modKey = extendKey(node);   			      //extend the keyword to be the length of current name
		/*  The case for uppercase letters */
		for(int i=0; i<node.length(); ++i){
			if(node[i] > 64 && node[i] < 91){
				index = node[i] - 64;
				keyIndex = modKey[i] - 64;
				if(index+keyIndex > 26){              //if adding to the letter exceeds the bounds on the alphabet
					indexMod = (index+keyIndex) % 26; //offset the letter by modding by 26 to get the remainder from a-z
					node[i] = indexMod + 64;		  //modify the value to be it's respective ascii value
				}
				else
					node[i] = node[i] + keyIndex;	 //otherwise just add the keys index value
			}

		/*(same technique as above except different values for ascii conversion for lower case letters)*/
			if(node[i] > 96 && node[i] < 123){
				index = node[i] - 96;
				keyIndex = modKey[i] - 96;
				if(index+keyIndex > 26){
					indexMod = (index+keyIndex) % 26;
					node[i] = indexMod + 96;
				}
				else
					node[i] = node[i] +keyIndex;
			}
		}
		/*starts with adding all the variable names, next adds all the function names, and finally adds all the encrypted function declarations*/
		if(varNames)
			addEncryptedVarName(node);  		//add the encrypted var name to the vector
		else{
			if(!varNames && funcNames)
				addEncryptedFuncName(node);     //add the encrypted func name to the vector
			else
				addEncryptedFuncRefName(node);	//add the encrypted func ref name to the vector
		}
	}
	varNames = false;							//finished with the variables, move onto function names
	complete = true;							//encryption is completed at least once and we can then modify the input file
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
		if(!cipher.complete){
			cipher.addVariableName(nodeName);
			cipher.anyVars = true;
		}
		else{
			SageInterface::set_name(name, cipher.getName(i, cipher.encryptedVariables));
			++i;
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
	/*see if the current header file is the same as the filename we are looking at. If it is we want to modify it*/
	fileName.replace((fileName.length()-4), (fileName.length()-1), "");
	tmpName.replace((tmpName.length()-4), (tmpName.length()-1), "");

	/*If the file we are looking at isn't apart of our input file from the user, we skip it */
	if(((node->get_file_info()->get_filenameString() == cipher.fileName) && node->get_file_info()->isCompilerGenerated() == false) || (tmpName == fileName)){
	    SgFunctionDeclaration* decl = isSgFunctionDeclaration (node);
	    string nodeName = decl->get_qualified_name().str ();
		for(int i=0; i<nodeName.length(); ++i){
			if(nodeName[i] == (char) 58){					   //remove the two double colons for function names
				nodeName.erase(nodeName.begin()-1);
				nodeName.erase(nodeName.begin());
				i = nodeName.length();
			}
		}
		if(!cipher.complete)  								   //only had the name if the cipher hasn't completed
			cipher.addFunctionName(nodeName);
		else{												   //if the cipher is complete, we want to apply the new name
			if(isClassName(nodeName)){
				return;
			}
			string tmp = cipher.getName(j, cipher.encryptedFunctions);
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
		nodeName.replace(0, 13, "");   							//replace the first 12 characters that are not part of the reference
		if(!cipher.complete){	      							//only had the name if the cipher hasn't completed
			if(isLocalRef(nodeName)){
				cipher.addFunctionRefName(nodeName);
				cipher.anyFuncRefs = true;
			}
		}
		else{													//if the cipher is complete, we want to apply the new name
			newName = new SgName(cipher.getName(k, cipher.encryptedFunctionRefs));
			SgFunctionDeclaration* newDec = ref->getAssociatedFunctionDeclaration();  //grab the declaration
			newDec->set_name(*newName);						   	//set the new name
			newSymbol = new SgFunctionSymbol(newDec);		    //create a new func symbol
			ref->set_symbol(newSymbol);						  	//set the new symbol for the func ref
			k++;
		}
	}
    delete(newName);
  // 	delete(newSymbol);
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

/*Prompt the user for the keyword in order to encrypt the input file*/
string promptKeyword(){
     string tmpKey;
     cout << "Awesome. Enter your passphrase: \n";
     cin >> tmpKey;
	 return tmpKey;
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

    /*capture the name of the file in order to skip header file info*/
    Rose_STL_Container<string> fileNames = proj->getAbsolutePathFileNames();
    cipher.fileName = fileNames.at(0);

    Rose_STL_Container<SgNode *> class_names = NodeQuery::querySubTree (proj, V_SgClassDeclaration);
    for_each (class_names.begin(), class_names.end(), printClassName);

    /*capture all the variable names*/
    Rose_STL_Container<SgNode *> init_names = NodeQuery::querySubTree (proj, V_SgInitializedName);
    for_each (init_names.begin(), init_names.end(), printInitializedName);

    /*capture all the function names*/
    Rose_STL_Container<SgNode *> func_decls = NodeQuery::querySubTree (proj, V_SgFunctionDeclaration);
    for_each (func_decls.begin(), func_decls.end(), printFunctionDeclaration);

    /*capture all function reference names*/
    Rose_STL_Container<SgNode *> func_refs = NodeQuery::querySubTree (proj, V_SgFunctionRefExp);
    for_each (func_refs.begin(), func_refs.end(), printFunctionReference);


    /*set the security level*/
    cipher.setSecurityLevel(atoi(argv[2]));
    /*prompt user for the key to encrypt with*/
    cipher.setKey(promptKeyword());

    /*encrypt all the words captured from the input file*/
    cout << "Enciphering using the key word: " << cipher.getKey() << endl;
    if(cipher.getSecurityLevel() >= 1)
    	cipher.encrypt(cipher.variableNames);   	//first encrypt the variable names
    if(cipher.getSecurityLevel() >= 2){
    	cipher.encrypt(cipher.functionNames);   	//next encrypt the function names
    	cipher.encrypt(cipher.functionRefNames);   //next encrypt the function references
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
