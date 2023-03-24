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
    if (argc != 6) {
        cout << "Usage: " << argv[0] << " <filename> <word_length> <threshold> <min_prob> <max_num_fails>" << endl;
        return 1;
    }

    string filename = argv[1];
    int word_length = stoi(argv[2]);
    double threshold = stod(argv[3]);
    double min_prob = stod(argv[4]);
    int max_num_fails = stoi(argv[5]);

    if (min_prob >= 0.5) {
        cout << "Error: min_prob value must be less than 0.5" << endl;
        return 1;
    }

    ifstream file(filename);
    if (!file) {
        cout << "Error opening file " << filename << endl;
        return 1;
    }

    unordered_map<string, WordData> word_map;

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
            if (word_map[word].num_consecutive_fails >= max_num_fails || word_map[word].prob < min_prob) {
                word_map.erase(word);
            } else {
                word_map[word].prob = static_cast<double>(word_map[word].hits + threshold) / (word_map[word].hits + word_map[word].fails + 2 * threshold);
                word_map[word].nextSymb = sequence[i + word_length];
            }
        }
    }

    // Generate text
    srand(time(0)); // seed random number generator
    int max_length = 1000; // maximum length of generated text
    string seed = sequence.substr(0, word_length);
    string generated_text = seed;
    for (int i = 0; i < max_length - word_length; i++) {
        string curr_word = generated_text.substr(i, word_length);
        double rand_val = static_cast<double>(rand()) / RAND_MAX; // random number between 0 and 1
        double sum_prob = 0;
        char next_symb;
        for (auto it = word_map.begin(); it != word_map.end(); it++) {
            if (it->first.substr(0, word_length - 1) == curr_word.substr(1)) {
                sum_prob += it->second.prob;
                if (sum_prob > rand_val) {
                    next_symb = it->second.nextSymb;
                    break;
                }
            }
        }
        generated_text += next_symb;
    }

    cout << "Generated text:" << endl;
    cout << generated_text << endl;

    return 0;
}
