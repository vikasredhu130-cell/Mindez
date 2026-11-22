#include "MoodEntry.h"
#include "Utils.h"
#include <iostream>
using namespace std;
using namespace mindease;

MoodList::MoodList()
{
    head = nullptr;
    tail = nullptr;
    count = 0;
}

MoodList::~MoodList()
{
    MoodEntry* cur = head;
    while (cur != nullptr)
    {
        MoodEntry* temp = cur;
        cur = cur->next;
        delete temp;
    }
}

string MoodList::moodName(int m)
{
    switch (m)
    {
    case 1:
        return "Very Low";
    case 2:
        return "Low";
    case 3:
        return "Okay";
    case 4:
        return "Good";
    case 5:
        return "Great";
    }
    return "Unknown";
}

void MoodList::addEntry(const string& date, int mood, const string& note)
{
    MoodEntry* node = new MoodEntry(date, mood, note);
    if (head == nullptr)
    {
        head = node;
        tail = node;
    }
    else
    {
        tail->next = node;
        tail = node;
    }
    count++;
}

void MoodList::addEntry(int mood, const string& note)
{
    addEntry(currentTime(), mood, note);
}

void MoodList::addEntry(int mood)
{
    addEntry(currentTime(), mood, "");
}

bool MoodList::removeLast()
{
    if (head == nullptr)
    {
        return false;
    }
    if (head == tail)
    {
        delete head;
        head = nullptr;
        tail = nullptr;
    }
    else
    {
        MoodEntry* p = head;
        while (p->next != tail)
        {
            p = p->next;
        }
        delete tail;
        tail = p;
        tail->next = nullptr;
    }
    count--;
    return true;
}

MoodEntry* MoodList::getHead() const
{
    return head;
}

int MoodList::size() const
{
    return count;
}

void MoodList::display() const
{
    if (head == nullptr)
    {
        cout << "No mood entries yet.\n";
        return;
    }
    int i = 1;
    for (MoodEntry* p = head; p != nullptr; p = p->next)
    {
        cout << i << ". " << p->date << " | " << moodName(p->mood)
             << " (" << p->mood << "/5)\n";
        if (!p->note.empty())
        {
            cout << "   Note: " << p->note << "\n";
        }
        i++;
    }
}

int MoodList::copyMoods(int arr[]) const
{
    int n = 0;
    for (MoodEntry* p = head; p != nullptr && n < MAX_MOODS; p = p->next)
    {
        arr[n] = p->mood;
        n++;
    }
    return n;
}

void showSummary(const MoodList& list)
{
    if (list.count == 0)
    {
        cout << "No entries to summarise.\n";
        return;
    }
    int total = 0;
    for (MoodEntry* p = list.head; p != nullptr; p = p->next)
    {
        total += p->mood;
    }
    cout << "Total entries: " << list.count << "\n";
    cout << "Average mood: " << (double)total / list.count << " / 5\n";
}
