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
static feature feature_vectors[ MAX_FEAT_VECS ];

void vq_initialize( int num_features )
{
   int vector, output, feature;

   for ( vector = 0 ; vector < MAX_FEAT_VECS ; vector++ )
   {
      for ( feature = 0 ; feature < MAX_FEATURES ; feature++ )
      {
         feature_vectors[ vector ].features[ feature ] = getRand( MAX_FEATURES );
      }
   }

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


int vq_feedforward( observation *obs )
{

   return 0;
}


void vq_updateweights( observation *obs, int clas )
{
   return;
}


void vq_train( FILE *fptr )
{
   int result;
   observation obs;

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
      }
      else
      {
         int class = vq_feedforward( &obs );
         vq_updateweights( &obs, class );
      }

   }

}
