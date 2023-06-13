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
#include "../CONTRIB/neural.h"
#include "user_info.h"
#include "user.h"

struct NeuralNetwork neural_net;

extern void        InitNeuralNetwork(struct NeuralNetwork *n);
extern void        CreateCrazyNeuralNetwork(struct NeuralNetwork *n);
extern void        ReadNeuralNetworkFromFile(struct NeuralNetwork *n,FILE *f);
extern void        WriteNeuralNetworkToFile(struct NeuralNetwork *n,FILE *f);
extern void        InitInputNeurons(struct Robot *r,struct NeuralNetwork *n);
extern void        DrawInputActivities(struct Neuron *n);
extern void        Think(struct NeuralNetwork *n);
extern void        DrawHiddenActivities(struct Neuron *n);
extern void        InitEffectors(struct Robot *r,struct NeuralNetwork *n);
extern void        DrawOutputActivities(struct Neuron *n);
extern void        ResetNeuralNetwork(struct NeuralNetwork *n);
extern void        PrintNeuralNetwork(struct NeuralNetwork *n);
extern void        ReadNeuralNetwork(struct NeuralNetwork *n);
extern void        DrawNeuralNetwork(struct NeuralNetwork *n);
extern void        DrawNeuralLegend();

FILE *gnuplot_file,*path_file;

void UserInit(struct Robot *robot)
{
  InitNeuralNetwork(&neural_net);
  CreateCrazyNeuralNetwork(&neural_net);
  gnuplot_file = popen("gnuplot","w");
  if (gnuplot_file == NULL)
  {
    fprintf(stderr,"Warning: gnuplot not available");
  }
  ShowUserInfo(2,1);
}

void UserClose(struct Robot *robot)
{
  if (gnuplot_file) pclose(gnuplot_file);
}

void NewRobot(struct Robot *robot)
{
  ReadNeuralNetwork(&neural_net);
  ShowUserInfo(1,1);
}

void LoadRobot(struct Robot *robot,FILE *file)
{
  ReadNeuralNetworkFromFile(&neural_net,file);
  ShowUserInfo(1,1);
}

void SaveRobot(struct Robot *robot,FILE *file)
{
  WriteNeuralNetworkToFile(&neural_net,file);
}

void RunRobotStart(struct Robot *robot)
{
  path_file = fopen("STATS/path.dat","a");
  if (GetUserInfo()!=1 || GetUserInfoPage()!=1) ShowUserInfo(1,1);
}

void RunRobotStop(struct Robot *robot)
{
  fclose(path_file);
}

boolean StepRobot(struct Robot *robot)
{
  InitInputNeurons(robot,&neural_net);
  DrawInputActivities(neural_net.InputNeuron);
  Think(&neural_net);
  DrawHiddenActivities(neural_net.HiddenNeuron);
  InitEffectors(robot,&neural_net);
  DrawOutputActivities(neural_net.OutputNeuron);
  fprintf(path_file,"%lg, %lg\n",robot->X,1000.0-robot->Y);
  return(TRUE);
}

void FastStepRobot(struct Robot *robot)
{
}

/********************************/
/*   delete the file path.dat   */
/* and reset the neural network */
/********************************/
void ResetRobot(struct Robot *robot)
{
  FILE *test;

  test = fopen("STATS/path.dat","r");
  if (test)
  {
    fclose(test);
    system("rm STATS/path.dat");
  }
  ResetNeuralNetwork(&neural_net);
  ShowUserInfo(1,1);
}

void UserCommand(struct Robot *robot,char *text)
{
  int     i,n;
  boolean stop = FALSE;
  char    comment[TEXT_BUF_SIZE];
  FILE    *file;

  if (strcmp(text,"print network")==0)
  {
    EraseComment();
    PrintNeuralNetwork(&neural_net);
    WriteComment("neural network printed on stdout");
  }
  else if (strcmp(text,"plot")==0)
  {
    fprintf(gnuplot_file,"load 'GNUPLOT/path.gnu'\n");
    fflush(gnuplot_file);
    WriteComment("path plotted");
  }
  else WriteComment("unknown command");
}

void DrawUserInfo(struct Robot *robot,u_char info,u_char page)
{
  char text[256];

  switch(info)
  {
    case 1:
      switch(page)
      {
        case 1: DrawNeuralNetwork(&neural_net);
                break;
        case 2: DrawNeuralLegend();
      }
      break;
    case 2:
      Color(BLACK);
      switch(page)
      {
        case 1:
        DrawText(10,20,
              "This example shows a possible use of Khepera simulator with a");
        DrawText(10,40,
              "neural network controller. The information window is used to ");
        DrawText(10,60,
              "display the neural network, and a pipe to gnuplot is built to");
        DrawText(10,80,
              "display the path of the robot.                               ");
        DrawText(10,120,"Possible commands are:");
        DrawText(10,150,
              "- plot: plot the path of the robot");
        DrawText(10,170,
              "- print network: print the structure on the neural network.  ");
        DrawText(10,190,
              "- set angle x: x represents the orientation of the robot.");
        DrawText(10,220,
              "RESET button initializes the neural network and the path plot");
        DrawText(10,250,
              "NEW button loads the neural network defined in the file:     ");
        DrawText(10,270," SIM/NEURAL/neural.network");
        DrawText(10,300,"Try to load and run \"big\", \"learning\" robots.");
        break;
      }
  }
}
