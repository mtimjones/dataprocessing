// parse.c

#include <stdio.h>
#include <string.h>
#include <linux/limits.h>
#include "symtypes.h"

unsigned int parseLine( working_t *working, char *schematype )
{
   unsigned int element;
   char token[LEN];
   int tokpos, linepos=0;

//printf( "In parseLine with %s", working->line );

   for ( element = 0 ; element < MAX_ELEMENTS ; element++ )
   {
      tokpos = 0;

      if ( working->line[linepos] == 0 ) break;

      // Grab the next token
      while ( working->line[linepos] != 0 && working->line[linepos] != ',' )
      {
         token[tokpos++] = working->line[linepos++];
      }
      token[tokpos] = 0;

      switch( schematype[ element ] )
      {
         case 's':
            strcpy( working->parsedLine[ element ].u.s, token );
//printf("%s ", working->parsedLine[ element ].u.s );
            working->parsedLine[ element ].type = STRING;
            break;

         case 'g':
            if ( sscanf( token, "%lg", &working->parsedLine[ element ].u.g ) == 0 )
            {
               working->errorField = element;
               working->error = 1;
            }
            else
            {
//printf("%g ", working->parsedLine[ element ].u.g );
               working->parsedLine[ element ].type = DOUBLE;
            }
            break;

         case 'd':
            if ( sscanf( token, "%d", &working->parsedLine[ element ].u.d ) == 0 )
            {
               working->errorField = element;
               working->error = 1;
            }
            else
            {
//printf("%d ", working->parsedLine[ element ].u.d );
               working->parsedLine[ element ].type = INT;
            }
            break;

         default:
            working->errorField = element;
            working->error = 1;
            break;
      }


//printf("\n");

      if ( working->line[linepos-1] == 0 ) break;
      else linepos++;

   }

   if ( element != strlen( schematype ) )
   {
      working->error = 1;
   }

   return ( element-1 );
}

void emit_line( FILE *outptr )
{

}
