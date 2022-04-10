#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <cmath>
#include <string>

using namespace std;

extern QStringList Switches;
/////////////////////////////////////////////////////////////////////


void multiFilter()
{

int f, i;
QString oneRow, Entry, Subentry;
QStringList list1,list2,ID;
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
else if(Delimname=="slash") Delim ="\\";
else if(Delimname=="bslash") Delim ="/";
else if(Delimname=="dash") Delim ="-";
else if(Delimname=="quote") Delim ='"';
else if(Delimname=="squote") Delim ="'";
else Delim = "\t";

int ignoremissing=0;
f=Switches.indexOf("-ignore-missing"); QString Ignoremissing;
if(f>=0 && f<Switches.size()-1) Ignoremissing=Switches.at(f+1);
if(Ignoremissing=="yes") ignoremissing=1;
else if(Ignoremissing=="no") ignoremissing=0;
else ignoremissing=0;

f=Switches.indexOf("-missing"); QString Missing; 
if(f>=0 && f<Switches.size()-1) Missing=Switches.at(f+1); else Missing="NA";

int logic=0;
f=Switches.indexOf("-logic"); QString Logic;
if(f>=0 && f<Switches.size()-1) Logic=Switches.at(f+1);
if(Logic=="or") logic=1;
else if(Logic=="and") logic=0;
else logic=0;

int dir=0;
f=Switches.indexOf("-dir"); QString Direction;
if(f>=0 && f<Switches.size()-1) Direction=Switches.at(f+1);
if(Direction=="exclude" || Direction=="out") dir=1;
else if(Direction=="include" || Direction=="in") dir=0;
else dir=0;


QFile outfile(Outname); if(!outfile.open(QIODevice::WriteOnly | QIODevice::Text)) {exit(1);}
QTextStream out_stream(&outfile);

QFile infile(Filename); if(!infile.open(QIODevice::ReadOnly | QIODevice::Text)) {exit(1);}
QTextStream in_stream(&infile);


int headeryes=1; int totalcolumns=0;
f=Switches.indexOf("-header"); QString Header;
if(f>=0 && f<Switches.size()-1) Header=Switches.at(f+1);
if(Header=="yes") headeryes=1;
else if(Header=="no") headeryes=0;
else headeryes=1;


if(!in_stream.atEnd() && headeryes==1) 
{
   QString Header = in_stream.readLine();
   out_stream << Header << "\n"; 
   list1=Header.split(Delim); totalcolumns=list1.size();
   for(i=0; i<totalcolumns; i++)ID.append(list1.at(i));

}//HEADER


f=Switches.indexOf("-filter"); QString Filter; 
if(f>=0 && f<Switches.size()-1) Filter=Switches.at(f+1); else {cout << "Filter data missing\n"; exit(1);}

Filter.replace("*","_"); Filter.replace(";",",");

QList<int> IDnumber; QList<int> Function; QList<int> Abs; QList<double> Value; double value; int intvalue; QString Fvalue, Vvalue;

int abs=0, counter=0, score=0;
list1 = Filter.split(",");
int l1size=list1.size();

int l2size=0;
for(i=0; i<list1.size(); i++)
{
Entry=list1.at(i);
list2=Entry.split("_"); l2size=list2.size();
if(l2size!=3){cout << "Filter incorrectly defined\n"; exit(1);}

//loc 0
Subentry=list2.at(0);
abs=0; if(Subentry.contains("abs:", Qt::CaseInsensitive)) {Subentry.remove("abs:", Qt::CaseInsensitive); abs=1;}

if(headeryes==0)
{
ok=0; intvalue=Subentry.toInt(&ok);
if(ok){IDnumber.append(intvalue); Abs.append(abs); }else {cout << "IDnumber " << i+1 << " incorrectly defined\n"; exit(1);}
}

else
{
    counter=0; int hit=-1;
    while(counter<totalcolumns)
    {
     if(Subentry==ID.at(counter)) {hit=counter; break;}
     counter+=1;
    }

    hit+=1;
    if(hit<1) {cout << "Header ID not found\n"; exit(1);}
    IDnumber.append(hit); Abs.append(abs);
}


//loc 1
Fvalue=list2.at(1);
Fvalue.replace("l","L"); Fvalue.replace("s","S"); Fvalue.replace("n","N"); Fvalue.replace("e","E"); 
Fvalue.replace("EE","E"); Fvalue.replace("ES","SE"); Fvalue.replace("EL","LE"); Fvalue.replace("EN","NE");

if(Fvalue=="S")Function.append(1);
else if(Fvalue=="SE")Function.append(2);
else if(Fvalue=="E")Function.append(3);
else if(Fvalue=="L")Function.append(4);
else if(Fvalue=="LE")Function.append(5);
else if(Fvalue=="NE")Function.append(6);
else {cout << "Function " << i+1 << " contains error\n"; exit(1);}

//loc 2
ok=0; Vvalue=list2.at(2);
if(Vvalue!=Missing)
{
  value=Vvalue.toDouble(&ok);
  if(!ok){cout << "Value at " <<  i+1 << " contains error\n"; exit(1);}
}
else 
{
 value=0;
 if(Function.at(i)==3) Function[i]=0;
 else Function[i]=-1;
}

Value.append(value); 

}

/////////////////do analysis
cout << "running mutifilter()\n\n";

while(!in_stream.atEnd())
{

oneRow = in_stream.readLine();

  if(!oneRow.isEmpty())
  {
  list1=oneRow.split(Delim);

    counter=0; score=0;
    while(counter<l1size)
    {
    double number=0.0;//elsewhere
    int loc=IDnumber.at(counter); 
        value=Value.at(counter);
    int function=Function.at(counter);
        abs = Abs.at(counter);

      if(list1.at(loc-1)==Missing)
      {
        if(ignoremissing){score+=1;}
        else
        {
        if(function==0){score+=1;}//E
        if(function==-1){;}//NE
        }
      }

      else
      {   
      number=list1.at(loc-1).toDouble(&ok);
      if(!ok){cout << "Error reading column " <<  counter+1 << " \n"; exit(1);}
      if(abs && number<0)number*=-1.0;  

      if(function==1){if(number<value) score+=1;}//S
      else if(function==2){if(number<=value) score+=1;}//SE
      else if(function==3){if(number==value) score+=1;}//E
        else if(function==0){;}//E
      else if(function==4){if(number>value) score+=1;}//L
      else if(function==5){if(number>=value) score+=1;}//LE
      else if(function==6){if(number!=value) score+=1;}//NE
        else if(function==-1){score+=1;}//NE
      else {cout << "Unknown function call\n"; exit(1);}
      }    

    counter+=1;
    }//while

 
    ///decide
    int printok=0;

    if(dir==0)//include
    {
      printok=0;
      if(logic==0 && score==l1size)printok=1;//AND
      if(logic==1 && score>0)printok=1;//OR
    }
    else//exclude
    {
      printok=1;
      if(logic==0 && score==l1size)printok=0;//AND
      if(logic==1 && score>0)printok=0;//OR
    }

    if(printok)out_stream << oneRow<<"\n";

  }//if
}//while


ID.clear(); IDnumber.clear(); Abs.clear(); Value.clear(); Function.clear();
list1.clear(); list2.clear();
infile.close(); outfile.close();
}


