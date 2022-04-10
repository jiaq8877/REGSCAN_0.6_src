#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <cmath>
#include <string>

using namespace std;

extern QStringList Switches;
/////////////////////////////////////////////////////////////////////


void multiCount()
{

int f, i, listsize=0, sublistsize=0;
QString oneRow, ID, ID1;
QStringList list1,list2;

///////input
f=Switches.indexOf("-list"); QString Listname; 
if(f>=0 && f<Switches.size()-1) Listname=Switches.at(f+1);
else {cout<<"You need to specify '-list'.\n\n"; exit(1);}

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

int splitnames=0;
f=Switches.indexOf("-iddelim"); QString iddelimname, iddelim;
if(f>=0 && f<Switches.size()-1) {iddelimname=iddelim=Switches.at(f+1); splitnames=1;} else splitnames=0;
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
cout << "running multiCount()\n\n";

QFile outfile(Outname); if(!outfile.open(QIODevice::WriteOnly | QIODevice::Text)) {exit(1);}
QTextStream out_stream(&outfile);

QFile infile(Filename); if(!infile.open(QIODevice::ReadOnly | QIODevice::Text)) {exit(1);}
QTextStream in_stream(&infile);

QFile listfile(Listname); if(!listfile.open(QIODevice::ReadOnly | QIODevice::Text)) {exit(1);}
QTextStream list_stream(&listfile);

////input list file
QStringList RefList;
QList<int> RefCount;

while(!list_stream.atEnd())
{
oneRow = list_stream.readLine();
  if(!oneRow.isEmpty())
  {
  RefList.append(oneRow); RefCount.append(0);
  }
}
listfile.close();

listsize=RefList.size();


/////while
int location=0;

if(!in_stream.atEnd() && headeryes==1) {
                                       QString Header = in_stream.readLine(); 
                                       //out_stream << Header << "\n"; 
                                       }//HEADER

if(splitnames==0)//0
{
  while(!in_stream.atEnd())
  {

  oneRow = in_stream.readLine();

    if(!oneRow.isEmpty())
    {
    list1=oneRow.split(Delim); 
    ID=list1.at(columnselect-1);

    location=RefList.indexOf(ID);
    if(location>-1) RefCount[location]+=1;
    }
  }
}

else//1
{
  
  while(!in_stream.atEnd())
  {

  oneRow = in_stream.readLine();

    if(!oneRow.isEmpty())
    {
    list1=oneRow.split(Delim); ID=list1.at(columnselect-1);
    list2=ID.split(iddelim); 
    sublistsize=list2.size();

      int whilecount=0;
      while(whilecount<sublistsize)
      {
       ID1=list2.at(whilecount);
       location=RefList.indexOf(ID1);
       if(location>-1) RefCount[location]+=1;
       whilecount+=1;
      }

    }
  }
}


////Write output
for(i=0; i<listsize; i++)
{
out_stream << RefList.at(i) << " " << RefCount.at(i);
if(i<listsize-1) out_stream << "\n";
}


infile.close(); outfile.close(); 
RefList.clear(); RefCount.clear();
}


