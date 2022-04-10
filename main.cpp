#include <QApplication>
#include <QCoreApplication>
#include <QString>
#include <QStringList>
#include <QDate>
#include <QTime>
#include <QFile>
#include <QTextStream>
#include <QVector>

#include "regscan1.cpp"
#include "convert.cpp"
#include "regscan2.cpp"
#include "regscan3_split.cpp" //V06
#include "regscan3_wilson.cpp" //V06
#include "dfref.cpp"

#include "extract.cpp"
#include "combitable.cpp"
#include "transpose.cpp"
#include "fragmentrows.cpp"
#include "singlemath.cpp"
#include "changedelim.cpp"
#include "multicount.cpp"
#include "remseqdup.cpp"
#include "findbetter.cpp"
#include "multifilter.cpp"
#include "combifilter.cpp"
#include "extractid.cpp"

#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <cmath>
#include <string>


#include "MissingValue.cpp"
#include "zlib.cpp"
#include "bgen.cpp"

char argument[200]="";
QStringList Switches;
using namespace std;

///////////////////////////////////////////////////////////////
void welcome()
{
  printf("\nREGSCAN v. 0.6\n\n* splitting outcome by phenotype is now an option.\n* missing chromosomes can now be set to a user-defined value.\n* results can now be directly written to Wilson Group (University of Edinburgh) format.\nContact: Tom Haller, toomashaller@gmail.com\nWebsite: http://www.geenivaramu.ee/en/tools/regscan\n\n"); //V06
}

////////////////////////////////////////////////////////////

int main(int argc, char *argv[])
{

QCoreApplication application(argc, argv); 
strcpy(argument,argv[0]); 
welcome();


int x;
for (x=1; x<argc; x++) printf("%d  %s\n",x, argv[x]);

char mode[50]="";
for (x=1; x<argc; x++) 
{ 
   if (x+1 != argc)
   {
   if(strcmp(argv[x],"-M")==0 || strcmp(argv[x],"-m")==0) {strcpy(mode, argv[x+1]);break;}  
   else {;}
   }

   if(x==argc-1){printf("You need to specify the mode '-M'\n\n"); exit(1);}
}



        for (x=1; x<argc; x++) 
        { 
            if (x+1 != argc)
            {   

                  if(strcmp(mode,"gwas")==0 || strcmp(mode,"GWAS")==0)
                  {
                   if(strcmp(argv[x],"-pfile")==0) {Switches.append(argv[x]); Switches.append(argv[x+1]);}
                   if(strcmp(argv[x],"-gfile")==0) {Switches.append(argv[x]); Switches.append(argv[x+1]);}
                   if(strcmp(argv[x],"-out")==0) {Switches.append(argv[x]); Switches.append(argv[x+1]);}
                   if(strcmp(argv[x],"-missing")==0) {Switches.append(argv[x]); Switches.append(argv[x+1]);}
                   if(strcmp(argv[x],"-delim")==0) {Switches.append(argv[x]); Switches.append(argv[x+1]);}
                   if(strcmp(argv[x],"-slope")==0) {Switches.append(argv[x]); Switches.append(argv[x+1]);}
                   if(strcmp(argv[x],"-maclimit")==0) {Switches.append(argv[x]); Switches.append(argv[x+1]);}
                   if(strcmp(argv[x],"-statistic")==0) {Switches.append(argv[x]); Switches.append(argv[x+1]);}
                   if(strcmp(argv[x],"-statlimit")==0) {Switches.append(argv[x]); Switches.append(argv[x+1]);}
                   if(strcmp(argv[x],"-selimit")==0) {Switches.append(argv[x]); Switches.append(argv[x+1]);}
                   if(strcmp(argv[x],"-buffer")==0) {Switches.append(argv[x]); Switches.append(argv[x+1]);}
                   if(strcmp(argv[x],"-summary")==0) {Switches.append(argv[x]); Switches.append(argv[x+1]);}
                   if(strcmp(argv[x],"-hzyg-effects")==0) {Switches.append(argv[x]); Switches.append(argv[x+1]);}
                   if(strcmp(argv[x],"-info")==0) {Switches.append(argv[x]); Switches.append(argv[x+1]);}
                   if(strcmp(argv[x],"-info2")==0) {Switches.append(argv[x]); Switches.append(argv[x+1]);}
                   if(strcmp(argv[x],"-statsonly")==0) {Switches.append(argv[x]); Switches.append(argv[x+1]);}
                   if(strcmp(argv[x],"-show-progress")==0) {Switches.append(argv[x]); Switches.append(argv[x+1]);}
                   if(strcmp(argv[x],"-chromosome")==0) {Switches.append(argv[x]); Switches.append(argv[x+1]);} //V06
                   if(strcmp(argv[x],"-split")==0) {Switches.append(argv[x]); Switches.append(argv[x+1]);} //V06
                   if(strcmp(argv[x],"-wilson")==0) {Switches.append(argv[x]); Switches.append(argv[x+1]);} //V06
                  }
 
                  else if(strcmp(mode,"extract")==0)
                  {
                   if(strcmp(argv[x],"-file")==0) {Switches.append(argv[x]); Switches.append(argv[x+1]);}
                   if(strcmp(argv[x],"-columns")==0) {Switches.append(argv[x]); Switches.append(argv[x+1]);}
                   if(strcmp(argv[x],"-rows")==0) {Switches.append(argv[x]); Switches.append(argv[x+1]);}
                   if(strcmp(argv[x],"-delim")==0) {Switches.append(argv[x]); Switches.append(argv[x+1]);}
                   if(strcmp(argv[x],"-out")==0) {Switches.append(argv[x]); Switches.append(argv[x+1]);}
                  }

                  else if(strcmp(mode,"combitable")==0)
                  {
                   if(strcmp(argv[x],"-header")==0) {Switches.append(argv[x]); Switches.append(argv[x+1]);}
                   if(strcmp(argv[x],"-file")==0) {Switches.append(argv[x]); Switches.append(argv[x+1]);}
                   if(strcmp(argv[x],"-columns")==0) {Switches.append(argv[x]); Switches.append(argv[x+1]);}
                   if(strcmp(argv[x],"-delim")==0) {Switches.append(argv[x]); Switches.append(argv[x+1]);}
                   if(strcmp(argv[x],"-out")==0) {Switches.append(argv[x]); Switches.append(argv[x+1]);}
                   if(strcmp(argv[x],"-function")==0) {Switches.append(argv[x]); Switches.append(argv[x+1]);}
                   if(strcmp(argv[x],"-missing")==0) {Switches.append(argv[x]); Switches.append(argv[x+1]);}
                  }

                  else if(strcmp(mode,"transpose")==0)
                  {
                   if(strcmp(argv[x],"-file")==0) {Switches.append(argv[x]); Switches.append(argv[x+1]);}
                   if(strcmp(argv[x],"-col-chunk")==0) {Switches.append(argv[x]); Switches.append(argv[x+1]);}
                   if(strcmp(argv[x],"-delim")==0) {Switches.append(argv[x]); Switches.append(argv[x+1]);}
                   if(strcmp(argv[x],"-out")==0) {Switches.append(argv[x]); Switches.append(argv[x+1]);}
                   if(strcmp(argv[x],"-columns")==0) {Switches.append(argv[x]); Switches.append(argv[x+1]);}
                  }

                  else if(strcmp(mode,"fragmentrows")==0)
                  {
                   if(strcmp(argv[x],"-file")==0) {Switches.append(argv[x]); Switches.append(argv[x+1]);}
                   if(strcmp(argv[x],"-fragmentsize")==0) {Switches.append(argv[x]); Switches.append(argv[x+1]);}
                   if(strcmp(argv[x],"-style")==0) {Switches.append(argv[x]); Switches.append(argv[x+1]);}
                   if(strcmp(argv[x],"-out")==0) {Switches.append(argv[x]); Switches.append(argv[x+1]);}
                  }

                  else if(strcmp(mode,"singlemath")==0)
                  {
                   if(strcmp(argv[x],"-file")==0) {Switches.append(argv[x]); Switches.append(argv[x+1]);}
                   if(strcmp(argv[x],"-columns")==0) {Switches.append(argv[x]); Switches.append(argv[x+1]);}
                   if(strcmp(argv[x],"-delim")==0) {Switches.append(argv[x]); Switches.append(argv[x+1]);}
                   if(strcmp(argv[x],"-out")==0) {Switches.append(argv[x]); Switches.append(argv[x+1]);}
                   if(strcmp(argv[x],"-function")==0) {Switches.append(argv[x]); Switches.append(argv[x+1]);}
                   if(strcmp(argv[x],"-header")==0) {Switches.append(argv[x]); Switches.append(argv[x+1]);}
                   if(strcmp(argv[x],"-missing")==0) {Switches.append(argv[x]); Switches.append(argv[x+1]);}
                  }

                  else if(strcmp(mode,"changedelim")==0)
                  {
                   if(strcmp(argv[x],"-file")==0) {Switches.append(argv[x]); Switches.append(argv[x+1]);}
                   if(strcmp(argv[x],"-delim1")==0) {Switches.append(argv[x]); Switches.append(argv[x+1]);}
                   if(strcmp(argv[x],"-delim2")==0) {Switches.append(argv[x]); Switches.append(argv[x+1]);}
                   if(strcmp(argv[x],"-out")==0) {Switches.append(argv[x]); Switches.append(argv[x+1]);}
                  }

                  else if(strcmp(mode,"remseqdup")==0)
                  {
                   if(strcmp(argv[x],"-file")==0) {Switches.append(argv[x]); Switches.append(argv[x+1]);}
                   if(strcmp(argv[x],"-delim")==0) {Switches.append(argv[x]); Switches.append(argv[x+1]);}
                   if(strcmp(argv[x],"-out")==0) {Switches.append(argv[x]); Switches.append(argv[x+1]);}
                   if(strcmp(argv[x],"-column")==0) {Switches.append(argv[x]); Switches.append(argv[x+1]);}
                   if(strcmp(argv[x],"-header")==0) {Switches.append(argv[x]); Switches.append(argv[x+1]);}
                  }

                  else if(strcmp(mode,"multicount")==0)
                  {
                   if(strcmp(argv[x],"-list")==0) {Switches.append(argv[x]); Switches.append(argv[x+1]);}
                   if(strcmp(argv[x],"-file")==0) {Switches.append(argv[x]); Switches.append(argv[x+1]);}
                   if(strcmp(argv[x],"-delim")==0) {Switches.append(argv[x]); Switches.append(argv[x+1]);}
                   if(strcmp(argv[x],"-out")==0) {Switches.append(argv[x]); Switches.append(argv[x+1]);}
                   if(strcmp(argv[x],"-column")==0) {Switches.append(argv[x]); Switches.append(argv[x+1]);}
                   if(strcmp(argv[x],"-header")==0) {Switches.append(argv[x]); Switches.append(argv[x+1]);}
                   if(strcmp(argv[x],"-iddelim")==0) {Switches.append(argv[x]); Switches.append(argv[x+1]);}
                  }

                  else if(strcmp(mode,"findbetter")==0)
                  {
                   if(strcmp(argv[x],"-file")==0) {Switches.append(argv[x]); Switches.append(argv[x+1]);}
                   if(strcmp(argv[x],"-delim")==0) {Switches.append(argv[x]); Switches.append(argv[x+1]);}
                   if(strcmp(argv[x],"-iddelim")==0) {Switches.append(argv[x]); Switches.append(argv[x+1]);}
                   if(strcmp(argv[x],"-out")==0) {Switches.append(argv[x]); Switches.append(argv[x+1]);}
                   if(strcmp(argv[x],"-column")==0) {Switches.append(argv[x]); Switches.append(argv[x+1]);}
                   if(strcmp(argv[x],"-valuecolumn")==0) {Switches.append(argv[x]); Switches.append(argv[x+1]);}
                   if(strcmp(argv[x],"-markercolumn")==0) {Switches.append(argv[x]); Switches.append(argv[x+1]);}
                   if(strcmp(argv[x],"-header")==0) {Switches.append(argv[x]); Switches.append(argv[x+1]);}
                   if(strcmp(argv[x],"-factor")==0) {Switches.append(argv[x]); Switches.append(argv[x+1]);}
                   if(strcmp(argv[x],"-dir")==0) {Switches.append(argv[x]); Switches.append(argv[x+1]);}
                   if(strcmp(argv[x],"-missing")==0) {Switches.append(argv[x]); Switches.append(argv[x+1]);}
                  }

                  else if(strcmp(mode,"multifilter")==0)
                  {
                   if(strcmp(argv[x],"-file")==0) {Switches.append(argv[x]); Switches.append(argv[x+1]);}
                   if(strcmp(argv[x],"-delim")==0) {Switches.append(argv[x]); Switches.append(argv[x+1]);}
                   if(strcmp(argv[x],"-missing")==0) {Switches.append(argv[x]); Switches.append(argv[x+1]);}
                   if(strcmp(argv[x],"-out")==0) {Switches.append(argv[x]); Switches.append(argv[x+1]);}
                   if(strcmp(argv[x],"-header")==0) {Switches.append(argv[x]); Switches.append(argv[x+1]);}
                   if(strcmp(argv[x],"-filter")==0) {Switches.append(argv[x]); Switches.append(argv[x+1]);}
                   if(strcmp(argv[x],"-logic")==0) {Switches.append(argv[x]); Switches.append(argv[x+1]);}
                   if(strcmp(argv[x],"-dir")==0) {Switches.append(argv[x]); Switches.append(argv[x+1]);}
                   if(strcmp(argv[x],"-ignore-missing")==0) {Switches.append(argv[x]); Switches.append(argv[x+1]);}
                  }

                  else if(strcmp(mode,"combifilter")==0)
                  {
                   if(strcmp(argv[x],"-file")==0) {Switches.append(argv[x]); Switches.append(argv[x+1]);}
                   if(strcmp(argv[x],"-delim")==0) {Switches.append(argv[x]); Switches.append(argv[x+1]);}
                   if(strcmp(argv[x],"-iddelim")==0) {Switches.append(argv[x]); Switches.append(argv[x+1]);}
                   if(strcmp(argv[x],"-out")==0) {Switches.append(argv[x]); Switches.append(argv[x+1]);}
                   if(strcmp(argv[x],"-column")==0) {Switches.append(argv[x]); Switches.append(argv[x+1]);}
                   if(strcmp(argv[x],"-valuecolumn")==0) {Switches.append(argv[x]); Switches.append(argv[x+1]);}
                   if(strcmp(argv[x],"-markercolumn")==0) {Switches.append(argv[x]); Switches.append(argv[x+1]);}
                   if(strcmp(argv[x],"-header")==0) {Switches.append(argv[x]); Switches.append(argv[x+1]);}
                   if(strcmp(argv[x],"-factor")==0) {Switches.append(argv[x]); Switches.append(argv[x+1]);}
                   if(strcmp(argv[x],"-dir")==0) {Switches.append(argv[x]); Switches.append(argv[x+1]);}
                   if(strcmp(argv[x],"-missing")==0) {Switches.append(argv[x]); Switches.append(argv[x+1]);}
                   if(strcmp(argv[x],"-missing-ok")==0) {Switches.append(argv[x]); Switches.append(argv[x+1]);}
                   if(strcmp(argv[x],"-threshold")==0) {Switches.append(argv[x]); Switches.append(argv[x+1]);}
                  }

                  else if(strcmp(mode,"extractid")==0)
                  {
                   if(strcmp(argv[x],"-file")==0) {Switches.append(argv[x]); Switches.append(argv[x+1]);}
                   if(strcmp(argv[x],"-delim")==0) {Switches.append(argv[x]); Switches.append(argv[x+1]);}
                   if(strcmp(argv[x],"-out")==0) {Switches.append(argv[x]); Switches.append(argv[x+1]);}
                   if(strcmp(argv[x],"-header")==0) {Switches.append(argv[x]); Switches.append(argv[x+1]);}
                   if(strcmp(argv[x],"-column")==0) {Switches.append(argv[x]); Switches.append(argv[x+1]);}
                   if(strcmp(argv[x],"-is")==0) {Switches.append(argv[x]); Switches.append(argv[x+1]);}
                   if(strcmp(argv[x],"-contains")==0) {Switches.append(argv[x]); Switches.append(argv[x+1]);}
                   if(strcmp(argv[x],"-dir")==0) {Switches.append(argv[x]); Switches.append(argv[x+1]);}
                  }

                  else {;}
       
            }
        }


QTime ti; ti.start();

char show[100];
QTime curTime = QTime::currentTime(); QString textTime = curTime.toString();
strcpy(show, textTime.toLocal8Bit().constData()); printf("\nSTART TIME: %s, ", show);
QDate curDate = QDate::currentDate(); QString textDate = curDate.toString();
strcpy(show, textDate.toLocal8Bit().constData()); printf("%s\n\n", show);
 

if(strcmp(mode,"gwas")==0) switches();
else if(strcmp(mode,"extract")==0) extract();
else if(strcmp(mode,"combitable")==0) combiTable();
else if(strcmp(mode,"transpose")==0) transpose();
else if(strcmp(mode,"fragmentrows")==0) fragmentRows();
else if(strcmp(mode,"singlemath")==0) singleMath();
else if(strcmp(mode,"changedelim")==0) changeDelim();
else if(strcmp(mode,"remseqdup")==0) remSeqDup();
else if(strcmp(mode,"multicount")==0) multiCount();
else if(strcmp(mode,"findbetter")==0) findBetter();
else if(strcmp(mode,"multifilter")==0) multiFilter();
else if(strcmp(mode,"combifilter")==0) combiFilter();
else if(strcmp(mode,"extractid")==0) extractID();
else {;}


QTime curTime2 = QTime::currentTime(); QString textTime2 = curTime2.toString();
strcpy(show, textTime2.toLocal8Bit().constData()); printf("\nEND TIME: %s, ", show);
QDate curDate2 = QDate::currentDate(); QString textDate2 = curDate2.toString();
strcpy(show, textDate2.toLocal8Bit().constData()); printf("%s\n\n", show);


int msecs = ti.elapsed(); 
int secs = msecs/1000; int mins = msecs/(1000*60); int hours = msecs/(1000*60*60);

mins = mins - (60*hours);
secs = secs-(60*mins)-(60*60*hours);
msecs = msecs - (1000*secs) - (1000*60*mins) - (1000*60*60*hours);  

if(hours>0) printf("\nDONE in %i h %i min %i sec %i msec\n\n", hours, mins, secs, msecs);
else if (mins>0) printf("DONE in %i min %i sec %i msec\n\n", mins, secs, msecs);
else if (secs>0) printf("DONE in %i sec %i msec\n\n", secs, msecs);
else printf("DONE in %i msec\n\n", msecs);


Switches.clear();


return 0;

}

