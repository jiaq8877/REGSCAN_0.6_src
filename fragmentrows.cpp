#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <cmath>
#include <string>

using namespace std;

extern QStringList Switches;


//////////////////////////////
void fragmentRows()
{

int f, counter=0, printOK=0, fragmentcounter=0;
QString Fragmentname;

f=Switches.indexOf("-file"); QString Filename; 
if(f>=0 && f<Switches.size()-1) Filename=Switches.at(f+1);
else {cout<<"You need to specify '-file'.\n\n"; exit(1);}

f=Switches.indexOf("-out"); QString Outname;
if(f>=0 && f<Switches.size()-1) Outname=Switches.at(f+1);
else {Outname=Filename; Outname.append(".out"); cout << "output name = input name + '.out'\n";}

int fragmentsize=1;
f=Switches.indexOf("-fragmentsize");
if(f>=0 && f<Switches.size()-1) {fragmentsize=Switches.at(f+1).toInt();}
else fragmentsize=1;
if(fragmentsize<1) fragmentsize=1;

int style=1;
f=Switches.indexOf("-style"); QString Style;
if(f>=0 && f<Switches.size()-1) Style=Switches.at(f+1);
if(Style=="prepend") style=1;
else if (Style=="append") style=0;
else style=1;

/////////////////do analysis
printf("running fragmentRows()\n");


QFile outfile; QTextStream out_stream(&outfile);
fragmentcounter+=1; Fragmentname=Outname; 
if(style==1) Fragmentname.prepend("_").prepend(QString::number(fragmentcounter));
else Fragmentname.append("_").append(QString::number(fragmentcounter));
outfile.setFileName(Fragmentname); 
if(!outfile.open(QIODevice::WriteOnly | QIODevice::Text)) {exit(1);}

QFile infile(Filename); if(!infile.open(QIODevice::ReadOnly | QIODevice::Text)) {exit(1);}
QTextStream in_stream(&infile);


while(!in_stream.atEnd())
{
QString oneRow = in_stream.readLine();

if(!oneRow.isEmpty())
{
  printOK=0; counter+=1;

     if(counter>fragmentsize)
     {
     outfile.close(); counter=0;

     counter+=1;

     fragmentcounter+=1; Fragmentname=Outname; 
     if(style==1) Fragmentname.prepend("_").prepend(QString::number(fragmentcounter));
     else Fragmentname.append("_").append(QString::number(fragmentcounter));

     outfile.setFileName(Fragmentname); 
     if(!outfile.open(QIODevice::WriteOnly | QIODevice::Text)) {exit(1);}

     }

out_stream << oneRow << "\n";


}//if

}//while
infile.close(); outfile.close();
cout << "Results are in " << Outname.toLocal8Bit().constData();
if(style==1) cout << " with prepended indexes\n"; else cout << " with appended indexes\n";

if(fragmentsize!=counter)cout << "Attn! Last file contains "<< counter << " rows\n";
}

