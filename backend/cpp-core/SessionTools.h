#ifndef SESSIONTOOLS_H
#define SESSIONTOOLS_H

#include <string>
#include "MoodEntry.h"
using namespace std;

// ---------- Stack: undo the last local action ----------
const int ACTION_MOOD_ADDED = 1;
const int ACTION_JOURNAL_ADDED = 2;

struct Action {
    int type;
    MoodEntry* entry;       // entry the action was done on
    string oldJournal;      // journal text before the change
    Action* next;
};

class UndoStack {
private:
    Action* top;
public:
    UndoStack();
    ~UndoStack();
    void push(int type, MoodEntry* entry, const string& oldJournal = "");
    bool pop(Action& out);
    bool isEmpty() const;
};

// ---------- Queue: reminders waiting for this session ----------
struct Reminder {
    string text;
    Reminder* next;
};

class ReminderQueue {
private:
    Reminder* front;
    Reminder* rear;
    int count;
public:
    ReminderQueue();
    ~ReminderQueue();
    void enqueue(const string& text);
    bool dequeue(string& out);
    bool isEmpty() const;
    void display() const;
};

#endif
