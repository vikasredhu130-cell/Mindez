#include "ResourceLibrary.h"
#include <iostream>
using namespace std;

ResourceLibrary::ResourceLibrary() {
    for (int i = 0; i < SIZE; i++) table[i] = nullptr;
}

ResourceLibrary::~ResourceLibrary() {
    for (int i = 0; i < SIZE; i++) {
        CategoryNode* cur = table[i];
        while (cur != nullptr) {
            CategoryNode* temp = cur;
            cur = cur->next;
            delete temp;
        }
    }
}

string ResourceLibrary::toLower(string s) const {
    for (size_t i = 0; i < s.size(); i++)
        if (s[i] >= 'A' && s[i] <= 'Z') s[i] = s[i] + 32;
    return s;
}

int ResourceLibrary::hashFunc(const string& key) const {
    unsigned long h = 0;
    for (size_t i = 0; i < key.size(); i++)
        h = h * 31 + key[i];
    return h % SIZE;
}

void ResourceLibrary::insertCategory(const string& category, const vector<string>& tips) {
    string key = toLower(category);
    int idx = hashFunc(key);

    CategoryNode* node = new CategoryNode;
    node->category = key;
    node->tips = tips;
    node->next = table[idx];    // chain at the front of the bucket
    table[idx] = node;
}

// predefined general self-care content, nothing medical
void ResourceLibrary::loadDefaults() {
    insertCategory("stress", {
        "Take 5 slow breaths: in for 4 counts, out for 6.",
        "Break big tasks into small steps and do one at a time.",
        "Step away from your desk for a 10 minute walk."
    });
    insertCategory("sleep", {
        "Try to sleep and wake at about the same time daily.",
        "Keep your phone away from the bed for the last 30 minutes.",
        "Avoid tea or coffee late in the evening."
    });
    insertCategory("focus", {
        "Study in 25 minute blocks with 5 minute breaks.",
        "Keep only the current subject on your desk.",
        "Write down tomorrow's top 3 tasks before you stop."
    });
    insertCategory("lonely", {
        "Message a friend or family member, even a short one.",
        "Join a college club or study group.",
        "Sit in a common area instead of studying alone in the room."
    });
    insertCategory("motivation", {
        "Pick one small task and finish it first.",
        "Note down one thing you did well today.",
        "Move around a bit, light exercise helps energy."
    });
}

bool ResourceLibrary::showCategory(const string& category) const {
    string key = toLower(category);
    int idx = hashFunc(key);

    for (CategoryNode* p = table[idx]; p != nullptr; p = p->next) {
        if (p->category == key) {
            cout << "\nTips for " << p->category << ":\n";
            for (size_t i = 0; i < p->tips.size(); i++)
                cout << " - " << p->tips[i] << "\n";
            return true;
        }
    }
    return false;   // category not found
}

void ResourceLibrary::showAllCategories() const {
    cout << "Available categories: ";
    bool first = true;
    for (int i = 0; i < SIZE; i++) {
        for (CategoryNode* p = table[i]; p != nullptr; p = p->next) {
            if (!first) cout << ", ";
            cout << p->category;
            first = false;
        }
    }
    cout << "\n";
}
