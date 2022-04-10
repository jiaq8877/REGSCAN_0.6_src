#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <cmath>
#include <string>

extern QList<double> p,t; 


/////////////////////////////////////////////////////////////

void initializeAnalysisWilson()
{
int i;

df10(); df20(); df40(); df65(); df100(); df150();
df250(); df500(); df1000(); df5000(); df10000(); df50000();

//HEADERS

char stat[10]=""; QString Stat;
Stat="P";

for(i = 0; i < phenorows; i++) {
  fprintf(sFiles[i],"rsid\tsnpid\tchr\tpos\ta1\ta0\tn\tfreq1\tbeta1\tse\tp\tr2");
  if(info) fprintf(sFiles[i],"\tinfo");
  if(info2) fprintf(sFiles[i],"\tinfo_pop");
  fprintf(sFiles[i],"\n");
}

if(summary){strcpy(stat, Stat.toLocal8Bit().constData()); fprintf(sF2,"Chromosome\tPosition\tAlleleA\tAlleleB\tMarker\tMarker#\tHits\tBestAbsSlope\tslope\t%s\tBestStat\tslope\t%s\n",stat,stat);}//V04

  
  
}

/////////////////////////////////////////////////////////////

void dataAnalysisWilson()
{
int i,j, counter;
double abs_max_slope=0.0, abs_best_statistic=0.0, abs_best_statistic_slope=0.0, abs_best_statistic_se=-1.0, abs_max_statistic=-1.0, x=0.0, y=0.0;
int bestpheno1_loc=0, bestpheno2_loc=0;
int hits=0;


if(atoi(chrom)==0){strcpy(chrom, default_chromosome);} else { sprintf(chrom,"%i",atoi(chrom));} //V06

abs_best_statistic=2.0;

for(counter=0; counter<phenorows; counter++)
{


   //linear regression
   int goodphenos=0, info2counter=-1;//V05
   double sumX=0.0, sumY=0.0, sumXY=0.0, sumXX=0.0;
   float AV1=0.0, AV2=0.0, AV3=0.0, MAC2=0.0;
   int AVXX=0;

   
   for(i=0; i<phenolength; i++)
   {

      y=phenoMat[counter][i];
      x=individual[i];
      
      if(y!=na_value)
      {
	if(x!=-1.0)
	{
        AV1+=countAA[i];
        AV2+=countAB[i];
        AV3+=countBB[i];
	
	   if(info2)//V05
	   {
	   info2counter+=1;
	   countAA2[info2counter]=countAA[i];
	   countAB2[info2counter]=countAB[i];
	   countBB2[info2counter]=countBB[i];
	   }
	   
	goodphenos+=1;

        sumX+=x;
        sumY+=y;
        sumXY+=x*y;
        sumXX+=x*x;
	
	}
        else AVXX+=1;
      }

   }
   //MAC2=AV2+(2*AV3);
   float INFOSCORE2=getInfo2(info2counter);

     if(AV3<=AV1) MAC2=AV2+(2*AV3);
     else MAC2=AV2+(2*AV1);

   double slope; if(sumX) slope=(((double)goodphenos*sumXY)-(sumX*sumY))/(((double)goodphenos*sumXX)-(sumX*sumX)); else slope=0.0;
   double intercept=(sumY - (slope*sumX))/(double)goodphenos;
          
   double abs_slope; if(slope<0) abs_slope=-1.0*slope; else abs_slope=slope;


           double R=-1.0,R2=-1.0,T=-1.0,P=-1.0, SE=-1.0;
           //if(abs_slope>=slope_lim && goodphenos>2)//0.3
	   if(abs_slope>=slope_lim && MAC2>=mac_lim && goodphenos>1)//0.4
           {

             ////r2 (statistic==0)
             
             double mean_y=0.0, sum_y=0.0; 
             double Y=0.0, Y2=0.0, sumY=0.0, sumY2=0.0;
             R2=0.0;

             for(j=0; j<phenolength; j++)
             {
               y=phenoMat[counter][j];
               x=individual[j];

               if(y!=na_value && x!=-1.0) //DO INSTEAD: if(y!=na_value && x>=0)
               {  
               Y=slope*x+intercept;
               Y=y-Y;
               Y*=Y;
               sumY+=Y;

               sum_y+=y;
               }
             }
             mean_y=sum_y/goodphenos;       

             for(j=0; j<phenolength; j++)
             {
               y=phenoMat[counter][j];
               x=individual[j];

               if(y!=na_value && x!=-1.0) //DO INSTEAD: if(y!=na_value && x>=0)
               {
               Y2=y-mean_y;
               Y2*=Y2; 
               sumY2+=Y2;
               }
             }          

             if(sumY2) R2=1.0-(sumY/sumY2); else R2=0.0; 
             if(R2<0.0) R2=0.0;         
                    

             ////t  

               R=sqrt(R2);
               if(R2<1)T=R*sqrt( ((double)goodphenos-2.0)/(1.0-R2) );
               else T=0;
               P=0.0;
             

              if(statistic==1 && T>=stat_lim && abs_slope>=slope_lim)//0.4
              {
              if(T>0)SE=abs_slope/T; else SE=-1.0;

                 //if(SE<=se_lim && SE!=-1.0)//0.3
		   if(SE<=se_lim)//0.4
                 {
                 fprintf(sFiles[counter],"%s\t%s_%s\t%s\t%s\t%s\t%s\t%i\t%4.f\t",rsnumber,chrom,rsposition,chrom,rsposition,alleleB,alleleA,goodphenos,(AV2+2*AV3)/(2*(AV1+AV2+AV3)));//V04

		 
	        if((MAC2<=0.0 || slope==0.0) && info) fprintf(sFiles[counter],"NA\tNA\tNA\tNA\t1\n");
	        else if ((MAC2<=0.0 || slope==0.0) && !info) fprintf(sF,"NA\tNA\tNA\tNA\t");
		 
	         else
	         {

            fprintf(sFiles[counter],"%f\t%f",slope,SE);

      		 //if(info && INFOSCORE>=0) fprintf(sF,"\t%f",INFOSCORE); 
           //            else if (info && INFOSCORE<0) fprintf(sF,"\tNA");
      	   //      else {;}
      	      
      	   //      if(info2 && INFOSCORE2>=0) fprintf(sF,"\t%f",INFOSCORE2); 
           //            else if (info2 && INFOSCORE2<0) fprintf(sF,"\tNA");
      	   //      else {;}
      	      
           //            fprintf(sF,"\n");  
	         }
	         
            if(summary)
            {
             hits+=1; 
                    if(abs_slope>abs_max_slope) {abs_max_slope=abs_slope; abs_max_statistic=T; bestpheno1_loc=counter;} 
                    if(T>abs_best_statistic) {abs_best_statistic=T; abs_best_statistic_slope=abs_slope; bestpheno2_loc=counter; abs_best_statistic_se=SE;}
                    }
                 }
              }
             

             ////p  
               if(T<=0) {P=-1.0; T=-1.0;}
               else
               {
               int best_location=0;
 
                 int count=1; double offset=0.0;
                 if(goodphenos<=10+2) 
                   {
                   if (T>=t10.at(941)) {P=1e-300;}
                   else{
                   while(count<942){if(T<t10.at(count)){best_location=count; break;} count+=1;}
                   offset= (T-t10.at(best_location-1)) / (t10.at(best_location) - t10.at(best_location-1));
                   P= p10.at(best_location-1) - (-p10.at(best_location) + p10.at(best_location-1))*offset;
                   }
                   }
           else if (goodphenos<=20+2) 
                   {
                   if (T>=t20.at(941)) {P=1e-300;}
                   else{
                   while(count<942){if(T<t20.at(count)){best_location=count; break;} count+=1;}
                   offset= (T-t20.at(best_location-1)) / (t20.at(best_location) - t20.at(best_location-1));
                   P= p20.at(best_location-1) - (-p20.at(best_location) + p20.at(best_location-1))*offset;
                   }
                   }
           else if (goodphenos<=40+2) 
                   {
                   if (T>=t40.at(941)) {P=1e-300;}
                   else{
                   while(count<942){if(T<t40.at(count)){best_location=count; break;} count+=1;}
                   offset= (T-t40.at(best_location-1)) / (t40.at(best_location) - t40.at(best_location-1));
                   P= p40.at(best_location-1) - (-p40.at(best_location) + p40.at(best_location-1))*offset;
                   }
                   }
           else if (goodphenos<=65+2) 
                   {
                   if (T>=t65.at(941)) {P=1e-300;}
                   else{
                   while(count<942){if(T<t65.at(count)){best_location=count; break;} count+=1;}
                   offset= (T-t65.at(best_location-1)) / (t65.at(best_location) - t65.at(best_location-1));
                   P= p65.at(best_location-1) - (-p65.at(best_location) + p65.at(best_location-1))*offset;
                   }
                   }
           else if (goodphenos<=100+2) 
                   {
                   if (T>=t100.at(941)) {P=1e-300;}
                   else{
                   while(count<942){if(T<t100.at(count)){best_location=count; break;} count+=1;}
                   offset= (T-t100.at(best_location-1)) / (t100.at(best_location) - t100.at(best_location-1));
                   P= p100.at(best_location-1) - (-p100.at(best_location) + p100.at(best_location-1))*offset;
                   }
                   }
           else if (goodphenos<=150+2) 
                   {
                   if (T>=t150.at(941)) {P=1e-300;}
                   else{
                   while(count<942){if(T<t150.at(count)){best_location=count; break;} count+=1;}
                   offset= (T-t150.at(best_location-1)) / (t150.at(best_location) - t150.at(best_location-1));
                   P= p150.at(best_location-1) - (-p150.at(best_location) + p150.at(best_location-1))*offset;
                   }
                   }
           else if (goodphenos<=250+2) 
                   {
                   if (T>=t250.at(941)) {P=1e-300;}
                   else{
                   while(count<942){if(T<t250.at(count)){best_location=count; break;} count+=1;}
                   offset= (T-t250.at(best_location-1)) / (t250.at(best_location) - t250.at(best_location-1));
                   P= p250.at(best_location-1) - (-p250.at(best_location) + p250.at(best_location-1))*offset;
                   }
                   }
           else if (goodphenos<=500+2) 
                   {
                   if (T>=t500.at(941)) {P=1e-300;}
                   else{
                   while(count<942){if(T<t500.at(count)){best_location=count; break;} count+=1;}
                   offset= (T-t500.at(best_location-1)) / (t500.at(best_location) - t500.at(best_location-1));
                   P= p500.at(best_location-1) - (-p500.at(best_location) + p500.at(best_location-1))*offset;
                   }
                   }
           else if (goodphenos<=1000+2) 
                   {
                   if (T>=t1000.at(941)) {P=1e-300;}
                   else{
                   while(count<942){if(T<t1000.at(count)){best_location=count; break;} count+=1;}
                   offset= (T-t1000.at(best_location-1)) / (t1000.at(best_location) - t1000.at(best_location-1));
                   P= p1000.at(best_location-1) - (-p1000.at(best_location) + p1000.at(best_location-1))*offset;
                   }
                   }
           else if (goodphenos<=5000+2) 
                   {
                   if (T>=t5000.at(941)) {P=1e-300;}
                   else{
                   while(count<942){if(T<t5000.at(count)){best_location=count; break;} count+=1;}
                   offset= (T-t5000.at(best_location-1)) / (t5000.at(best_location) - t5000.at(best_location-1));
                   P= p5000.at(best_location-1) - (-p5000.at(best_location) + p5000.at(best_location-1))*offset;
                   }
                   }
           else if (goodphenos<=10000+2) 
                  { 
                  if (T>=t10000.at(941)) {P=1e-300;}
                  else{
                  while(count<942){if(T<t10000.at(count)){best_location=count; break;} count+=1;}
                  offset= (T-t10000.at(best_location-1)) / (t10000.at(best_location) - t10000.at(best_location-1));
                  P= p10000.at(best_location-1) - (-p10000.at(best_location) + p10000.at(best_location-1))*offset;
                  }
                  }

              else 
                  { 
                  if (T>=t50000.at(941)) {P=1e-300;}
                  else{
                  while(count<942){if(T<t50000.at(count)){best_location=count; break;} count+=1;}
                  offset= (T-t50000.at(best_location-1)) / (t50000.at(best_location) - t50000.at(best_location-1));
                  P= p50000.at(best_location-1) - (-p50000.at(best_location) + p50000.at(best_location-1))*offset;
                  }
                  }

               }

                       //if(P<=stat_lim && abs_slope>=slope_lim && P!=-1.0) //0.3
                       if(P<=stat_lim && abs_slope>=slope_lim) //0.4
                       {
                       if(T>0)SE=abs_slope/T; else SE=-1.0;

                           //if(SE<=se_lim && SE!=-1.0)//0.3
			   if(SE<=se_lim)//0.4
                           { 
			                     fprintf(sFiles[counter],"%s\t%s_%s\t%s\t%s\t%s\t%s\t%i\t%.4f\t",rsnumber,chrom,rsposition,chrom,rsposition,alleleB,alleleA,goodphenos,(AV2+2*AV3)/(2*(AV1+AV2+AV3)));//V05x
			   
			  if((MAC2<=0.0 || slope==0.0) && info + info2 == 0) fprintf(sFiles[counter],"NA\tNA\tNA\tNA\n");
	                  else if ((MAC2<=0.0 || slope==0.0) && info + info2 == 1) fprintf(sFiles[counter],"NA\tNA\tNA\tNA\tNA\n");
                    else if ((MAC2<=0.0 || slope==0.0) && info + info2 == 2) fprintf(sFiles[counter],"NA\tNA\tNA\tNA\tNA\tNA\n");
			   
	                   else
	                   {	
			   fprintf(sFiles[counter],"%f\t%f",slope,SE);
                           if(P>0) fprintf(sFiles[counter],"\t%.2e",P); else fprintf(sFiles[counter],"\tNA");
			                     if(R2>0) fprintf(sFiles[counter],"\t%.3e",R2); else fprintf(sFiles[counter],"\tNA"); 

                           if(info && INFOSCORE>=0) fprintf(sFiles[counter],"\t%f",INFOSCORE); 
                           else if (info && INFOSCORE<0) fprintf(sFiles[counter],"\tNA");
	                   else {;}
	      
	                   if(info2 && INFOSCORE2>=0) fprintf(sFiles[counter],"\t%f",INFOSCORE2); 
                           else if (info2 && INFOSCORE2<0) fprintf(sFiles[counter],"\tNA");
	                   else {;}
	      
                           fprintf(sFiles[counter],"\n");  
	                   }
	                   
                              if(summary)
                              {
                              hits+=1; 
                              if(abs_slope>abs_max_slope) {abs_max_slope=abs_slope; abs_max_statistic=P; bestpheno1_loc=counter;} 
                              if(P<abs_best_statistic) {abs_best_statistic=P; abs_best_statistic_slope=abs_slope; bestpheno2_loc=counter; abs_best_statistic_se=SE;}
                              }
                           }
                       }

             
             
           }//if slope and goodphenos, main entrance
           else {T=-1.0; P=-1.0; R2=-1.0;}



}//PHENOS

         if(summary)
         {
            int printok=0; 

            if(abs_max_slope>=slope_lim && abs_best_statistic_se<=se_lim && (abs_best_statistic_se!=-1.0 || statistic==0))
            {
              if(statistic==2 && abs_best_statistic<=stat_lim && abs_best_statistic!=2.0)
              { 
              printok=1;
              }
              else if (statistic<2 && abs_best_statistic>=stat_lim && abs_best_statistic!=0.0)
              { 
              printok=1;
              }
              else {;}
            }


            if(printok>0)
             {
              //fprintf(sF2,"%s\t%i\t%i\t",rsnumber, progress, hits);
              fprintf(sF2,"%s\t%s\t%s\t%s\t%s\t%i\t%i\t",chrom, rsposition, alleleA, alleleB, rsnumber, progress, hits);//V04

              char bestpheno1[100]=""; char ch1='0'; int n1=0;
              while(ch1!='\0'){ch1=phenoNames[bestpheno1_loc][n1]; bestpheno1[n1]=ch1; n1+=1;}
              if(abs_max_slope>0)fprintf(sF2,"%s\t%f\t%.2e\t",bestpheno1, abs_max_slope, abs_max_statistic);
              else fprintf(sF2,"%s\tNA\tNA\t",bestpheno1);
	      
              char bestpheno2[100]=""; char ch2='0'; int n2=0;
              while(ch2!='\0'){ch2=phenoNames[bestpheno2_loc][n2]; bestpheno2[n2]=ch2; n2+=1;} 
              if(abs_best_statistic_slope>0)fprintf(sF2,"%s\t%f\t%.2e\n",bestpheno2, abs_best_statistic_slope, abs_best_statistic);
	      else fprintf(sF2,"%s\tNA\tNA\n",bestpheno2);
             }
            
         }
     
    if(progstep)//V05
    {  
      progress_block+=1;  
      if(progress_block>=progstep)
      {
      printf("marker %i\n", progress);
      progress_block=0;
      }
    }
    

}


