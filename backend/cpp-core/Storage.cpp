#include "Storage.h"
#include <fstream>
#include <iostream>
#include <vector>
using namespace std;

// '|' is our separator so it cannot be part of the text
static string clean(string s) {
    for (size_t i = 0; i < s.size(); i++)
        if (s[i] == '|') s[i] = '/';
    return s;
}

bool saveEntries(const MoodList& list, const string& filename) {
    ofstream out(filename.c_str());
    if (!out) return false;

    for (MoodEntry* p = list.getHead(); p != nullptr; p = p->next) {
        out << p->date << "|" << p->mood << "|"
            << clean(p->note) << "|" << clean(p->journal) << "\n";
    }
    return out.good();
}

int loadEntries(MoodList& list, const string& filename) {
    ifstream in(filename.c_str());
    if (!in) return -1;     // first run, file not created yet

    string line;
    int loaded = 0;

    while (getline(in, line)) {
        vector<string> parts;
        string cur = "";
        for (size_t i = 0; i < line.size(); i++) {
            if (line[i] == '|') {
                parts.push_back(cur);
                cur = "";
            } else cur += line[i];
        }
        parts.push_back(cur);

        if (parts.size() != 4) continue;    // skip damaged lines

        int mood = 0;
        try { mood = stoi(parts[1]); }
        catch (...) { continue; }
        if (mood < 1 || mood > 5) continue;

        MoodEntry* e = list.addEntry(parts[0], mood, parts[2]);
        e->journal = parts[3];
        loaded++;
    }
    return loaded;
}
