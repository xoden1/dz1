#include <fstream>
#include <iostream>
#include <string>
#include <vector>

int main(int argc, char* argv[]) {
	if (argc != 5) {
		std::cout << "Incorrect amount of arguments, first argument should be E or D if you want to encrypt or decrypt a file, second and third arguments are input and output file paths, fourth argument is a key (should be unsigned integer)." << std::endl;
		return 0;
	}
	bool is_encryption;
	if (static_cast<std::string>(argv[1]) == "E") is_encryption = true;
	else if (static_cast<std::string>(argv[1]) == "D") is_encryption = false;
	else {
		std::cout << "Incorrect first argument, it should be E or D if you want to encrypt or decrypt a file." << std::endl;
		return 0;
	}
	if (!std::stoi(argv[4]) || (std::stoi(argv[4]) < 0)) {
		std::cout << "Incorrect last argument, key should be unsigned integer";
		return 0;
	}
	srand(std::stoi(argv[4]));
	std::ifstream input(argv[2], std::ios_base::binary);
	if (!input) {
		std::cout << "File can't be opened!";
		return 0;
	}
	if (is_encryption) {
		std::string plaintext;
		char symbol;
		while (input.get(symbol))
			plaintext.push_back(symbol);
		input.close();
		std::vector<std::string> blocks;
		std::string ciphertext = plaintext;
		for (int i = 0; i < (8 - plaintext.size() % 8); i++) {
			ciphertext += "&";
		}
		int cnt = 0;
		for (int i = 0; i < ciphertext.size() / 8; i++) {
			blocks.push_back(ciphertext.substr(cnt, 8));
			cnt += 8;
		}
		for (std::string& i : blocks) {
			std::string gamma;
			for (int j = 0; j < 8; j++) {
				gamma.push_back(static_cast<char>(rand() % 256));
			}
			for (int k = 0; k < 8; k++) {
				i[k] ^= gamma[k];
			}
		}
		ciphertext.clear();
		for (std::string& i : blocks) {
			ciphertext += i;
		}
		rotate(ciphertext.begin(), ciphertext.begin() + 7, ciphertext.end());
		std::ofstream output(argv[3], std::ios_base::binary);
		output << ciphertext;
		output.close();
	}
	else {
		std::string ciphertext;
		char symbol;
		while (input.get(symbol))
			ciphertext.push_back(symbol);
		input.close();
		std::vector<std::string> blocks;
		std::string plaintext = ciphertext;
		std::rotate(plaintext.rbegin(), plaintext.rbegin() + 7, plaintext.rend());
		int cnt = 0;
		for (int i = 0; i < plaintext.size() / 8; i++) {
			blocks.push_back(plaintext.substr(cnt, 8));
			cnt += 8;
		}
		for (std::string& i : blocks) {
			std::string gamma;
			for (int j = 0; j < 8; j++) {
				gamma.push_back(static_cast<char>(rand() % 256));
			}
			for (int k = 0; k < 8; k++) {
				i[k] ^= gamma[k];
			}
		}
		while (blocks.back().back() == '&') blocks.back().pop_back();
		plaintext.clear();
		for (std::string& i : blocks) {
			plaintext += i;
		}
		std::ofstream output(argv[3], std::ios_base::binary);
		output << plaintext;
		output.close();
	}
	return 0;
}
