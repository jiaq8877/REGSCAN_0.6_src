#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <cmath>
#include <string>

using namespace std;

extern QStringList Switches;
/////////////////////////////////////////////////////////////////////

void singleMath()
{

int f, i;
bool ok;
double value=0.0;
QString oneRow;
QStringList list1;

///////input
f=Switches.indexOf("-file"); QString Filename; 
if(f>=0 && f<Switches.size()-1) Filename=Switches.at(f+1);
else {cout<<"You need to specify '-file'.\n\n"; exit(1);}

f=Switches.indexOf("-out"); QString Outname;
if(f>=0 && f<Switches.size()-1) Outname=Switches.at(f+1);
else {Outname=Filename; Outname.append(".out"); cout << "output name = input name + '.out'\n";}

int headeryes=0;
f=Switches.indexOf("-header"); QString Header;
if(f>=0 && f<Switches.size()-1) Header=Switches.at(f+1);
if(Header=="yes") headeryes=1;
else if(Header=="no") headeryes=0;
else headeryes=0;

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

f=Switches.indexOf("-columns"); QString Num=Switches.at(f+1);
Num.replace(";",",");//just in case
if(f<0 || f>Switches.size()-1) {cout << "Columns incorrectly specified\n"; exit(1);}
QStringList Semicolon = Num.split(",");
int semicolonsize = Semicolon.size();

  QList<int> startList, endList;
  for(i=0; i<semicolonsize; i++)
  {
  QStringList Range = Semicolon.at(i).split("-");
  if(Range.size()==1) {startList.append(Range.at(0).toInt()); endList.append(Range.at(0).toInt());}
  else if (Range.size()==2) {startList.append(Range.at(0).toInt()); endList.append(Range.at(1).toInt());}
  else {;}
  }

int function=0;
f=Switches.indexOf("-function"); QString Function;
if(f>=0 && f<Switches.size()-1) Function=Switches.at(f+1);
if(Function=="") function=0;
else if(Function=="exp") function=1;
else if(Function=="ln") function=2;
else if(Function=="log") function=3;
else if(Function=="sqrt") function=4;
else if(Function=="square") function=5;
else if(Function=="inv") function=6;
else if(Function=="rev") function=7;
else function=0;

f=Switches.indexOf("-missing"); QString Missing; 
if(f>=0 && f<Switches.size()-1) Missing=Switches.at(f+1); else Missing="NA";


/////////////////do analysis
cout << "running singleMath()\n\n";

QFile outfile(Outname); if(!outfile.open(QIODevice::WriteOnly | QIODevice::Text)) {exit(1);}
QTextStream out_stream(&outfile);

QFile infile(Filename); if(!infile.open(QIODevice::ReadOnly | QIODevice::Text)) {exit(1);}
QTextStream in_stream(&infile);

int startlistsize = startList.size();
int mathOK=0, valueOK=0, maxcol=0;

////find real maxcol
oneRow = in_stream.readLine(); list1 = oneRow.split(Delim); 
int listsize = list1.size(); in_stream.seek(0);

for(i=0; i<startlistsize;i++)
{
if(startList.at(i)>listsize) startList[i]=listsize;
if(endList.at(i)>listsize) endList[i]=listsize;

if(endList.at(i)>maxcol) maxcol=endList.at(i);
}

/////while
if(!in_stream.atEnd() && headeryes==1) {
                                       QString Header = in_stream.readLine(); 
                                       out_stream << Header << "\n"; 
                                       }//HEADER

while(!in_stream.atEnd())
{

oneRow = in_stream.readLine();

if(!oneRow.isEmpty())
{
list1 = oneRow.split(Delim);
listsize = list1.size();

  for(i=0; i<listsize; i++)
  {
     int counter=0; mathOK=0;
     QString Entry=list1.at(i);

     while(counter<startlistsize)
     {
     if(i>=startList.at(counter)-1 && i<=endList.at(counter)-1) {mathOK=1; break;}
     counter+=1;
     }

     if(mathOK==1)//MATH
     {
    
        if(Entry!=Missing)//domath
        {
        value=list1.at(i).toDouble(&ok); 
          if(ok==1) 
          {
          valueOK=1;
            if(function==0) {;}
            else if (function==1) {value=exp(value);}
            else if (function==2) {if(value>0)value=log(value); else valueOK=0;}
            else if (function==3) {if(value>0)value=log10(value); else valueOK=0;}
            else if (function==4) {if(value>=0)value=sqrt(value); else valueOK=0;}
            else if (function==5) {value=value*value;}
            else if (function==6) {if(value!=0)value=1.0/value; else valueOK=0;}
            else if (function==7) {value=-1.0*value;}
            else {;}
          }
 
          else valueOK=0;
        }
        else valueOK=0;

        if(valueOK)//print
        {
        if(i<listsize-1)out_stream << value << Delim;
        else out_stream << value << "\n";
        }
        else
        {
        if(i<listsize-1)out_stream << Missing << Delim;
        else out_stream << Missing << "\n";
        }

     }

     else//NO MATH
     {
     if(i<listsize-1)out_stream << Entry << Delim;
     else out_stream << Entry << "\n";
     }


  }//for
}//if
}//while
infile.close(); outfile.close();
}


