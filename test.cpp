class FooBar{
public:
	int test;
	bool testing;
	char evenChars;
	char *andArrays[10];

	void methodOne();
	int anotherMethod();
	bool finalFunction();
	void orMaybeMoreWithSomeCapsAndALongName();
};


bool FooBar::finalFunction(){
	return testing;
}

int FooBar::anotherMethod(){
	return test;
}

void FooBar::orMaybeMoreWithSomeCapsAndALongName(){
	int thisIsAnEvenLongerNameToTestOurEncrypting;
}

int foo(){
	int testing = 0;
	int anotherTestWithALongName = 10;
	return (testing + anotherTestWithALongName);
}

int main(){

}
