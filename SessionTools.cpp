#include "SessionTools.h"
#include <iostream>
using namespace std;


UndoStack::UndoStack()
{
    top = -1;
}

bool UndoStack::push(int action)
{
    if (top == MAX_UNDO - 1)
    {
        return false;
    }
    top++;
    items[top] = action;
    return true;
}

bool UndoStack::pop(int& action)
{
    if (top == -1)
    {
        return false;
    }
    action = items[top];
    top--;
    return true;
}

bool UndoStack::isEmpty() const
{
    return top == -1;
}

ReminderQueue::ReminderQueue()
{
    front = nullptr;
    rear = nullptr;
    count = 0;
}

ReminderQueue::~ReminderQueue()
{
    string dummy;
    while (dequeue(dummy))
    {
    }
}

void ReminderQueue::enqueue(const string& text)
{
    Reminder* r = new Reminder;
    r->text = text;
    r->next = nullptr;
    if (rear == nullptr)
    {
        front = r;
        rear = r;
    }
    else
    {
        rear->next = r;
        rear = r;
    }
    count++;
}

bool ReminderQueue::dequeue(string& out)
{
    if (front == nullptr)
    {
        return false;
    }
    Reminder* temp = front;
    out = temp->text;
    front = front->next;
    if (front == nullptr)
    {
        rear = nullptr;
    }
    delete temp;
    count--;
    return true;
}

bool ReminderQueue::isEmpty() const
{
    return front == nullptr;
}

void ReminderQueue::display() const
{
    if (front == nullptr)
    {
        cout << "No pending reminders.\n";
        return;
    }
    int i = 1;
    for (Reminder* p = front; p != nullptr; p = p->next)
    {
        cout << i << ". " << p->text << "\n";
        i++;
    }
}
