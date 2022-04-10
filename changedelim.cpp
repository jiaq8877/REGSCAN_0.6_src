#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <cmath>
#include <string>

using namespace std;

extern QStringList Switches;
/////////////////////////////////////////////////////////////////////

void changeDelim()
{
char row_in[999999];
FILE *infile;
FILE *outfile;
int f, i;


f=Switches.indexOf("-file"); char filename[200]=""; QString Filename=Switches.at(f+1); strcpy(filename, Filename.toLocal8Bit().constData());
if(f<0 || strcmp(filename,"\0")==0) exit(1);

f=Switches.indexOf("-out"); char outname[200]=""; QString Outname=Switches.at(f+1); strcpy(outname, Outname.toLocal8Bit().constData());
if(f<0 || strcmp(outname,"\0")==0) 
{
Outname=Filename; Outname.append(".out");
strcpy(outname, Outname.toLocal8Bit().constData());
}

f=Switches.indexOf("-delim1"); char delimcommand[20]=""; strcpy(delimcommand, Switches.at(f+1).toLocal8Bit().constData()); char delim[20]="";
if(f<0 || strcmp(delimcommand,"\0")==0) {strcpy(delimcommand,"tab"); strcpy(delim,"\t");}
else if(strcmp(delimcommand,"tab")==0 || strcmp(delimcommand,"TAB")==0) strcpy(delim,"\t");
else if(strcmp(delimcommand,"space")==0) strcpy(delim," ");
else if(strcmp(delimcommand,"comma")==0) strcpy(delim,",");
else if(strcmp(delimcommand,"semicolon")==0) strcpy(delim,";");
else if(strcmp(delimcommand,"colon")==0) strcpy(delim,":");
else strcpy(delim,delimcommand);

f=Switches.indexOf("-delim2"); char delimcommand2[20]=""; strcpy(delimcommand2, Switches.at(f+1).toLocal8Bit().constData()); char delim2[20]="";
if(f<0 || strcmp(delimcommand2,"\0")==0) {strcpy(delimcommand2,"tab"); strcpy(delim2,"\t");}
else if(strcmp(delimcommand2,"tab")==0 || strcmp(delimcommand2,"TAB")==0) strcpy(delim2,"\t");
else if(strcmp(delimcommand2,"space")==0) strcpy(delim2," ");
else if(strcmp(delimcommand2,"comma")==0) strcpy(delim2,",");
else if(strcmp(delimcommand2,"semicolon")==0) strcpy(delim2,";");
else if(strcmp(delimcommand2,"colon")==0) strcpy(delim2,":");
else if(strcmp(delimcommand2,"none")==0) strcpy(delim2,"");
else strcpy(delim2,delimcommand2);

if(strcmp(delim,delim2)==0) exit(1);

/////////////////do analysis
printf("running changeDelim()\n");

if((outfile=fopen(outname,"w")) == 0) {exit(1);}
if((infile=fopen(filename,"r")) == 0) {exit(1);}

while(!feof(infile))
{

fgets(row_in, 999999, infile);
QString oneRow=row_in;
QStringList list1 = oneRow.split(delim);
int listsize= list1.size();

if(!feof(infile)) 
{ 

      for(i=0; i<listsize;i++)
      {
      QString member=list1.at(i); member.remove('\n');
      char printit[200]=""; strcpy(printit, member.toLocal8Bit().constData()); fprintf(outfile,"%s", printit);
         
      if(i==listsize-1) fprintf(outfile,"\n");
      else fprintf(outfile,"%s", delim2);

      }
  
}
}

fclose(infile); fclose(outfile);
}


