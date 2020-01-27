// PROJECT IDENTIFIER = 01BD41C3BF016AD7E8B6F837DF18926EC3E83350
#include <iostream>
#include <unordered_map>
#include <string>
#include <cstring>
#include <vector>
#include <deque>
#include <algorithm>
#ifdef _MSC_VER
inline int strcasecmp(const char *s1, const char *s2) {
    return _stricmp(s1, s2);
}
#endif // _MSC_VER

class Book {
public:
    Book(unsigned entryIn, std::string ISBNIn, uint64_t yearIn, std::string authFIn, std::string authLIn, std::string titleIn) :
        ISBNStr(ISBNIn), year(yearIn), authFirst(authFIn), authLast(authLIn), title(titleIn), entryID(entryIn) {
        uint64_t ISBN = 0;
        for (size_t i = 0; i < ISBNIn.size(); ++i) {
            if (ISBNIn[i] != '-') {
                ISBN *= 10;
                ISBN += (static_cast<int>(ISBNIn[i]) - '0');
            }
        }
        this->ISBNInt = ISBN;
    }
    
    friend std::ostream& operator<<(std::ostream& os, Book const& le);

    uint64_t ISBNInt;
    uint64_t year;
    std::string ISBNStr;
    std::string authFirst;
    std::string authLast;
    std::string title;
    unsigned entryID;
    unsigned sortedID;
};

struct BookComp {
    BookComp(std::vector<Book*> &masterLib) : lib(masterLib) {}
    bool operator()(unsigned const& lhs, unsigned const& rhs) {
        Book * left = lib[lhs];
        Book * right = lib[rhs];
        if (left->year != right->year) {
            return left->year < right->year;
        }
        if (left->ISBNInt != right->ISBNInt) {
            return left->ISBNInt < right->ISBNInt;
        }
        int cmp = strcasecmp((left->authLast).c_str(), (right->authLast).c_str());
        if (cmp != 0) {
            if (cmp < 0) {
                return true;
            }
            else {
                return false;
            }
        }
        cmp = strcasecmp((left->authFirst).c_str(), (right->authFirst).c_str());
        if (cmp != 0) {
            if (cmp < 0) {
                return true;
            }
            else {
                return false;
            }
        }
        return left->entryID < right->entryID;
    }
    std::vector<Book*> &lib;
 };

class Library {
public:
    Library() : searchConducted(false), catSuccess(false), 
        excerptSorted(false) {}
    ~Library() {
        for (auto elt : input) {
            delete elt;
        }
    }
    void getOptions(int argc, char * argv[]);
    void getInput();
    void yearSearch();
    void ISBNSearch();
    void catSearch();
    void keySearch();
    void append();
    void appendResults();
    void deleteEntry();
    void moveBegin();
    void moveEnd();
    void sortExcerpt();
    void clearExcerpt();
    void printResults();
    void printExcerpt();

private:
    std::unordered_map<std::string, std::vector<int>> keyDict;
    std::unordered_map<std::string, std::vector<int>> authDict;
    std::vector<Book*> input;
    std::vector<int> sortedInput;
    std::vector<int> keyResults;
    std::vector<int> excerptList;
    std::string logFileName;
    std::vector<int>::iterator year1It;
    std::vector<int>::iterator year2It;
    std::string catStr;
    char lastSearch;
    bool searchConducted;
    bool catSuccess;
    bool excerptSorted;
};