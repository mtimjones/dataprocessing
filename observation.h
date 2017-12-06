// vq.h

#define MAX_NAME      40

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

} observation;



