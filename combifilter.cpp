#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <cmath>
#include <string>

using namespace std;

extern QStringList Switches;
/////////////////////////////////////////////////////////////////////


void combiFilter()
{
bool ok;
int f;
double value, value1, value2;
QString oneRow, Pheno, Value, Value1="", Value2="", Marker, Pheno1="", Pheno2="", Merged, Merged1, Merged2;
QStringList list1, list2;
QList<double> MergedValue;
QStringList MergedList;

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

f=Switches.indexOf("-iddelim"); QString iddelimname, iddelim;
if(f>=0 && f<Switches.size()-1) {iddelimname=Switches.at(f+1);}
if(iddelimname=="tab") iddelim ="\t";
else if(iddelimname=="tab") iddelim ="\t";
else if(iddelimname=="space") iddelim =" ";
else if(iddelimname=="comma") iddelim =",";
else if(iddelimname=="semicolon") iddelim =";";
else if(iddelimname=="colon") iddelim =":";
else if(iddelimname=="bslash") iddelim ="\\";
else if(iddelimname=="slash" || iddelimname=="divide" || iddelimname=="div") iddelim ="/";
else if(iddelimname=="dash" || iddelimname=="subtract") iddelim ="-";
else if(iddelimname=="quote") iddelim ='"';
else if(iddelimname=="squote") iddelim ="'";
else if(iddelimname=="point" || iddelimname=="period" || iddelimname=="dot") iddelim =".";
else if(iddelimname=="add" || iddelimname=="sum") iddelim ="+";
else if(iddelimname=="star" || iddelimname=="multiply" || iddelimname=="asterisk") iddelim ="*";
else {iddelim=iddelimname;}

f=Switches.indexOf("-factor"); double factor=1.0;
if(f>=0 && f<Switches.size()-1) factor=Switches.at(f+1).toDouble();
if(factor<=0) {factor=1.0; cout << "Factor = " << factor << "\n";} 

f=Switches.indexOf("-column"); int columnselect=2;
if(f>=0 && f<Switches.size()-1) columnselect=Switches.at(f+1).toInt();
if(columnselect<1){cout << "Column must be greater than 1\n"; exit(1);}

f=Switches.indexOf("-valuecolumn"); int valuecolumn=12;
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

f=Switches.indexOf("-threshold"); double threshold; if(direction==0)threshold=1e50; else threshold=1e-50;
if(f>=0 && f<Switches.size()-1) threshold=Switches.at(f+1).toDouble();

f=Switches.indexOf("-missing"); QString Missing; 
if(f>=0 && f<Switches.size()-1) Missing=Switches.at(f+1); else Missing="NA";

int missingok=0;
f=Switches.indexOf("-missing-ok"); QString Missingok;
if(f>=0 && f<Switches.size()-1) Missingok=Switches.at(f+1);
if(Missingok=="no") missingok=0;
else if(Missingok=="yes") missingok=1;
else missingok=0;

int headeryes=1;
f=Switches.indexOf("-header"); QString Header;
if(f>=0 && f<Switches.size()-1) Header=Switches.at(f+1);
if(Header=="yes") headeryes=1;
else if(Header=="no") headeryes=0;
else headeryes=1;

/////////////////do analysis
cout << "running combiFilter()\n\n";

QFile outfile(Outname); if(!outfile.open(QIODevice::WriteOnly | QIODevice::Text)) {exit(1);}
QTextStream out_stream(&outfile);

QFile infile(Filename); if(!infile.open(QIODevice::ReadOnly | QIODevice::Text)) {exit(1);}
QTextStream in_stream(&infile);

////Populate reference table
if(!in_stream.atEnd() && headeryes==1) {QString Header = in_stream.readLine(); }//HEADER

while(!in_stream.atEnd())
{

oneRow = in_stream.readLine();

if(!oneRow.isEmpty())
{
list1=oneRow.split(Delim); 
Pheno=list1.at(columnselect-1);

  if(!Pheno.contains(iddelim))
  {
  Value=list1.at(valuecolumn-1);
  value = Value.toDouble(&ok);
  if(ok)
    {
    MergedValue.append(value);
    Marker=list1.at(markercolumn-1);
    Merged.clear(); Merged.append(Marker).append(iddelim).append(Pheno);
    MergedList.append(Merged);
    }
  }


}//if
}//while


////scan file
out_stream << "Marker" << Delim << "Trait1" << Delim << "Trait2" << Delim << "Combination" << Delim << "Trait1" << Delim << "Trait2"<< Delim << "OK" << "\n";

in_stream.seek(0); int rownumber=0; int loc=-1; int isOK=0;
if(!in_stream.atEnd() && headeryes==1) {QString Header = in_stream.readLine(); rownumber+=1;}//HEADER
while(!in_stream.atEnd())
{

oneRow = in_stream.readLine();

if(!oneRow.isEmpty())
{
rownumber+=1;
list1=oneRow.split(Delim); 
Pheno=list1.at(columnselect-1);

  if(Pheno.contains(iddelim))//is combinatorial
  {
  isOK=0; value1=-1.0; value2=-1.0;
  Value=list1.at(valuecolumn-1);
  value = Value.toDouble(&ok); 

  if(ok)//has value
    {
    if((direction==0 && value<=threshold) || (direction==1 && value>=threshold))
    {
    Marker=list1.at(markercolumn-1);
    list2=Pheno.split(iddelim); 
    if(list2.size()!=2){cout << "Trait name on row " << rownumber << " contains error\n"; exit(1);}
    else//is normal size
       {
       Pheno1=list2.at(0); Pheno2=list2.at(1);
       Merged1.clear(); Merged1.append(Marker).append(iddelim).append(Pheno1);
          loc=MergedList.indexOf(Merged1);
          if(loc>-1)//has ref1
          {
            value1=MergedValue.at(loc);
            if(direction==0 && (value*factor)<=value1) isOK+=1;
            if(direction==1 && (value/factor)>=value1) isOK+=1;
            Value1=QString::number(value1);
          }
          else {Value1=Missing; if(missingok)isOK+=1;}

       Merged2.clear(); Merged2.append(Marker).append(iddelim).append(Pheno2);
          loc=MergedList.indexOf(Merged2);
          if(loc>-1)//has ref2
          {
            value2=MergedValue.at(loc);
            if(direction==0 && (value*factor)<=value2) isOK+=1;
            if(direction==1 && (value/factor)>=value2) isOK+=1;
            Value2=QString::number(value2);
          }
          else {Value2=Missing; if(missingok)isOK+=1;}

       }//if size normal
     }//threshold ok
    }//if has value


  if(isOK>0) out_stream << Marker << Delim << Pheno1 << Delim << Pheno2 << Delim << value << Delim << Value1 << Delim << Value2 << Delim << isOK <<"\n";




  }//if combinatorial


}//if
}//while


MergedList.clear(); MergedValue.clear();
infile.close(); outfile.close();
}


