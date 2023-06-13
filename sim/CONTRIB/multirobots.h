/*****************************************************************************/
/* File:        multirobots.h (Khepera Simulator)                            */
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

#ifndef MULTIROBOTS_H
#define MULTIROBOTS_H

#define D_MAX         50.0   /* max. dist. between 2 robots for mutual perc. */
#define IR_MAX        1023   /* max. value of IR captors */

/* useful external functions */
extern struct Robot *CreateRobot();
extern void    DrawLittleRobot(struct Robot *sr,struct Robot *r);
extern void    ChooseRandomPosition(struct World *world,double *x,double *y,
                                       double *alpha);
extern u_short IRSensorDistanceValue(struct World *world,short int x,short int y,
                                        double alpha);
extern u_short IRSensorLightValue(struct World *world,short int x,short int y,
                                     double alpha);
extern void RefreshWindow(struct Context *context);
extern void DrawWorldSquare(struct World *world);
extern void InitSensors(struct Context *context);
extern void SolveEffectors(struct Context *context);
extern boolean TestCollision(struct Context *context,double ox,double oy,
                               double oalpha);

/* The MultiRobots structure manage the use of several robots */
/* Some services are provided with this structure */
/* It is highly recommended to use them (or to create others) */
/* instead of using those of the Robot structure */

struct MultiRobots
{
  struct Robot **robots;  /* Array of pointers to Robot */
  short int     current;  /* Current Robot being treated -- use this carefully */
  short int     number;   /* Number of Robots in the structure */
};

/* CreateMultiRobots create and initialyze the structure with number Robots */
struct MultiRobots *CreateMultiRobots(long int number);

/* Free the structure and the Robots */
void FreeMultiRobots(struct  MultiRobots *multirobots);

/* Calculate the influence of the group on the current Robot's captor (xc,yc,alpha) */
/* Used in MultiInitSensors */ 
short int MutualInfluence(short int xc,short int yc,double alpha,
                            short int value,struct MultiRobots *multirobots);

/* Calculate the value of IR-Distance captors of the Robots in the structure */
/* Used in MultiRobotRunFast */
void MultiInitSensors(struct Context *context,struct MultiRobots *multirobots);

/* This is the most useful service. This is THE one you have to use in standart situations */
/* There is two version (Fast and Normal) */
/* The fast one (without the display of robots on the screen) use 
/*     FastStepMultiRobots(MultiRobots *multirobots)*/
/* The other one use StepMultiRobots(MultiRobots *multirobots) */
/* You have to implement these functions in the same way that you have to do it
/* for one robot */ 
void MultiRobotRunFast(struct Context *context,struct MultiRobots *multirobots);
void MultiRobotRun(struct Context *context,struct MultiRobots *multirobots);

/* nothing important */
double DistanceBetRobots(struct Robot *rob1,struct Robot *rob2);

/* This service allow you to place the robots at random place in the world */
void PlaceRobots(struct Context *context,struct MultiRobots *multirobots);

/* The functions you have to implement */
extern boolean FastStepMultiRobots(struct MultiRobots *multi);
extern boolean StepMultiRobots(struct MultiRobots *multi);

#endif
