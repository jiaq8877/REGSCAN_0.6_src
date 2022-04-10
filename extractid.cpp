#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <cmath>
#include <string>

using namespace std;

extern QStringList Switches;
/////////////////////////////////////////////////////////////////////


void extractID()
{

int f;
QString oneRow, Entry;
QStringList list1;
bool ok;

///////input
f=Switches.indexOf("-file"); QString Filename; 
if(f>=0 && f<Switches.size()-1) Filename=Switches.at(f+1);
else {cout<<"You need to specify '-file'.\n\n"; exit(1);}

f=Switches.indexOf("-out"); QString Outname;
if(f>=0 && f<Switches.size()-1) Outname=Switches.at(f+1);
else {Outname=Filename; Outname.append(".out"); cout << "output name = input name + '.out'\n";}

f=Switches.indexOf("-delim"); QString Delimname, Delim;
if(f>=0 && f<Switches.size()-1) Delimname=Switches.at(f+1); else Delimname = "tab";
if(Delimname=="tab") Delim ="\t";
else if(Delimname=="tab") Delim ="\t";
else if(Delimname=="space") Delim =" ";
else if(Delimname=="comma") Delim =",";
else if(Delimname=="semicolon") Delim =";";
else if(Delimname=="colon") Delim =":";
else if(Delimname=="bslash") Delim ="\\";
else if(Delimname=="slash") Delim ="/";
else if(Delimname=="dash") Delim ="-";
else if(Delimname=="quote") Delim ='"';
else if(Delimname=="squote") Delim ="'";
else Delim = "\t";

f=Switches.indexOf("-column"); int column=0;
if(f>=0 && f<Switches.size()-1) column=Switches.at(f+1).toInt(&ok);
if(column<0 || !ok){cout << "Column must be greater than 1 or 0 (for all columns)\n"; exit(1);}

int headeryes=0; 
f=Switches.indexOf("-header"); QString Header;
if(f>=0 && f<Switches.size()-1) Header=Switches.at(f+1);
if(Header=="yes") headeryes=1;
else if(Header=="no") headeryes=0;
else headeryes=0;

f=Switches.indexOf("-is"); QString Is; int is=0;
if(f>=0 && f<Switches.size()-1) {Is=Switches.at(f+1); is=1;}

f=Switches.indexOf("-contains"); QString Contains; int contains=0;
if(f>=0 && f<Switches.size()-1) {Contains=Switches.at(f+1); contains=1;}

int dir=0;
f=Switches.indexOf("-dir"); QString Direction;
if(f>=0 && f<Switches.size()-1) Direction=Switches.at(f+1);
if(Direction=="exclude" || Direction=="out") dir=1;
else if(Direction=="include" || Direction=="in") dir=0;
else dir=0;


/////////////////do analysis
cout << "running extractID()\n\n";

QFile outfile(Outname); if(!outfile.open(QIODevice::WriteOnly | QIODevice::Text)) {exit(1);}
QTextStream out_stream(&outfile);

QFile infile(Filename); if(!infile.open(QIODevice::ReadOnly | QIODevice::Text)) {exit(1);}
QTextStream in_stream(&infile);

////find size
oneRow = in_stream.readLine();
list1=oneRow.split(Delim);
int listsize= list1.size();
if(column>listsize) {cout << "Column number exceeded or wrong delimiter used\n"; exit(1);}
in_stream.seek(0);

////scan file
if(!in_stream.atEnd() && headeryes==1) 
{
 QString Header = in_stream.readLine(); out_stream << Header << "\n"; 
}//HEADER


while(!in_stream.atEnd())
{

oneRow = in_stream.readLine();

  if(!oneRow.isEmpty())
  {
  list1=oneRow.split(Delim);
  int printOK=0;

    if(column>0)
    { 
      Entry=list1.at(column-1);
      if(is)
        {
         if(Entry==Is) printOK=1;
        }
      else if(contains)
        {
         if(Entry.contains(Contains)) printOK=1;
        }
      else {cout << "You have to select reference string (either '-is' or '-contains')\n"; exit(1);}
    }
  
    else if(column==0)
    {
      int counter=0;
      while(counter<listsize)
      {
      Entry=list1.at(counter);
      if(is) 
       {
        if(Entry==Is) {printOK=1; break;}
       }
      else if(contains)
       {
        if(Entry.contains(Contains)) {printOK=1; break;}
       }
      else {cout << "You have to select reference string (either '-is' or '-contains')\n"; exit(1);}
      counter+=1;
      }
    }
 
    else {cout << "Column number incorrectly selected\n"; exit(1);}

    if(printOK && !dir) out_stream << oneRow << "\n";
    else if(!printOK && dir) out_stream << oneRow << "\n";
    else {;}

  }//if
}//while


list1.clear();
infile.close(); outfile.close();
}


