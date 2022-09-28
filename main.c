#include <math.h>
#include <stdio.h>
#include "cmnCmdOptions2.h"
#include "siprint.h"
#include "numcol.h"

#define nothing    0
#define inductance 1
#define turns      2
#define constant   8

// gcc *.c -lm -o inductor

void calcK(double L, double N,  double *K);
void calcL(double *L, double N, double K);
void calcN(double L, double *N,  double K);


int Help (char * argv) ;
void setupParams(CLOSet_t ** options) ;
int SetL  (char * argv) ;
int SetN  (char * argv) ;
int SetK  (char * argv) ;

unsigned char given, helpOnly;
double L, N, K;


int main(int argc, char** argv) {
  CLOSet_t * options;
  
  char * s;
  
  options  = NULL;
  given    = nothing;
  helpOnly = 0;
  
  setupParams(&options); // define the arguments
  parseOptions(options, argc-1, &argv[1]);   // then parse the command line
  
  if (helpOnly)
    return 0;
  
  switch(given) {
    case nothing:
      printf("You need to know about the paramiter --help.\n");
      return 1;
    break;
    
    case (inductance+turns):
      calcK(L, N, &K);
    break;    
            
    case (turns+constant):
      calcL(&L, N, K);
    break;
     
    case (inductance+constant):
      calcN(L, &N, K);
    break;
        
    default:
      printf("You need to specify two of the paramiters\n");
      return 2;
    break;    
    
  }  
    
  printf("Inductance is:    %sH\n",    s = adoubleToPrefixS(L)); free(s);
  printf("Turns      is:    %s\n",     s = adoubleToPrefixS(N)); free(s);
  printf("Constant   is:    %s\n",     s = adoubleToPrefixS(K)); free(s);
  
  cmdLineOptionFini(&options);
  return 0;

}

void setupParams(CLOSet_t ** options) {

  // this is outright ugly, but we have to because of limits to C
  static char* optsInductance[2] =       {"-l","--inductance"};
  static char* optsTurns[2] =            {"-n","--turns"};
  static char* optsConstant[2] =         {"-k","--constant"};
  static char* optsHelp[2] =             {"-h","--help"};

  // first init 
  cmdLineOptionInit(options);
  // then add your options (note I ignore any errors (BAD! BAD!))
  addCmdLineOption(*options, optsInductance,     2, SetL);
  addCmdLineOption(*options, optsTurns,          2, SetN);
  addCmdLineOption(*options, optsConstant,       2, SetK);
  addCmdLineOption(*options, optsHelp,           2, Help);
}

int Help (char * argv) {
  printf("This program solves constant stuff.\n\n");
  printf("-h --help            is the help your looking at\n");
  printf("-l --inductance      set inductance\n");
  printf("-n --turns           number of turns \n");
  printf("-k --constant        core constant (H/n^2)\n\n");
  printf("Program understand scientific notation and postfixes\n");
  printf("  example 1e-7, 100n, 0.1u are all ok.\n");
  printf("  don't include unit or it'll get confused.\n");
  printf("Example: inductor -n 10 -l 0.04m \n");
  
  helpOnly = 1;
  return 0;
}

int SetL (char * argv) {
  L = NumColExtract(argv);
  given |= inductance;
  return 1;
}

int SetN (char * argv) {
  N = NumColExtract(argv);
  given |= turns;
  return 1;
}

int SetK (char * argv) {
  K = NumColExtract(argv);
  given |= constant;
  return 1;
}


// ----=====| sphere conversions |=====-----


void calcK(double L, double N,  double *K) {
   *K = L/N/N;
}

void calcL(double *L, double N, double K) {
   *L = N*K*N;
}

void calcN(double L, double *N,  double K) {
   *N = sqrt(L/K);
}

