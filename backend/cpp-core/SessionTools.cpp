#include "SessionTools.h"
#include <iostream>
using namespace std;

// ---- UndoStack ----
UndoStack::UndoStack() { top = nullptr; }

UndoStack::~UndoStack() {
    while (top != nullptr) {
        Action* temp = top;
        top = top->next;
        delete temp;
    }
}

void UndoStack::push(int type, MoodEntry* entry, const string& oldJournal) {
    Action* a = new Action;
    a->type = type;
    a->entry = entry;
    a->oldJournal = oldJournal;
    a->next = top;
    top = a;
}

bool UndoStack::pop(Action& out) {
    if (top == nullptr) return false;
    Action* temp = top;
    out.type = temp->type;
    out.entry = temp->entry;
    out.oldJournal = temp->oldJournal;
    top = top->next;
    delete temp;
    return true;
}

bool UndoStack::isEmpty() const { return top == nullptr; }

// ---- ReminderQueue ----
ReminderQueue::ReminderQueue() {
    front = rear = nullptr;
    count = 0;
}

ReminderQueue::~ReminderQueue() {
    string dummy;
    while (dequeue(dummy)) {}
}

void ReminderQueue::enqueue(const string& text) {
    Reminder* r = new Reminder;
    r->text = text;
    r->next = nullptr;
    if (rear == nullptr) front = rear = r;
    else {
        rear->next = r;
        rear = r;
    }
    count++;
}

bool ReminderQueue::dequeue(string& out) {
    if (front == nullptr) return false;
    Reminder* temp = front;
    out = temp->text;
    front = front->next;
    if (front == nullptr) rear = nullptr;
    delete temp;
    count--;
    return true;
}

bool ReminderQueue::isEmpty() const { return front == nullptr; }

void ReminderQueue::display() const {
    if (front == nullptr) {
        cout << "No pending reminders.\n";
        return;
    }
    int i = 1;
    for (Reminder* p = front; p != nullptr; p = p->next)
        cout << i++ << ". " << p->text << "\n";
}
