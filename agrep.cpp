/*
Pattern aware grep version 0.1
Made by Annttu (at) <repeatmynick> dot fi

TODO:
-t option (search patterns first and print output on second interation)
-r option (recursive search)

better algorithm to separate english words from patterns
*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <string.h>

using namespace std;

const int DATEPART = 1;
const int PATTERNT_MIN_LENGTH = 10;
const string RED = "\033[1;31m";
const string GREEN = "\033[1;32m";
const string RESET = "\033[0m";
char const validChars[] = "abcdefghijklmnopqrstuvwxyz"
                          "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                          "0123456789"
                          "-";

vector<std::string> patterns;
// Ignore these words
vector<std::string> ignored;

void add_ignored(void)
{
    ignored.push_back((std::string)"SAEximRunCond");
    ignored.push_back((std::string)"Message-Id");
}

bool number_lines = false;
bool iterate_twice = false;
bool DEBUG = false;

void help(void) {
    cout << "Usage agrep [-n] pattern file [file2 ...]" << endl;
}

string get_date(string* line) {
    string date = "";
    char prev_char;
    int count = 0;
    for (std::string::size_type i = 0; i < (*line).length(); i++)
    {
        if (isspace((*line)[i]))
        {
            if (!isspace(prev_char)) {
                ++count;
                if (count > DATEPART) break;
                else date += ' ';
            }
        }
        else {
            date += (*line)[i];
        }
        prev_char = (*line)[i];
    }
    return date;
}

inline bool validate_pattern(string* pattern) {
    if ((*pattern).length() < PATTERNT_MIN_LENGTH) {
        return false;
    }
    for (std::vector<string>::iterator it = ignored.begin() ; it != ignored.end(); ++it)
    {
        if (it->compare(*pattern) == 0) return false;
    }
    return strspn((*pattern).c_str(), validChars) == strlen((*pattern).c_str());
}

void get_patterns(string* line) {
    // Use fuzzy search to find patterns
    // patterns contains alphabetic characters and digits
    // propably combined with slashes
    string pattern = "";
    int count = 0;
    for (std::string::size_type i = 0; i < (*line).length(); i++)
    {
        if ((*line)[i] ==  ' ' or (*line)[i] ==  ':')
        {
            // found space, is pattern enough good to add?
            if ( validate_pattern(&pattern) == true){
                if (DEBUG) {
                    std::cout << "Found valid pattern " << pattern << std::endl;
                }
                patterns.push_back(pattern);
                break;
            }
            else if (DEBUG) {
                std::cout << "Found invalid pattern " << pattern << std::endl;
            }
            pattern = "";
        }
        else pattern += (*line)[i];
    }
}

inline string get_message(string* line) {
    string message = "";
    int count = 0;
    char prev_char;
    for (std::string::size_type i = 0; i < (*line).length(); i++)
    {
        if (isspace((*line)[i]))
        {
            if (!isspace(prev_char)) ++count;
        }
        if (count > DATEPART) {
            message += (*line)[i];
        }
        prev_char = (*line)[i];
    }
    return message;
}

void colorize_pattern(string* line, string* pattern) {
    string::size_type pos  = (*line).find(*pattern);
    if (pos != string::npos) {
        string replacement = "";
        replacement += RED;
        replacement += *pattern;
        replacement += GREEN;
        (*line).replace(pos, (*pattern).length(), replacement);
    }
}

int main(int argc, char* argv[]) {
    int index = 1;
    for (;index<argc; index++) {
        if (strcmp(argv[index],"-n") == 0) {
            number_lines = true;
        }
        else break;
    }
    add_ignored();
    string line;
    string date, message;
    if (index+2 > argc) {
        help();
        return 1;
    }
    patterns.push_back(argv[index]);
    index += 1;
    do {
        string logfile (argv[index]);
        ifstream fhandle(logfile.c_str());
        unsigned long long int line_number = 0;
        if (fhandle.is_open()) {
            // yep, it opened
            while (fhandle.good()) {
                 getline(fhandle, line);
                 line_number++;
                 date = get_date(&line);
                 if (date.length() == 0) continue;
                 message = get_message(&line);
                 for (long index=0; index<(long)patterns.size(); ++index) {
                    if (message.find(patterns.at(index)) != string::npos) {
                        // search patterns only from rows containing
                        // user given pattern
                        if (index == 0) get_patterns(&message);
                        string pattern = patterns.at(index);
                        colorize_pattern(&message, &pattern);
                        if (number_lines) std::cout << line_number << " ";
                        std::cout << RED << date << " " << GREEN
                                  << message << RESET << std::endl;
                        break;
                    }
                 }
            }
            fhandle.close();
        }
        else std::cout << "Unable to open file " << logfile << std::endl;
        index++;
    } while (index < argc);
    return 0;
}
