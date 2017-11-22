// cleanse.c -- Data cleansing tool.

#include <stdio.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>
#include <linux/limits.h>

char inputFilename[ NAME_MAX ] = {0};
char outputFilename[ NAME_MAX ] = {0};
char errorFilename[ NAME_MAX] = {0};
char schema[ NAME_MAX ] = {0};
double splitOption = 0.0;

void usage( char* progname )
{
   fprintf( stderr, "Usage: %s -i inputfile -o outputfile"
                    " [-s splitprob] [-c schema]\n", progname );
   return;
}

int main( int argc, char *argv[] )
{
   int opt;

   if ( argc == 1 )
   {
      usage( argv[0] );
      return -1;
   }

   while ( ( opt = getopt( argc, argv, "i:o:s:c:" ) ) != -1 )
   {
      switch( opt )
      {
         case 'i': // Input filename
            strcpy( inputFilename, optarg );
            break;
         case 'o': // Output filename
            strcpy( outputFilename, optarg );
            strcat( outputFilename, ".dat" );
            strcpy( errorFilename, optarg );
            strcat( errorFilename, ".err" );
            break;
         case 's': // Split probability
            if ( sscanf( optarg, "%lg", &splitOption ) != 1 )
            {
               fprintf( stderr, "Unable to parse split probability (-s)\n" );
            }
            break;
         case 'c': // Schema definition
            strcpy( schema, optarg );
            break;
         default:
            usage( argv[0] );
            return -1;
            break;
      }
   }

   printf("%s, %s, %s, %s, %g\n",
            inputFilename, outputFilename, errorFilename, schema, splitOption);

   return 0;
}

