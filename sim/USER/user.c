/*****************************************************************************/
/* File:        user.c (Khepera Simulator)                                   */
/* Author:      Olivier MICHEL <om@alto.unice.fr>                            */
/* Date:        Thu Sep 21 14:39:05 1995                                     */
/* Description: example of user.c file                                       */
/*                                                                           */
/* Copyright (c) 1995                                                        */
/* Olivier MICHEL                                                            */
/* MAGE team, i3S laboratory,                                                */
/* CNRS, University of Nice - Sophia Antipolis, FRANCE                       */
/*                                                                           */
/* Permission is hereby granted to copy this package for free distribution.  */
/* The author's name and this copyright notice must be included in any copy. */
/* Commercial use is forbidden.                                              */
/*****************************************************************************/

#include "../SRC/include.h"
#include "user_info.h"
#include "user.h"

#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/timeb.h> 


// Initializing the shared memeoy /////
typedef struct memspace{
   short int comm_flag;
   short int IR_Distance[6];
   short int Motor_Value[2];
} MemSpace;

MemSpace robot_values, *sharedMem;

int shmID;
key_t key;
///////////////////////////////////////

double waiting=0.01;

static int steps;
static double old_x, old_y;
static double len;

double weights[ 7 ] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 6 };

int msg_waiter = 0;



void UserInit(struct Robot *robot)
{
   steps = 0; len = 0;
   old_x = robot->X; old_y = robot->Y;
   ShowUserInfo(1,1);

   // creating and opening the shared memory ////
   key = 42069;

   if ((shmID = shmget(key, 2*sizeof(MemSpace), IPC_CREAT | 0666)) == -1){
      perror("shmget");
      exit(1);
   }

   if ((sharedMem = (MemSpace *) shmat(shmID, NULL, 0)) == (MemSpace *) -1){
      perror("shmat");
      exit(1);
   }

   sharedMem->comm_flag = 0;
   sharedMem->Motor_Value[0] = 6;
   sharedMem->Motor_Value[1] = 6;
   //////////////////////////////////////////////
}

void UserClose(struct Robot *robot)
{
   // closing the shared memory /////////////////
   // (MemSpace *) shmdt(robot_values.p);
   //////////////////////////////////////////////
}

void NewRobot(struct Robot *robot)
{
   steps = 0; len = 0;
   old_x = robot->X; old_y = robot->Y;
}

void LoadRobot(struct Robot *robot,FILE *file)
{
   int i;
   double d[ 7 ];
   for( i = 0; i < 7; i++ )
      if ( fscanf( file, "%lf", d + i ) < 1 )
      {
	 Color( RED );
         DrawText( 10, 50, "Sorry, not enought values           " );
	 EraseComment();
	 return ;
      }
   for( i = 0; i < 7; i++ )
      weights[ i ] = d[ i ];
   steps = 0; len = 0;
   old_x = robot->X; old_y = robot->Y;
   ShowUserInfo(1,1);
}

void SaveRobot(struct Robot *robot,FILE *file)
{
   int i;
   for( i = 0; i < 7; i++ )
      fprintf( file, "%f ", weights[ i ] );
   fprintf( file, "\n" );
}

void RunRobotStart(struct Robot *robot)
{
   // steps = 0; len = 0;
   // old_x = robot->X; old_y = robot->Y;
   ShowUserInfo(1,1);
}

void RunRobotStop(struct Robot *robot)
{
}

static int slowmode()
       {

  	  struct timeb timebuffer;
  	  double time1, time2;

  	  ftime(&timebuffer);
  	  time1 = (double)timebuffer.time + (double)timebuffer.millitm / 1000;
  	  ftime(&timebuffer);
  	  time2 = (double)timebuffer.time + (double)timebuffer.millitm / 1000;
  	  while( (time2 - time1) < waiting )
  	  {
    	  ftime(&timebuffer);
    	  time2 = (double)timebuffer.time + (double)timebuffer.millitm / 1000;
  	  }
       }

boolean StepRobot(struct Robot *robot)
{
  int i;
  double v, sqrt();
  char text[ 100 ];

  for( i = 0, v = 0; i < 6; i++ )
     v += robot->IRSensor[ i ].DistanceValue * weights[ i ];
  v /= 1024.0;
//   robot->Motor[ LEFT ].Value = v + weights[ 6 ];
  for( i = 0, v = 0; i < 6; i++ )
     v += robot->IRSensor[ i ].DistanceValue * weights[ 5 - i ];
  v /= 1024.0;
//   robot->Motor[ RIGHT ].Value = v + weights[ 6 ];

  len += sqrt( (robot->X - old_x) * (robot->X - old_x) + 
  	       (robot->Y - old_y) * (robot->Y - old_y) );
  old_x = robot->X; old_y = robot->Y;
  steps++;
  if ( steps % 5 == 1 )
  {
     sprintf( text, "avg speed: %10.5f", (steps == 0)? 0.0: len/steps );
     Color( GREY_69 );
     FillRectangle( 80, 230, 150, 20 );
     Color( BLACK );
     Color( BLUE );
     DrawText( 10, 250, "avg speed:              " );
     DrawText( 10, 250, text );
     // ShowUserInfo(1,1);
  }


   // MY STUFF /////////////////////////////////////////////////////////////////
      while(sharedMem->comm_flag == 1){sleep(0.0001);}
         for(int i=0; i<6; i++)
            sharedMem->IR_Distance[i] = robot->IRSensor[i].DistanceValue;
            
         sharedMem->comm_flag = 1;

      while(sharedMem->comm_flag == 1){sleep(0.0001);}
         robot->Motor[LEFT].Value  = sharedMem->Motor_Value[0];
         robot->Motor[RIGHT].Value = sharedMem->Motor_Value[1];
   // END MY STUFF /////////////////////////////////////////////////////////////

  slowmode();
  return(TRUE);
}

void FastStepRobot(struct Robot *robot)
{
}

void ResetRobot(struct Robot *robot)
{
}

void UserCommand(struct Robot *robot,char *text)
{
   int r, s, i;
   double d;
   r = sscanf( text, "set %ld %lf", & i, & d );
   if ( r == 2 )
      if ( i >= 0 && i < 7 )
         weights[ i ] = d;
      
   ShowUserInfo(1,1);
}

void DrawUserInfo(struct Robot *robot,u_char info,u_char page)
{
  char text[256];
  if ( info == 1 && page == 1 )
  {
     DrawText( 10, 20, "A simple Braitenberg Architecture" );
     DrawText( 10, 35, "Current connections:" );
     DrawText( 10, 70, "          0      1      2      3      4      5      6" );
     DrawText( 10, 85, "-------------------------------------------------------" );
     sprintf( text, "left : %6.2f %6.2f %6.2f %6.2f %6.2f %6.2f %6.2f",
     		weights[ 0 ], weights[ 1 ], weights[ 2 ], weights[ 3 ],
     		weights[ 4 ], weights[ 5 ], weights[ 6 ] );
     DrawText( 10, 100, text );
     sprintf( text, "right: %6.2f %6.2f %6.2f %6.2f %6.2f %6.2f %6.2f",
     		weights[ 5 ], weights[ 4 ], weights[ 3 ], weights[ 2 ],
     		weights[ 1 ], weights[ 0 ], weights[ 6 ] );
     DrawText( 10, 115, text );
     DrawText( 10, 150, "Change connection: press the command button and type:" );
     DrawText( 10, 175, "                   set number value" );
     DrawText( 10, 200, "       with number representing the connection number" );
     DrawText( 10, 215, "       and value representing a floating-point value" );

     sprintf( text, "avg speed: %10.5f", (steps == 0)? 0.0: len/steps );
     // DrawText( 10, 250, "avg speed:              " );
     Color( BLUE );
     DrawText( 10, 250, text );
     Color( BLACK );
  }

}



