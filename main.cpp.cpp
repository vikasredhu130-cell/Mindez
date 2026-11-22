#include <iostream>
#include <string>
#include <ctime>
#include "MoodEntry.h"
#include "SessionTools.h"
#include "ResourceLibrary.h"
#include "Storage.h"
using namespace std;

const string DATA_FILE = "moods.txt";

int readInt(const string &prompt, int low, int high)
{
    string line;
    while (true)
    {
        cout << prompt;
        getline(cin, line);
        try
        {
            size_t used;
            int v = stoi(line, &used);
            if (used == line.size() && v >= low && v <= high)
                return v;
        }
        catch (...)
        {
        }
        cout << "Please enter a number between " << low << " and " << high << ".\n";
    }
}

string readNonEmpty(const string &prompt)
{
    string line;
    while (true)
    {
        cout << prompt;
        getline(cin, line);
        if (!line.empty())
            return line;
        cout << "This field cannot be empty.\n";
    }
}

string today()
{
    time_t now = time(nullptr);
    char buf[20];
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M", localtime(&now));
    return string(buf);
}

void moodCheckIn(MoodList &moods, UndoStack &undo)
{
    cout << "\n1 Very Low  2 Low  3 Okay  4 Good  5 Great\n";
    int m = readInt("How are you feeling right now? ", 1, 5);

    cout << "Short description (optional, press Enter to skip): ";
    string note;
    getline(cin, note);

    MoodEntry *e = moods.addEntry(today(), m, note);
    undo.push(ACTION_MOOD_ADDED, e);
    cout << "Mood saved for this session.\n";

    if (m == 1)
    {
        cout << "\nIt sounds like things are really hard right now. MindEase is not a\n"
             << "medical tool, so please consider talking to your college counselling\n"
             << "cell, a doctor, or someone you trust.\n";
    }
}

void addJournal(MoodList &moods, UndoStack &undo)
{
    MoodEntry *last = moods.getLast();
    if (last == nullptr)
    {
        cout << "Do a mood check-in first, the journal is attached to it.\n";
        return;
    }
    string old = last->journal;
    string text = readNonEmpty("Write your journal note: ");
    last->journal = text;
    undo.push(ACTION_JOURNAL_ADDED, last, old);
    cout << "Journal added to your latest entry.\n";
}

void undoLast(MoodList &moods, UndoStack &undo)
{
    Action a;
    if (!undo.pop(a))
    {
        cout << "Nothing to undo.\n";
        return;
    }
    if (a.type == ACTION_MOOD_ADDED)
    {
        moods.removeEntry(a.entry);
        cout << "Last mood entry removed.\n";
    }
    else if (a.type == ACTION_JOURNAL_ADDED)
    {
        a.entry->journal = a.oldJournal;
        cout << "Last journal note undone.\n";
    }
}

void resourceMenu(ResourceLibrary &lib)
{
    lib.showAllCategories();
    string cat = readNonEmpty("Enter a category: ");
    if (!lib.showCategory(cat))
        cout << "No resources found for '" << cat << "'.\n";
}

void reminderMenu(ReminderQueue &q)
{
    cout << "\n1. Add reminder\n2. Show next reminder\n3. View all pending\n";
    int c = readInt("Choice: ", 1, 3);

    if (c == 1)
    {
        q.enqueue(readNonEmpty("Reminder text: "));
        cout << "Reminder added.\n";
    }
    else if (c == 2)
    {
        string r;
        if (q.dequeue(r))
            cout << "Reminder: " << r << "\n";
        else
            cout << "No pending reminders.\n";
    }
    else
    {
        q.display();
    }
}

void showMenu()
{
    cout << "\n===== MindEase =====\n"
         << "1. Mood check-in\n"
         << "2. Add journal note\n"
         << "3. View mood history\n"
         << "4. Self-care resources\n"
         << "5. Reminders\n"
         << "6. Undo last action\n"
         << "7. Save and exit\n";
}

int main()
{
    MoodList moods;
    UndoStack undo;
    ReminderQueue reminders;
    ResourceLibrary library;
    library.loadDefaults();

    cout << "MindEase - student wellbeing prototype\n"
         << "Note: this is not a diagnosis tool and does not replace professional help.\n"
         << "Your entries are stored in a local file (" << DATA_FILE << ") on this computer.\n";

    int loaded = loadEntries(moods, DATA_FILE);
    if (loaded == -1)
        cout << "No saved data found, starting fresh.\n";
    else
        cout << loaded << " saved entries loaded.\n";

    int choice;
    do
    {
        showMenu();
        choice = readInt("Enter choice: ", 1, 7);

        switch (choice)
        {
        case 1:
            moodCheckIn(moods, undo);
            break;
        case 2:
            addJournal(moods, undo);
            break;
        case 3:
            moods.display();
            break;
        case 4:
            resourceMenu(library);
            break;
        case 5:
            reminderMenu(reminders);
            break;
        case 6:
            undoLast(moods, undo);
            break;
        case 7:
            if (saveEntries(moods, DATA_FILE))
                cout << "Saved. Take care!\n";
            else
                cout << "Could not save the file, your entries from this run were not stored.\n";
            break;
        }
    } while (choice != 7);

    return 0;
}
