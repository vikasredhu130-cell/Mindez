# MindEase

A C++ console prototype for student mental-health self-management, built to show data structures through real features.

Team PICTURE: Anukul Ojha, Anuraj Singh Gusain, Vikas

## Data structure mapping
- Mood history: singly linked list (MoodList)
- Undo: stack (UndoStack)
- Reminders: queue (ReminderQueue)
- Self-care resources: hash table with chaining (ResourceLibrary)
- Saving records: file handling (Storage)

## Build and run (PowerShell)
g++ -std=c++11 -Wall -Wextra main.cpp MoodEntry.cpp SessionTools.cpp ResourceLibrary.cpp Storage.cpp -o MindEase.exe
.\MindEase.exe

All .cpp files must be compiled together, otherwise the linker gives undefined-reference errors.

## Note
This is an educational prototype. It does not diagnose or treat any condition. Doctor login uses demo credentials only and is not secure. Records are stored in plain local text files.
