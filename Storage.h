#ifndef STORAGE_H
#define STORAGE_H

#include <string>
#include "MoodEntry.h"
using namespace std;

// File format: date|mood|note|journal   (one entry per line)
bool saveEntries(const MoodList& list, const string& filename);
int loadEntries(MoodList& list, const string& filename);   // returns entries loaded, -1 if no file

#endif
