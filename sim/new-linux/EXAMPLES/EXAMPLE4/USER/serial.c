/*****************************************************************************/
/* File:        serial.c (Khepera Simulator user file)                       */
/* Author:      Olivier MICHEL <om@alto.unice.fr>                            */
/* Date:        Fri Feb 23 11:02:00 MET 1996                                 */
/* Description: main user program for simulated serial control of sim.       */
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


#include <stdio.h>
#include <string.h>
#include <fcntl.h>

void main()
{
  int sim_serial_in,sim_serial_out,n,l;
  char q[64],a[64];


  /* opening of the pipes in and out sim */

  printf("establishing pipe (.sim_serial_in/.sim_serial_out)\n");
  sim_serial_in  = -1;
  sim_serial_out = -1;
  while((sim_serial_in == -1)||(sim_serial_out == -1))
  {
    if (sim_serial_in  == -1)
     sim_serial_in  = open(".sim_serial_in", O_WRONLY|O_NDELAY,0);
    if (sim_serial_out == -1)
     sim_serial_out = open(".sim_serial_out",O_RDONLY|O_NDELAY,0);
  }
  printf("Pipe established\n");

  /* two processes: the first one reads and displays the outputs from sim, */
  /*                the other one asks the user and writes inputs to sim.  */

  n = fork();
  do
  if (n)   /* outputs from sim */
  {
    l=read(sim_serial_out,a,64);
    if (l>0)
    {
      a[l-1]='\0';           /* suppress the carriage return */
      printf("(%s)\n",a);
      printf("S> ");
      fflush(stdout);
    }
  }
  else    /* inputs to sim */
  {
    printf("S> ");
    scanf("%s",q);
    strcat(q,"\n");
    write(sim_serial_in,q,strlen(q));
  }
  while(1);
}
