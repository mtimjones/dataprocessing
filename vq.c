// vq.c
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "learn.h"

typedef struct feature
{
   int features[ MAX_FEATURES ];
   int class;
} feature;

#define OUTPUTS           7
#define MAX_FEAT_VECS   200

#define RATE            0.01

#define SQR( x )        ( ( x ) * ( x ) )

static double  outputs[ OUTPUTS ];
static double  weights[ OUTPUTS ][ MAX_FEATURES ];
static int     input_feature_vector[ MAX_FEATURES ];

#define NORMALIZE( x )    ( ( ( double )x + 1.0 ) / 2.0 )

double distance( double x, double y )
{
   return sqrt( SQR( ( x - y ) ) );
}

void vq_initialize( void )
{
   int output, feature;

   // Initialize weights
   for ( output = 0 ; output < OUTPUTS ; output++ )
   {
      outputs[ output ] = 0.0;

      for ( feature = 0 ; feature < MAX_FEATURES ; feature++ )
      {
         weights[ output ][ feature ] = 0.0;
      }
   }

   return;
}


int vq_feedforward( void )
{
   int best;
   double best_value;

   // Given the current input feature vector, compute each output node.
   for ( int output = 0 ; output < OUTPUTS ; output++ )
   {
      outputs[ output ] = 0.0;
      for ( int feature = 0 ; feature < MAX_FEATURES ; feature++ )
      {
         outputs[ output ] += 
            distance( NORMALIZE( weights[ output ][ feature ] ) , input_feature_vector[ feature ] );
      }

      // Keep track of the best activation
      if ( output == 0 )
      {
         best = 0;
         best_value = outputs[ output ];
      }
      else
      {
         if ( outputs[ output ] < best_value )
         {
            best = output;
            best_value = outputs[ output ];
         }
      }
   }

   return best;
}


void vq_updateweights( int class )
{
   for ( int feature = 0 ; feature < MAX_FEATURES ; feature++ )
   {
      weights[ class ][ feature ] += 
         RATE * ( (double) NORMALIZE( input_feature_vector[ feature ] ) - 
                           weights[ class ][ feature ] );
   }

   return;
}

void vq_set_input_vector( observation *obs )
{
   input_feature_vector[ 0 ] = obs->hair;
   input_feature_vector[ 1 ] = obs->feathers;
   input_feature_vector[ 2 ] = obs->eggs;
   input_feature_vector[ 3 ] = obs->milk;
   input_feature_vector[ 4 ] = obs->airborne;
   input_feature_vector[ 5 ] = obs->aquatic;
   input_feature_vector[ 6 ] = obs->predator;
   input_feature_vector[ 7 ] = obs->toothed;
   input_feature_vector[ 8 ] = obs->backbone;
   input_feature_vector[ 9 ] = obs->breathes;
   input_feature_vector[ 10 ] = obs->venomous;
   input_feature_vector[ 11 ] = obs->fins;
   input_feature_vector[ 12 ] = obs->legs_0;
   input_feature_vector[ 13 ] = obs->legs_2;
   input_feature_vector[ 14 ] = obs->legs_4;
   input_feature_vector[ 15 ] = obs->legs_5;
   input_feature_vector[ 16 ] = obs->legs_6;
   input_feature_vector[ 17 ] = obs->legs_8;
   input_feature_vector[ 18 ] = obs->tail;
   input_feature_vector[ 19 ] = obs->domestic;
   input_feature_vector[ 20 ] = obs->catsize;

   return;
}

void vq_train( FILE *fptr, long iterations )
{
   int result;
   observation obs;
   long iteration = 0;

   // Initialize the first N observations to the N classes.
   for ( int class = 0 ; class < OUTPUTS ; class++ )
   {
      result = get_observation( fptr, &obs );
      vq_set_input_vector( &obs );
      vq_updateweights( class );
   }

   while ( iteration < iterations )
   {
      result = get_observation( fptr, &obs );

      if ( !result )
      {
         // Reset the file position to the beginning.
         fseek( fptr, 0L, SEEK_SET );
         iteration++;
      }
      else
      {
         vq_set_input_vector( &obs );
         int class = vq_feedforward( );
         vq_updateweights( class );
      }
   }

   return;
}


void vq_validate( FILE *fptr, FILE *fout )
{
   int result;
   observation obs;

   // Reset the input file
   fseek( fptr, 0L, SEEK_SET );

   while ( 1 )
   {
      result = get_observation( fptr, &obs );

      if ( !result )
      {
         break;
      }
      else 
      {
         vq_set_input_vector( &obs );
         int class = vq_feedforward( );
         fprintf( fout, "%s,%d\n", obs.name, class );
      }
   }

   return;
}

