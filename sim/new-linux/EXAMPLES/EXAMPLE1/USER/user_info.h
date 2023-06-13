/*****************************************************************************/
/* File:        user_info.h (Khepera Simulator)                              */
/* Author:      Olivier MICHEL <om@alto.unice.fr>                            */
/* Date:        Thu Sep 21 14:39:05 1995                                     */
/* Description: example of user_info.h file containing 2 infos on 2 and 1    */
/*              page.                                                        */
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

#define NUMBER_OF_INFO  2   /* between 0 and 4   */
#define PAGES_INFO_1    2   /* between 1 and 255 */
#define TITLE_INFO_1    "Graphics"
#define PAGES_INFO_2    1   /* between 1 and 255 */
#define TITLE_INFO_2    "Steps"
#define PAGES_INFO_3    0   /* unused            */
#define TITLE_INFO_3    ""
#define PAGES_INFO_4    0   /* unused            */
#define TITLE_INFO_4    ""

#include "../SRC/user_info.c"   /* assigns the values for the info pages */
