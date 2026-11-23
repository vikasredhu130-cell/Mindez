#ifndef RESOURCELIBRARY_H
#define RESOURCELIBRARY_H

#include <string>
#include <vector>
using namespace std;

// Hash table with chaining: category name -> list of tips
struct CategoryNode {
    string category;
    vector<string> tips;
    CategoryNode* next;
};

class ResourceLibrary {
private:
    static const int SIZE = 7;
    CategoryNode* table[SIZE];

    int hashFunc(const string& key) const;
    string toLower(string s) const;
    void insertCategory(const string& category, const vector<string>& tips);

public:
    ResourceLibrary();
    ~ResourceLibrary();

    void loadDefaults();
    bool showCategory(const string& category) const;
    void showAllCategories() const;
};

#endif
