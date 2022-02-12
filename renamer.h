//Methoden zum Umgang mit Dateien (umbenennen...) und Dateinamen
#ifndef __RENAMER_H
#define __RENAMER_H

#include <iostream>
#include <cstring>
#include <fstream>
#include <vector>
#include "auxilliary.H"

using namespace std; //wichtig für string

class renamer
{

public:
    static string AppendIntToFilename (string old_filename, string textvor, int nach);
    static string ChangeEnding (string old_filename, string new_ending);
    static void SubstituteCharInFile (char old_ch, char new_ch, string filename);
    static bool FileExists(string filename);
    static void RenameFile(string file_old, string file_new); 
};

#endif
