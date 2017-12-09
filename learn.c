// vq.c -- Clustering with Vector Quantization

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>
#include <strings.h>
#include <time.h>
#include "symtypes.h"
#include "learn.h"

char testDataFilename[ NAME_MAX ] = {0};
char validationFilename[ NAME_MAX ] = {0};
char outputFilename[ NAME_MAX ] = {0};

static int changed = 0;

void set_changed( int value )
{
   changed = value;
}

int  is_changed_set( void )
{
   return changed;
}

void reset_file( FILE *fptr )
{
   fseek( fptr, 0L, SEEK_SET );
}

int get_observation( FILE *fptr, observation *obs )
{
   int  legs;
   int  ret = 0;

   bzero( obs, sizeof( observation ) );

   if ( !feof( fptr ) )
   {
      ret = fscanf( fptr, "%[^,],%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n",
                       obs->name, &obs->hair, &obs->feathers, &obs->eggs, &obs->milk,
                       &obs->airborne, &obs->aquatic, &obs->predator, &obs->toothed,
                       &obs->backbone, &obs->breathes, &obs->venomous, &obs->fins,
                       &legs, &obs->tail, &obs->domestic, &obs->catsize, &obs->class );

      if ( ret == 18 ) 
      {
         switch( legs )
         {
            case 0:
               obs->legs_0 = 1;
               break;
            case 2:
               obs->legs_2 = 1;
               break;
            case 4:
               obs->legs_4 = 1;
               break;
            case 5:
               obs->legs_5 = 1;
               break;
            case 6:
               obs->legs_6 = 1;
               break;
            case 8:
               obs->legs_8 = 1;
               break;
         }
         ret = 1;
      }
      else ret = 0;

   }

   return ret;
}

void usage( char* progname )
{
   fprintf( stderr, "Usage: %s -t <testdata> -v <validation> "
                    " -o <outputfile> -a <vq | art>\n", progname );
   return;
}

int main( int argc, char *argv[] )
{
   int opt;
   FILE *ftest, *fvalid, *fout;
   int art = 0, vq = 0;
   long iterations = 1000L;

   srand( time( NULL ) );

   if ( argc == 1 )
   {
      usage( argv[0] );
      return -1;
   }

   while ( ( opt = getopt( argc, argv, "t:v:o:a:i:" ) ) != -1 )
   {
      switch( opt )
      {
         case 't': // Test Data filename
            strcpy( testDataFilename, optarg );
            break;

         case 'v': // Validation filename
            strcpy( validationFilename, optarg );
            break;

         case 'o': // Output filename
            strcpy( outputFilename, optarg );
            break;

         case 'a': // Algorithm
            if ( !strncmp( optarg, "vq", 2 ) )
            {
               vq = 1;
               break;
            }
            else if ( !strncmp( optarg, "art", 3 ) )
            {
               art = 1;
               break;
            }

         case 'i': // Iterations
            iterations = atol( optarg );
            break;

         default:
            usage( argv[0] );
            return -1;
            break;
      }
   }

   if ( vq == 0 && art == 0 )
   {
      usage( argv[ 0 ] );
      return -1;
   }

   // Open file pointers
   ftest = fopen( testDataFilename, "r" );
   if ( ftest == ( FILE * )0 )
   {
      fprintf( stderr, "Could not open test data file %s.\n", testDataFilename );
      return -1;
   }

   fvalid = fopen( validationFilename, "r" );

   fout = fopen( outputFilename, "w" );
   if ( fout == ( FILE * )0 )
   {
      fprintf( stderr, "Could not open output file %s.\n", outputFilename );
      fclose( ftest );
      fclose( fvalid );
      return -1;
   }

   if ( vq )
   {
      vq_initialize( );

      vq_train( ftest, iterations );

      vq_validate( ftest, fout );

      if ( fvalid != ( FILE * ) 0)
      {
         vq_validate( fvalid, fout );
      }
   }
   else if ( art )
   {
      // Initialize and ingest the dataset into memory.
      art_initialize( ftest );

      art_train( fout );

      if ( fvalid != ( FILE * ) 0)
      {
         art_validate( fvalid, fout );
      }
   }

   // Close file pointers
   fclose( ftest );
   if ( fvalid != ( FILE * )0) fclose( fvalid );
   fclose( fout );

   return 0;
}

