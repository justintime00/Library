// PROJECT IDENTIFIER = 01BD41C3BF016AD7E8B6F837DF18926EC3E83350
#include <algorithm>
#include <iostream>
#include <getopt.h>
#include <string.h>
#include <unordered_map>
#include "Library.h"


using namespace std;

int main(int argc, char * argv[]) {
    ios_base::sync_with_stdio(false);
    Library intLog;
    intLog.getOptions(argc, argv);
    intLog.getInput();
    char command = ' ';
    cout << "% ";
    while (cin >> command && command != 'q') {
        // Search functions (clear search results before search)
        if (command == 't') {
            intLog.timeSearch();
        }
        else if (command == 'm') {
            intLog.matchSearch();
        }
        else if (command == 'c') {
            intLog.catSearch();
        }
        else if (command == 'k') {
            intLog.keySearch();
        } //Excerpt list functions
        else if (command == 'l') {
            intLog.clearExcerpt();
        }
        else if (command == 'a') {
            intLog.append();
        }
        else if (command == 'd') {
            intLog.deleteEntry();
        }
        else if (command == 'b') {
            intLog.moveBegin();
        }
        else if (command == 'e') {
            intLog.moveEnd();
        }
        else if (command == 'r') {
            intLog.appendResults();
        }
        else if (command == 's') {
            intLog.sortExcerpt();
        }//Print functions
        else if (command == 'p') {
            intLog.printExcerpt();
        }
        else if (command == 'g') {
            intLog.printResults();
        } //Misc command for comments
        else if (command == '#') {
            string temp;
            getline(cin, temp);
        }
        cout << "% ";
    }
    return 0;
}