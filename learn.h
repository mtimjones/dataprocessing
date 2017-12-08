// learn.h

#define getSRand()      ( ( float ) rand( ) / ( float ) RAND_MAX )
#define getRand( x )    ( int ) ( ( x ) * getSRand( ) )
#define prob( x )       ( getSRand( ) > x )

#define MAX_NAME      40

#define MAX_FEATURES  21

typedef struct observation
{
   char name[ MAX_NAME ];

   // 21 Features
   int  hair;
   int  feathers;
   int  eggs;
   int  milk;
   int  airborne;
   int  aquatic;
   int  predator;
   int  toothed;
   int  backbone;
   int  breathes;
   int  venomous;
   int  fins;
   int  legs_0;
   int  legs_2;
   int  legs_4;
   int  legs_5;
   int  legs_6;
   int  legs_8;
   int  tail;
   int  domestic;
   int  catsize;
   
   int  class;

   int  computed_class;

} observation;

void set_changed( int );
int is_changed_set( void );
int get_observation( FILE*, observation* );

