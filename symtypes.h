// symtypes.h
#include <linux/limits.h>
#include <values.h>

#define INT       1
#define DOUBLE    2
#define STRING    3

#define LEN       80

typedef struct element_s
{
   unsigned int type;

   union
   {
      int    d;
      double g;
      char   s[LEN];
   } u;
} element_t;

#define MAX_ELEMENTS      20

typedef struct working_s
{
   char line[ NAME_MAX ];

   element_t parsedLine[ MAX_ELEMENTS ];

   int error;

   unsigned int errorField;

} working_t;

extern unsigned int parseLine( working_t *, char * );
extern void InitializeSummary( void );
extern void AddElementToSummary( unsigned int element, element_t *elemobj );
extern void Summarize( void );
