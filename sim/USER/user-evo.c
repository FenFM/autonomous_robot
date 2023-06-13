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
#include "inc/ea.h"
#include "inc/rand.h"

#include <sys/timeb.h>

#define ABS(a) ((a>0.0)?a:-a)

double waiting=0.01;

static int steps;
static double old_x, old_y;
static double len;

static double weights[ 7 ] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 6 };

int x,y,a;
long int proximity=0;
int generation=0, child=0;
double sigma = 0.01;
int mu=3, lambda=6, maxsteps=3000;
int strategy = PLUS;

void UserInit(struct Robot *robot)
{
  int i;

  steps = 0; len = 0;
  x = old_x = robot->X;
  y = old_y = robot->Y;
  a = 0.0;
  ShowUserInfo(1,1);
  make_populations(mu, lambda, 9);
  init_parents(-5.0, 5.0, sigma);
  set_parameter(PARENT, 0, 0, drand()/2.0-0.5);
  set_parameter(PARENT, 0, 1, drand()/2.0-0.5);
  set_parameter(PARENT, 0, 2, drand()/2.0-0.5);
  set_parameter(PARENT, 0, 3, drand()/2.0-0.5);
  set_parameter(PARENT, 0, 4, drand()/2.0-0.5);
  set_parameter(PARENT, 0, 5, drand()/2.0-0.5);
  set_parameter(PARENT, 0, 6, drand()/2.0-0.5);
  set_parameter(PARENT, 0, 7, drand()/2.0-0.5);
  set_parameter(PARENT, 0, 8, drand()/10.0);
  for (i=0; i<lambda; i++)
  {
    copy_objects(PARENT, 0, OFFSPRING, i);
    mutate_offspring(i);
  }
}

void UserClose(struct Robot *robot)
{
  destroy_populations();
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
  x = old_x = robot->X;
  y = old_y = robot->Y;
  a = robot->Alpha;
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

double calc_fitness(struct Robot *robot)
{
  double f, p=0.0, dv, v;
  int i;

  // max proximity
  for (i=0; i<8; i++)
    p = (p < robot->IRSensor[i].DistanceValue) ? robot->IRSensor[i].DistanceValue : p;
  p /= 1024.0;

  // speed
  v = (robot->Motor[LEFT].Value + robot->Motor[RIGHT].Value) / 2.0 / 40.0;

  // speed difference
  dv = (robot->Motor[LEFT].Value - robot->Motor[RIGHT].Value) / 40.0;
  dv = (dv<0.0) ? -dv : dv;

  // fitness
  f = v * (1.0-sqrt(dv)) * (1.0-p);
  printf ("%f\n", f);

  return f;
}

boolean StepRobot(struct Robot *robot)
{
  int i, j, n1, n2;
  double v, sqrt(), p;
  char text[ 100 ];
  static double f=0;

  len += sqrt( (robot->X - old_x) * (robot->X - old_x) +
               (robot->Y - old_y) * (robot->Y - old_y) );
  old_x = robot->X; old_y = robot->Y;

  if (steps<maxsteps)
  {
    if (steps!=0)
      f += calc_fitness(robot);

    for( i = 0, v = 0; i < 8; i++ )
      v += robot->IRSensor[ i ].DistanceValue/1024.0 * get_parameter((child==-1)?PARENT:OFFSPRING, (child==-1)?0:child, i);
    v +=  get_parameter((child==-1)?PARENT:OFFSPRING, (child==-1)?0:child, 8);
    v = (v<= 1.0) ? v :  1.0;
    v = (v>=-1.0) ? v : -1.0;
    robot->Motor[ LEFT ].Value = v * 40;

    for( i = 0, v = 0; i < 8; i++ )
      v += robot->IRSensor[ i ].DistanceValue/1024.0 * get_parameter((child==-1)?PARENT:OFFSPRING, (child==-1)?0:child, 7-i);
    v +=  get_parameter((child==-1)?PARENT:OFFSPRING, (child==-1)?0:child, 8);
    v = (v<= 1.0) ? v :  1.0;
    v = (v>=-1.0) ? v : -1.0;
    robot->Motor[ RIGHT ].Value = v * 40;
  }
  else
  {
    f += calc_fitness(robot);
    set_fitness((child==-1)?PARENT:OFFSPRING, (child==-1)?0:child, f);
    steps = 0;
    len = 0;
    f = 0;
    child++;
    robot->X = old_x = x;
    robot->Y = old_y = y;
    robot->Alpha = a;
    if (child>=lambda)
    {
      int better = 0;
      child = 0;

      for (f=0.0,i=0; i<mu; i++)
        f += get_fitness(PARENT, i)/mu;

      for (i=0; i<lambda; i++)
        if (f < get_fitness(OFFSPRING, i))
          better++;
      if (better > lambda/4.0)
        sigma *= 1.5;
      if (better < lambda/4.0)
        sigma /= 1.5;
      if (sigma>2.0)
        sigma=2.0;

      selection(strategy);

      // mutation only (1/3 of population)
      for (i=0; i<lambda/3; i++)
      {
        copy_objects(PARENT, n_choice(mu), OFFSPRING, i);
        set_sigma(OFFSPRING, i, sigma);
        mutate_offspring(i);
      }

      // uniform recombination
      for (i=lambda/3; i<lambda*2/3; i++)
      {
        n1 = n_choice(mu);
        while ((n2 = n_choice(mu)) == n1);

        for (j=0;j<9;j++)
          set_parameter(OFFSPRING, i, j, get_parameter(PARENT, (n_choice(2)?n1:n2), j));

        set_sigma(OFFSPRING, i, sigma);
        mutate_offspring(i);
      }

      // intermediate recombination
      for (i=lambda*2/3; i<lambda; i++)
      {
        n1 = n_choice(mu);
        while ((n2 = n_choice(mu)) == n1);

        for (j=0;j<9;j++)
        {
          double a;
          a = 0.25 + drand()*1.5;
          set_parameter(OFFSPRING, i, j, get_parameter(PARENT, n1, j) * a + get_parameter(PARENT, n2, j) * (1.0-a));
        }

        set_sigma(OFFSPRING, i, sigma);
        mutate_offspring(i);
      }
      generation++;
    }
  }


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

void UserCommand(struct Robot *robot,char *text)
{
   int r, i;
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
     DrawText( 10, 70, "  w1    w2    w3    w4    w5    w6    w7    w8     b" );
     DrawText( 10, 85, "-------------------------------------------------------" );
     if (steps!=0)
     {
       sprintf( text, "%5.2f %5.2f %5.2f %5.2f %5.2f %5.2f %5.2f %5.2f %5.2f %5.2f",
     		get_parameter((child==-1)?PARENT:OFFSPRING, (child==-1)?0:child, 0),
                get_parameter((child==-1)?PARENT:OFFSPRING, (child==-1)?0:child, 1),
                get_parameter((child==-1)?PARENT:OFFSPRING, (child==-1)?0:child, 2),
                get_parameter((child==-1)?PARENT:OFFSPRING, (child==-1)?0:child, 3),
                get_parameter((child==-1)?PARENT:OFFSPRING, (child==-1)?0:child, 4),
                get_parameter((child==-1)?PARENT:OFFSPRING, (child==-1)?0:child, 5),
                get_parameter((child==-1)?PARENT:OFFSPRING, (child==-1)?0:child, 6),
                get_parameter((child==-1)?PARENT:OFFSPRING, (child==-1)?0:child, 7),
                get_parameter((child==-1)?PARENT:OFFSPRING, (child==-1)?0:child, 8));
       DrawText( 10, 100, text );

       sprintf( text, "generation: %3d, child: %2d, steps: %4d, sigma: %7.5f", generation, child, steps, sigma);
       DrawText( 10, 130, text );

       sprintf( text, "avg fitness: %5.3f", get_faverage());
       DrawText( 10, 145, text );
     }
/*
     DrawText( 10, 150, "Change connection: press the command button and type:" );
     DrawText( 10, 175, "                   set number value" );
     DrawText( 10, 200, "       with number representing the connection number" );
     DrawText( 10, 215, "       and value representing a floating-point value" );
*/
     sprintf( text, "avg speed: %10.5f", (steps == 0)? 0.0: len/steps );
     // DrawText( 10, 250, "avg speed:              " );
     Color( BLUE );
     DrawText( 10, 250, text );
     Color( BLACK );
  }

}



