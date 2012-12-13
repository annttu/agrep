/*
Pattern aware grep
Made by Annttu (at) <repeatmynick> dot fi
*/

#include <iostream>
#include <fstream>
#include <string>
#include <iterator>

using namespace std;

const int DATEPART = 2;

string get_date(string* line) {
    string date = "";
    int count = 0;
    for (std::string::size_type i = 0; i < (*line).length(); i++)
    {
        if ((*line)[i] ==  ' ')
        {
            ++count;
            if (count > DATEPART) break;
            else date += ' ';
        }
        else date += (*line)[i];
    }
    //std::cout << date << std::endl; 
    //for (string::size_type i = 0; i < *line.length(); i++)
    return date;
}

string get_message(string* line) {
    string message = "";
    int count = 0;
    for (std::string::size_type i = 0; i < (*line).length(); i++)
    {
        if ((*line)[i] ==  ' ')
        {
            ++count;
        }
        if (count > DATEPART) {
            message += (*line)[i];
        }
    }
    return message;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cout << "Usage " << argv[0] << " pattern file" << std::endl;
        return 1;
    } 
    string pattern (argv[1]);
    string logfile (argv[2]);
    string line;
    string date, message;
    ifstream fhandle(logfile.c_str());
    if (fhandle.is_open()) {
        // yep, it opened
        while (fhandle.good()) {
             getline(fhandle, line);
             date = get_date(&line);
             if (date.length() == 0) continue;
             message = get_message(&line);
             if (message.find(pattern) != string::npos) {
                 std::cout << "\033[1;31m" << date << " \033[1;32m" 
                           << message << "\033[0m" << std::endl;
             }
             
        }
        fhandle.close();
    }
    else {
        std::cout << "unable to open file " << logfile << std::endl;
        return 1;
    }
    return 0;
}

