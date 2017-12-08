// vq.c
#include <stdio.h>
#include <stdlib.h>
#include "learn.h"

typedef struct feature
{
   int features[ MAX_FEATURES ];
   int class;
} feature;

#define OUTPUTS           7
#define MAX_FEAT_VECS   200

#define RATE          0.01

static double  outputs[ OUTPUTS ];
static double  weights[ OUTPUTS ][ MAX_FEATURES ];
static int     input_feature_vector[ MAX_FEATURES ];

void vq_initialize( int num_features )
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

   return 0;
}


void vq_updateweights( observation *obs, int class )
{
   for ( int weight = 0 ; weight < MAX_FEATURES ; weight++ )
   {
//      weights[ class ][ weight ] += RATE *

   }

   obs->computed_class = class;

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

void vq_train( FILE *fptr )
{
   int result;
   observation obs;

   // Initialize the first N observations to the N classes.
   for ( int class = 0 ; class < OUTPUTS ; class++ )
   {
      vq_set_input_vector( &obs );
      result = get_observation( fptr, &obs );
      vq_updateweights( &obs, class );
   }

   set_changed( 1 );

   while ( is_changed_set( ) )
   {
      set_changed( 0 );

      result = get_observation( fptr, &obs );

      if ( !result )
      {
         if ( !is_changed_set( ) )
         {
            // We've gone through all samples with no cluster changes.
            return;
         }

         // Reset the file position to the beginning.
         fseek( fptr, 0L, SEEK_SET );
      }
      else
      {
         vq_set_input_vector( &obs );
         int class = vq_feedforward( );
         vq_updateweights( &obs, class );
      }

   }

}
