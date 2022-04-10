#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <cmath>
#include <string>
#include <zlib.h>

#include <cassert>
#include <stdexcept>
#include <memory>

#include "bgen.hpp"

///////////////////////
extern QStringList Switches;

///////////////////////
QList<double> p10,p20,p40,p65,p100,p150,p250,p500,p1000,p5000,p10000,p50000;
QList<double> t10,t20,t40,t65,t100,t150,t250,t500,t1000,t5000,t10000,t50000;
QString Phenoname, Genoname, Outname, Statsname, Gfile, Delim, Missing, Chromosome, Split, Wilson; //V06
double slope_lim, r2_lim, stat_lim, se_lim, mac_lim;
int groups, testgroups, statistic;
void df10(),df20(),df40(),df65(),df100(),df150(),df250(),df500(),df1000(),df5000(),df10000(),df50000();
long buffersize=100000000;

char ctext[10]=""; 
char rsnumber[500]="RS", rsposition[20]="";
char alleleA[1000]="", alleleB[1000]="";//V04
char chrom[10]="";//V04
char default_chromosome[10] = {'N','A','\0'};; //V06

FILE *sF, *sF2;
FILE **sFiles = (FILE **)malloc(2*sizeof(FILE*));
float *individual; float *gdata; double **phenoMat; char **phenoNames;
float *countAA, *countAB, *countBB;
float *countAA2, *countAB2, *countBB2;//V05
double *meanPdata;
double na_value=-99.99;
int genolength=0, phenolength=0, phenorows=0;
double mean_ad=0.0;
int progress=0, progress_block=0, summary=0, hzyg=0, info=0, info2=0, statsonly=0, progstep=0, split=0, wilson=0; //V06
float AA=0.0, AB=0.0, BB=0.0, MAC=0.0, INFOSCORE=-1.0; 
int XX=0;

////////////////prototypes///////////////////////////////

float ctextToFloat(int);
void initializeAnalysis();
void dataAnalysis();
void initializeAnalysisSplit(); //V06
void dataAnalysisSplit(); //V06
void initializeAnalysisWilson(); //V06
void dataAnalysisWilson(); //V06
float getInfo();
float getInfo2(int);
void statsOnly();

////////////////////////////////////////////////adeopted from Gavin Band's bgen_to_vcf.cpp

struct ProbSetter {
	typedef std::vector< std::vector< double > > Data ;
	ProbSetter( Data* result ):
		m_result( result ),
		m_sample_i(0)
	{}
		
	// Called once allowing us to set storage.
	void initialise( std::size_t number_of_samples, std::size_t number_of_alleles ) {
		m_result->clear() ;
		m_result->resize( number_of_samples ) ;
	}
	
	// If present with this signature, called once after initialise()
	// to set the minimum and maximum ploidy and numbers of probabilities among samples in the data.
	// This enables us to set up storage for the data ahead of time.
	void set_min_max_ploidy( uint32_t min_ploidy, uint32_t max_ploidy, uint32_t min_entries, uint32_t max_entries ) {
		for( std::size_t i = 0; i < m_result->size(); ++i ) {
			m_result->at( i ).reserve( max_entries ) ;
		}
	}
	
	// Called once per sample to determine whether we want data for this sample
	bool set_sample( std::size_t i ) {
		m_sample_i = i ;
		// Yes, here we want info for all samples.
		return true ;
	}
	
	// Called once per sample to set the number of probabilities that are present.
	void set_number_of_entries(
		std::size_t ploidy,
		std::size_t number_of_entries,
		genfile::OrderType order_type,
		genfile::ValueType value_type
	) {
		assert( value_type == genfile::eProbability ) ;
		m_result->at( m_sample_i ).resize( number_of_entries ) ;
		m_entry_i = 0 ;
	}

	void set_value( uint32_t, double value ) {
		m_result->at( m_sample_i ).at( m_entry_i++ ) = value ;
	}

	void set_value( uint32_t, genfile::MissingValue value ) {
                // Here we encode missing probabilities with 0
                m_result->at( m_sample_i ).at( m_entry_i++ ) = 0 ;
	}

	// If present with this signature, called once after all data has been set.
	void finalise() {
		// nothing to do in this implementation.
	}

private:
	Data* m_result ;
	std::size_t m_sample_i ;
	std::size_t m_entry_i ;
} ;

// BgenParser is a thin wrapper around the core functions in genfile/bgen/bgen.hpp.
// This class tracks file state and handles passing the right callbacks.
struct BgenParser {
	
	BgenParser( std::string const& filename ):
		m_filename( filename ),
		m_state( e_NotOpen ),
		m_have_sample_ids( false )
	{
		// Open the stream
		m_stream.reset(
			new std::ifstream( filename, std::ifstream::binary )
		) ;
		if( !*m_stream ) {
			throw std::invalid_argument( filename ) ;
		}
		m_state = e_Open ;

		// Read the offset, header, and sample IDs if present.
		genfile::bgen::read_offset( *m_stream, &m_offset ) ;
		genfile::bgen::read_header_block( *m_stream, &m_context ) ;
		if( m_context.flags & genfile::bgen::e_SampleIdentifiers ) {
			genfile::bgen::read_sample_identifier_block(
				*m_stream, m_context,
				[this]( std::string id ) { m_sample_ids.push_back( id ) ; }
			) ;
			m_have_sample_ids = true ;
		}
		
		// Jump to the first variant data block.
		m_stream->seekg( m_offset + 4 ) ;

		// We keep track of state (though it's not really needed for this implementation.)
		m_state = e_ReadyForVariant ;
	}

        
        
	std::ostream& summarise( std::ostream& o ) const {
                
                
		o << "BgenParser: bgen file ("
			<< ( m_context.flags & genfile::bgen::e_v12Layout ? "v1.2 layout" : "v1.1 layout" )
			<< ", "
			<< ( m_context.flags & genfile::bgen::e_CompressedSNPBlocks ? "compressed" : "uncompressed" ) << ")"
			<< " with " 
			<< m_context.number_of_samples << " " << ( m_have_sample_ids ? "named" : "anonymous" ) << " samples and "
			<< m_context.number_of_variants << " variants.\n" ;
                
		return o ;
	}
       

	std::size_t number_of_samples() const {
		return m_context.number_of_samples ;
	}


	// Report the sample IDs in the file using the given setter object
	// (If there are no sample IDs in the file, we report a dummy identifier).
	template< typename Setter >
	void get_sample_ids( Setter setter ) {
		if( m_have_sample_ids ) {
			for( std::size_t i = 0; i < m_context.number_of_samples; ++i ) {
				setter( m_sample_ids[i] ) ;
			}
		} else {
			for( std::size_t i = 0; i < m_context.number_of_samples; ++i ) {
				setter( "(unknown_sample_" + std::to_string( i+1 ) + ")" ) ;
			}
		}
	}

	// Attempt to read identifying information about a variant from the bgen file, returning
	// it in the given fields.
	// If this method returns true, data was successfully read, and it should be safe to call read_probs()
	// or ignore_probs().
	// If this method returns false, data was not successfully read indicating the end of the file.
	bool read_variant(
		std::string* chromosome,
		uint32_t* position,
		std::string* rsid,
		std::vector< std::string >* alleles
	) {
		assert( m_state == e_ReadyForVariant ) ;
		std::string SNPID ; // read but ignored in this toy implementation
		
		if(
			genfile::bgen::read_snp_identifying_data(
				*m_stream, m_context,
				&SNPID, rsid, chromosome, position,
				[&alleles]( std::size_t n ) { alleles->resize( n ) ; },
				[&alleles]( std::size_t i, std::string const& allele ) { alleles->at(i) = allele ; }
			)
		) {
			m_state = e_ReadyForProbs ;
			return true ;
		} else {
			return false ;
		}
	}
	
	// Read genotype probability data for the SNP just read using read_variant()
	// After calling this method it should be safe to call read_variant() to fetch
	// the next variant from the file.
	void read_probs( std::vector< std::vector< double > >* probs ) {
		assert( m_state == e_ReadyForProbs ) ;
		ProbSetter setter( probs ) ;
		genfile::bgen::read_and_parse_genotype_data_block< ProbSetter >(
			*m_stream,
			m_context,
			setter,
			&m_buffer1,
			&m_buffer2
		) ;
		m_state = e_ReadyForVariant ;
	}

	// Ignore genotype probability data for the SNP just read using read_variant()
	// After calling this method it should be safe to call read_variant()
	// to fetch the next variant from the file.
	void ignore_probs() {
		genfile::bgen::ignore_genotype_data_block( *m_stream, m_context ) ;
		m_state = e_ReadyForVariant ;
	}

private:
	std::string const m_filename ;
	std::unique_ptr< std::istream > m_stream ;

	// bgen::Context object holds information from the header block,
	// including bgen flags
	genfile::bgen::Context m_context ;

	// offset byte from top of bgen file.
	uint32_t m_offset ;

	// We keep track of our state in the file.
	// Not strictly necessary for this implentation but makes it clear that
	// calls must be read_variant() followed by read_probs() (or ignore_probs())
	// repeatedly.
	enum State { e_NotOpen = 0, e_Open = 1, e_ReadyForVariant = 2, e_ReadyForProbs = 3, eComplete = 4 } ;
	State m_state ;

	// If the BGEN file contains samples ids, they will be read here.
	bool m_have_sample_ids ;
	std::vector< std::string > m_sample_ids ;
	
	// Buffers, these are used as working space by bgen implementation.
	std::vector< genfile::byte_t > m_buffer1, m_buffer2 ;
} ;

////////////////////////////////////////////////

void getData()
{

int i,j,k, counter=0, nonmissing_counter=0;
QString Row, Phenotype, BestPheno;
QStringList list1;
double readDouble, sum_ad=0.0;
char readString[100];

////Is gfile normal gen (0), gzipped (1) or bgen (2)
int filetype=0; 
if(Genoname.endsWith(".gz"))filetype=1;
else if(Genoname.endsWith(".bgen"))filetype=2;
else filetype=0;

int Genoname_size = Genoname.size()+1; 
char genoname[Genoname_size]; strcpy(genoname, Genoname.toLocal8Bit().constData());

////Find genofile column number
Row.clear();
if(filetype==1)//GZIP
{
char *gbuffer; gbuffer = (char*) malloc (sizeof(char)*100000000); if (gbuffer == NULL) {fputs ("GZip test error",stderr); exit (1);}

gzFile F;
if(F = gzopen(genoname,"rb")) {;} else {printf("Error when opening gzip file\n"); exit(1);}
gzgets(F,gbuffer,100000000);

long count=0;
while(gbuffer[count]!='\n')
{
Row.append(gbuffer[count]); count+=1;
}

list1 = Row.split(Delim); genolength=list1.size()-5;
Row.clear();list1.clear();

free(gbuffer);
}

else if(filetype==0)//GEN
{
QFile genofile(Genoname); if(!genofile.open(QIODevice::ReadOnly | QIODevice::Text)) {exit(1);}
QTextStream geno_stream(&genofile);

  Row = geno_stream.readLine(); 

  list1 = Row.split(Delim); genolength=list1.size()-5;
  Row.clear();list1.clear();

genofile.close();
}

else//BGEN
{
std::string const filename=genoname;

        BgenParser bgenParser( filename ) ;
 
        std::string chromosome ;
        uint32_t position ;
        std::string rsid ;
        std::vector< std::string > alleles ;
        std::vector< std::vector< double > > probs ;

        bgenParser.read_variant( &chromosome, &position, &rsid, &alleles ) ;
        bgenParser.read_probs( &probs ) ;
	
	int prsize=probs.size();
        genolength = 3*prsize;

}

////Find phenofile column and row number

QFile phenofile(Phenoname); if(!phenofile.open(QIODevice::ReadOnly | QIODevice::Text)) {exit(1);}
QTextStream pheno_stream(&phenofile);

while(!pheno_stream.atEnd())
{
   Row = pheno_stream.readLine(); 

   if(!Row.isEmpty())
   {
   phenorows+=1;
   if(phenorows==1){list1 = Row.split(Delim); phenolength=list1.size()-1;list1.clear();}
   }
}
Row.clear();
phenofile.close();


////See if pheno and geno file column numbers match

if(genolength/3!=phenolength){printf("Phenotype and genotype files contain different number of individuals\n"); exit(1);}
if(genolength<3){printf("No genotypes\n"); exit(1);}
if(phenolength<1){printf("No phenotypes\n"); exit(1);}
printf("Data for %i individuals\n", phenolength);
printf("%i MB of read buffer allocated\n", buffersize/1000000);

////Memory allocations
char *buffer; buffer = (char*) malloc (sizeof(char)*buffersize); if (buffer == NULL) {fputs ("Memory error1",stderr); exit (1);}
 individual = (float*) malloc (sizeof(float)*phenolength); if(individual == NULL) {fputs ("Memory error2",stderr); exit(1);}
 countAA = (float*) malloc (sizeof(float)*phenolength); if(countAA == NULL) {fputs ("Memory error2a",stderr); exit(1);}
 countAB = (float*) malloc (sizeof(float)*phenolength); if(countAB == NULL) {fputs ("Memory error2b",stderr); exit(1);}
 countBB = (float*) malloc (sizeof(float)*phenolength); if(countBB == NULL) {fputs ("Memory error2c",stderr); exit(1);}

 if(info2)
 {
 countAA2 = (float*) malloc (sizeof(float)*phenolength); if(countAA2 == NULL) {fputs ("Memory error3a",stderr); exit(1);}//V05
 countAB2 = (float*) malloc (sizeof(float)*phenolength); if(countAB2 == NULL) {fputs ("Memory error3b",stderr); exit(1);}//V05
 countBB2 = (float*) malloc (sizeof(float)*phenolength); if(countBB2 == NULL) {fputs ("Memory error3c",stderr); exit(1);}//V05
 }

 gdata = (float*) malloc (sizeof(float)*genolength); if(gdata == NULL) {fputs ("Memory error3",stderr); exit(1);}
 meanPdata = (double*) malloc (sizeof(double)*phenorows); if(meanPdata == NULL) {fputs ("Memory error4",stderr); exit(1);}

phenoMat = (double**) malloc((phenorows+1) * sizeof(double*)); if(phenoMat == NULL) {exit(1);}
for(i = 0; i < phenorows+1; i++)
{phenoMat[i] = (double*)malloc((phenolength+1) * sizeof(double)); if(phenoMat[i] == NULL) {exit(1);}}

phenoNames = (char**) malloc((phenorows+1) * sizeof(char*)); if(phenoNames == NULL) {exit(1);}
for(i = 0; i < phenorows+1; i++)
{phenoNames[i] = (char*)malloc((100) * sizeof(char)); if(phenoNames[i] == NULL) {exit(1);}}

sFiles = (FILE**) realloc(sFiles, (phenorows) * sizeof(FILE*)); //V06





////Read phenotypes into phenoMat, phenoNames and meanPdata
int Phenoname_size = Phenoname.size()+1; char phenoname[Phenoname_size]; strcpy(phenoname, Phenoname.toLocal8Bit().constData());
int Missing_size = Missing.size()+1; char missing[Missing_size]; strcpy(missing, Missing.toLocal8Bit().constData());
printf("Phenotypes from %s\n", phenoname);
double sumP; int countP;
long result,ii;

FILE *pF; if((pF=fopen(phenoname,"r")) == 0) {exit(1);}
for(j=0; j<phenorows; j++)
{
 sumP=0.0; countP=0;
 for(i=0; i<phenolength+1; i++)
 {
   if(i)
   {
   fscanf(pF,"%s", &readString); 
   if(strcmp(readString,missing)!=0){readDouble=atof(readString); sumP+=readDouble; countP+=1;}
   else {readDouble=na_value;} 
   phenoMat[j][i-1]=readDouble;
   }
   else
   { 
   fscanf(pF,"%s", &readString);
    for(k=0; k<strlen(readString)+1; k++) phenoNames[j][k]=readString[k];
   }
 }
 meanPdata[j]=sumP/countP;
} 
fclose(pF);




////Open output files

printf("Genotypes from %s", genoname); if(filetype==1) printf(" (unzip)\n"); else printf("\n");


int Outname_size = Outname.size()+1; char outname[Outname_size]; strcpy(outname, Outname.toLocal8Bit().constData());


if(split || wilson) //V06
{
for(int i = 0; i < phenorows; i++) {
    
    char ph[50], filename[100];
    
    std::string pheno = phenoNames[i]; //Convert phenotype name to string
    int index = pheno.find("/"); //Check for division character
    if(index != std::string::npos) pheno.replace(index,1,"_"); //Replace division character
    pheno.copy(ph, pheno.size()+1); //Convert back to character vector
    ph[pheno.size()] = '\0'; //Terminate character vector

    sprintf(filename, "%s.%s.tsv", outname, ph); //Append phenoname to output
    
    
    if((sFiles[i]=fopen(filename,"w")) == 0) {fputs("Ouput filename error!",stderr); exit(1);}

}


} else {

if((sF=fopen(outname,"w")) == 0) {exit(1);}
printf("Main output is %s\n", outname);

}

int Statsname_size = Statsname.size()+1; char statsname[Statsname_size]; 
if(summary)
{
strcpy(statsname, Statsname.toLocal8Bit().constData());
if((sF2=fopen(statsname,"w")) == 0) {exit(1);}
printf("Summary file is %s\n", statsname);
}



////Initialize analyses

if(wilson) initializeAnalysisWilson();
	else if (split) initializeAnalysisSplit();
	else initializeAnalysis();




////Read and analyze genotypes and write new array
int entry=0;
float value=0.0;
int loc=-1;

if(filetype==1)//GZIP FILE
{
gzFile F = gzopen(genoname,"rb");

while(gzgets(F,buffer,buffersize)!=0)
{

  ii=0;
  while(buffer[ii]!='\0')
  {

     value=0.0;
     if(buffer[ii]==32) {
                        if(entry>4) {value=ctextToFloat(loc); gdata[entry-5]=value;}
                        else if (entry==0) {chrom[loc+1]='\0';}//V04
                        else if (entry==1) {rsnumber[loc+1]='\0';}
                        else if (entry==2) {rsposition[loc+1]='\0';}
                        else if (entry==3) {alleleA[loc+1]='\0';}//V04
                        else if (entry==4) {alleleB[loc+1]='\0';}//V04
                        else {;}

                        entry+=1; loc=-1;          
                        }

     else if(buffer[ii]=='\n')
                        {
                        if(entry>4) {value=ctextToFloat(loc); gdata[entry-5]=value;}
                        else if (entry==0) {chrom[loc+1]='\0';}//V04
                        else if (entry==1) {rsnumber[loc+1]='\0';}
                        else if (entry==2) {rsposition[loc+1]='\0';}
                        else if (entry==3) {alleleA[loc+1]='\0';}//V04
                        else if (entry==4) {alleleB[loc+1]='\0';}//V04
                        else {;}

                        entry=0; loc=-1;

                        sum_ad=0.0; mean_ad=0.0; counter=0; nonmissing_counter=0;
                        AA=0.0; AB=0.0; BB=0.0; XX=0; MAC=0.0;
                        progress+=1;
                        for(j=0; j<genolength-2; j=j+3)
                        {
			  
			       float ad_entry=0; float ad1=gdata[j]; float ad2=gdata[j+1]; float ad3=gdata[j+2];
			       countAA[counter]=ad1; countAB[counter]=ad2; countBB[counter]=ad3;
			       float adsum=ad1+ad2+ad3;
			       
			       if(adsum>0 && adsum<1.01)
			       {
				       AA+=ad1; AB+=ad2; BB+=ad3;
                                       if(!hzyg) ad_entry=ad2+(2.0*ad3); else ad_entry=ad2;
				       
				       individual[counter]=ad_entry; sum_ad+=ad_entry; counter+=1; nonmissing_counter+=1;

			       }
			       
			       else 
			       {
			       individual[counter]=-1.0;
			       counter+=1;
			       XX+=1;
			       }


                        }
                        mean_ad=sum_ad/nonmissing_counter;

                        if(BB<=AA) MAC=AB+2*BB;
                        else MAC=AB+2*AA;

			if(info) INFOSCORE=getInfo();
			
			if(statsonly)statsOnly();
                        else if(MAC < mac_lim) continue; //V06
                        else if(wilson) dataAnalysisWilson(); //V06
                        else if(split) dataAnalysisSplit(); //V06
                        else dataAnalysis(); //V06
			//else {;}

                        }

     else {
          if(entry>4){loc+=1; ctext[loc]=buffer[ii];}
          else if (entry==0) {loc+=1; chrom[loc]=buffer[ii];}//V04
          else if(entry==1) {loc+=1; rsnumber[loc]=buffer[ii];}
          else if(entry==2) {loc+=1; rsposition[loc]=buffer[ii];}
          else if (entry==3) {loc+=1; alleleA[loc]=buffer[ii];}//V04
          else if (entry==4) {loc+=1; alleleB[loc]=buffer[ii];}//V04
          else {;}
          }

  ii+=1;
  }//for,while
}//while
}

//*******************//

else if(filetype==0)//NORMAL GEN FILE
{
FILE *gF; gF = fopen(genoname,"rb"); if (gF==NULL) {fputs("File error",stderr); exit(1);}
while(!feof(gF))
{
  result = fread(buffer,1,buffersize,gF);

  for(ii=0; ii<result; ii++)
  {

     value=0.0;
     if(buffer[ii]==32) {
                        if(entry>4) {value=ctextToFloat(loc); gdata[entry-5]=value;}
                        else if (entry==0) {chrom[loc+1]='\0';}//V04
                        else if (entry==1) {rsnumber[loc+1]='\0';}
                        else if (entry==2) {rsposition[loc+1]='\0';}
                        else if (entry==3) {alleleA[loc+1]='\0';}//V04
                        else if (entry==4) {alleleB[loc+1]='\0';}//V04
                        else {;}

                        entry+=1; loc=-1;          
                        }

     else if(buffer[ii]=='\n')
                        {
                        if(entry>4) {value=ctextToFloat(loc); gdata[entry-5]=value;}
                        else if (entry==0) {chrom[loc+1]='\0';}//V04
                        else if (entry==1) {rsnumber[loc+1]='\0';}
                        else if (entry==2) {rsposition[loc+1]='\0';}
                        else if (entry==3) {alleleA[loc+1]='\0';}//V04
                        else if (entry==4) {alleleB[loc+1]='\0';}//V04
                        else {;}

                        entry=0; loc=-1;

                        sum_ad=0.0; mean_ad=0.0; counter=0; nonmissing_counter=0;
                        AA=0.0; AB=0.0; BB=0.0; XX=0; MAC=0.0;
                        progress+=1;
                        for(j=0; j<genolength-2; j=j+3)
                        {
			  
			       float ad_entry=0; float ad1=gdata[j]; float ad2=gdata[j+1]; float ad3=gdata[j+2];
			       countAA[counter]=ad1; countAB[counter]=ad2; countBB[counter]=ad3;
			       float adsum=ad1+ad2+ad3;
			       
			       if(adsum>0 && adsum<1.01)
			       {
				       AA+=ad1; AB+=ad2; BB+=ad3;
                                       if(!hzyg) ad_entry=ad2+(2.0*ad3); else ad_entry=ad2;
				       
				       individual[counter]=ad_entry; sum_ad+=ad_entry; counter+=1; nonmissing_counter+=1;

			       }
			       
			       else 
			       {
			       individual[counter]=-1.0;
			       counter+=1;
			       XX+=1;
			       }

                        }
                        mean_ad=sum_ad/nonmissing_counter;

                        if(BB<=AA) MAC=AB+2*BB;
                        else MAC=AB+2*AA;
			
                        if(info) INFOSCORE=getInfo();
			
			if(statsonly)statsOnly();
                        else if(MAC < mac_lim) continue; //V06
                        else if(wilson) dataAnalysisWilson(); //V06
                        else if(split) dataAnalysisSplit(); //V06
                        else dataAnalysis(); //V06
			//else {;}

                        }

     else {
          if(entry>4){loc+=1; ctext[loc]=buffer[ii];}
          else if (entry==0) {loc+=1; chrom[loc]=buffer[ii];}//V04
          else if(entry==1) {loc+=1; rsnumber[loc]=buffer[ii];}
          else if(entry==2) {loc+=1; rsposition[loc]=buffer[ii];}
          else if (entry==3) {loc+=1; alleleA[loc]=buffer[ii];}//V04
          else if (entry==4) {loc+=1; alleleB[loc]=buffer[ii];}//V04
          else {;}
          }


  }//for
}//while
fclose(gF);
}

else//BGEN FILE
{
int tripletcounter=0;
float ad1=0.0, ad2=0.0, ad3=0.0;

    std::string const filename=genoname;

    try {
            BgenParser bgenParser( filename ) ;
            bgenParser.summarise( std::cerr ) ;


            // Output variants
            std::string chromosome ;
            uint32_t position ;
            std::string rsid ;
            std::vector< std::string > alleles ;
            std::vector< std::vector< double > > probs ;



            while( bgenParser.read_variant( &chromosome, &position, &rsid, &alleles ))
            {

                    strcpy(rsnumber,rsid.c_str());
                    sprintf(rsposition, "%d", position);
                    strcpy(chrom,chromosome.c_str());//V04
                    strcpy(alleleA,alleles[0].c_str());//V04
                    strcpy(alleleB,alleles[1].c_str());//V04

                    assert( alleles.size() > 0 ) ;

                    /*
                    for( std::size_t i = 1; i < alleles.size(); ++i )
                    {
                            std::cout << ( i > 1 ? "," : "" ) << alleles[i] ;
                    }
                    *///V05

                    sum_ad=0.0; mean_ad=0.0; counter=0; nonmissing_counter=0;
                    AA=0.0; AB=0.0; BB=0.0; XX=0; MAC=0.0;
                    progress+=1;

                    bgenParser.read_probs( &probs );

                    for( std::size_t i = 0; i < probs.size(); ++i )
                    {
                            for( std::size_t j = 0; j < probs[i].size(); ++j )
                            {
                                    if( probs[i][j] < 0 )
                                    {
                                        std::cout << "Unrecognizable data type in bgen file!\n" ; exit(1);
                                    }

                                    else
                                    {

                                            tripletcounter+=1;
                                            if(tripletcounter==1){ad1=probs[i][j];}
                                                else if(tripletcounter==2){ad2=probs[i][j];}
                                                else {ad3=probs[i][j];}

                                    }

                                    if(tripletcounter==3)
                                    {

                                        countAA[counter]=ad1; countAB[counter]=ad2; countBB[counter]=ad3;
					float adsum=ad1+ad2+ad3;
					float ad_entry=0;
					
			            if(adsum>0 && adsum<1.01)
			            {
				       AA+=ad1; AB+=ad2; BB+=ad3;
                                       if(!hzyg) ad_entry=ad2+(2.0*ad3); else ad_entry=ad2;
				       
				       individual[counter]=ad_entry; sum_ad+=ad_entry; counter+=1; nonmissing_counter+=1;

			            }
			       
			            else 
			            {
			            individual[counter]=-1.0;
			            counter+=1;
			            XX+=1;
			            }

                                    tripletcounter=0; ad1=ad2=ad3=0.0;
                                    }



                            }
                    }

                    mean_ad=sum_ad/nonmissing_counter;

                    if(BB<=AA) MAC=AB+2*BB;
                    else MAC=AB+2*AA;
		    
                    if(info) INFOSCORE=getInfo();
		    
	            if(statsonly)statsOnly();
                        else if(MAC < mac_lim) continue; //V06
                        else if(wilson) dataAnalysisWilson(); //V06
                        else if(split) dataAnalysisSplit(); //V06
                        else dataAnalysis(); //V06
            }

    }
    catch( genfile::bgen::BGenError const& e ) {
    std::cerr << "Error parsing bgen file.\n" ; exit(1);

    }

}

//free buffer here

////End and free memory
p10.clear(); p20.clear(); p40.clear(); p65.clear(); p100.clear(); p150.clear(); 
p250.clear(); p500.clear(); p1000.clear(); p5000.clear(); p10000.clear(); p50000.clear();
t10.clear(); t20.clear(); t40.clear(); t65.clear(); t100.clear(); t150.clear(); 
t250.clear(); t500.clear(); t1000.clear(); t5000.clear(); t10000.clear(); t50000.clear();

if(summary)fclose(sF2);
free(buffer); free(individual); free(gdata); free(meanPdata);
free(countAA); free(countAB); free(countBB);
free(countAA2); free(countAB2); free(countBB2);
for(i=0; i<phenorows+1; i++) {free(phenoMat[i]); free(phenoNames[i]); } free(phenoMat); free(phenoNames); //V06
if(split || wilson) {for(i=0; i<phenorows; i++) {fclose(sFiles[i]);}} else {fclose(sF);}
}

//////////////////////////////////////////////////////////////////////////

void switches()
{
int f;
bool ok;

///////switches

f=Switches.indexOf("-pfile"); if(f>=0 && f<Switches.size()-1) Phenoname=Switches.at(f+1);
else {printf("You need to specify '-pfile'.\n\n"); exit(1);}

f=Switches.indexOf("-out"); if(f>=0 && f<Switches.size()-1) Outname=Switches.at(f+1);
else {Outname=Phenoname; Outname.append(".out");}

Statsname=Outname; Statsname.append(".stats");

f=Switches.indexOf("-gfile"); if(f>=0 && f<Switches.size()-1) Genoname=Switches.at(f+1);
else {printf("You need to specify '-gfile'.\n\n"); exit(1);}

f=Switches.indexOf("-delim"); QString Delimname;
if(f>=0 && f<Switches.size()-1) Delimname=Switches.at(f+1); else Delimname = "space";
if(Delimname=="tab") Delim ="\t";
else if(Delimname=="space") Delim =" ";
else if(Delimname=="comma") Delim =",";
else if(Delimname=="semicolon") Delim =";";
else if(Delimname=="colon") Delim =":";
else Delim=Delimname;

f=Switches.indexOf("-missing");
if(f>=0 && f<Switches.size()-1) Missing=Switches.at(f+1); else Missing = "NA";
if(Missing=="none") Missing="";

f=Switches.indexOf("-slope");
if(f>=0 && f<Switches.size()-1) slope_lim=Switches.at(f+1).toDouble(); else slope_lim=0.0;
if(slope_lim<0.0) slope_lim=0.0;

f=Switches.indexOf("-selimit");
if(f>=0 && f<Switches.size()-1) se_lim=Switches.at(f+1).toDouble(); else se_lim=1e12;

f=Switches.indexOf("-maclimit");
if(f>=0 && f<Switches.size()-1) mac_lim=Switches.at(f+1).toDouble(); else mac_lim=0.0;

stat_lim=1.0;
f=Switches.indexOf("-statistic"); QString Statname;
if(f>=0 && f<Switches.size()-1) Statname=Switches.at(f+1); else Statname = "p";
if(Statname=="r2") {statistic=0; stat_lim=0.0;}
else if(Statname=="t") {statistic=1; stat_lim=0.0;}
else if(Statname=="p") {statistic=2; stat_lim=1.0;}
else {statistic=2; stat_lim=1.0;}

f=Switches.indexOf("-statlimit");
if(f>=0 && f<Switches.size()-1) stat_lim=Switches.at(f+1).toDouble();

f=Switches.indexOf("-summary"); QString Summary;
if(f>=0 && f<Switches.size()-1) Summary=Switches.at(f+1); else Summary = "no";
if(Summary=="no") summary=0;
else if(Summary=="yes") summary=1;
else summary=0;

f=Switches.indexOf("-hzyg-effects"); QString Hzyg;
if(f>=0 && f<Switches.size()-1) Hzyg=Switches.at(f+1); else Hzyg = "no";
if(Hzyg=="no") hzyg=0;
else if(Hzyg=="yes") {hzyg=1; printf("\nAttn - heterozygous mode used! A.D.is calculated as A.D. = P(AB), not A.D. = P(AB) + 2P(BB)\n");}
else hzyg=0;

f=Switches.indexOf("-info"); QString Info;
if(f>=0 && f<Switches.size()-1) Info=Switches.at(f+1); else Info = "no";
if(Info=="no") {info=0; printf("Info score not computed\n");}
else if(Info=="yes") {info=1; printf("Info score computed based on all genotypes, errors excluded\n");}
else {info=0; printf("Info score not computed\n");}

f=Switches.indexOf("-info2"); QString Info2;
if(f>=0 && f<Switches.size()-1) Info2=Switches.at(f+1); else Info2 = "no";
if(Info2=="no") {info2=0; printf("Info2 score not computed\n");}
else if(Info2=="yes") {info2=1; printf("Info2 score computed based on genotypes that have corresponding phenotypes\n");}
else {info2=0; printf("Info2 score not computed\n");}

f=Switches.indexOf("-buffer"); QString Buffersize; buffersize=100000000;
if(f>=0 && f<Switches.size()-1) 
{
Buffersize=Switches.at(f+1);
buffersize=Buffersize.toInt(&ok);
if(ok) buffersize*=1000000;
}

f=Switches.indexOf("-statsonly"); QString Statsonly;
if(f>=0 && f<Switches.size()-1) Statsonly=Switches.at(f+1); else Statsonly = "no";
if(Statsonly=="no") statsonly=0;
else if(Statsonly=="yes") {statsonly=1; info=1; summary=0; printf("Attn: Only genotype statistics are computed!\n");}
else statsonly=0;

progstep=0;
f=Switches.indexOf("-show-progress"); QString Progstep;
if(f>=0 && f<Switches.size()-1)
{
Progstep=Switches.at(f+1);
progstep = Progstep.toInt(&ok);
if(!ok)progstep=0;
}

f=Switches.indexOf("-chromosome"); QString Chromosome; //V06
if(f>=0 && f<Switches.size()-1) 
{
	Chromosome=Switches.at(f+1);
	strcpy(default_chromosome, Chromosome.toLocal8Bit().constData());
	printf("Missing chromosomes will be set to '%s'\n", default_chromosome);
}

f=Switches.indexOf("-split"); QString Split; //V06
if(f>=0 && f<Switches.size()-1) Split=Switches.at(f+1); else Split = "no";
if(Split=="no") {split=0;}
else if(Split=="yes" && statsonly==0) {split=1; printf("Output statistics will be split by phenotype\n");}
else {split=0;}


f=Switches.indexOf("-wilson"); QString Wilson; //V06
if(f>=0 && f<Switches.size()-1) Wilson=Switches.at(f+1); else Wilson = "no";
if(Wilson=="no") {wilson=0;}
else if(Wilson=="yes" && statsonly==0) {wilson=1; printf("Output statistics will be in wilson-format and split by phenotype\n");}
else {wilson=0;}



if(statistic==2)printf("Main statistic = P\n");
else if(statistic==1)printf("Main statistic = T\n");
else if(statistic==0)printf("Main statistic = R2, SE not calculated\n");
else printf("Main statistic = error\n");

printf("Slope limit = %.3e\n", slope_lim);
printf("Stat limit = %.3e\n", stat_lim);
if(statistic>0)printf("SE limit = %.3e\n", se_lim);
printf("MAC limit = %.3e\n", mac_lim);

getData();
}

