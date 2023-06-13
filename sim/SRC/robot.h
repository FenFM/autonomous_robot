/*****************************************************************************/
/* File:        robot.h (Khepera Simulator)                                  */
/* Author:      Olivier MICHEL <om@alto.unice.fr>                            */
/* Date:        Thu Sep 21 14:39:05 1995                                     */
/* Description: robot header file                                            */
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

#ifndef ROBOT_H

#define ROBOT_H 
#define NormRad(x) {while(x > M_PI) x-=2*M_PI; while(x < -M_PI) x+=2*M_PI;}

#define BUMP                     1
#define REAL_ROBOT_FLAG          2
#define DISTANCE_SENSOR_FLAG     4
#define LIGHT_SENSOR_FLAG        8
#define MOTOR_VALUES_FLAG        16

#define ROBOT_DIAMETER           (double)55.0 /* robot diameter in mm */
#define ROBOT_WHEEL_BASE         (double)50.0 /* robot space between two
                                                 wheels in mm         */
#define WHEEL_DIAMETER           (double)13.0

struct Motor
{
  double    X,Y,Alpha;
  short int Value;
};

struct IRSensor
{
  double    X,Y,Alpha;
  short int DistanceValue;
  short int LightValue;
};

struct Robot
{
  u_char                State;
  char                  Name[16];
  double                X,Y,Alpha; /* X and Y (millimeter), Alpha (rad) */
  double                Diameter;  /* (millimeter) */
  struct Motor          Motor[2];
  struct IRSensor       IRSensor[8];
};

#endif
