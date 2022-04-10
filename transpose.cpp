#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <cmath>
#include <string>


using namespace std;

extern QStringList Switches;
/////////////////////////////////////////////////////////////////////

void transpose()
{
int f, i, j, r, listsize;
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

int cnumber=1000;
f=Switches.indexOf("-col-chunk"); QString Cnumber;
if(f>=0 && f<Switches.size()-1) {Cnumber=Switches.at(f+1); cnumber=Cnumber.toInt();}
else cnumber=1000;

int wcolumns=0;
f=Switches.indexOf("-columns"); QString Wcolumns;
if(f>=0 && f<Switches.size()-1) {Wcolumns=Switches.at(f+1); wcolumns=Wcolumns.toInt();}
else wcolumns=0;

/////////////////do analysis
cout << "running transpose()\n\n";

QFile outfile(Outname); if(!outfile.open(QIODevice::WriteOnly | QIODevice::Text)) {exit(1);}
QTextStream out_stream(&outfile);

QFile infile(Filename); if(!infile.open(QIODevice::ReadOnly | QIODevice::Text)) {exit(1);}
QTextStream in_stream(&infile);


////find table dimensions
int columns=0, rows=0;

if(wcolumns<0) 
{
 oneRow = in_stream.readLine(); list1 = oneRow.split(Delim); columns=list1.size(); in_stream.seek(0);
 while(!in_stream.atEnd()){oneRow=in_stream.readLine(); rows+=1;} in_stream.seek(0);
}
 
else if(wcolumns==0) 
{
  int maxcol=0;
  while(!in_stream.atEnd())
  {
  oneRow=in_stream.readLine(); 
  list1 = oneRow.split(Delim);
  if(list1.size()>maxcol) maxcol=list1.size();
  rows+=1;
  } 
  columns=maxcol;
  in_stream.seek(0);
}

else 
{
 columns=wcolumns;
 while(!in_stream.atEnd()){oneRow=in_stream.readLine(); rows+=1;} in_stream.seek(0);
}  




if(cnumber>columns) cnumber=columns;
if(cnumber<1) cnumber=1;

////make table 

QVector<QVector<QString> > Matrix;//[cnumber-1,rows-1]
Matrix.resize(cnumber);
for(r=0; r<cnumber; r++) {Matrix[r].resize(rows);}



/////while
int colcounter=0, colsfilled=0; 

while(colcounter<columns)
{
int rowcounter=0;

while(!in_stream.atEnd())
{

oneRow = in_stream.readLine(); list1 = oneRow.split(Delim); listsize=list1.size(); rowcounter+=1; 
if(colcounter==0 && listsize!=columns){cout << "Column number unusual for row " << rowcounter << "\n";}

   colsfilled=0;
   for(i=colcounter; i<colcounter+cnumber; i++)
   {

     if(i<listsize) 
     {
     Matrix[i-colcounter][rowcounter-1]=list1.at(i); colsfilled+=1;
     }
     else Matrix[i-colcounter][rowcounter-1]="";
   
   }



}//while2

  ////dump into output
  for(i=0; i<colsfilled; i++)
  {
    for(j=0;j<rows; j++)
    {
    out_stream << Matrix.at(i).at(j);
    if(j<rows-1) out_stream << Delim;
    else out_stream << "\n";
    }
  }

colcounter+=cnumber;
in_stream.seek(0);
}//while1

Matrix.clear();
infile.close(); outfile.close();
}


