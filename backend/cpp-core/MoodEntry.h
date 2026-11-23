#ifndef MOODENTRY_H
#define MOODENTRY_H

#include <string>
using namespace std;

// One node of the mood history linked list
struct MoodEntry {
    string date;
    int mood;          // 1 to 5
    string note;       // optional short description
    string journal;    // optional longer journal text
    MoodEntry* next;

    MoodEntry(string d, int m, string n) {
        date = d;
        mood = m;
        note = n;
        journal = "";
        next = nullptr;
    }
};

// Singly linked list that keeps entries in the order they were added
class MoodList {
private:
    MoodEntry* head;
    MoodEntry* tail;
    int count;

public:
    MoodList();
    ~MoodList();

    MoodEntry* addEntry(const string& date, int mood, const string& note);
    bool removeEntry(MoodEntry* target);
    MoodEntry* getLast() const;
    MoodEntry* getHead() const;
    int size() const;
    void display() const;

    static string moodName(int m);
};

#endif
