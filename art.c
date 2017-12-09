// art.c
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "learn.h"

#define CLUSTERS           9

#define MAX_FEAT_VECS    200

#define BETA             ( double )4.0
#define RHO              ( double )0.3

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

   // Indicate that this feature has not been clustered.
   feature_vectors[ index ].cluster = CLUSTERS;

   return;
}

int vMagnitude( vector *x )
{
  int sum=0;

  // Count the '1's in the feature vector.
  for (int i = 0 ; i < MAX_FEATURES ; i++) {
    sum += x->features[ i ];
  }

  return sum;
}

void vAnd( vector *result, vector *x, vector *y )
{
  // Boolean AND operation on two vectors.
  for (int feature = 0 ; feature < MAX_FEATURES ; feature++) {
    result->features[ feature ] = x->features[ feature ] & y->features[ feature ];
  }

  return;
}

void recalculate_cluster( int cluster )
{
   int first = 0;

   for ( int vec = 0 ; vec < max_feature_vectors ; vec++ )
   {
      if ( feature_vectors[ vec ].cluster == cluster )
      {
         if ( !first )
         {
            first = 1;

            // Set the cluster to the first feature vector for this cluster.
            for ( int feature = 0 ; feature < MAX_FEATURES ; feature++ )
            {
               clusters[ cluster ].features[ feature ] =
                  feature_vectors[ vec ].features[ feature ];
            }
         }
         else
         {
            // Boolean AND the next feature vectors into the cluster.
            for ( int feature = 0 ; feature < MAX_FEATURES ; feature++ )
            {
               clusters[ cluster ].features[ feature ] &=
                  feature_vectors[ vec ].features[ feature ];
            }
         }
      }
   }

   return;
}

int find_empty_cluster( void )
{
   for ( int cluster = 0 ; cluster < CLUSTERS ; cluster++ )
   {
      if ( clusters[ cluster ].count == 0 )
      {
         return cluster;
      }
   }

   return CLUSTERS;
}

void cluster_create( int feature )
{
   feature_vectors[ feature ].cluster = find_empty_cluster( );

   if ( feature_vectors[ feature ].cluster != CLUSTERS )
   {
      for ( int i = 0 ; i < MAX_FEATURES ; i++ )
      {
         clusters[ feature_vectors[ feature ].cluster ].features[ i ] = 
            feature_vectors[ feature ].features[ i ];
      }
      clusters[ feature_vectors[ feature ].cluster ].count = 1;
printf("Feature %d added to new cluster %d\n", feature, feature_vectors[feature].cluster );
   }

   return;
}

void cluster_add( int cluster, int vector )
{
   // If the current feature vector has been classified, pull it out.
   if ( feature_vectors[ vector ].cluster != CLUSTERS )
   {
      int prior_cluster = feature_vectors[ vector ].cluster;
      feature_vectors[ vector ].cluster = CLUSTERS;
      clusters[ prior_cluster ].count--;
      recalculate_cluster( prior_cluster );
   }

   // Add the feature vector to the new cluster.
   feature_vectors[ vector ].cluster = cluster;
   clusters[ cluster ].count++;
   recalculate_cluster( cluster );

   return;
}

void cluster_debug( void )
{
   for ( int cluster = 0 ; cluster < CLUSTERS ; cluster++ )
   {
      printf("Cluster %d\n", cluster );

      for ( int feature = 0 ; feature < max_feature_vectors ; feature++ )
      {
         if ( feature_vectors[ feature ].cluster == cluster )
         {
            printf( "%s (%d)\n", 
                     feature_vectors[ feature ].name,
                     feature_vectors[ feature ].actual_class );
         }
      }
      printf("\n");
   }

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

   return;
}

#if 0
int cluster_observation( int feature )
{
   vector result;
   int cluster;

   for ( cluster = 0 ; cluster < CLUSTERS ; cluster++ )
   {
      // If a cluster has no members, skip it.
      if ( clusters[ cluster ].count == 0 ) continue;

      // result = feature vector vAnd cluster vector
      vAnd( &result, &feature_vectors[ feature ], &clusters[ cluster ] );

      // Compute the magnitudes (1 counts ).
      double resultMag = ( double )vMagnitude( &result );
      double featureMag = ( double )vMagnitude( &feature_vectors[ feature ] );
      double clusterMag = ( double )vMagnitude( &clusters[ cluster ] );

      double maximum    = resultMag / ( BETA + clusterMag );
      double similarity = featureMag / ( BETA + ( double ) MAX_FEATURES );

//printf("ART: %g %g %g %g %g\n", resultMag, featureMag, clusterMag, maximum, similarity);

      // See if the feature vector is similar to the cluster
      if ( maximum > similarity )
      {
         if ( ( resultMag / clusterMag ) >= RHO )
         {
            return cluster;
         }
      }
   }

   return cluster;
}
#else
int cluster_observation( int feature )
{
   vector result;
   int cluster;
   double best_max = 0.0;
   int best_cluster = CLUSTERS;

   double featureMag = ( double )vMagnitude( &feature_vectors[ feature ] );

   for ( cluster = 0 ; cluster < CLUSTERS ; cluster++ )
   {
      // If a cluster has no members, skip it.
      if ( clusters[ cluster ].count == 0 ) continue;

      // result = feature vector vAnd cluster vector
      vAnd( &result, &feature_vectors[ feature ], &clusters[ cluster ] );
      double resultMag = ( double )vMagnitude( &result );

      double clusterMag = ( double )vMagnitude( &clusters[ cluster ] );

      double maximum = resultMag / ( BETA + clusterMag );

      if ( maximum > best_max )
      {
         best_max = maximum;
         best_cluster = cluster;
      }

   }

   if ( best_cluster != CLUSTERS )
   {
      vAnd( &result, &feature_vectors[ feature ], &clusters[ best_cluster ] );

      // Compute the magnitudes (1 counts ).
      double resultMag = ( double )vMagnitude( &result );
      double clusterMag = ( double )vMagnitude( &clusters[ best_cluster ] );

      double maximum    = resultMag / ( BETA + clusterMag );
      double similarity = featureMag / ( BETA + ( double ) MAX_FEATURES );

printf("ART: %g %g %g %g %g\n", resultMag, featureMag, clusterMag, maximum, similarity);

      // See if the feature vector is similar to the cluster
      if ( maximum > similarity )
      {
         if ( ( resultMag / clusterMag ) >= RHO )
         {
            return best_cluster;
         }
         else
         {
            best_cluster = CLUSTERS;
         }
      }
      else
      {
         best_cluster = CLUSTERS;
      }
   }

   return best_cluster;
}
#endif


void art_train( void )
{
   int changes = 1;
   int cluster;

   while ( changes )
   {
      changes = 0;

      for ( int feature = 0 ; feature < max_feature_vectors ; feature++ )
      {
         cluster = cluster_observation( feature );

         if ( cluster == CLUSTERS)
         {
            cluster_create( feature );
            changes++;
         }
         else
         {
            // If this feature vector has moved, move it.
            if ( feature_vectors[ feature ].cluster != cluster )
            {
               cluster_add( cluster, feature );
printf("Feature %d moved to cluster %d\n", feature, cluster );
               changes++;
            }
         }
      }
      printf("Changes %d\n", changes);
   }

   for ( int cluster = 0 ; cluster < CLUSTERS ; cluster++ )
   {
      printf("Cluster %d: Count %3d : [ ", cluster, clusters[cluster].count);
      for ( int feature = 0 ; feature < MAX_FEATURES ; feature++ )
      {
         printf("%1d ", clusters[cluster].features[feature]);
      }
      printf("]\n");
   }

   cluster_debug( );

   return;
}


void art_validate( FILE *fptr, FILE *fout )
{
  // use the get_observation API to grab an observation.
  // use max_feature_vector+1 to load the obwservation in for validation.

}

