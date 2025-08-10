#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <cctype>

// NOTE: The FIRST set computation is based on the provided reference file.
// The FOLLOW set computation logic is added based on standard compiler design principles.

/*
 * Computation of FOLLOW(X)
 *
 * 1. FOLLOW(S) = {$} (S is the start symbol)
 *
 * 2. If A -> pBq is a production, where p and q can be any grammar string,
 * then FOLLOW(B) = FIRST(q) - {e}. (e represents epsilon)
 *
 * 3. If A -> pB is a production, then FOLLOW(B) = FOLLOW(A).
 *
 * 4. If A -> pBq is a production and FIRST(q) contains e,
 * then FOLLOW(B) = (FIRST(q) - {e}) U FOLLOW(A).
 */

// maps for non-terminal to index mapping and vice-versa.
std::map<char, int> nt_to_index;
std::map<int, char> index_to_nt;

// Stores the productions for each non-terminal.
std::vector<std::vector<std::string>> prods;

// Stores the computed FIRST and FOLLOW sets for each non-terminal.
std::vector<std::set<char>> first_sets;
std::vector<std::set<char>> follow_sets;

// Function to print a set of characters.
void print_set(const std::set<char>& s) {
    for (char c : s) {
        std::cout << c << " ";
    }
    std::cout << std::endl;
}

// Parses productions like "E->T|a" into a vector {"T", "a"}.
std::vector<std::string> parse_productions(const std::string& raw_productions, const std::string& delimiter) {
    std::string production;
    std::vector<std::string> productions;
    size_t pos = 0;
    std::string temp_raw = raw_productions;

    while ((pos = temp_raw.find(delimiter)) != std::string::npos) {
        production = temp_raw.substr(0, pos);
        productions.push_back(production);
        temp_raw.erase(0, pos + delimiter.length());
    }
    productions.push_back(temp_raw);
    return productions;
}

// Initializes the production vector from the input string.
void initialize_productions(const std::string& input) {
    std::vector<std::string> productions_str = parse_productions(input, " ");
    int index = 0;
    for (const auto& p_str : productions_str) {
        char nt = p_str[0];
        nt_to_index[nt] = index;
        index_to_nt[index] = nt;
        
        std::string rhs = p_str.substr(3);
        prods.push_back(parse_productions(rhs, "|"));
        index++;
    }
    first_sets.resize(prods.size());
    follow_sets.resize(prods.size());
}


// Computes the FIRST set for a given non-terminal.
void compute_first(int nt_index) {
    if (!first_sets[nt_index].empty()) {
        return; // Already computed
    }

    for (const auto& rule : prods[nt_index]) {
        char first_char = rule[0];
        if (islower(first_char) || !isalpha(first_char)) { // Terminal or epsilon
            first_sets[nt_index].insert(first_char);
        } else { // Non-terminal
            int first_nt_index = nt_to_index[first_char];
            compute_first(first_nt_index);
            for (char c : first_sets[first_nt_index]) {
                first_sets[nt_index].insert(c);
            }
        }
    }
}

// Main function to compute all FIRST sets.
void compute_all_firsts() {
    for (size_t i = 0; i < prods.size(); ++i) {
        compute_first(i);
    }
}

// Computes the FOLLOW set for all non-terminals.
void compute_all_follows() {
    // Rule 1: Place $ in FOLLOW(S) where S is the start symbol (first non-terminal).
    follow_sets[0].insert('$');

    // Iterate until no new terminals can be added to any FOLLOW set.
    bool changed = true;
    while (changed) {
        changed = false;
        for (size_t i = 0; i < prods.size(); ++i) {
            char A = index_to_nt[i];
            for (const auto& rule : prods[i]) {
                for (size_t j = 0; j < rule.length(); ++j) {
                    char B = rule[j];
                    if (isupper(B)) { // B is a non-terminal
                        int b_index = nt_to_index[B];
                        size_t original_size = follow_sets[b_index].size();

                        // Check for characters after B
                        if (j + 1 < rule.length()) {
                            char next_char = rule[j + 1];
                            if (islower(next_char) || !isalpha(next_char)) { // Terminal
                                follow_sets[b_index].insert(next_char);
                            } else { // Non-terminal
                                int next_nt_index = nt_to_index[next_char];
                                for (char c : first_sets[next_nt_index]) {
                                    if (c != 'e') {
                                        follow_sets[b_index].insert(c);
                                    }
                                }
                                // If FIRST(next_char) contains epsilon
                                if (first_sets[next_nt_index].count('e')) {
                                     for (char c : follow_sets[i]) {
                                        follow_sets[b_index].insert(c);
                                    }
                                }
                            }
                        } else { // B is the last symbol in the rule
                             for (char c : follow_sets[i]) {
                                follow_sets[b_index].insert(c);
                            }
                        }
                        if (follow_sets[b_index].size() > original_size) {
                            changed = true;
                        }
                    }
                }
            }
        }
    }
}


int main() {
    // Example productions: E->TK K->+TK|e T->FZ Z->*FZ|e F->(E)|i
    // 'e' represents epsilon.
    std::string productions = "E->TK K->+TK|e T->FZ Z->*FZ|e F->(E)|i";
    
    initialize_productions(productions);

    std::cout << "--- Computing FIRST sets ---" << std::endl;
    compute_all_firsts();
    for (size_t i = 0; i < first_sets.size(); ++i) {
        std::cout << "FIRST(" << index_to_nt[i] << ") = { ";
        print_set(first_sets[i]);
        std::cout << "}" << std::endl;
    }

    std::cout << "\n--- Computing FOLLOW sets ---" << std::endl;
    compute_all_follows();
    for (size_t i = 0; i < follow_sets.size(); ++i) {
        std::cout << "FOLLOW(" << index_to_nt[i] << ") = { ";
        print_set(follow_sets[i]);
        std::cout << "}" << std::endl;
    }

    return 0;
}
