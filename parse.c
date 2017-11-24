// parse.c

#include <stdio.h>
#include <string.h>
#include "symtypes.h"

unsigned int parseLine( working_t *working, char *schematype )
{
   unsigned int element;
   char token[LEN];
   int tokpos, linepos=0;

   for ( element = 0 ; element < MAX_ELEMENTS ; element++ )
   {
      if ( working->line[linepos] == 0 ) break;

      // Grab the next token
      tokpos = 0;
      while ( working->line[linepos] != 0 && working->line[linepos] != ',' &&
              working->line[linepos] != 10 && working->line[linepos] != 13 )
      {
         token[tokpos++] = working->line[linepos++];
      }
      token[tokpos] = 0;

      switch( schematype[ element ] )
      {
         case 's':
            strcpy( working->parsedLine[ element ].u.s, token );
            working->parsedLine[ element ].type = STRING;
            break;

         case 'g':
            if ( sscanf( token, "%lg", &working->parsedLine[ element ].u.g ) <= 0 )
            {
               working->errorField = element;
               working->error = 1;
            }
            else
            {
               working->parsedLine[ element ].type = DOUBLE;
            }
            break;

         case 'd':
            if ( sscanf( token, "%d", &working->parsedLine[ element ].u.d ) <= 0 )
            {
               working->errorField = element;
               working->error = 1;
            }
            else
            {
               working->parsedLine[ element ].type = INT;
            }
            break;

         default:
            working->errorField = element;
            working->error = 1;
            break;
      }

      if ( !working->error )
      {
         AddElementToSummary( element, &working->parsedLine[ element ] );
      }

      if ( working->line[linepos-1] == 0 ) break;
      else linepos++;

   }

   if ( element != strlen( schematype ) )
   {
      working->error = 1;
   }

   return ( element-1 );
}

