/**
	Scott C. Stauffer <scottyeatscode.blogspot.com>
	27 September 2013

	This is my Vigenere cipher written from scratch.
 */

#include <iostream>
#include <algorithm>
#include <cctype>
using namespace std;

string caesar_shift(string);
void filter(string&);
string increase_key(string, int);
void input(string prompt, string &out);
char** build_vigenere_table();
void display_vigenere_table(char** table);
string encrypt(char**, string, string);
string decrypt(char**, string, string);

int main() {
	char** table = build_vigenere_table();
	
	string key(""), plaintext(""), ciphertext("");
	
	input("enter key: ", key);
	input("enter plaintext: ", plaintext);
	
	filter(key);
	filter(plaintext);
	
	key = increase_key(key, (int)plaintext.length());
	
	ciphertext = encrypt(table, plaintext, key);
	cout << "\nciphertext: " << ciphertext << endl;
	cout << "plaintext: " << decrypt(table, ciphertext, key) << endl;
}

void display_vigenere_table(char** table) {
	for (int row = 0; row <= 25; row++) {
		for (int col = 0; col <= 25; col++)
			cout << table[row][col] << " ";
		
		cout << endl;
	}
}

string encrypt(char** table, string plaintext, string key) {
	string ciphertext("");
	
	for (unsigned int i = 0; i < plaintext.length(); i++) {
		for (int row = 0; row <= 25; row++) {
			if (table[row][0] == plaintext[i]) {
				ciphertext.push_back(table[row][key[i] - 'A']);
				break;
			}
		}
	}
	
	return ciphertext;
}

string decrypt(char** table, string ciphertext, string key) {
	string plaintext("");
	
	for (unsigned int i = 0; i < ciphertext.length(); i++) {
		for (int col = 0; col <= 25; col++) {
			if (table[0][col] == key[i]) {
				for (int row = 0; row <= 25; row++) {
					if (table[row][col] == ciphertext[i]) {
						plaintext.push_back(table[row][0]);
						break;
					}
				}
			}
		}
	}
	
	return plaintext;
}

char** build_vigenere_table() {
	string alphabet("ABCDEFGHIJKLMNOPQRSTUVWXYZ");
	char** table = new char*[26];
	
	for (int i = 0; i <= 25; i++)
		table[i] = new char[26];
	
	for (int row = 0, i = 0; row <= 25; row++) {
		for (int col = 0; col <= 25; col++, i++)
			table[row][col] = alphabet[i];
		
		if (i == 25)
			i = 0;
			
		alphabet = caesar_shift(alphabet);
	}
	
	return table;
}

string increase_key(string key, int max_size) {
	string inc(key);
	
	while (inc.length() < max_size) {
		inc.push_back(key[0]);
		rotate(key.begin(), key.begin() + 1, key.end());
	}
	
	return inc;
}

void input(string prompt, string &out) {
	cout << prompt;
	getline(cin, out, '\n');
}

string caesar_shift(string input) {	
	rotate(input.begin(), input.begin() + 1, input.end());
	
	return input;
}

void filter(string &input) {
	string output("");
	
	for (unsigned int i = 0; i < input.length(); i++)
		if (isalpha(input[i]))
			output.push_back(toupper(input[i]));
	
	input = output;
}
