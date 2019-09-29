#include<iostream>  //base 1ppl
#include<string>
#include<vector>

using namespace std;

string rotary[6]; // 1~5
string reflector;  //A | B | C
int selectRol[3];
int iniNum[6], curNum[3], plugNum = 0;

class Wire {
public:
	Wire(){}
	Wire(char p1, char p2) :w1(p1), w2(p2) {}
	int w1, w2;
};
vector<Wire> wire;

void decrypt();

void initial(int[], string);  //rotary index, state  1ppl  //iniNum, curNum, rotary*5, wire*10
string encrypt(string);  //1ppl  call encry()
char encry(char);  //3ppl
void rotate();  //1ppl  rotate curNum
int searchRotary(char, int);  //1ppl
char searchPlugBoard(char);

int main(int args, char* argv[]) {
	if(args > 1)
	if (argv[1][0] == '-' && argv[1][1] == 'd') {
		decrypt();
		return 0;
	}


	string curState, plugState;  //each 3 char
								 // 3 2 1
	cout << "Input three rotor numbers which you want to use.(1~5)\n"
		<< "Separate these numbers with space:";
	for (int i = 0; i < 3; ++i)
		cin >> selectRol[i];
	// WFR XDH
	cout << "Input the three rotor's starting alphabet and current alphabet:(XDH)";
	cin >> curState;

	cout << "Input plugboard:";
	cin.ignore();
	getline(cin, plugState);
	for (int i = 0; i < plugState.length(); i += 3) {
		Wire tempWire(plugState[i], plugState[i + 1]);
		wire.push_back(tempWire);
		/*wire[plugNum].w1 = plugState[i];
		wire[plugNum++].w2 = plugState[i + 1];*/
	}

	initial(selectRol, curState);
	string input, output;
	while (true) {
		cout << "Input the plaintext what you want to encrypt:";
		cin >> input;
		output = encrypt(input);
		cout << "Output:";
		cout << output << endl;
	}
}

void initial(int selectRol[], string curState) {
	rotary[1] = "EKMFLGDQVZNTOWYHXUSPAIBRCJ";
	rotary[2] = "AJDKSIRUXBLHWTMCQGZNPYFVOE";
	rotary[3] = "BDFHJLCPRTXVZNYEIWGAKMUSQO";
	rotary[4] = "ESOVPZJAYQUIRHXLNFTGKDCMWB";
	rotary[5] = "VZBRGITYUPSDNHLXAWMJQOFECK";
	reflector = "YRUHQSLDPXNGOKMIEBFZCWVJAT";  //B now
	iniNum[1] = 'R' - 'A';
	iniNum[2] = 'F' - 'A';
	iniNum[3] = 'W' - 'A';
	iniNum[4] = 'K' - 'A';
	iniNum[5] = 'A' - 'A';
	for (int i = 0; i < 3; ++i) {
		curNum[i] = curState[i] - 'A';
	}
	/*
	// check it works well
	for(int i = 0; i < 3; ++i)
	cout << rotary[selectRol[i]][iniNum[i]] << rotary[selectRol[i]][curNum[i]] << endl;
	*/
}

string encrypt(string plaintext) {
	string ciphertext;
	for (char i : plaintext) {
		char c = encry(i);
		ciphertext.push_back(c);
	}
	return ciphertext;
}

char encry(char input) {
	int c = searchPlugBoard(input) - 'A';

	rotate();  //rotate rotary
	for (int i = 0; i < 3; ++i) {  //first 3 rotary
		c = (rotary[selectRol[i]][(c + curNum[i]) % 26] + 26 - curNum[i] - 'A') % 26;
		//cout << c << char(c + 'A') << endl;
	}

	for (int i = 0; i < 26; ++i) {  // reflect
		if (c == reflector[i] - 'A') {
			c = i;
			//cout << c << char(c + 'A') << endl;
			break;
		}
	}

	for (int i = 2; i >= 0; --i) {  //last 3 rotary
		c = (searchRotary(char((c + curNum[i]) % 26 + 'A'), i) + 26 - curNum[i]) % 26;
		//cout << c << char(c + 'A') << endl;
	}

	return searchPlugBoard(char(c + 'A'));
}

void rotate() {
	curNum[0] = (curNum[0] + 1) % 26;
	if (curNum[0] == iniNum[selectRol[0]] || curNum[1] + 1 == iniNum[selectRol[1]]) {		//�P�_�O�_����Ц�m
		curNum[1] = (curNum[1] + 1) % 26;
		if (curNum[1] == iniNum[selectRol[1]]) {
			curNum[2] = (curNum[2] + 1) % 26;
		}
	}
}

int searchRotary(char c, int rotaryNum) {
	for (int i = 0; i < 26; ++i) {
		if (c == rotary[selectRol[rotaryNum]][i])
			return i;
	}
}

char searchPlugBoard(char c) {
	if (wire.size() == 0) return c;  //no plug set
	for (int i = 0; i < wire.size(); ++i) {  //plug
		if (c == wire[i].w1) {
			return wire[i].w2;
		}
		else if (c == wire[i].w2) {
			return wire[i].w1;
		}
	}
	return c;
}


//initialCurrent
//int wireSize = 0;  //max 6
string plain =  "HEILHITLER";
string cipher = "IPQHUGCXZM";
//(I,H) (I,Q) (I,G) (H,L) (H,U) (L,X)  //first level 4 or 5 plug --> go down  7 plug = wrong  6 plug --> test encry
//(E,P) (E,Z)  //5 plug --> go down   7 plug = wrong  6 plug --> test
//(T,C) (R,M)  //                     7 plug = wrong  6 plug --> test
Wire correctPair[10] = { Wire('I','H'), Wire('I','Q'), Wire('I','G'), Wire('H','L'), Wire('H','U'), Wire('L','X'), Wire('E','P'), Wire('E','Z'), Wire('T','C'), Wire('R','M') };
int pairPosition[10] = { 0, 2, 5, 3, 4, 7, 1, 8, 6, 9 };
char guessChars[14] = { 'I', 'H', 'Q', 'G', 'L', 'U', 'X', 'E', 'P', 'Z', 'T', 'C', 'R', 'M' };
//vector<Wire> testPlug;  //max 6
char testChar[3] = { 'I', 'E', 'C' };
//plug test I, E, C    (I,A) (I,B) ... (I,Z)    (E,A) ... (E,Z)   (C,A) ... (C,Z)
//I, H, Q, G, L, U, X; E, P, Z; T, C, R, M

int curNumSave[3];  //use this to guess 26*26*26 cases
vector<int> curPlug;  //current choosed plug set
vector<Wire> guessPlug;
vector<char> guessSingle;
vector<char> isSingle;
vector<Wire> wrongPlug;
vector<char> wrongSingle;

bool searchGuessPlug(Wire&);  //search the same
bool checkGuessPlug(Wire&);  //check if contradiction, true for no contradiction
bool searchGuessSingle(char);
bool checkGuessSingle(char);
bool searchIsSingle(char);
bool searchWrongPlug(Wire&);
bool searchWrongSingle(char);

void changeRotary();
//void decryptInitial();
bool rotaryChoose();  //first rotary, call rotaryChoose2() recursively
bool rotaryChoose2();  //second rotary, call rotaryChoose3() recursively
bool rotaryChoose3();  //third rotary, call plugTest()

bool plugTest();  //first level plug set, call plugTest2() recursively
bool pt(char);
//bool plugTest2();  //second level, call plugTest3()
//bool plugTest3();  //third level, call exhaustiveAtk()

char encryRotary(char);

bool exhaustiveAtk(int, string&);  //exhaustive attack recursively, can input the except string of letters
bool ex(int plugAmount, string& exception, int lastPlugNum);
bool checkExcept(int, string&);
bool checkAns();  //call setMachine() then throw "HEILHITLER" into encrypt() and check if equal to "IPQHUGCXZM"

void setMachine();  //set the setting of the test  e.g. rotary, plug    ; call setPlug()
void setPlug();  //set curPlug(vector<int>) into wire[12](Wire)  //can combine into setMachine()

void unrotate();  //rotate inversed

void decrypt() {




}

bool plugTest() {
	curPlug.clear();
	guessPlug.clear();
	guessSingle.clear();
	isSingle.clear();
	wrongPlug.clear();
	wrongSingle.clear();
	for (int i = 0; i < 3; ++i)
		curNum[i] = curNumSave[i];

	for (int i = 0; i < 14; ++i) {
		if (pt(guessChars[i]))
			return true;
	}

	return false;
}

bool pt(char guessChar) {

	curPlug.push_back(guessChar-'A');
	Wire guess;
	guess.w1 = guessChar;
	for (int i = 0; i < 25; ++i) {
		if (i == guessChar - 'A') continue;
		guess.w2 = i + 'A';
		if (searchGuessPlug(guess) || searchWrongPlug(guess)) continue;

		curPlug.push_back(i);
		guessPlug.push_back(guess);  //first guess

		for (int j = 0; j < 10; ++j) {
			if (correctPair[j].w1 == guessChar) {  //(I,H) (I,Q) (I,G)
				for (int k = 0; k < 3; ++k)
					curNum[i] = curNumSave[i];
				for (int k = 0; k < pairPosition[j]; ++k)
					rotate();
				
				char outc = encryRotary(i + 'A');
				if (outc != correctPair[j].w2) {  //Wire(outc, correctPiar[j].w2)
					Wire newGuess(outc, correctPair[j].w2);
					guessPlug.push_back(newGuess);
					if (searchWrongPlug(newGuess)) {
						for (int k = 0; k < guessPlug.size(); ++k) {
							if (!searchWrongPlug(guessPlug[k]))
								wrongPlug.push_back(guessPlug[k]);
						}
						guessPlug.clear();
						break;
					}
					if (checkGuessPlug(newGuess) == false) {  //contradiction
						for (int k = 0; k < guessPlug.size(); ++k) {
							if (!searchWrongPlug(guessPlug[k]))
								wrongPlug.push_back(guessPlug[k]);
						}
						guessPlug.clear();
						break;
					}
					else {
						//nothing, check next pair
					}
				}
				else {  //single
					if (checkGuessSingle(outc)) {  //no contradiction
						guessSingle.push_back(outc);
					}
					else {  //contradiction
						for (int k = 0; k < guessPlug.size(); ++k) {
							if (!searchWrongPlug(guessPlug[k]))
								wrongPlug.push_back(guessPlug[k]);
						}
						guessPlug.clear();
						break;
					}
				}


			}
		}
		//call exhaustiveAtk if less than 6 plug



	}
	//guessChar is single



	return false;
}


char encryRotary(char input) {  //encrypt only on rotary
	int c = input - 'A';
	for (int i = 0; i < 3; ++i)  //first 3 rotary
		c = (rotary[selectRol[i]][(c + curNum[i]) % 26] + 26 - curNum[i] - 'A') % 26;

	for (int i = 0; i < 26; ++i)  // reflect
		if (c == reflector[i] - 'A') {
			c = i;
			//cout << c << char(c + 'A') << endl;
			break;
		}

	for (int i = 2; i >= 0; --i)  //last 3 rotary
		c = (searchRotary(char((c + curNum[i]) % 26 + 'A'), i) + 26 - curNum[i]) % 26;

	return char(c + 'A');
}

bool exhaustiveAtk(int plugAmount, string& exception) {
	int p[12] = { 0 };
	for (int i = 0; i < 25 - 2 * plugAmount; ++i) {
		if (checkExcept(i, exception))
			continue;
		curPlug.push_back(i);
		if (ex(plugAmount, exception, i))
			return true;
		else
			curPlug.pop_back();
	}
	return false;
}

bool ex(int plugAmount, string& exception, int lastPlugNum) {
	bool second = curPlug.size() % 2 ? true : false;
	for (int i = lastPlugNum + 1; i < (second ? 26 : 25) - 2 * plugAmount; ++i) {
		if (checkExcept(i, exception))
			continue;
		curPlug.push_back(i);
		if (second) {
			if (plugAmount == 1) {  //last plug choosed
				if (checkAns())
					return true;
				else
					curPlug.pop_back();
			}
			else {
				if (ex(plugAmount - 1, exception, i))
					return true;
				else
					curPlug.pop_back();
			}
		}
		else {
			if (ex(plugAmount, exception, i))
				return true;
			else
				curPlug.pop_back();
		}
	}


	return false;
}

bool checkExcept(int letter, string& exception) {
	char c = letter + 'A';
	if (exception.find(c) != exception.npos)  //c is in exception
		return true;
	return false;
}

bool checkAns() {
	return false;
}



bool searchGuessPlug(Wire& plug) {
	for (int i = 0; i < guessPlug.size(); ++i)
		if ((plug.w1 == guessPlug[i].w1 && plug.w2 == guessPlug[i].w2) || (plug.w1 == guessPlug[i].w2 && plug.w2 == guessPlug[i].w1))
			return true;
	return false;
}

bool checkGuessPlug(Wire& plug) {  //check if no contradiction of plug, true for no contradiction
	if (searchGuessPlug(plug))  //same
		return true;
	for (int i = 0; i < guessPlug.size(); ++i) {
		if (plug.w1 == guessPlug[i].w1 || plug.w1 == guessPlug[i].w2 || plug.w2 == guessPlug[i].w1 || plug.w2 == guessPlug[i].w2)  //contradiction
			return false;
	}
	for (int i = 0; i < guessSingle.size(); ++i) {
		if (plug.w1 == guessSingle[i] || plug.w2 == guessSingle[i])  //contradiction
			return false;
	}
	for (int i = 0; i < isSingle.size(); ++i) {
		if (plug.w1 == isSingle[i] || plug.w2 == isSingle[i])  //contradiction
			return false;
	}
	return true;
}

bool searchGuessSingle(char c) {
	for (int i = 0; i < guessSingle.size(); ++i) {
		if (c == guessSingle[i])
			return true;
	}
	return false;
}

bool checkGuessSingle(char c) {  //check if single
	if (searchIsSingle(c) || searchGuessSingle(c))
		return true;
	for (int i = 0; i < guessPlug.size(); ++i) {
		if (c == guessPlug[i].w1 || c == guessPlug[i].w2)  //contradiction
			return false;
	}
	if (searchWrongSingle(c))  //contradiction
		return false;
	return true;
}

bool searchIsSingle(char c) {
	for (int i = 0; i < isSingle.size(); ++i) {
		if (c == isSingle[i])
			return true;
	}
	return false;
}

bool searchWrongPlug(Wire& plug) {
	for (int i = 0; i < wrongPlug.size(); ++i)
		if ((plug.w1 == wrongPlug[i].w1 && plug.w2 == wrongPlug[i].w2) || (plug.w1 == wrongPlug[i].w2 && plug.w2 == wrongPlug[i].w1))
			return true;
	return false;
}

bool searchWrongSingle(char c) {
	for (int i = 0; i < wrongSingle.size(); ++i) {
		if (c == wrongSingle[i])
			return true;
	}
	return false;
}