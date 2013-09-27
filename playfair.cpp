/**
	Scott C. Stauffer <scottyeatscode.blogspot.com>
	26 September 2013

	This is my Playfair cipher written from scratch.
 */

#include <iostream>
#include <cctype>
#include <vector>
using namespace std;

void input(string, string&);
vector<string> make_digraphs(string);
vector<string> make_cipher_digraphs(string);
void filter(string&);
bool contains(string, char);
string unduplicate(string);
string strip_x(string);
char** build_matrix(string, bool);
void display_matrix(char**);
pair<int,int> get_location(char**, char);
string encrypt(char**, vector<string>);
string decrypt(char**, vector<string>);
void enc_example(bool), dec_example(bool);

int main(int param_count, char ** params) {
	cout << "playfair cipher by scstauf" << endl << endl;
	string arg_one(""), arg_two("");

	if (param_count == 3) {
		arg_one = params[1],
		arg_two = params[2];
		
		if (arg_one.compare("encrypt") == 0) {
			if (arg_two.compare("qless") == 0)
				enc_example(true);
			else if (arg_two.compare("jless") == 0)
				enc_example(false);
			else
				cerr << "invalid parameter: " << params[2] << endl;
		} else if (arg_one.compare("decrypt") == 0) {
			if (arg_two.compare("qless") == 0)
				dec_example(true);
			else if (arg_two.compare("jless") == 0)
				dec_example(false);
			else
				cerr << "invalid parameter: " << params[2] << endl;
		} else
			cerr << "invalid parameter: " << params[1] << endl;
	} else
		cout << "usage: " << params[0] << " [crypt_option] [omission_type]" << endl << endl
			 << "crypt_options: encrypt, decrypt" << endl
			 << "omission_type: qless, jless" << endl;
}

// display_matrix(char**)
// displays the contents of the matrix
// used for manual debugging
void display_matrix(char** matrix) {
	for (int row = 0; row < 5; row++) {
		for (int col = 0; col < 5; col++)
			cout << matrix[row][col] << " ";
		cout << endl;
	}
}

// encrypt(char**, vector<string>)
// returns an encrypted Playfair ciphertext from plaintext
string encrypt(char** matrix, vector<string> digraphs) {
	pair<int,int> one, two;
	string cipher("");
	
	for (unsigned int i = 0; i < digraphs.size(); i++) {
		one = get_location(matrix, digraphs.at(i)[0]), 
		two = get_location(matrix, digraphs.at(i)[1]);
		
		if (one.first == two.first) {
			// same row
			if (one.second == 4)
				cipher.push_back(matrix[one.first][0]);
			else
				cipher.push_back(matrix[one.first][one.second + 1]);
				
			if (two.second == 4)
				cipher.push_back(matrix[two.first][0]);
			else
				cipher.push_back(matrix[two.first][two.second + 1]);
				
		} else if (one.second == two.second) {
			// same column
			if (one.first == 4)
				cipher.push_back(matrix[0][one.second]);
			else
				cipher.push_back(matrix[one.first + 1][one.second]);
				
			if (two.first == 4)
				cipher.push_back(matrix[0][two.second]);
			else
				cipher.push_back(matrix[two.first + 1][two.second]);
		} else {
			// form a rectangle, get corners
			cipher.push_back(matrix[one.first][two.second]);
			cipher.push_back(matrix[two.first][one.second]);
		}
	}
	
	return cipher;
}

// decrypt(char**, vector<string>)
// returns a decrypted Playfair plaintext from ciphertext
string decrypt(char** matrix, vector<string> digraphs) {
	pair<int,int> one, two;
	string plaintext("");
	
	for (unsigned int i = 0; i < digraphs.size(); i++) {
		one = get_location(matrix, digraphs.at(i)[0]), 
		two = get_location(matrix, digraphs.at(i)[1]);
		
		if (one.first == two.first) {
			// same row
			if (one.second == 0)
				plaintext.push_back(matrix[one.first][4]);
			else
				plaintext.push_back(matrix[one.first][one.second - 1]);
				
			if (two.second == 0)
				plaintext.push_back(matrix[two.first][4]);
			else
				plaintext.push_back(matrix[two.first][two.second - 1]);
				
		} else if (one.second == two.second) {
			// same column
			if (one.first == 0)
				plaintext.push_back(matrix[4][one.second]);
			else
				plaintext.push_back(matrix[one.first - 1][one.second]);
				
			if (two.first == 0)
				plaintext.push_back(matrix[4][two.second]);
			else
				plaintext.push_back(matrix[two.first - 1][two.second]);
		} else {
			// form a rectangle, get corners
			plaintext.push_back(matrix[one.first][two.second]);
			plaintext.push_back(matrix[two.first][one.second]);
		}
	}
	
	return strip_x(plaintext);
}

// get_location(char**,char)
// gets the location of a given char in the matrix
pair<int,int> get_location(char** matrix, char test) {
	// a pair of ints for row and col of given char in matrix
	pair<int,int> location;
	// helps halt iteration when row and col found
	bool found = false;
	
	// iterate the matrix
	for (int row = 0; row < 5; row++) {
		for (int col = 0; col < 5; col++) {
			// if current row and column value matches given char
			if (matrix[row][col] == test) {
				location.first = row; // set first int to row
				location.second = col; // set second int to col
				found = true; // we've found the location, so halt.
				break; // break this loop
			}
		}
		
		// if location is found, break this loop.
		if (found)
			break;
	}

	return location;
}

// contains(string,char)
// returns true if string contains given char
// returns false if string does not contain given char
bool contains(string input, char test) {
	// for each char in string
	for (unsigned int i = 0; i < input.length(); i++)
		// if match found
		if (input[i] == test)
			return true;
	
	return false;
}

// unduplicate(string)
// removes duplicate chars in string
string unduplicate(string input) {
	string output(""); // filtered string
	// for each char in input string
	for (unsigned int i = 0; i < input.length(); i++)
		// if output string does not contain the char
		if (!contains(output, input[i]))
			// append the char to the output string
			output.push_back(input[i]);
	// return new string
	return output;
}

// build_matrix(string)
// returns 5x5 Playfair matrix
char** build_matrix(string keyword, bool qless) {
	string salt(unduplicate(keyword)), // salt for the matrix
		   qless_alphabet("ABCDEFGHIJKLMNOPRSTUVWXYZ"), // Q-less filler
		   jless_alphabet("ABCDEFGHIKLMNOPQRSTUVWXYZ"), // J-less filler
		   alphabet(""), // chosen alphabet
		   content(""); // string form of matrix
	
	// create a two-dimensional char array
	char** matrix = new char*[5];
	
	for (int i = 0; i < 5; i++)
		matrix[i] = new char[5];
	
	// salt the content of the matrix
	content.append(salt);
	
	alphabet = ((qless) ? qless_alphabet : jless_alphabet);
	
	// fill the rest of the content with the Q-less or J-less alphabet
	// being sure not to add chars that already exist in
	// the content...
	
	for (unsigned int i = 0; i < alphabet.length(); i++)
		if (!contains(content, alphabet[i]))
			content.push_back(alphabet[i]);
	
	// populate the actual matrix with the contents
	for (int row = 0, i = 0; row < 5; row++)
		for (int col = 0; col < 5; col++, i++)
			matrix[row][col] = content[i];
	
	return matrix;
}

// input(string,string&)
// makes code easier to read
void input(string prompt, string &out) {
	cout << prompt;
	getline(cin, out, '\n');
}

// filter(string&)
// strips punctuation and fixes case
void filter(string &text) {
	string filtered(""); // filtered text
	
	// for each char in text,
	for (unsigned int i = 0; i < text.length(); i++)
		// if char is a letter,
		if (isalpha(text[i]))
			// append uppercase letter to filtered text
			filtered.push_back(toupper(text[i]));
	
	// replace original text with filtered text
	text = filtered;
}

// strip_x(string)
// strips the 'X' chars that Playfair inserts between
// a digraph with two identical chars
string strip_x(string text) {
	string stripped("");
	
	for (unsigned int i = 0; i < text.length(); i++) {
		if (text[i] == 'X' && !(i == (text.length() - 1))) {
			if (text[i - 1] != text[i + 1])
				stripped.push_back('X');
		} else {
			if (text[i] == 'X' && (i == (text.length() - 1)))
				continue;
			stripped.push_back(text[i]);
		}
	}
	
	return stripped;
}

// make_cipher_digraphs(string)
// returns a vector of digraphs (2-letter tokens)
vector<string> make_cipher_digraphs(string ciphertext) {
	vector<string> digraphs;
	string digraph(""); // for building digraph vector		   
	int expected_char = 1; // first or second char in digraph
	
	// for each char in fixed text,
	for (unsigned int i = 0; i < ciphertext.length(); i++) {
		// if second char is expected,
		if (expected_char == 2) {
			// finish building digraph
			digraph.push_back(ciphertext[i]);		
			// push the current digraph to digraph vector
			digraphs.push_back(digraph);
			digraph.clear(); // clear current digraph
			expected_char = 1; // reset the expected char
		} else {
			// start building digraph
			digraph.push_back(ciphertext[i]);
			expected_char++;
		}
	}

	return digraphs;
}

// make_digraphs(string)
// returns a vector of digraphs (2-letter tokens)
vector<string> make_digraphs(string plaintext) {
	vector<string> digraphs;
	
	string digraph(""), // for building digraph vector
		   fixed_text(""); // text ready for tokenization
		   
	int expected_char = 1; // first or second char in digraph
	
	char prev_char = ' ';
	
	// if we encounter a sequence where a char is the same
	// as the next, insert an 'X' and continue building
	// the fixed text. "HELLOWORLD" becomes: "HELXLOWORLD"
	
	// for each char in text
	for (unsigned int i = 0; i < plaintext.length(); i++) {
		// if prev char is same as current char
		if (prev_char == plaintext[i]) {
			fixed_text.push_back('X'); // append an 'X'
			fixed_text.push_back(plaintext[i]); // append current char
			prev_char == 'X'; // set prev char to 'X'
		} else {
			fixed_text.push_back(plaintext[i]); // append new char
			prev_char = plaintext[i]; // set prev char to current char
		}
	}
	
	// if fixed text has odd length
	if ((int)fixed_text.length() % 2 != 0)
		fixed_text.push_back('X'); // make even with a terminal 'X'
	
	// for each char in fixed text,
	for (unsigned int i = 0; i < fixed_text.length(); i++) {
		// if second char is expected,
		if (expected_char == 2) {
			// finish building digraph
			digraph.push_back(fixed_text[i]);		
			// push the current digraph to digraph vector
			digraphs.push_back(digraph);
			digraph.clear(); // clear current digraph
			expected_char = 1; // reset the expected char
		} else {
			// start building digraph
			digraph.push_back(fixed_text[i]);
			expected_char++;
		}
	}

	return digraphs;
}

void enc_example(bool qless) {
	string keyword(""), message(""), cipher("");
	vector<string> digraphs;	
	char** matrix;

	input("enter the keyword: ", keyword);
	filter(keyword);
	
	input("enter the plain-text: ", message);
	filter(message);
	
	digraphs = make_digraphs(message);
	matrix = (qless) ? build_matrix(keyword, true) : build_matrix(keyword, false);
	cipher = encrypt(matrix, digraphs);
	
	cout << "\ncipher-text:\t" << encrypt(matrix, digraphs) << endl;
}

void dec_example(bool qless) {
	string keyword(""), cipher("");
	char** matrix;
	
	input("enter the keyword: ", keyword);
	filter(keyword);
	input("enter the cipher-text: ", cipher);
	// no need to filter the ciphertext

	matrix = (qless) ? build_matrix(keyword, true) : build_matrix(keyword, false);
	
	cout << "\nplain-text:\t" << decrypt(matrix, make_cipher_digraphs(cipher)) << endl;
}
