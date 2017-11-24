#include <stdio.h>
#include <string.h>
#include "symtypes.h"

#define NUM_HISTORY   10

element_t Elements[ MAX_ELEMENTS ][ NUM_HISTORY ];

#define VMIN  0
#define VMAX  1
#define VCNT  2
#define VSUM  3
#define VTYP  VMIN

// For INT/DOUBLE, 0 = max, 1 = min, 2 = count, 3 = sum
// For STRING, 0..N-1 is the unique values.

void InitializeSummary( void )
{
   memset( Elements, 0, sizeof( Elements ) );

   return;
}

void AddElementToSummary( unsigned int element, element_t *elemobj )
{
   switch( elemobj->type )
   {
      case INT:
         if ( Elements[ element ][ VTYP ].type == 0 )
         {
            // If we have no type, then initialize
            Elements[ element ][ VTYP ].type = INT;
            Elements[ element ][ VMAX ].u.d = INT_MIN;
            Elements[ element ][ VMIN ].u.d = INT_MAX;
         }

         if ( Elements[ element ][ VMAX ].u.d < elemobj->u.d )
         {
            // Update max value.
            Elements[ element ][ VMAX ].u.d = elemobj->u.d;
         }
         else if ( Elements[ element ][ VMIN ].u.d > elemobj->u.d )
         {
            // Update min value.
            Elements[ element ][ VMIN ].u.d = elemobj->u.d;
         }

         // Update count and sum.
         Elements[ element ][ VCNT ].u.d++;
         Elements[ element ][ VSUM ].u.d += elemobj->u.d;

         break;

      case DOUBLE:
         if ( Elements[ element ][ VTYP ].type == 0 )
         {
            // If we have no type, then initialize
            Elements[ element ][ VTYP ].type = DOUBLE;
            Elements[ element ][ VMAX ].u.g = DBL_MIN;
            Elements[ element ][ VMIN ].u.g = DBL_MAX;
         }

         if ( Elements[ element ][ VMAX ].u.g < elemobj->u.g )
         {
            // Update max value.
            Elements[ element ][ VMAX ].u.g = elemobj->u.g;
         }
         else if ( Elements[ element ][ VMIN ].u.g > elemobj->u.g )
         {
            // Update min value.
            Elements[ element ][ VMIN ].u.g = elemobj->u.g;
         }

         // Update count and sum.
         Elements[ element ][ VCNT ].u.g += 1.0;
         Elements[ element ][ VSUM ].u.g += elemobj->u.g;

         break;

      case STRING:

         for ( int i = 0 ; i < NUM_HISTORY ; i++ )
         {
            if ( Elements[ element ][ i ].type == STRING )
            {
               // Compare
               if ( !strcmp( Elements[ element ][ i ].u.s, elemobj->u.s ) )
               {
                  // Found duplicate, exit.
                  return;
               }
            }
            else
            {
               // Found empty slot, add.
               memcpy( &Elements[ element ][ i ], elemobj, sizeof( element_t ) );
               Elements[ element ][ i ].type = STRING;
               return;
            }
         }

         break;

   }

   return;
}


void Summarize( void )
{
   printf("Data Profile:\n\n");

   for ( int i = 0 ; i < MAX_ELEMENTS ; i++ )
   {
      if ( Elements[ i ][ VTYP ].type == INT )
      {
         printf("Field %d: (INT)\n", i );
         printf("     Min: %d\n", Elements[ i ][ VMIN ].u.d );
         printf("     Max: %d\n", Elements[ i ][ VMAX ].u.d );
         printf("     Avg: %g\n", 
            ( double ) Elements[ i ][ VSUM ].u.d / ( double ) Elements[ i ][ VCNT ].u.d );
         printf("\n");
      }
      else if ( Elements[ i ][ VTYP ].type == DOUBLE )
      {
         printf("Field %d: (DOUBLE)\n", i );
         printf("     Min: %g\n", Elements[ i ][ VMIN ].u.g );
         printf("     Max: %g\n", Elements[ i ][ VMAX ].u.g );
         printf("     Avg: %g\n", Elements[ i ][ VSUM ].u.g / Elements[ i ][ VCNT ].u.g );
         printf("\n");
      }
      else if ( Elements[ i ][ VTYP ].type == STRING )
      {
         int j;
         printf("Field %d: (STRING)\n", i );
         for ( j = 0 ; j < NUM_HISTORY ; j++ )
         {
            if ( Elements[ i ][ j ].type == STRING )
            {
               printf( "     %s\n", Elements[ i ][ j ].u.s );
            }
            else break;
         }
         if ( j == NUM_HISTORY ) 
         {
            printf( "     ...\n" );
         }
         printf("\n");
      }
   }

   return;
}
