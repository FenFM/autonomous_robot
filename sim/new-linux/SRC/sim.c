/*****************************************************************************/
/* File:        sim.c (Khepera Simulator)                                    */
/* Author:      Olivier MICHEL <om@alto.unice.fr>                            */
/* Date:        Thu Sep 21 14:39:05 1995                                     */
/* Description: main program                                                 */
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

#include <sys/types.h>
#include <sys/stat.h>

#include "header.h"
#include "sim.h"
#include "world.h"
#include "graphics.h"
#include "robot.h"

struct Context *context;

/* this function returns a random value between 0 and x-1 */
long int Rnd(int x)
{
  return(((long)(rand() & 0x7fff)*(long)(x))/(long)(0x8000));
}

void OpenProgram()
{
  struct Button  *but;
  FILE           *file;

  file = fopen("WORLD/home.world","r");
  if (file)
  {
    context->World = (struct World *)malloc(sizeof(struct World));
    context->World->Objects = NULL;
    CreateEmptyWorld(context);
    ReadWorldFromFile(context->World,file);
    strcpy(context->World->Name,"home");
    fclose(file);
  }
  else
  {
    perror("unable to find home.world in WORLD directory");
    exit(0);
  }

  CreateDefaultRobot(context);

  strcpy(context->TextInput,"");
  context->Info           = INFO_ABOUT;
  but= context->Buttons= CreateButton(COMMAND,"command",
                               WINDOW_W/2+344,WINDOW_W/2-34);
  but= but->Next=CreateButton(KHEPERA_BUTTON,">",WINDOW_W/2+220,40);
  but= but->Next=CreateButton(SENSORS_BUTTON,">",WINDOW_W/2+220,60);
  but= but->Next=CreateButton(MOTORS_BUTTON,">",WINDOW_W/2+220,80);

  but= but->Next=CreateButton(NEW_WORLD,"new",48,WINDOW_W/2-34);
  but= but->Next=CreateButton(LOAD_WORLD,"load",92,WINDOW_W/2-34);
  but= but->Next=CreateButton(SAVE_WORLD,"save",140,WINDOW_W/2-34);
  but= but->Next=CreateButton(SET_ROBOT,"set robot",208,WINDOW_W/2-34);
  but= but->Next=CreateButton(REDRAW_WORLD,"!",264,WINDOW_W/2-34);
  but= but->Next=CreateButton(SCAN_WORLD,"scan",300,WINDOW_W/2-34);
  but= but->Next=CreateButton(REMOVE_OBJECT,"remove",356,WINDOW_W/2-34);
  but= but->Next=CreateButton(ADD_OBJECT,"add",408,WINDOW_W/2-34);
  but= but->Next=CreateButton(TURN_OBJECT,"turn",452,WINDOW_W/2-34);
  but= but->Next=CreateButton(OBJECT_PLUS,"+",488,WINDOW_W/2-41);
  but= but->Next=CreateButton(OBJECT_MINUS,"-",488,WINDOW_W/2-23);

  but= but->Next=CreateButton(NEW_ROBOT,"new",WINDOW_W/2+48,WINDOW_W/2-34);
  but= but->Next=CreateButton(LOAD_ROBOT,"load",WINDOW_W/2+92,WINDOW_W/2-34);
  but= but->Next=CreateButton(SAVE_ROBOT,"save",WINDOW_W/2+140,WINDOW_W/2-34);
  but= but->Next=CreateButton(STEP_ROBOT,"step",WINDOW_W/2+188,WINDOW_W/2-34);
  but= but->Next=CreateButton(RUN_ROBOT,"run",WINDOW_W/2+232,WINDOW_W/2-34);
  but= but->Next=CreateButton(RESET_ROBOT,"reset",
                               WINDOW_W/2+280,WINDOW_W/2-34);
  but= but->Next=CreateButton(HELP,"?",
                               WINDOW_W/2+392,WINDOW_W/2-34);
  but= but->Next=CreateButton(INFO_NEXT,"info",
                              WINDOW_W/2+428,WINDOW_W/2-34);
  but= but->Next=CreateButton(PAGE_PLUS,"+",WINDOW_W-106,WINDOW_W/2-34);
  but= but->Next=CreateButton(PAGE_MINUS,"-",WINDOW_W-90,WINDOW_W/2-34);
  but= but->Next=CreateButton(QUIT,"quit",WINDOW_W-54,WINDOW_W/2-34);

  context->UserInfo = SetUserInfo();
  OpenGraphics(context);
  InitSensors(context);
}

void CloseProgram()
{
  UserClose(context->Robot);
  FreeRobot(context);
  FreeWorld(context);
  FreeButtons(context);
  free(context);
  CloseGraphics();
}

boolean ExistOption(char *opt,int argc,char *argv[])
{
  int     i;
  boolean ans = FALSE;

  for(i=1;i<argc;i++) if (argv[i][0]=='-')
   if (strcmp(&(argv[i][1]),opt)==0) ans = TRUE;
  return(ans);
}

void ReadConfigFile()
{
  char    text[TEXT_BUF_SIZE];
  boolean ok;
  FILE    *file;

  file = fopen(".simrc","r");

  if (file)
  {
    while(fscanf(file,"%s\n",text)!=EOF)
    {
      if (strcmp(text,"MONODISPLAY:")==0)
      {
        fscanf(file,"%s\n",text);
        if (strcmp(text,"TRUE")==0) context->MonoDisplay = TRUE;
        else context->MonoDisplay = FALSE;  
      }
      if (strcmp(text,"KHEPERA_AVAILABLE:")==0)
      {
        fscanf(file,"%s\n",text);
        if (strcmp(text,"TRUE")==0) context->KheperaAvailable = TRUE;
        else context->KheperaAvailable = FALSE;
      }
      if (strcmp(text,"SERIAL_PORT:")==0)
      {
        fscanf(file,"%s\n",text);
        InitKheperaSerial(text);
      }
    }
    fclose(file);
  }
  else
  {
    perror("unable to find .simrc in current directory");
    exit(0);
  }
}

int main(int argc,char *argv[])
{
  char              text[TEXT_BUF_SIZE],name[TEXT_BUF_SIZE],
                    file_name[TEXT_BUF_SIZE];
  boolean           quit=FALSE,ok;
  struct World      *world;
  struct Robot      *robot,saved_robot;
  XPoint            *point;
  struct Button     *button;
  struct Object     *object,*obj;
  double            val0;
  long int          i,j;
  int               angle;
  FILE              *ffile;
  int               sim_serial_in,sim_serial_out,f;

  context = (struct Context *)malloc(sizeof(struct Context));
  ReadConfigFile();
  if (ExistOption("s",argc,argv)) context->KheperaAvailable = FALSE;
  if (ExistOption("p",argc,argv))
  {
    context->Pipe = TRUE;
    context->KheperaAvailable = FALSE;
    mknod(".sim_serial_in", S_IFIFO|0666,0);
    mknod(".sim_serial_out",S_IFIFO|0666,0);
  }
  else context->Pipe=FALSE; 
  OpenProgram();
  world = context->World;
  robot = context->Robot;
  UserInit(robot);
  while(quit == FALSE)
  {
    button=PressButton(context);
    switch(button->Value)
    {
      case QUIT:
       DisplayComment(context,"bye !");
       quit = TRUE;
       break;

      case NEW_WORLD:
       DisplayComment(context,"creating new world");
       CreateDefaultWorld(context);
       strcpy(world->Name,"new");
       DrawWorld(context);
       DisplayComment(context,"new world created");
       break;

      case LOAD_WORLD:
       strcpy(name,ReadText(context,FILE_NAME_TEXT,button));
       if (name[0]!='\0')
       {
         sprintf(text,"loading %s.world",name);
         DisplayComment(context,text);
         strcpy(text,name);
         strcat(name,".world");
         strcpy(file_name,"WORLD/");
         strcat(file_name,name);
         ffile = fopen(file_name,"r");
         if (ffile)
         {
           WaitCursor();
           CreateEmptyWorld(context);
           ReadWorldFromFile(world,ffile);
           strcpy(world->Name,text);
           fclose(ffile);
           DrawWorld(context);
           if (!(robot->State & REAL_ROBOT_FLAG)) InitSensors(context);
           DrawRobotIRSensors(robot);
           sprintf(text,"%s loaded",name);
           PointerCursor();
         }
         else sprintf(text,"unable to find %s in WORLD directory",name);
         DisplayComment(context,text);
       }
       else DisplayComment(context,"nothing done");
       break;

      case SAVE_WORLD:
       strcpy(name,ReadText(context,FILE_NAME_TEXT,button));
       if (name[0]!='\0')
       {
         WaitCursor();
         strcpy(world->Name,name);
         strcat(name,".world");
         sprintf(text,"saving %s",name);
         strcpy(file_name,"WORLD/");
         strcat(file_name,name);
         DrawWorld(context);
         DisplayComment(context,text);
         ffile = fopen(file_name,"w");
         WriteWorldToFile(world,ffile);
         fclose(ffile);
         sprintf(text,"%s saved",name);
         PointerCursor();
         DisplayComment(context,text);
       }
       else DisplayComment(context,"nothing done");
       break;

      case SET_ROBOT:
        DisplayComment(context,"click in the world to set the robot");
        ok = FALSE;
        while(ok == FALSE)
        {
          point = ClickInWorld(context,button);
          if (point->x != -1)
          {
            robot->X = point->x;
            robot->Y = point->y;
            DrawWorld(context);
            if (!(robot->State & REAL_ROBOT_FLAG)) InitSensors(context);
             DrawRobotIRSensors(robot);
            ok = TRUE;
          }
          else ok = TRUE;
        }
        UndisplayComment(context);
       break;

      case REDRAW_WORLD:
       DrawObstacles(context);
       DrawWorld(context);
       if (!(robot->State & REAL_ROBOT_FLAG)) InitSensors(context);
        DrawRobotIRSensors(robot);
       break;

      case SCAN_WORLD:
        WaitCursor();
        ScanWorld(context);
        PointerCursor();
        break;

      case REMOVE_OBJECT:
       DisplayComment(context,"click in the world to remove objects");
       ok = FALSE;
       while(ok == FALSE)
       {
         point = ClickInWorld(context,button);
         if (point->x != -1)
         {
           object = FindObject(world,point->x,point->y);
           if (object)
           {
             sprintf(text,"%s removed at (%d,%d)",
                     world->ObjectName[object->Type],object->X,object->Y);
             RemoveObject(world,object);
             DrawWorld(context);
             DisplayComment(context,text);
           }
           else DisplayComment(context,"oops");
         }
         else ok = TRUE;
       }
       UndisplayComment(context);
       break;

      case ADD_OBJECT:
       DisplayComment(context,"click in the world to put objects");
       ok = FALSE;
       while(ok == FALSE)
       {
         obj = AddObjectInWorld(context,button);
         if (obj)
         {
           object = CreateObject(obj->Type,obj->X,obj->Y,obj->Alpha);
           AddObject(world,object);
           DrawObject(object);
           sprintf(text,"%s added at (%d,%d)",
                   world->ObjectName[obj->Type],obj->X,obj->Y);
           DisplayComment(context,text);
         }
         else ok = TRUE;
       }
       UndisplayComment(context);
       break;

      case TURN_OBJECT:
        world->ObjectAlpha[world->ObjectType] += 15;
        if (world->ObjectAlpha[world->ObjectType] >= 360) 
         world->ObjectAlpha[world->ObjectType] -= 360;
        DrawConsObject(world);
        sprintf(text,"turning %s",
                world->ObjectName[world->ObjectType]);
        DisplayComment(context,text);
        break;

      case OBJECT_PLUS:
        if (world->ObjectType == N_OBJECTS-1) 
         world->ObjectType= 0;
        else world->ObjectType++;
        DrawConsObject(world);
        sprintf(text,"%s selected",
                world->ObjectName[world->ObjectType]);
        DisplayComment(context,text);
        break;

      case OBJECT_MINUS:
        if (world->ObjectType == 0) 
         world->ObjectType = N_OBJECTS-1;
        else world->ObjectType--;
        DrawConsObject(world);
        sprintf(text,"%s selected",
                world->ObjectName[world->ObjectType]);
        DisplayComment(context,text);
        break;

      case NEW_ROBOT:
       NewRobot(robot);
       DisplayComment(context,"new robot created");
       DrawRobotIRSensors(robot);
       DrawRobotEffectors(robot);
       break;

      case LOAD_ROBOT:
       strcpy(name,ReadText(context,FILE_NAME_TEXT,button));
       if (name[0]!='\0')
       {
         sprintf(text,"loading %s.robot",name);
         DisplayComment(context,text);      
         strcpy(text,name);
         strcat(name,".robot");
         strcpy(file_name,"ROBOT/");
         strcat(file_name,name);
         ffile = fopen(file_name,"r");
         if (ffile)
         {
           WaitCursor();
           saved_robot.X = robot->X;
           saved_robot.Y = robot->Y;
           ReadRobotFromFile(robot,ffile);
           fclose(ffile);
           strcpy(robot->Name,text);
           DrawRobot(context);
           if (!(robot->State & REAL_ROBOT_FLAG)) InitSensors(context);
           else InitKheperaSensors(context);
           DrawLittleRobot(&saved_robot,robot);
           sprintf(text,"%s loaded",name);
           PointerCursor();
         }
         else sprintf(text,"unable to find %s in ROBOT directory",name);
         DisplayComment(context,text);
       }
       else DisplayComment(context,"nothing done");
       break;

      case SAVE_ROBOT:
       strcpy(name,ReadText(context,FILE_NAME_TEXT,button));
       if (name[0]!='\0')
       {
         WaitCursor();
         strcpy(robot->Name,name);
         strcat(name,".robot");
         sprintf(text,"saving %s",name);
         strcpy(file_name,"ROBOT/");
         strcat(file_name,name);
         DrawRobot(context);
         DisplayComment(context,text);
         ffile = fopen(file_name,"w");
         WriteRobotToFile(robot,ffile);
         fclose(ffile);
         sprintf(text,"%s saved",name);
         PointerCursor();
         DisplayComment(context,text);
       }
       else DisplayComment(context,"nothing done");
       break;

      case STEP_ROBOT:
       RunRobotStart(robot);
       if (robot->State & REAL_ROBOT_FLAG)
       {
         RunKhepera(context);
         i = robot->Motor[0].Value;
         j = robot->Motor[1].Value;
         robot->Motor[0].Value = 0;
         robot->Motor[1].Value = 0;
         SolveKheperaEffectors(context);
         robot->Motor[0].Value = i;
         robot->Motor[1].Value = j;
         DrawRobotEffectors(robot);
         sprintf(text,"robot step done");
       }
       else
       {
         RobotRun(context);
         if (robot->State & BUMP) sprintf(name,"BUMP");
                             else sprintf(name,"OKAY");
         sprintf(text,"x = %lg, y = %lg, alpha = %lg deg, state = %s",
                 floor(robot->X+0.5),floor(robot->Y+0.5),
                 floor(robot->Alpha*180/M_PI+0.5),name);
       }
       DisplayComment(context,text);
       RunRobotStop(robot);
       break;

      case RUN_ROBOT:
       DisplayComment(context,"running Khepera");
       CancelCursor();
       if (context->Pipe)
       {
         DisplayComment(context,
                       "establishing pipe (.sim_serial_in/.sim_serial_out)");
         sim_serial_in  = -1;
         sim_serial_out = -1;
         while((sim_serial_in == -1)||(sim_serial_out == -1))
         {
           if (sim_serial_in  == -1) 
            sim_serial_in  = open(".sim_serial_in", O_RDONLY|O_NDELAY,0);
           if (sim_serial_out == -1) 
            sim_serial_out = open(".sim_serial_out",O_WRONLY|O_NDELAY,0);
           if (UnpressButton(context,button)) break;
         }
         if ((sim_serial_in != -1)&&(sim_serial_out != -1))
         {
           DisplayComment(context,"running simulated Khepera from pipe");
           while ((UnpressButton(context,button)==FALSE)
                  &&(PipedRobotRun(context,sim_serial_in,sim_serial_out)));
           close(sim_serial_in);
           close(sim_serial_out);
           robot->Motor[0].Value = 0;
           robot->Motor[1].Value = 0;
           DrawRobotEffectors(robot);
           DisplayComment(context,"pipe closed");
         }
         else DisplayComment(context,"aborted");
       }
       else
       {
         RunRobotStart(robot);
         if (robot->State & REAL_ROBOT_FLAG)
         {
           DisplayComment(context,"running real Khepera");
           while((UnpressButton(context,button)==FALSE)&&
                 (RunKhepera(context)));
           robot->Motor[0].Value = 0;
           robot->Motor[1].Value = 0;
           SolveKheperaEffectors(context);
           DrawRobotEffectors(robot);
         }
         else
         {
           DisplayComment(context,"running simulated Khepera");
           while ((UnpressButton(context,button)==FALSE)&&(RobotRun(context)));
           robot->Motor[0].Value = 0;
           robot->Motor[1].Value = 0;
           DrawRobotEffectors(robot);
         }
         DisplayComment(context,"Khepera stopped");
         RunRobotStop(robot);
       }
       PointerCursor();
       break;

      case RESET_ROBOT:
       InitRobot(context);
       DrawRobotIRSensors(robot);
       DrawRobotEffectors(robot);
       DisplayComment(context,"Khepera reset");
       break;

      case COMMAND:
       strcpy(name,ReadText(context,COMMAND_TEXT,button));
       if (name[0]!='\0')
       {
         UndisplayComment(context);
         CancelCursor();
         if (sscanf(name,"set angle %d",&angle)==1)
         {
           robot->Alpha = (angle*M_PI)/180.0;
           DrawLittleRobot(robot,robot);
         } else UserCommand(robot,name);
         PointerCursor();
       }
       else DisplayComment(context,"nothing done");
       break;

      case HELP:
       if (robot->State & REAL_ROBOT_FLAG)
       {
         DisplayComment(context,"testing real Khepera");
         while (UnpressButton(context,button)==FALSE)
	 {
           InitKheperaSensors(context);
           DrawRobotIRSensors(robot);
         }
       }
       else
       {
         DisplayComment(context,"testing simulated Khepera");
         while (UnpressButton(context,button)==FALSE)
	 {
           InitSensors(context);
           DrawRobotIRSensors(robot);
         }
       }
       DisplayComment(context,"done");
       break;

      case INFO_NEXT:
       switch(context->Info)
       {
         case INFO_ABOUT:      if (context->UserInfo->Info >= 1)
                                context->Info = INFO_USER0;
                               break;
         case INFO_USER0:      if (context->UserInfo->Info >= 2)
                                context->Info = INFO_USER1;
                               else context->Info = INFO_ABOUT;
                               break;
         case INFO_USER1:      if (context->UserInfo->Info >= 3)
                                context->Info = INFO_USER2;
                               else context->Info = INFO_ABOUT;
                               break;
         case INFO_USER2:      if (context->UserInfo->Info >= 4)
                                context->Info = INFO_USER3;
                               else context->Info = INFO_ABOUT;
                               break;
         case INFO_USER3:      context->Info = INFO_ABOUT;
                               break;
       }
       strcpy(context->Comment,"");
       DrawInfo(context);
       break;

      case PAGE_PLUS:
       switch(context->Info)
       {
         case INFO_ABOUT: context->InfoAbout++;
                          if (context->InfoAbout >= 3)
                           context->InfoAbout = 0;
                          break;
         case INFO_USER0: context->InfoUser[0]++;
                          if (context->InfoUser[0] >=
                              context->UserInfo->Pages[0])
                           context->InfoUser[0] = 0;
                          break;
         case INFO_USER1: context->InfoUser[1]++;
                          if (context->InfoUser[1] >=
                              context->UserInfo->Pages[1])
                           context->InfoUser[1] = 0;
                          break;
         case INFO_USER2: context->InfoUser[2]++;
                          if (context->InfoUser[2] >=
                              context->UserInfo->Pages[2])
                           context->InfoUser[2] = 0;
                          break;
         case INFO_USER3: context->InfoUser[3]++;
                          if (context->InfoUser[3] >=
                              context->UserInfo->Pages[3])
                           context->InfoUser[3] = 0;
                          break;
       }
       strcpy(context->Comment,"");
       DrawInfo(context);
       break;

      case PAGE_MINUS:
       switch(context->Info)
       {
         case INFO_ABOUT:      context->InfoAbout--;
                               if (context->InfoAbout < 0)
                                context->InfoAbout = 2;
                               break;
         case INFO_USER0:      context->InfoUser[0]--;
                               if (context->InfoUser[0] < 0)
                                context->InfoUser[0] =
                                 context->UserInfo->Pages[0] - 1;
                               break;
         case INFO_USER1:      context->InfoUser[1]--;
                               if (context->InfoUser[1] < 0)
                                context->InfoUser[1] =
                                 context->UserInfo->Pages[1] - 1;
                               break;
         case INFO_USER2:      context->InfoUser[2]--;
                               if (context->InfoUser[2] < 0)
                                context->InfoUser[2] =
                                 context->UserInfo->Pages[2] - 1;
                               break;
         case INFO_USER3:      context->InfoUser[3]--;
                               if (context->InfoUser[3] < 0)
                                context->InfoUser[3] =
                                 context->UserInfo->Pages[3] - 1;
                               break;
       }
       strcpy(context->Comment,"");
       DrawInfo(context);
       break;

       case KHEPERA_BUTTON:    if (context->KheperaAvailable)
	                       {
                                 if (robot->State & REAL_ROBOT_FLAG)
                                   CloseKheperaSerial(context);
                                 else OpenKheperaSerial(context);
			       }
                               else DisplayComment(context,
                                                 "no real Khepera available");
                               DrawRobotToggleButtons(robot);
                               break;
       case SENSORS_BUTTON:    if (robot->State & DISTANCE_SENSOR_FLAG)
	                       {
                                 robot->State ^= DISTANCE_SENSOR_FLAG;
                                 robot->State ^= LIGHT_SENSOR_FLAG;
			       }
                               else if (robot->State & LIGHT_SENSOR_FLAG)
                                robot->State ^= LIGHT_SENSOR_FLAG;
                               else robot->State ^= DISTANCE_SENSOR_FLAG;
                               DrawRobotToggleButtons(robot);
                               DrawRobotIRSensors(robot);
                               UndisplayComment(context);
                               break;
       case MOTORS_BUTTON:     robot->State ^= MOTOR_VALUES_FLAG;
                               DrawRobotToggleButtons(robot);
                               DrawRobotEffectors(robot);
                               UndisplayComment(context);
                               break;
    }
    button->State = PLATE_UP;
    DrawButton(button);
  }
  if (context->Pipe)
  {
    system("rm .sim_serial_in");
    system("rm .sim_serial_out");
  }
  CloseProgram();
}

