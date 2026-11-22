#ifndef SESSIONTOOLS_H
#define SESSIONTOOLS_H

#include <string>
using namespace std;

const int MAX_UNDO = 50;
const int ACTION_MOOD_ADDED = 1;
const int ACTION_JOURNAL_ADDED = 2;

class UndoStack
{
private:
    int items[MAX_UNDO];
    int top;

public:
    UndoStack();
    bool push(int action);
    bool pop(int& action);
    bool isEmpty() const;
};


struct Reminder
{
    string text;
    Reminder* next;
};

class ReminderQueue
{
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
