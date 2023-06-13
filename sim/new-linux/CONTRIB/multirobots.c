/*****************************************************************************/
/* File:        multirobots.c (Khepera Simulator)                            */
/* Author:      Manuel CLERGUE <clerguem@alto.unice.fr>                      */
/* Date:        Thu Jan 16 14:39:05 1996                                     */
/* Description: Extension of Khepera Simulator                               */
/*              for multi-agents simulation                                  */
/* Copyright (c) 1995                                                        */
/* Olivier MICHEL                                                            */
/* MAGE team, i3S laboratory,                                                */
/* CNRS, University of Nice - Sophia Antipolis, FRANCE                       */
/*                                                                           */
/* Copyright (c) 1996                                                        */
/* Manuel CLERGUE                                                            */
/* MAGE team, i3S laboratory,                                                */
/* CNRS, University of Nice - Sophia Antipolis, FRANCE                       */
/*                                                                           */
/* Permission is hereby granted to copy this package for free distribution.  */
/* The author's name and this copyright notice must be included in any copy. */
/* Commercial use is forbidden.                                              */
/*****************************************************************************/

#include "../SRC/include.h"
#include "multirobots.h"

#define NormRad(x)    {while(x > M_PI) x-=2*M_PI; while(x < -M_PI) x+=2*M_PI;}


struct MultiRobots *CreateMultiRobots(long int number)
{

  struct MultiRobots *multirobots;
  long int i;
  
  multirobots = (struct MultiRobots *)malloc(sizeof(struct MultiRobots));
  multirobots->number = number;
  multirobots->current = 0;
  multirobots->robots = (struct Robot **)malloc(sizeof(struct Robot *));
  for(i = 0;i< multirobots->number;i++)
     multirobots->robots[i] = CreateRobot();

  return multirobots;
}

void FreeMultiRobots(struct MultiRobots *multirobots)
{
  long int i;
  for(i = 0;i< multirobots->number;i++)
     free(multirobots->robots[i]);
  free(multirobots->robots);
  free(multirobots);
}

short int MutualInfluence(short int xc,short int yc,double alpha,short int value,struct MultiRobots *multirobots)
{
  short int deltamax,x1,y1,delta,j;
  double    alpha1,alpha2,d;

  deltamax = 0;
  for(j=0;j<multirobots->number;j++)
    {
      if(j==multirobots->current)
	delta = 0;
      else
	{
	  x1 = (short int)multirobots->robots[j]->X;
	  y1 = (short int)multirobots->robots[j]->Y;

	  d = hypot((xc-x1),(yc-y1)) - (ROBOT_DIAMETER/2.0);
	  alpha2 = atan2((y1-yc),(x1-xc));
	  NormRad(alpha2);
	  alpha1 = - alpha - alpha2;
	  NormRad(alpha1);

	  if((alpha1 < (M_PI/3.0)) && (alpha1 > (-M_PI/3.0)) && (d < D_MAX)&& (d > 0.0))
	    {
	    delta = (short int) (cos(alpha1)*
                                 (D_MAX-d)*
                                 (IR_MAX - (double)value)
			         /D_MAX);
	    }
	  else
	    delta = 0;
	}
      deltamax = (delta > deltamax)?delta:deltamax;
      }
return deltamax;
}

void MultiInitSensors(struct Context *context,struct MultiRobots *multirobots)
{
  short int i,xc,yc,delta;
  double x,y,x0,y0;
  double alpha,alpha0;
  struct Robot *robot;

  robot = context->Robot;

  alpha0 = robot->Alpha;
  x0     = robot->X;
  y0     = robot->Y;
  for(i=0;i<8;i++)
  {
    x    = robot->IRSensor[i].X;
    y    = robot->IRSensor[i].Y;

    xc    = (short int)(x0  - y*sin(alpha0) + x*cos(alpha0));
    yc    = (short int)(y0  - y*cos(alpha0) - x*sin(alpha0));
    alpha = alpha0 + robot->IRSensor[i].Alpha;
    NormRad(alpha);
    robot->IRSensor[i].DistanceValue = 
                            IRSensorDistanceValue(context->World,xc,yc,alpha);

    delta = MutualInfluence(xc,yc,alpha,robot->IRSensor[i].DistanceValue,multirobots);
    robot->IRSensor[i].DistanceValue = robot->IRSensor[i].DistanceValue + delta;
    robot->IRSensor[i].LightValue = 
                            IRSensorLightValue(context->World,xc,yc,alpha);
  }
}

void MultiRobotRun(struct Context *context,struct MultiRobots *multirobots)
{
  struct Robot          *robot,saved_robot,*c_robot;
  short int      i;
  boolean        ans;

  c_robot = context->Robot;
  for(i=0;i<multirobots->number;i++)
    {
      multirobots->current = i;
      robot          = multirobots->robots[multirobots->current];
      context->Robot = robot;

      saved_robot.X         = robot->X;
      saved_robot.Y         = robot->Y;
      saved_robot.Alpha     = robot->Alpha;

      MultiInitSensors(context,multirobots);
      StepMultiRobots(multirobots);
      SolveEffectors(context);
      DrawLittleRobot(&saved_robot,multirobots->robots[multirobots->current]);  
      if (TestCollision(context,saved_robot.X,saved_robot.Y,saved_robot.Alpha))
	robot->State |= BUMP; else robot->State &= ~BUMP;
    }
  context->Robot = c_robot;;
}
void MultiRobotRunFast(struct Context *context,struct MultiRobots *multirobots)
{
  struct Robot          *robot,saved_robot,*c_robot;
  short int      i;
  boolean        ans;

  c_robot = context->Robot;
  for(i=0;i<multirobots->number;i++)
    {
      multirobots->current = i;
      robot          = multirobots->robots[multirobots->current];
      context->Robot = robot;

      saved_robot.X         = robot->X;
      saved_robot.Y         = robot->Y;
      saved_robot.Alpha     = robot->Alpha;

      MultiInitSensors(context,multirobots);
      FastStepMultiRobots(multirobots);
      SolveEffectors(context);
      if (TestCollision(context,saved_robot.X,saved_robot.Y,saved_robot.Alpha))
	robot->State |= BUMP; else robot->State &= ~BUMP;
    }
  context->Robot = c_robot;;
}

double DistanceBetRobots(struct Robot *rob1,struct Robot *rob2)
{
  return hypot((rob1->X-rob2->X),(rob1->Y-rob2->Y));
}

void PlaceRobots(struct Context *context,struct MultiRobots *multirobots)
{
  short int  i,j;
  boolean    success = FALSE;

  for(i=0;i<multirobots->number;i++)
    {
      success = FALSE;
      while(success == FALSE)
	{
	  ChooseRandomPosition(context->World,&(multirobots->robots[i]->X),
                                              &(multirobots->robots[i]->Y),
                                              &(multirobots->robots[i]->Alpha)); 
	  success = TRUE;
	  for(j=0;j<multirobots->number;j++)
	    {
	      if(i != j)
		if(DistanceBetRobots(multirobots->robots[i],multirobots->robots[j]) < ROBOT_DIAMETER)
		  success = FALSE;
	    }
	}
    }
}



