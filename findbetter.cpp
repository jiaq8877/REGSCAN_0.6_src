#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <cmath>
#include <string>

using namespace std;

extern QStringList Switches;
/////////////////////////////////////////////////////////////////////


void findBetter()
{

int f;
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

f=Switches.indexOf("-iddelim"); QString iddelimname, iddelim;
if(f>=0 && f<Switches.size()-1) iddelimname=iddelim=Switches.at(f+1);
if(iddelimname=="tab") iddelim ="\t";
else if(iddelimname=="tab") iddelim ="\t";
else if(iddelimname=="space") iddelim =" ";
else if(iddelimname=="comma") iddelim =",";
else if(iddelimname=="semicolon") iddelim =";";
else if(iddelimname=="colon") iddelim =":";
else if(iddelimname=="slash") iddelim ="\\";
else if(iddelimname=="bslash" || "divide") iddelim ="/";
else if(iddelimname=="dash" || iddelimname=="subtract") iddelim ="-";
else if(iddelimname=="quote") iddelim ='"';
else if(iddelimname=="squote") iddelim ="'";
else if(iddelimname=="point" || iddelimname=="period") iddelim =".";
else if(iddelimname=="add" || iddelimname=="sum") iddelim ="+";
else if(iddelimname=="star" || iddelimname=="multiply" || iddelimname=="asterisk") iddelim ="*";
else {;}

f=Switches.indexOf("-factor"); double factor=1.0;
if(f>=0 && f<Switches.size()-1) factor=Switches.at(f+1).toDouble();

f=Switches.indexOf("-column"); int columnselect=2;
if(f>=0 && f<Switches.size()-1) columnselect=Switches.at(f+1).toInt();
if(columnselect<1){cout << "Column must be greater than 1\n"; exit(1);}

f=Switches.indexOf("-valuecolumn"); int valuecolumn=11;
if(f>=0 && f<Switches.size()-1) valuecolumn=Switches.at(f+1).toInt();
if(valuecolumn<1){cout << "Valuecolumn must be greater than 1\n"; exit(1);}

f=Switches.indexOf("-markercolumn"); int markercolumn=1;
if(f>=0 && f<Switches.size()-1) markercolumn=Switches.at(f+1).toInt();
if(markercolumn<1){cout << "Markercolumn must be greater than 1\n"; exit(1);}

int direction=0;
f=Switches.indexOf("-dir"); QString Direction;
if(f>=0 && f<Switches.size()-1) Direction=Switches.at(f+1);
if(Direction=="smaller") direction=0;
else if(Direction=="larger") direction=1;
else direction=0;

int missing=0;
f=Switches.indexOf("-missing"); QString Missing; 
if(f>=0 && f<Switches.size()-1) Missing=Switches.at(f+1); else Missing="NA";

int headeryes=1;
f=Switches.indexOf("-header"); QString Header;
if(f>=0 && f<Switches.size()-1) Header=Switches.at(f+1);
if(Header=="yes") headeryes=1;
else if(Header=="no") headeryes=0;
else headeryes=1;

/////////////////do analysis
cout << "running findBetter()\n\n";

QFile outfile(Outname); if(!outfile.open(QIODevice::WriteOnly | QIODevice::Text)) {exit(1);}
QTextStream out_stream(&outfile);

QFile infile(Filename); if(!infile.open(QIODevice::ReadOnly | QIODevice::Text)) {exit(1);}
QTextStream in_stream(&infile);


/////while
QString Entry="", Entry1="", Entry2="", Singlet="NA", Marker, Oldmarker="NA", Oldentry="NA", Value, Refmarker;
double value=-2.0, refvalue=0.0;
bool ok;


if(!in_stream.atEnd() && headeryes==1) {
                                       QString Header = in_stream.readLine(); 
                                       out_stream << Header << "\n"; 
                                       }//HEADER

while(!in_stream.atEnd())
{

oneRow = in_stream.readLine();

if(!oneRow.isEmpty())
{

list1=oneRow.split(Delim); 
Entry=list1.at(columnselect-1);
Value=list1.at(valuecolumn-1); 
Marker=list1.at(markercolumn-1);



if(Marker!=Oldmarker)
{
  missing=0;
  Oldmarker=Marker; 
  list2=Entry.split(iddelim); 
  if(list2.size()==1) 
  {

   if(Value==Missing) missing=1;
   else {refvalue=Value.toDouble(&ok); if(!ok){cout << "Some values from column " << valuecolumn << " could not be read\n"; exit(1);}}
   
  }


  out_stream << oneRow <<"\n";
}

else if(Marker==Oldmarker)
{
  if(missing<1)
  {
  value=Value.toDouble(&ok); if(!ok){cout << "Some values from column " << valuecolumn << " could not be read\n"; exit(1);}

  if(direction==0 && (value*factor)<refvalue) out_stream << oneRow << "\n";
  if(direction==1 && (value/factor)>refvalue) out_stream << oneRow << "\n";
  }
}

else {;}


}//if
}//while

infile.close(); outfile.close();
}


