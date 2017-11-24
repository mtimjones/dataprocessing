// cleanse.c -- Data cleansing tool.

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>
#include <time.h>
#include "symtypes.h"

char inputFilename[ NAME_MAX ] = {0};
char outputFilename[ NAME_MAX ] = {0};
char testDataFilename[ NAME_MAX ] = {0};
char errorFilename[ NAME_MAX] = {0};
char schema[ NAME_MAX ] = {0};
double splitOption = 1.0;

#define getSRand()      ( ( float ) rand( ) / ( float ) RAND_MAX )
#define prob( x )       ( getSRand( ) > x )

void usage( char* progname )
{
   fprintf( stderr, "Usage: %s -i inputfile -o outputfile -c schema"
                    " [-s splitprob]\n", progname );
   return;
}

int main( int argc, char *argv[] )
{
   int opt;
   working_t working;
   FILE *fin, *fout, *ferr, *ftst;
   int emitSummary = 0;

   srand( time( NULL ) );

   if ( argc == 1 )
   {
      usage( argv[0] );
      return -1;
   }

   while ( ( opt = getopt( argc, argv, "i:o:s:c:y" ) ) != -1 )
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
            strcpy( testDataFilename, optarg );
            strcat( testDataFilename, ".tst" );
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

         case 'y': // Summary option
            emitSummary = 1;
            break;

         default:
            usage( argv[0] );
            return -1;
            break;
      }
   }

   // Open file pointers
   fin = fopen( inputFilename, "r" );
   if ( fin == ( FILE * )0 )
   {
      fprintf( stderr, "Could not open input file %s.\n", inputFilename );
      return -1;
   }

   fout = fopen( outputFilename, "w" );
   if ( fout == ( FILE * )0 )
   {
      fprintf( stderr, "Could not open output file %s.\n", outputFilename );
      fclose( fin );
      return -1;
   }

   ferr = fopen( errorFilename, "w" );
   if ( ferr == ( FILE * )0 )
   {
      fprintf( stderr, "Could not open error file %s.\n", errorFilename );
      fclose( fin );
      fclose( fout );
      return -1;
   }

   if ( splitOption != 1.0 )
   {
      ftst = fopen( testDataFilename, "w" );
      if ( ftst == ( FILE * )0 )
      {
         fprintf( stderr, "Could not open test data file %s.\n", testDataFilename );
         fclose( fin );
         fclose( fout );
         fclose( ferr );
         return -1;
      }
   }

   InitializeSummary( );

   while ( !feof( fin ) )
   {
      memset( &working, 0, sizeof( working ) );

      fgets( working.line, NAME_MAX, fin );

      if ( ( working.line[0] == 0 ) || ( working.line[0] == 13 ) ) continue;

      parseLine( &working, schema );

      if ( working.error )
      {
         fprintf( ferr, "# Error in field %d\n", working.errorField );
         fprintf( ferr, "%s", working.line );
      }
      else
      {
         if ( prob( splitOption ) )
         {
            fprintf( ftst, "%s", working.line );
         }
         else
         {
            fprintf( fout, "%s", working.line );
         }
      }
   }

   if ( emitSummary )
   {
      Summarize( );
   }

   // Close file pointers
   fclose( fin );
   fclose( fout );
   fclose( ferr );

   return 0;
}

