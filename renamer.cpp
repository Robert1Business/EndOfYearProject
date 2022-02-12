#include "renamer.h"

string renamer::AppendIntToFilename (string old_filename, string textvor, int nach)
{
    string mitte = old_filename; 
    string anfang = textvor;
    char endechar [10];
    string ende = itoa(nach, 10);    char *endung = new char[4]; // Dateiendung
    
    char *file;
    
    file = new char[80];
    for (int i = 0; i< 20 ; i++) file[i] = 0;
    
    // Dateiname beginnt mit textvor
    int zeig = 0;
    int i = 0;
    for (i = 0; i < (int)anfang.size(); i++)
        file[i] = anfang[i];
    zeig = i;
    
    //mittlere Bereich
    for (i = 0; i < (int)mitte.size()-4; i++)
	    file[i+zeig] = mitte[i];
    int end = i;
    zeig += i;
    for (i = 0; i < 4; i++) //Dateiendung kopieren
        endung[i] = mitte[end+i];
    
    //Dateiende
    if (nach != 0)
    {
        file[zeig] = '_';
        zeig++;
        for (i = 0; i < (int)ende.size(); i++)
    	    file[i+zeig] = ende[i];
        zeig += i;
    }
    
    //Dateiendung
    for (i = 0; i < 4; i++)
	    file[i+zeig] = endung[i];
    zeig += 4;
    
    for (int i = zeig; i< 80 ; i++) file[i] = 0;
    
    //cout << "Ausgabe in Datei " << file << endl;
    string file_ = file;
    return (file_);
}

string renamer::ChangeEnding (string old_filename, string new_ending)
{
    string mitte = old_filename; 
    string anfang = old_filename;
    const char *endung = new_ending.c_str(); // Dateiendung
    
    char *file;
    
    file = new char[80];
    for (int i = 0; i< 20 ; i++) file[i] = 0;
    
    // Dateiname beginnt mit old_filename ohne Endung
    int zeig = 0;
    int i = 0;
    for (i = 0; i < (int)anfang.size()-4; i++)
        file[i] = anfang[i];
    file[i]='.';
    i++;
    zeig = i;
    
    //Dateiendung
    for (i = 0; i < 4; i++)
	    file[i+zeig] = endung[i];
    zeig += 4;
    
    for (int i = zeig; i< 80 ; i++) file[i] = 0;
    
    //cout << "Ausgabe in Datei " << file << endl;
    string file_ = file;
    return (file_);
}

void renamer::SubstituteCharInFile (char old_ch, char new_ch, string filename)
{
     vector<char> file_;
     ifstream f (filename.c_str(), ios::in|ios::binary|ios::ate);
     f.seekg (0, ios::beg); //Erstes Zeichen suchen
       
     if (!f.good())
     {
         cout << "Datei nicht gefunden!"<< endl;
         exit(1);
     }      
     else 
     {
         char s;
         int i = -1;
         while (!f.eof())
         {
             f.get (s);
             if (s == old_ch) s = new_ch;
             file_.push_back(s);
             i++;
         }
         f.close();
         fstream f (filename.c_str(), ios::out|ios::binary);
         for (int j = 0; j < i; j++)
         {
             f<<file_[j];
         }
         f.close();
     }
}

bool renamer::FileExists(string filename)
{
    bool found = true;
       
    ifstream f (filename.c_str(), ios::in);
       
    if (!f.good())
        found = false;        
    f.close();
    return (found);
}

void renamer::RenameFile(string file_old, string file_new)
{
     if (renamer::FileExists(file_old))
     {
         if (renamer::FileExists(file_new))
         {
             cout<<"Datei "<<file_new<<" wird ueberschrieben!"<<endl;
             string com;
             com = "rm ";
             com.append(file_new);
             system(com.c_str());
         }
         string command;
         command = "mv ";
         command.append(file_old);
         command.append(" ");
         command.append(file_new);
         system(command.c_str());
     }
}
