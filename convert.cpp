#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <cmath>
#include <string>

/////////////////////////////////////////////////////////////

float ctextToFloat(int loc)
{
 float value=0.0;
 char x;

    if(loc==0)
    {
      if(ctext[0]=='1') value=1.0; else value=0.0;
    }

    else
    {
      if(ctext[0]=='1') value=1.0; else value=0.0;//against further format change (VERSION1)
      //value=0.0; //faster (VERSION2)
      
      if(loc>=2)
      {
      x = ctext[2];
          if(x=='1') value+=0.1;
          else if (x=='2') value+=0.2; else if (x=='3') value+=0.3;
          else if (x=='4') value+=0.4; else if (x=='5') value+=0.5;
          else if (x=='6') value+=0.6; else if (x=='7') value+=0.7;
          else if (x=='8') value+=0.8; else if (x=='9') value+=0.9;
          else {;}
       }

      if(loc>=3)
      {
      x = ctext[3];
          if(x=='1') value+=0.01;
          else if (x=='2') value+=0.02; else if (x=='3') value+=0.03;
          else if (x=='4') value+=0.04; else if (x=='5') value+=0.05;
          else if (x=='6') value+=0.06; else if (x=='7') value+=0.07;
          else if (x=='8') value+=0.08; else if (x=='9') value+=0.09;
          else {;}
      }

      if(loc>=4)
      {
      x = ctext[4];
          if(x=='1') value+=0.001;
          else if (x=='2') value+=0.002; else if (x=='3') value+=0.003;
          else if (x=='4') value+=0.004; else if (x=='5') value+=0.005;
          else if (x=='6') value+=0.006; else if (x=='7') value+=0.007;
          else if (x=='8') value+=0.008; else if (x=='9') value+=0.009;
          else {;}
      }
     
    }


return value;
}



