#include <iostream>
#include <string>
#include <ctime>
#include <fstream>
#include <iomanip>
#include "MoodEntry.h"
#include "SessionTools.h"
#include "ResourceLibrary.h"
#include "Storage.h"
using namespace std;

const string DATA_FILE = "moods.txt";
const string PROFILE_FILE = "student_profile.txt";
const string CONSENT_FILE = "doctor_access.txt";
const string FEEDBACK_FILE = "doctor_feedback.txt";
const string DOCTOR_USERNAME = "doctor";
const string DOCTOR_PASSWORD = "MindEase@123"; // Demo credentials only.

int readInt(const string &prompt, int low, int high) {
    string line;
    while (true) {
        cout << prompt;
        getline(cin, line);
        try {
            size_t used;
            int value = stoi(line, &used);
            if (used == line.size() && value >= low && value <= high) return value;
        } catch (...) {}
        cout << "Please enter a number between " << low << " and " << high << ".\n";
    }
}

string readNonEmpty(const string &prompt) {
    string line;
    while (true) {
        cout << prompt;
        getline(cin, line);
        if (!line.empty()) return line;
        cout << "This field cannot be empty.\n";
    }
}

string today() {
    time_t now = time(nullptr);
    tm *local = localtime(&now);
    char buffer[32];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M", local);
    return string(buffer);
}

string loadStudentName() {
    ifstream in(PROFILE_FILE.c_str());
    string name;
    if (in && getline(in, name) && !name.empty()) return name;
    name = readNonEmpty("Enter student name for this local profile: ");
    ofstream out(PROFILE_FILE.c_str());
    out << name << '\n';
    return name;
}

bool hasDoctorConsent() {
    ifstream in(CONSENT_FILE.c_str());
    string value;
    if (!in || !getline(in, value)) return false;
    return value == "YES";
}

void setDoctorConsent(bool allowed) {
    ofstream out(CONSENT_FILE.c_str());
    out << (allowed ? "YES" : "NO") << '\n';
}

void moodCheckIn(MoodList &moods, UndoStack &undo) {
    cout << "\n1 Very Low  2 Low  3 Okay  4 Good  5 Great\n";
    int mood = readInt("How are you feeling right now? ", 1, 5);
    cout << "Short description (optional, press Enter to skip): ";
    string note;
    getline(cin, note);
    MoodEntry *entry = moods.addEntry(today(), mood, note);
    undo.push(ACTION_MOOD_ADDED, entry);
    cout << "Mood saved for this session.\n";
    if (mood == 1) {
        cout << "\nIt sounds like things are really hard right now. MindEase is not a\n"
             << "medical tool. Consider contacting your college counselling cell,\n"
             << "a qualified professional, or someone you trust.\n";
    }
}

void addJournal(MoodList &moods, UndoStack &undo) {
    MoodEntry *last = moods.getLast();
    if (last == nullptr) {
        cout << "Do a mood check-in first; the journal is attached to it.\n";
        return;
    }
    string oldText = last->journal;
    last->journal = readNonEmpty("Write your journal note: ");
    undo.push(ACTION_JOURNAL_ADDED, last, oldText);
    cout << "Journal added to your latest entry.\n";
}

void undoLast(MoodList &moods, UndoStack &undo) {
    Action action;
    if (!undo.pop(action)) {
        cout << "Nothing to undo.\n";
        return;
    }
    if (action.type == ACTION_MOOD_ADDED) {
        moods.removeEntry(action.entry);
        cout << "Last mood entry removed.\n";
    } else if (action.type == ACTION_JOURNAL_ADDED) {
        action.entry->journal = action.oldJournal;
        cout << "Last journal note undone.\n";
    }
}

void resourceMenu(ResourceLibrary &library) {
    library.showAllCategories();
    string category = readNonEmpty("Enter a category: ");
    if (!library.showCategory(category)) cout << "No resources found for '" << category << "'.\n";
}

void reminderMenu(ReminderQueue &queue) {
    cout << "\n1. Add reminder\n2. Show and remove next reminder\n3. View all pending\n";
    int choice = readInt("Choice: ", 1, 3);
    if (choice == 1) {
        queue.enqueue(readNonEmpty("Reminder text: "));
        cout << "Reminder added.\n";
    } else if (choice == 2) {
        string reminder;
        if (queue.dequeue(reminder)) cout << "Next reminder: " << reminder << "\n";
        else cout << "No pending reminders.\n";
    } else queue.display();
}

void manageSharing() {
    bool allowed = hasDoctorConsent();
    cout << "\nCurrent doctor access: " << (allowed ? "ON" : "OFF") << "\n";
    cout << "If enabled, a doctor who signs in on this computer can view your saved\n"
         << "mood history and journal entries. You can revoke access at any time.\n";
    int choice = readInt("1. Allow access  2. Revoke access  3. Keep current setting: ", 1, 3);
    if (choice == 1) { setDoctorConsent(true); cout << "Doctor access enabled.\n"; }
    else if (choice == 2) { setDoctorConsent(false); cout << "Doctor access revoked.\n"; }
    else cout << "No changes made.\n";
}

void viewDoctorFeedback() {
    ifstream in(FEEDBACK_FILE.c_str());
    if (!in) { cout << "No doctor feedback is available yet.\n"; return; }
    cout << "\n===== Doctor Feedback =====\n";
    string line;
    bool any = false;
    while (getline(in, line)) { cout << line << '\n'; any = true; }
    if (!any) cout << "No doctor feedback is available yet.\n";
}

void showStudentMenu() {
    cout << "\n========== MINDEASE | STUDENT DASHBOARD ==========\n"
         << "1. Mood check-in\n"
         << "2. Add journal note\n"
         << "3. View mood history\n"
         << "4. Self-care resources\n"
         << "5. Reminders\n"
         << "6. Undo last action\n"
         << "7. Manage doctor access (consent)\n"
         << "8. View doctor feedback\n"
         << "9. Save and return to home\n";
}

void studentDashboard(const string &studentName, MoodList &moods, UndoStack &undo,
                      ReminderQueue &reminders, ResourceLibrary &library) {
    cout << "\nWelcome, " << studentName << "!\n";
    int choice;
    do {
        showStudentMenu();
        choice = readInt("Enter choice: ", 1, 9);
        switch (choice) {
            case 1: moodCheckIn(moods, undo); break;
            case 2: addJournal(moods, undo); break;
            case 3: moods.display(); break;
            case 4: resourceMenu(library); break;
            case 5: reminderMenu(reminders); break;
            case 6: undoLast(moods, undo); break;
            case 7: manageSharing(); break;
            case 8: viewDoctorFeedback(); break;
            case 9:
                if (saveEntries(moods, DATA_FILE)) cout << "Records saved. Returning to home.\n";
                else cout << "Could not save records.\n";
                break;
        }
    } while (choice != 9);
}

void showMoodAnalytics(const MoodList &moods) {
    int count = 0, total = 0, lowCount = 0;
    for (MoodEntry *entry = moods.getHead(); entry != nullptr; entry = entry->next) {
        ++count;
        total += entry->mood;
        if (entry->mood <= 2) ++lowCount;
    }
    if (count == 0) { cout << "No mood records are available for analysis.\n"; return; }
    cout << fixed << setprecision(2)
         << "Total entries: " << count << "\n"
         << "Average mood (1-5): " << static_cast<double>(total) / count << "\n"
         << "Entries rated Very Low or Low: " << lowCount << "\n";
    cout << "Note: These are descriptive summaries, not a diagnosis or risk assessment.\n";
}

void doctorDashboard(const string &studentName, const MoodList &moods) {
    cout << "\n========== MINDEASE | DOCTOR DASHBOARD ==========\n";
    if (!hasDoctorConsent()) {
        cout << "Access denied: the student has not granted permission to share records.\n";
        cout << "Ask the student to enable doctor access from their dashboard.\n";
        return;
    }
    cout << "Student profile: " << studentName << "\n"
         << "Permission: granted by student on this local computer.\n";
    int choice;
    do {
        cout << "\n1. View student mood history\n"
             << "2. View mood summary / analytics\n"
             << "3. Review shared journal entries\n"
             << "4. Add feedback for student\n"
             << "5. Logout\n";
        choice = readInt("Enter choice: ", 1, 5);
        if (choice == 1) moods.display();
        else if (choice == 2) showMoodAnalytics(moods);
        else if (choice == 3) {
            bool found = false;
            for (MoodEntry *entry = moods.getHead(); entry != nullptr; entry = entry->next) {
                if (!entry->journal.empty()) {
                    cout << "\n" << entry->date << " | " << MoodList::moodName(entry->mood)
                         << "\nJournal: " << entry->journal << "\n";
                    found = true;
                }
            }
            if (!found) cout << "No journal entries have been shared.\n";
        } else if (choice == 4) {
            string feedback = readNonEmpty("Write supportive feedback (not a diagnosis): ");
            ofstream out(FEEDBACK_FILE.c_str(), ios::app);
            if (out) { out << "[" << today() << "] Doctor: " << feedback << "\n"; cout << "Feedback saved.\n"; }
            else cout << "Could not save feedback.\n";
        }
    } while (choice != 5);
}

bool doctorLogin() {
    cout << "\n========== DOCTOR LOGIN ==========\n";
    string username = readNonEmpty("Username: ");
    string password = readNonEmpty("Password: ");
    if (username == DOCTOR_USERNAME && password == DOCTOR_PASSWORD) return true;
    cout << "Invalid doctor credentials.\n";
    return false;
}

int main() {
    MoodList moods;
    UndoStack undo;
    ReminderQueue reminders;
    ResourceLibrary library;
    library.loadDefaults();

    cout << "MindEase - C++ Student Wellbeing Prototype\n"
         << "This educational prototype is not a diagnosis or emergency service.\n";
    string studentName = loadStudentName();
    int loaded = loadEntries(moods, DATA_FILE);
    if (loaded == -1) cout << "No saved mood data found; starting fresh.\n";
    else cout << loaded << " saved mood entries loaded.\n";

    int role;
    do {
        cout << "\n=============== MINDEASE HOME ===============\n"
             << "1. Student page\n"
             << "2. Doctor page\n"
             << "3. Exit\n";
        role = readInt("Select page: ", 1, 3);
        if (role == 1) studentDashboard(studentName, moods, undo, reminders, library);
        else if (role == 2 && doctorLogin()) doctorDashboard(studentName, moods);
        else if (role == 3) {
            if (saveEntries(moods, DATA_FILE)) cout << "All mood records saved. Goodbye!\n";
            else cout << "Warning: records could not be saved.\n";
        }
    } while (role != 3);
    return 0;
}
