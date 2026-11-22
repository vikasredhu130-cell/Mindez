#ifndef MOODENTRY_H
#define MOODENTRY_H

#include <string>
using namespace std;

struct MoodEntry
{
    string date;
    int mood;          
    string note;       
    MoodEntry* next;

    MoodEntry(string d, int m, string n)
    {
        date = d;
        mood = m;
        note = n;
        next = nullptr;
    }
};

class MoodList
{
private:
    MoodEntry* head;
    MoodEntry* tail;
    int count;

public:
    MoodList();
    ~MoodList();

    void addEntry(const string& date, int mood, const string& note);
    void addEntry(int mood, const string& note);
    void addEntry(int mood);

    bool removeLast();
    MoodEntry* getHead() const;
    int size() const;
    void display() const;
    int copyMoods(int arr[]) const;

    static string moodName(int m);


    friend void showSummary(const MoodList& list);
};

void showSummary(const MoodList& list);

#endif
