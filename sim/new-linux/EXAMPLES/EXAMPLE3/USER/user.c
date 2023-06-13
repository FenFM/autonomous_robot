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
#include "../CONTRIB/multirobots.h"
#include "user.h"
#include "user_info.h"

#define FORWARD_SPEED   5                    /* normal (slow) forward speed*/
#define TURN_SPEED      4                    /* normal (slow) turn speed */
#define COLLISION_TH    500                  /* value of IR sensors to be 
                                                  considered as collision */
int pas=0;

void DrawPas()
{
  char text[256];

  sprintf(text,"pas = %d",pas);
  Color(GREY);
  UndrawText(200,100,"pas = 0000");
  Color(BLUE);
  DrawText(200,100,text);
}

void UserInit(struct Robot *robot){}
void UserClose(struct Robot *robot){}
void NewRobot(struct Robot *robot){  pas = 0;}
boolean FastStepRobot(struct Robot *robot){return FALSE;}
boolean FastStepMultiRobots(struct MultiRobots *multi){return FALSE;}
void LoadRobot(struct Robot *robot,FILE *file){}
void SaveRobot(struct Robot *robotrobot,FILE *file){}

void RunRobotStart(struct Robot *robot){ShowUserInfo(2,1);}
void RunRobotStop(struct Robot *robot){ShowUserInfo(1,2);}

boolean StepRobot(struct Robot *robot)
{
  pas++;
  DrawPas();
  if ((robot->IRSensor[0].DistanceValue > COLLISION_TH) || /* if there is a collision on the*/
      (robot->IRSensor[1].DistanceValue > COLLISION_TH) || /* left side of the robot */
      (robot->IRSensor[2].DistanceValue > COLLISION_TH))
  {
    robot->Motor[LEFT].Value  =  TURN_SPEED;
    robot->Motor[RIGHT].Value = -TURN_SPEED;          /* turn right */
  }
  else if ((robot->IRSensor[3].DistanceValue > COLLISION_TH) || /* if there is a collision on the*/
           (robot->IRSensor[4].DistanceValue > COLLISION_TH) || /* right side of the robot */
           (robot->IRSensor[5].DistanceValue > COLLISION_TH))
  {
    robot->Motor[LEFT].Value  = -TURN_SPEED;
    robot->Motor[RIGHT].Value =  TURN_SPEED;          /* turn left */
  }
  else 
  {
    robot->Motor[LEFT].Value  = FORWARD_SPEED;
    robot->Motor[RIGHT].Value = FORWARD_SPEED;  /* else go forward (default) */
  }

  if ((robot->IRSensor[6].DistanceValue > COLLISION_TH)||  /* collision in */
      (robot->IRSensor[7].DistanceValue > COLLISION_TH))   /*   the back   */
   return(FALSE);
  else
   return(TRUE);
}
boolean StepMultiRobots(struct MultiRobots *multi)
{
  struct Robot *robot;
  boolean      result;

  robot  = multi->robots[multi->current];
  result = StepRobot(robot);

return result;
}

void ResetRobot(struct Robot *robot)
{
  pas = 0;
}

void UserCommand(struct Robot *robot,char         *text)
{
  int i;
  struct MultiRobots  *multi;
  boolean stop = FALSE;

  if (strcmp(text,"test")==0)
  {
    WriteComment("Demonstration");
    ShowUserInfo(1,1);
    multi = CreateMultiRobots(5);
    PlaceRobots(context,multi);
    DrawWorldSquare(context->World);
    for(i=0;i<2000;i++)
    {
      MultiRobotRun(context,multi);
      if (StopCommand())
      {
        stop = TRUE;
        break;
      }
    }
    if (stop) WriteComment("break");
    else WriteComment("Done");
    ShowUserInfo(1,2);
  }
  else WriteComment("unknown command");
  FreeMultiRobots(multi);
}

void DrawUserInfo(struct Robot *robot,u_char info,u_char page)
{
  char text[256];

  switch(info)
  {
    case 1:
      switch(page)
      {
        case 1: Color(MAGENTA);
                FillRectangle(0,0,40,40);
                Color(BLUE);
                DrawLine(100,100,160,180);
                Color(WHITE);
                DrawPoint(200,200);
                Color(YELLOW);
                DrawRectangle(240,100,80,40);
                Color(GREEN);
                DrawText(240,230,"bonjour");
                break;
        case 2: Color(RED);
                DrawArc(200,50,100,100,0,360*64);
                Color(YELLOW);
                FillArc(225,75,50,50,0,360*64);
                Color(BLACK);
                DrawText(140,170,"This is the brain of the robot");
      }
      break;
    case 2:     DrawPas();
  }
}



