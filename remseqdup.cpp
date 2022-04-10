#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <cmath>
#include <string>

using namespace std;

extern QStringList Switches;
/////////////////////////////////////////////////////////////////////


void remSeqDup()
{

int f, count=0;
QString oneRow;
QStringList list1,list2;

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
else if(Delimname=="slash") Delim ="\\";
else if(Delimname=="bslash") Delim ="/";
else if(Delimname=="dash") Delim ="-";
else if(Delimname=="quote") Delim ='"';
else if(Delimname=="squote") Delim ="'";
else Delim = "\t";


f=Switches.indexOf("-column"); int columnselect=1;
if(f>=0 && f<Switches.size()-1) columnselect=Switches.at(f+1).toInt();
if(columnselect<1){cout << "Column must be greater than 1\n"; exit(1);}


int headeryes=0;
f=Switches.indexOf("-header"); QString Header;
if(f>=0 && f<Switches.size()-1) Header=Switches.at(f+1);
if(Header=="yes") headeryes=1;
else if(Header=="no") headeryes=0;
else headeryes=0;

/////////////////do analysis
cout << "running remSeqDup()\n\n";

QFile outfile(Outname); if(!outfile.open(QIODevice::WriteOnly | QIODevice::Text)) {exit(1);}
QTextStream out_stream(&outfile);

QFile infile(Filename); if(!infile.open(QIODevice::ReadOnly | QIODevice::Text)) {exit(1);}
QTextStream in_stream(&infile);


/////while
QString Marker, Oldmarker="";

if(!in_stream.atEnd() && headeryes==1) {
                                       QString Header = in_stream.readLine(); 
                                       }//HEADER

while(!in_stream.atEnd())
{

oneRow = in_stream.readLine();

if(!oneRow.isEmpty())
{

list1=oneRow.split(Delim); 
Marker=list1.at(columnselect-1);


  if(Marker!=Oldmarker)
  {
  if(Oldmarker!="") out_stream << Oldmarker << " " << count+1<< "\n";
  Oldmarker=Marker; count=0;
  }

  else if(Marker==Oldmarker)
  {
  count+=1;
  }

else {;}


}//if
}//while
out_stream << Marker << " " << count+1<< "\n";

infile.close(); outfile.close();
}


