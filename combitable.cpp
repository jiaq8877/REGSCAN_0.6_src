#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <cmath>
#include <string>

using namespace std;

extern QStringList Switches;
/////////////////////////////////////////////////////////////////////


void combiTable()
{

int f, i, j, k, list1size, doOK=0, OK1=0, OK2=0;
QString oneRow;
QStringList list1;

///////input
f=Switches.indexOf("-file"); char filename[200]=""; QString Filename=Switches.at(f+1); strcpy(filename, Filename.toLocal8Bit().constData());
if(f<0 || strcmp(filename,"\0")==0){ cout<<"You need to specify '-file'.\n\n"; exit(1);}

f=Switches.indexOf("-out"); char outname[200]=""; QString Outname=Switches.at(f+1); strcpy(outname, Outname.toLocal8Bit().constData());
if(f<0 || strcmp(outname,"\0")==0) 
{
Outname=Filename; Outname.append(".out");
strcpy(outname, Outname.toLocal8Bit().constData());
}

f=Switches.indexOf("-delim"); char delimcommand[20]=""; strcpy(delimcommand, Switches.at(f+1).toLocal8Bit().constData()); char delim[20]="";
if(f<0 || strcmp(delimcommand,"\0")==0) {strcpy(delimcommand,"space"); strcpy(delim," ");}
else if(strcmp(delimcommand,"tab")==0 || strcmp(delimcommand,"TAB")==0) strcpy(delim,"\t");
else if(strcmp(delimcommand,"space")==0) strcpy(delim," ");
else if(strcmp(delimcommand,"comma")==0) strcpy(delim,",");
else if(strcmp(delimcommand,"semicolon")==0) strcpy(delim,";");
else if(strcmp(delimcommand,"colon")==0) strcpy(delim,":");
else strcpy(delim,delimcommand);


f=Switches.indexOf("-columns"); char num[50000]=""; QString Num=Switches.at(f+1); strcpy(num, Num.toLocal8Bit().constData());
Num.replace(";",",");
if(f<0 || strcmp(num,"\0")==0) exit(1);
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

int startlistsize = startList.size();

int headeryes=0;
f=Switches.indexOf("-header"); char headerverdict[20]=""; strcpy(headerverdict, Switches.at(f+1).toLocal8Bit().constData());
if(f<0 || strcmp(headerverdict,"\0")==0) headeryes=0;
else if(strcmp(headerverdict,"yes")==0 || strcmp(headerverdict,"YES")==0) headeryes=1;
else if(strcmp(headerverdict,"no")==0 || strcmp(headerverdict,"NO")==0) headeryes=0;
else headeryes=0;

int function=0; QString Symbol;
f=Switches.indexOf("-function"); char functionverdict[20]=""; strcpy(functionverdict, Switches.at(f+1).toLocal8Bit().constData());
if(f<0 || strcmp(functionverdict,"\0")==0) {function=0; Symbol="/";}
else if(strcmp(functionverdict,"diff")==0 || strcmp(functionverdict,"subtract")==0) {function=3; Symbol="-";}
else if(strcmp(functionverdict,"sum")==0 || strcmp(functionverdict,"add")==0) {function=2; Symbol="+";}
else if(strcmp(functionverdict,"mult")==0 || strcmp(functionverdict,"multiply")==0) {function=1; Symbol="*";}
else if(strcmp(functionverdict,"div")==0 || strcmp(functionverdict,"divide")==0) {function=0; Symbol="/";}
else function=0;

f=Switches.indexOf("-missing"); QString Missing; 
char missing[200]=""; strcpy(missing, Switches.at(f+1).toLocal8Bit().constData());
if(f<0 || strcmp(missing,"\0")==0) {strcpy(missing,"NA"); Missing=missing;}
else if(strcmp(missing,"none")==0) {strcpy(missing,""); Missing=missing;}
else {strcpy(missing,"NA"); Missing=missing;}

/////////////////do analysis
cout << "running combiTable()\n\n";

QFile outfile(outname); if(!outfile.open(QIODevice::WriteOnly | QIODevice::Text)) {exit(1);}
QTextStream out_stream(&outfile);

QFile infile(filename); if(!infile.open(QIODevice::ReadOnly | QIODevice::Text)) {exit(1);}
QTextStream in_stream(&infile);

/////header
  oneRow = in_stream.readLine(); list1 = oneRow.split(delim); list1size=list1.size(); if(headeryes==0)in_stream.seek(0);

 
  if(!in_stream.atEnd())
  { 
    
  for(i=0; i<list1size; i++)
  {
  if(headeryes==1) out_stream << list1.at(i);
  else out_stream << i+1;  
  if(i<list1size-1) out_stream << delim;
  }
 

    for(i=0; i<list1size; i++) 
    {
      for(j=i; j<list1size; j++) 
      {

        doOK=0; OK1=0; OK2=0;
        if(i!=j)
        {  
          for(k=0; k<startlistsize; k++)
          {
          if(i>=startList.at(k)-1 && i<=endList.at(k)-1) OK1=1;
          if(j>=startList.at(k)-1 && j<=endList.at(k)-1) OK2=1;
          if(OK1==1 && OK2==1) {doOK=1; break;}
          }
        }

        if(doOK)
        {
        if(headeryes==1)out_stream << delim << list1.at(i) << Symbol << list1.at(j);
        else out_stream << delim << i+1 << Symbol << j+1;
        }

      }//j
    }//i
    out_stream << "\n";
  }//if


////////while
while(!in_stream.atEnd())
{
oneRow = in_stream.readLine();

  if(!oneRow.isEmpty())
  {
  list1 = oneRow.split(delim); list1size=list1.size();

    out_stream << oneRow;

    for(i=0; i<list1size; i++) 
    {
      for(j=i; j<list1size; j++)
      {

        doOK=0; OK1=0; OK2=0;
        if(i!=j)
        {  
          for(k=0; k<startlistsize; k++)
          {
          if(i>=startList.at(k)-1 && i<=endList.at(k)-1) OK1=1;
          if(j>=startList.at(k)-1 && j<=endList.at(k)-1) OK2=1;
          if(OK1==1 && OK2==1) {doOK=1; break;}
          }
        }

        if(doOK)
        {  
           bool ok1, ok2;
           double value1=list1.at(i).toDouble(&ok1), value2=list1.at(j).toDouble(&ok2);

           if(ok1 && ok2 && list1.at(i)!=Missing && list1.at(j)!=Missing)//can do
           {
        
             if(function==0){if(value2!=0) out_stream << delim << value1/value2; else out_stream << delim << Missing;}

             else if(function==1){out_stream << delim << value1*value2;}

             else if(function==2){out_stream << delim << value1+value2;}

             else if(function==3){out_stream << delim << value1-value2;}

             else {;}        

           }

         else //cannot do
         {
            out_stream << delim << Missing;
         }


        }

      }//j
    }//i
    out_stream << "\n";

  }//if

}//while

infile.close(); outfile.close();
}


