// PROJECT IDENTIFIER = 01BD41C3BF016AD7E8B6F837DF18926EC3E83350
#include <algorithm>
#include <iostream>
#include <getopt.h>
#include <string.h>
#include <iterator>
#include <functional>
#include <sstream>
#include <fstream>
#include <unordered_map>
#include "Library.h"

using namespace std;

std::ostream& operator<<(std::ostream& os, Book const& le) {
    os << le.ISBNStr << "|" << le.year << "|"
       << le.authLast << ", " << le.authFirst << "|" << le.title << "\n";
    return os;
}

void Library::getOptions(int argc, char * argv[]) {
    if (argc != 2) {
        cerr << "Invalid num of arguments\n";
    }
    if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0) {
        cout << "This program provides an interface for handling a library catalog.\n";
        exit(0);
    }
    logFileName = argv[1];
    return;
}

void Library::getInput() {
    ifstream logFile(logFileName);
    unsigned entryID = 0;
    unsigned year;
    char misc;
    string ISBN, authFirst, authLast, title, comment;
    getline(logFile, comment);
    while (getline(logFile, ISBN, '|')) {
        logFile >> year >> misc;
        getline(logFile, authLast, ',');
        getline(logFile, authFirst, '|');
        getline(logFile, title);
        Book * newEntry = new Book(entryID, ISBN, year, authFirst, authLast, title);
        input.push_back(newEntry);
        sortedInput.push_back(entryID);
        ++entryID;
    }
    logFile.close();
    std::sort(sortedInput.begin(), sortedInput.end(), BookComp(input));
    for (unsigned i = 0; i < sortedInput.size(); ++i) {
        input[sortedInput[i]]->sortedID = i;
        string temp = input[sortedInput[i]]->authFirst;
        transform(temp.begin(), temp.end(), temp.begin(), ::tolower);
        if (keyDict[temp].empty()
            || keyDict[temp].back() != sortedInput[i]) {
            keyDict[temp].push_back(sortedInput[i]);
        }
        temp = input[sortedInput[i]]->authLast;
        transform(temp.begin(), temp.end(), temp.begin(), ::tolower);
        if (keyDict[temp].empty()
            || keyDict[temp].back() != sortedInput[i]) {
            keyDict[temp].push_back(sortedInput[i]);
        }
        string keyword = "";
        temp = input[sortedInput[i]]->title;
        for (size_t k = 0; k < temp.size(); ++k) {
            if (isalnum(temp[k])) {
                keyword += static_cast<char>(tolower(temp[k]));
            }
            else if (keyword != "") {
                if (keyDict[keyword].empty() 
                        || keyDict[keyword].back() != sortedInput[i]) {
                    keyDict[keyword].push_back(sortedInput[i]);
                }
                keyword = "";
            }
            if (k == temp.size() - 1 && keyword != "") {
                if (keyDict[keyword].empty() 
                        || keyDict[keyword].back() != sortedInput[i]) {
                    keyDict[keyword].push_back(sortedInput[i]);
                }
            }
        }
    }
    cout << input.size() << " entries read\n";
    return;
}

void Library::yearSearch() {
    searchConducted = true;
    unsigned year1, year2;
    string misc;
    cin >> year1 >> misc >> year2;
    lastSearch = 't';
    year1It = std::lower_bound(sortedInput.begin(), sortedInput.end(), 
        year1, YearComp(input));
    year2It = std::upper_bound(year1It, sortedInput.end(), 
        year2, YearCompReverse(input));
    cout << "Year search: " << year2It - year1It << " entries found\n";
}

void Library::keySearch() {
    searchConducted = true;
    lastSearch = 'k';
    keyResults.clear();
    string search;
    getline(cin, search, ' ');
    getline(cin, search);
    transform(search.begin(), search.end(), search.begin(), ::tolower);
    vector<string> wordsVec;
    string keyword;
    for (size_t k = 0; k < search.size(); ++k) {
        if (isalnum(search[k])) {
            keyword += static_cast<char>(tolower(search[k]));
        }
        else if (keyword != "") {
            wordsVec.push_back(keyword);
            keyword = "";
        }
        if (k == search.size() - 1 && keyword != "") {
            wordsVec.push_back(keyword);
        }
    }
    for (string keyword : wordsVec) {
        if (keyDict.find(keyword) == keyDict.end()) {
            cout << "Keyword search: 0 entries found\n";
            return;
        }
    }
    keyResults = keyDict[wordsVec[0]];
    for (size_t i = 1; i < wordsVec.size(); ++i) {
        auto it = std::set_intersection(keyResults.begin(), keyResults.end(),
            keyDict[wordsVec[i]].begin(), keyDict[wordsVec[i]].end(), 
            keyResults.begin(), BookComp(input));
        keyResults.erase(it, keyResults.end());
    }
    cout << "Keyword search: " << keyResults.size() << " entries found\n";
    return;
}

void Library::append() {
    int index = 0;
    cin >> index;
    if (!(index < 0 || index >= static_cast<int>(input.size()))) {
        excerptList.push_back(index);
        cout << "log entry " << index << " appended\n";
        excerptSorted = false;
        return;
    }
}

void Library::appendResults() {
    if (!searchConducted) {
        return;
    }
    if (lastSearch == 't') {
        for (auto it = year1It; it != year2It; ++it) {
            excerptList.push_back(*it);
        }
        cout << year2It - year1It << " log entries appended\n";
        excerptSorted = false;
        return;
    }
    else if (lastSearch == 'k') {
        for (auto elt : keyResults) {
            excerptList.push_back(elt);
        }
        cout << keyResults.size() << " log entries appended\n";
        excerptSorted = false;
        return;
    }
    cout << "0 log entries appended\n";
    return;
}


void Library::deleteEntry() {
    int index = 0;
    cin >> index;
    if (!(index < 0 || index >= static_cast<int>(excerptList.size()))) {
        excerptList.erase((excerptList.begin() + index));
        cout << "Deleted excerpt list entry " << index << "\n";
        return;
    }
}

void Library::moveBegin() {
    int index = 0;
    cin >> index;
    if (!(index < 0 || index >= static_cast<int>(excerptList.size()))) {
        int temp = excerptList[index];
        excerptList.erase((excerptList.begin() + index));
        excerptList.insert(excerptList.begin(), temp);
        cout << "Moved excerpt list entry " << index << "\n";
        excerptSorted = false;
        return;
    }
}

void Library::moveEnd() {
    int index = 0;
    cin >> index;
    if (!(index < 0 || index >= static_cast<int>(excerptList.size()))) {
        int temp = excerptList[index];
        excerptList.erase((excerptList.begin() + index));
        excerptList.push_back(temp);
        cout << "Moved excerpt list entry " << index << "\n";
        excerptSorted = false;
        return;
    }
}

void Library::sortExcerpt() {
    cout << "excerpt list sorted\n";
    if (excerptList.size() != 0) {
        Book * fPtr = input[excerptList[0]];
        Book * lPtr = input[excerptList[excerptList.size() - 1]];
        cout << "previous ordering:\n0|" << *fPtr << "...\n"
             << excerptList.size() - 1 << "|" << *lPtr;
        if (!excerptSorted) {
            vector<int> buckets;
            buckets.resize(sortedInput.size(), 0);
            for (size_t i = 0; i < excerptList.size(); ++i) {
                Book * Book = input[excerptList[i]];
                ++buckets[Book->sortedID];
            }
            excerptList.clear();
            for (size_t i = 0; i < buckets.size(); ++i) {
                for (int k = 0; k < buckets[i]; ++k) {
                    excerptList.push_back(sortedInput[i]);
                }
            }
        }
        fPtr = input[excerptList[0]];
        lPtr = input[excerptList[excerptList.size() - 1]];
        cout << "new ordering:\n0|" << *fPtr << "...\n"
             << excerptList.size() - 1 << "|" << *lPtr;
    }
    else {
        cout << "(previously empty)\n";
    }
    excerptSorted = true;
}

void Library::clearExcerpt() {
    cout << "excerpt list cleared\n";
    if (excerptList.size() != 0) {
        Book * fPtr = input[excerptList[0]];
        Book * lPtr = input[excerptList[excerptList.size() - 1]];
        cout << "previous contents:\n0|" << *fPtr << "...\n" 
             << excerptList.size() - 1 << "|" << *lPtr;
        excerptList.clear();
    }
    else {
        cout << "(previously empty)\n";
    }
}

void Library::printResults() {
    if (!searchConducted) {
        return;
    }
    if (lastSearch == 't') {
        for (auto it = year1It; it != year2It; ++it) {
            cout << *input[*it];
        }
        return;
    }
    else if (lastSearch == 'k') {
        for (auto elt : keyResults) {
            cout << *input[elt];
        }
        return;
    }
    return;
}

void Library::printExcerpt() {
    for (size_t i = 0; i < excerptList.size(); ++i) {
        cout << i << "|";
        cout << *(input[excerptList[i]]);
    }
}