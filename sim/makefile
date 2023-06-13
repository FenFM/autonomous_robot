#File:     Makefile for Khepera Simulator (example 0)
#Author:   Olivier MICHEL <om@alto.unice.fr>
#Date:     Tue Feb 13 11:29:50 MET 1996

#   Copyright (c) 1995, 1996
#   Olivier MICHEL
#   MAGE team, i3S laboratory,
#   CNRS, University of Nice - Sophia Antipolis

#   Permission is hereby granted to copy all or any part of
#   this program for free distribution.   The author's name
#   and this copyright notice must be included in any copy.
#   Commercial use is forbidden.

VPATH=OBJ/:SRC/:USER/

CFLAGS = -I/usr/include \
         -O3 \

LIBS   = -L/usr/lib -L/usr/X11/lib -lX11 -lm

cc = gcc $(CFLAGS) -c 

CC = gcc $(LIBS)

sim:	sim.o robot.o world.o graphics.o khep_serial.o user.o my_change.o
	$(CC) OBJ/sim.o OBJ/robot.o OBJ/world.o OBJ/graphics.o OBJ/khep_serial.o OBJ/user.o OBJ/my_change.o $(LIBS) -o sim

sim.o:  sim.c sim.h
	$(cc) SRC/sim.c -o OBJ/sim.o
robot.o: robot.c robot.h
	$(cc) SRC/robot.c -o OBJ/robot.o
world.o: world.c world.h
	$(cc) SRC/world.c -o OBJ/world.o
graphics.o: graphics.c graphics.h
	$(cc) SRC/graphics.c -o OBJ/graphics.o
khep_serial.o: khep_serial.c khep_serial.h gen_types.h
	$(cc) SRC/khep_serial.c -o OBJ/khep_serial.o
user.o: user.c user.h user_info.h include.h
	$(cc) USER/user.c -o OBJ/user.o
my_change.o: my_change.c user.h include.h
	$(cc) USER/my_change.c -o OBJ/my_change.o
header.h: types.h graphics.h sim.h robot.h world.h
	touch SRC/header.h

clean:
	rm -f sim OBJ/*.o
