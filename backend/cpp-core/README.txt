MINDEASE - C++ AND DSA WORKING PROTOTYPE

WHAT IS INCLUDED
- Student dashboard and doctor dashboard (console-based C++ interface)
- Singly linked list: mood history
- Stack: undo last mood or journal action
- Queue: reminders (FIFO)
- Hash table with chaining: self-care resource categories
- File handling: save/load mood entries, student profile, doctor consent and feedback
- Doctor access is blocked unless the student explicitly enables sharing
- Doctor can view mood history, simple descriptive mood averages, shared journals and add supportive feedback

HOW TO COMPILE (g++)
Open a terminal in this folder and run:
g++ -std=c++11 -Wall -Wextra main.cpp MoodEntry.cpp SessionTools.cpp ResourceLibrary.cpp Storage.cpp -o MindEase

Run on Windows:
MindEase.exe

Run on Linux/macOS:
./MindEase

DEMO DOCTOR LOGIN
Username: doctor
Password: MindEase@123

IMPORTANT DEMO LIMITATIONS
- This is a local, single-student prototype. It is not a multi-user network system.
- Doctor credentials are hard-coded for demonstration. Do not use these credentials in a real deployment.
- Consent is a local permission flag on this computer, not a legally compliant clinical consent workflow.
- Mood averages are descriptive only and must not be used for diagnosis or emergency triage.
- Reminders and undo history are session-only; mood entries and doctor feedback are stored locally.
- Keep the data files private. This prototype does not encrypt personal records.

DATA FILES CREATED AT RUNTIME
moods.txt, student_profile.txt, doctor_access.txt, doctor_feedback.txt
