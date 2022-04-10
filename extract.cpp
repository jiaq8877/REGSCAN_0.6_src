#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <cmath>
#include <string>

using namespace std;

extern QStringList Switches;
/////////////////////////////////////////////////////////////////////

void extract()
{

int f, i, emptyrows=0;
QString oneRow;
QStringList list1;

///////input
f=Switches.indexOf("-file"); QString Filename; 
if(f>=0 && f<Switches.size()-1) Filename=Switches.at(f+1);
else {cout<<"You need to specify '-file'.\n\n"; exit(1);}

f=Switches.indexOf("-out"); QString Outname;
if(f>=0 && f<Switches.size()-1) Outname=Switches.at(f+1);
else {Outname=Filename; Outname.append(".out"); cout << "output name = input name + '.out'\n";}

f=Switches.indexOf("-delim"); QString Delimname, Delim;
if(f>=0 && f<Switches.size()-1) Delimname=Switches.at(f+1); else Delimname = "space";
if(Delimname=="tab") Delim ="\t";
else if(Delimname=="tab") Delim ="\t";
else if(Delimname=="space") Delim =" ";
else if(Delimname=="comma") Delim =",";
else if(Delimname=="semicolon") Delim =";";
else if(Delimname=="colon") Delim =":";
else Delim = " ";

f=Switches.indexOf("-columns"); QString Num=Switches.at(f+1);
Num.replace(";",",");//just in case
if(f<0 || f>Switches.size()-1) {cout << "Columns incorrectly specified"; exit(1);}
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


f=Switches.indexOf("-rows"); QString Num2=Switches.at(f+1);
Num2.replace(";",",");
if(f<0 || f>Switches.size()-1) {cout << "Rows incorrectly specified"; exit(1);}
QStringList Semicolon2 = Num2.split(",");
int semicolonsize2 = Semicolon2.size();

  QList<int> rowstartList, rowendList;
  for(i=0; i<semicolonsize2; i++)
  {
  QStringList Range2 = Semicolon2.at(i).split("-");
  if(Range2.size()==1) {rowstartList.append(Range2.at(0).toInt()); rowendList.append(Range2.at(0).toInt());}
  else if (Range2.size()==2) {rowstartList.append(Range2.at(0).toInt()); rowendList.append(Range2.at(1).toInt());}
  else {;}
  }

  int maxrow=0;
  Num2.replace("-",","); QStringList splitNum2 = Num2.split(",");
  for(i=0; i<splitNum2.size();i++) {if(splitNum2.at(i).toInt()>maxrow) maxrow=splitNum2.at(i).toInt();}


/////////////////do analysis
cout << "running extract()\n\n";

QFile outfile(Outname); if(!outfile.open(QIODevice::WriteOnly | QIODevice::Text)) {exit(1);}
QTextStream out_stream(&outfile);

QFile infile(Filename); if(!infile.open(QIODevice::ReadOnly | QIODevice::Text)) {exit(1);}
QTextStream in_stream(&infile);

int startlistsize = startList.size();
int startlistsize2 = rowstartList.size();
int counter=0, printOK=0, maxcol=0;

////find real maxcol
oneRow = in_stream.readLine(); list1 = oneRow.split(Delim); 
int listsize = list1.size(); in_stream.seek(0);

for(i=0; i<startlistsize;i++)
{
if(startList.at(i)>listsize) startList[i]=listsize;
if(endList.at(i)>listsize) endList[i]=listsize;

if(startList.at(i)>maxcol) maxcol=startList.at(i);
if(endList.at(i)>maxcol) maxcol=endList.at(i);
}



/////while
int rowcounter=0;
while(!in_stream.atEnd())
{

oneRow = in_stream.readLine();

if(!oneRow.isEmpty())
{
rowcounter+=1; //empty rows are not counted as rows, they are simply removed as nonexistent

counter=0; printOK=0;
while(counter<startlistsize2)
{
if(rowcounter>=rowstartList.at(counter) && rowcounter<=rowendList.at(counter)) {printOK=1; break;}
counter+=1;
}

if(printOK==1) 
{

list1 = oneRow.split(Delim);
listsize = list1.size();

  for(i=0; i<listsize; i++)
  {
     counter=0; printOK=0;
     while(counter<startlistsize)
     {
     if(i>=startList.at(counter)-1 && i<=endList.at(counter)-1) {printOK=1; break;}
     counter+=1;
     }

     if(printOK==1)
     {
     if(i<maxcol-1)out_stream << list1.at(i) << Delim;
     else out_stream << list1.at(i) << "\n";
     }


  }//for
}//if

}//if
else emptyrows+=1;
}//while


if(emptyrows) cout << "Warning: " << emptyrows << " of empty rows detected\n";
if(rowcounter>maxrow) cout << "Note: The max row you selected is less than the number of rows in the input file\n";
if(listsize>maxcol) cout << "Note: The max column you selected is less than the number of columns in the input file\n";

infile.close(); outfile.close();
}


