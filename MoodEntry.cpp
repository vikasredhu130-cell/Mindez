#include "MoodEntry.h"
#include <iostream>
using namespace std;

MoodList::MoodList() {
    head = nullptr;
    tail = nullptr;
    count = 0;
}

MoodList::~MoodList() {
    MoodEntry* cur = head;
    while (cur != nullptr) {
        MoodEntry* temp = cur;
        cur = cur->next;
        delete temp;
    }
}

string MoodList::moodName(int m) {
    switch (m) {
        case 1: return "Very Low";
        case 2: return "Low";
        case 3: return "Okay";
        case 4: return "Good";
        case 5: return "Great";
    }
    return "Unknown";
}

// insert at the end so history stays oldest -> newest
MoodEntry* MoodList::addEntry(const string& date, int mood, const string& note) {
    MoodEntry* node = new MoodEntry(date, mood, note);
    if (head == nullptr) {
        head = tail = node;
    } else {
        tail->next = node;
        tail = node;
    }
    count++;
    return node;
}

bool MoodList::removeEntry(MoodEntry* target) {
    MoodEntry* prev = nullptr;
    MoodEntry* cur = head;

    while (cur != nullptr && cur != target) {
        prev = cur;
        cur = cur->next;
    }
    if (cur == nullptr) return false;   // not found

    if (prev == nullptr) head = cur->next;
    else prev->next = cur->next;

    if (cur == tail) tail = prev;

    delete cur;
    count--;
    return true;
}

MoodEntry* MoodList::getLast() const { return tail; }
MoodEntry* MoodList::getHead() const { return head; }
int MoodList::size() const { return count; }

void MoodList::display() const {
    if (head == nullptr) {
        cout << "No mood entries yet.\n";
        return;
    }
    int i = 1;
    for (MoodEntry* p = head; p != nullptr; p = p->next) {
        cout << i++ << ". " << p->date << " | " << moodName(p->mood)
             << " (" << p->mood << "/5)\n";
        if (!p->note.empty())    cout << "   Note: " << p->note << "\n";
        if (!p->journal.empty()) cout << "   Journal: " << p->journal << "\n";
    }
}
