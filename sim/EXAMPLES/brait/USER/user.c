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

static double reight[ 9 ] = { -0.5, -0.5, -0.5, 0.5, 0.5, 0.5, 0.0, 0.0, 6 };
static double left[ 9 ]   = { 0.5, 0.5, 0.5, -0.5, -0.5, -0.5, 0.0, 0.0, 6 };

void UserInit(struct Robot *robot)
{
}

void UserClose(struct Robot *robot)
{
}

void NewRobot(struct Robot *robot)
{
}

void LoadRobot(struct Robot *robot,FILE *file)
{
}

void SaveRobot(struct Robot *robot,FILE *file)
{
}

void RunRobotStart(struct Robot *robot)
{
}

void RunRobotStop(struct Robot *robot)
{
}

boolean StepRobot(struct Robot *robot)
{
  int i;
  double v;
  for( i = 0, v = 0; i < 7; i++ )
     v += robot->IRSensor[ i ] * right[ i ];
  robot->Motor[ RIGHT ].Value = v + right[ 8 ];
  for( i = 0, v = 0; i < 7; i++ )
     v += robot->IRSensor[ i ] * left[ i ];
  robot->Motor[ LEFT ].Value = v + left[ 8 ];
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
}

void DrawUserInfo(struct Robot *robot,u_char info,u_char page)
{
}



