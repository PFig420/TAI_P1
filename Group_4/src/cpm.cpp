#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <cmath>

using namespace std;

struct WordData {
    int hits;
    int fails;
    double prob;
    char nextSymb;
    int num_consecutive_fails;
};

int main(int argc, char* argv[])
{
    if (argc != 5) {
        cout << "Usage: " << argv[0] << " <filename> <word_length> <threshold> <max_num_fails>" << endl;
        return 1;
    }

    string filename = argv[1];
    int word_length = stoi(argv[2]);
    double threshold = stod(argv[3]);
    int max_num_fails = stoi(argv[4]);

    ifstream file(filename);
    if (!file) {
        cout << "Error opening file " << filename << endl;
        return 1;
    }

    unordered_map<string, WordData> word_map;
    unordered_map<char, int> symbol_counts;

    string sequence;
    file >> sequence;

    int len = sequence.length();
    for (int i = 0; i <= len - word_length; i++) {
        string word = sequence.substr(i, word_length);

        if (word_map.find(word) == word_map.end()) {
            // Word not found in map, initialize entry
            word_map[word] = {1, 0, 0.5, sequence[i + word_length]};
        } else {
            // Word found in map, update values
            if (sequence[i + word_length] == word_map[word].nextSymb) {
                word_map[word].hits++;
                word_map[word].num_consecutive_fails = 0;
            } else {
                word_map[word].fails++;
                word_map[word].num_consecutive_fails++;
            }
            if (word_map[word].num_consecutive_fails >= max_num_fails) {
                word_map.erase(word);
            } else {
                word_map[word].prob = static_cast<double>(word_map[word].hits + threshold) / (word_map[word].hits + word_map[word].fails + 2 * threshold);
                word_map[word].nextSymb = sequence[i + word_length];
            }
        }
    }

    int total_symbols = 0;
    for (int i = 0; i < len; i++) {
        char symb = sequence[i];
        if (symbol_counts.find(symb) == symbol_counts.end()) {
            symbol_counts[symb] = 1;
        } else {
            symbol_counts[symb]++;
        }
        total_symbols++;
    }

    cout << "List of words and their associated data:" << endl;
    for (auto it = word_map.begin(); it != word_map.end(); it++) {
        cout << it->first << ": hits = " << it->second.hits
             << ", fails = " << it->second.fails
             << ", prob = " << it->second.prob
             << ", nextSymb = " << it->second.nextSymb
             << ", num_consecutive_fails = " << it->second.num_consecutive_fails << endl;
    }

    double total_bits = 0.0;

    cout << "\nList of symbols and their associated probabilities:" << endl;
    for (auto it = symbol_counts.begin(); it != symbol_counts.end(); it++) {
        double prob = static_cast<double>(it->second) / total_symbols;
        total_bits += -log2(prob);
        cout << it->first << ": " << prob << endl;
    }

    double avg_bits_per_symbol = total_bits / total_symbols;

    cout << "\nEstimated total number of bits: " << total_bits << endl;
    cout << "Average number of bits per symbol: " << avg_bits_per_symbol << endl;

    return 0;
}
