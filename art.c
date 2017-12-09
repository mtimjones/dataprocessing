// art.c
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "learn.h"

#define CLUSTERS              9

#define MAX_FEAT_VECS    200

typedef struct vector
{
   char name[ MAX_NAME ];
   int  features[ MAX_FEATURES ];
   int  count;    // Prototype cluster count
   int  cluster;  // Feature vector cluster
   int  actual_class;
} vector;

// In-memory observations (feature vectors)
vector feature_vectors[ MAX_FEAT_VECS ];
int    max_feature_vectors;

// Prototype feature vectors
vector clusters[ CLUSTERS ];


void art_translate_observation( int index, observation *obs )
{
   strcpy( feature_vectors[ index ].name, obs->name );
   feature_vectors[ index ].features[ 0] = obs->hair;
   feature_vectors[ index ].features[ 1] = obs->feathers;
   feature_vectors[ index ].features[ 2] = obs->eggs;
   feature_vectors[ index ].features[ 3] = obs->milk;
   feature_vectors[ index ].features[ 4] = obs->airborne;
   feature_vectors[ index ].features[ 5] = obs->aquatic;
   feature_vectors[ index ].features[ 6] = obs->predator;
   feature_vectors[ index ].features[ 7] = obs->toothed;
   feature_vectors[ index ].features[ 8] = obs->backbone;
   feature_vectors[ index ].features[ 9] = obs->breathes;
   feature_vectors[ index ].features[10] = obs->venomous;
   feature_vectors[ index ].features[11] = obs->fins;
   feature_vectors[ index ].features[12] = obs->legs_0;
   feature_vectors[ index ].features[13] = obs->legs_2;
   feature_vectors[ index ].features[14] = obs->legs_4;
   feature_vectors[ index ].features[15] = obs->legs_5;
   feature_vectors[ index ].features[16] = obs->legs_6;
   feature_vectors[ index ].features[17] = obs->legs_8;
   feature_vectors[ index ].features[18] = obs->tail;
   feature_vectors[ index ].features[19] = obs->domestic;
   feature_vectors[ index ].features[20] = obs->catsize;
   feature_vectors[ index ].actual_class = obs->class;

   return;
}



void art_initialize( FILE *fptr )
{
   int result;
   observation obs;

   memset( ( void * )feature_vectors, 0, sizeof( feature_vectors ) );
   max_feature_vectors = 0;
   memset( ( void * )clusters, 0, sizeof( clusters ) );

   // Ingest the feature vectors
   while ( ( result = get_observation( fptr, &obs ) ) )
   {
      art_translate_observation( max_feature_vectors, &obs );
      max_feature_vectors++;
   }

   printf( "max feat vecs = %d\n", max_feature_vectors );

   return;
}
