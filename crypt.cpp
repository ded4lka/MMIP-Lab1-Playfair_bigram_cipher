//
// БИГРАММНЫЙ ШИФР ПЛЕЙФЕРА
//

#include <iostream>
#include <string>
#include <algorithm>

// Function that check string for letters
bool CheckString(std::string text) {
    for (int i = 0; i < text.size(); i++)
    {
        if ((text[i] >= 'a' && text[i] <= 'z') || (text[i] >= 'A' && text[i] <= 'Z') || (text[i] == ' '))
        {
            return true;
        }
        else
            return false;
    }
}

// Function that removes spaces in a string
std::string RemoveSpaces(const std::string& input) {
    std::string result;
    std::remove_copy_if(input.begin(), input.end(), std::back_inserter(result),
        ::isspace); // Copies elements from input to result, omitting spaces
    return result;
}

// Function that converts a character to uppercase
char toUpper(char c) {
    return std::toupper(c);
}


// Encrypting function
std::string Encrypt(const std::string& plaintext, const std::string& key) {

    // Remove spaces
    std::string cleanPlaintext = RemoveSpaces(plaintext);
    std::string cleanKey = RemoveSpaces(key);

    // Convert to uppercase
    for (int i = 0; i < cleanPlaintext.size(); ++i) {
        cleanPlaintext[i] = toUpper(cleanPlaintext[i]);
    }
    for (int i = 0; i < cleanKey.size(); ++i) {
        cleanKey[i] = toUpper(cleanKey[i]);
    }

    // Creating a 5x5 matrix for the key
    std::string alphabet = "ABCDEFGHIKLMNOPQRSTUVWXYZ";
    std::string matrix = cleanKey;
    
    // Adding remaining letters to the matrix
    for (char c : alphabet) {
        if (matrix.find(c) == std::string::npos)
            matrix += c;
    }

    // Replacing J with I
    std::replace(matrix.begin(), matrix.end(), 'J', 'I');

    // Partitioning into bigrams
    std::string ciphertext;
    for (int i = 0; i < cleanPlaintext.size(); i += 2) {
        char a = cleanPlaintext[i];
        char b = (i + 1 < cleanPlaintext.size()) ? cleanPlaintext[i + 1] : 'X'; // Adding X if last group contains one character
        if (a == b)
            b = 'X'; // Adding X if two characters in a group are the same

        // Finding character positions in a matrix
        size_t posA = matrix.find(a);
        size_t posB = matrix.find(b);

        // Getting the coordinates of characters in a matrix
        size_t rowA = posA / 5;
        size_t colA = posA % 5;
        size_t rowB = posB / 5;
        size_t colB = posB % 5;

        // Characters encryption
        if (rowA == rowB) { // If the characters are on the same line
            ciphertext += matrix[rowA * 5 + (colA + 1) % 5];
            ciphertext += matrix[rowB * 5 + (colB + 1) % 5];
        }
        else if (colA == colB) { // If the characters are in the same column
            ciphertext += matrix[((rowA + 1) % 5) * 5 + colA];
            ciphertext += matrix[((rowB + 1) % 5) * 5 + colB];
        }
        else { // In other cases
            ciphertext += matrix[rowA * 5 + colB];
            ciphertext += matrix[rowB * 5 + colA];
        }
    }

    return ciphertext;
}

// Decrypting function
std::string Decrypt(const std::string& ciphertext, const std::string& key) {
    // Remove spaces
    std::string cleanCiphertext = RemoveSpaces(ciphertext);
    std::string cleanKey = RemoveSpaces(key);

    // Convert to uppercase
    for (int i = 0; i < cleanCiphertext.size(); ++i) {
        cleanCiphertext[i] = toUpper(cleanCiphertext[i]);
    }
    for (int i = 0; i < cleanKey.size(); ++i) {
        cleanKey[i] = toUpper(cleanKey[i]);
    }

    // Creating a 5x5 matrix for the key
    std::string alphabet = "ABCDEFGHIKLMNOPQRSTUVWXYZ";
    std::string matrix = cleanKey;

    // Adding remaining letters to the matrix
    for (char c : alphabet) {
        if (matrix.find(c) == std::string::npos)
            matrix += c;
    }

    // Replacing J with I
    std::replace(matrix.begin(), matrix.end(), 'J', 'I');

    // Partitioning into bigrams
    std::string plaintext;
    for (int i = 0; i < cleanCiphertext.size(); i += 2) {
        char a = cleanCiphertext[i];
        char b = (i + 1 < cleanCiphertext.size()) ? cleanCiphertext[i + 1] : 'X'; // Adding X if last group contains one character
        if (a == b)
            b = 'X'; // Adding X if two characters in a group are the same

        // Finding character positions in a matrix
        size_t posA = matrix.find(a);
        size_t posB = matrix.find(b);

        // Getting the coordinates of characters in a matrix
        size_t rowA = posA / 5;
        size_t colA = posA % 5;
        size_t rowB = posB / 5;
        size_t colB = posB % 5;

        // Characters encryption
        if (rowA == rowB) { // If the characters are on the same line
            if (colA == 0) colA = 5;
            if (colB == 0) colB = 5;
            plaintext += matrix[rowA * 5 + (colA - 1) % 5];
            plaintext += matrix[rowB * 5 + (colB - 1) % 5];
        }
        else if (colA == colB) { // If the characters are in the same column
            plaintext += matrix[((rowA - 1) % 5) * 5 + colA];
            plaintext += matrix[((rowB - 1) % 5) * 5 + colB];
        }
        else { // In other cases
            plaintext += matrix[rowA * 5 + colB];
            plaintext += matrix[rowB * 5 + colA];
        }
    }
    //std::cout << std::endl << matrix << std::endl;
    return plaintext;
}


int main() {
    std::string plaintext;
    std::string key;

    // Enter plaintext
    std::cout << "Enter the text you want to encrypt: ";
    std::getline(std::cin, plaintext);
    while (!CheckString(plaintext)) {
        std::cin.clear();
        std::cout << "Invalid Input. The text can only contain Latin letters. Try again: ";
        std::getline(std::cin, plaintext);
    }

    // Enter keyword
    std::cout << "Enter the encryption keyword: ";
    std::getline(std::cin, key);
    while (!CheckString(plaintext)) {
        std::cin.clear();
        std::cout << "Invalid Input. The keyword can only contain Latin letters. Try again: ";
        std::getline(std::cin, plaintext);
    }

    // Encrypt and print result
    std::string ciphertext = Encrypt(plaintext, key);
    std::cout << "Encrypted text: " << ciphertext << std::endl;

    // Decrypt
    std::string decryptedText = Decrypt(ciphertext, key);
    std::cout << "Decrypted text: " << decryptedText << std::endl;
    return 0;
}