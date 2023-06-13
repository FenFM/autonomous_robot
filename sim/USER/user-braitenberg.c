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

#include <sys/timeb.h>
double waiting=0.01;

static int steps;
static double old_x, old_y;
static double len;

static double weights[ 7 ] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 6 };

void UserInit(struct Robot *robot)
{
   steps = 0; len = 0;
   old_x = robot->X; old_y = robot->Y;
   UserCommand(robot, 0 );
   ShowUserInfo(1,1);
}

void UserClose(struct Robot *robot)
{
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
  robot->Motor[ LEFT ].Value = v + weights[ 6 ];
  for( i = 0, v = 0; i < 6; i++ )
     v += robot->IRSensor[ i ].DistanceValue * weights[ 5 - i ];
  v /= 1024.0;
  robot->Motor[ RIGHT ].Value = v + weights[ 6 ];

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

  slowmode();
  return(TRUE);
}

void FastStepRobot(struct Robot *robot)
{
}

void ResetRobot(struct Robot *robot)
{
}

static double demo[ 4 ][ 7 ] = {
		{   0.0, 0.0, 0.0,   0.0,   0.0,   0.0, 6 },
		{   0.0, 0.0, 3.0, -10.0,   0.0,   0.0, 6 },
		{ -30.0, 0.0, 3.0,  20.0,   0.0,  10.0, 1 },
		{   4.0, 4.0, 6.0, -15.0, -10.0, -10.0, 6 }
			};

void UserCommand(struct Robot *robot,char *text)
{
   static int stage = -1;
   int i;
   stage = (stage + 1) % 4;
   for( i = 0; i < 7; i++ )
      weights[ i ] = demo[ stage ][ i ];
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



