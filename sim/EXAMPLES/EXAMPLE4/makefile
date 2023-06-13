#File:     Makefile for Khepera Simulator (example 4)
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
         -O3

LIBS   = -L/usr/lib -lX11 -lm

cc = gcc $(CFLAGS) -c

CC = gcc $(LIBS)

cc1 = gcc

sim:	sim.o robot.o world.o graphics.o khep_serial.o user.o
	$(CC) OBJ/sim.o OBJ/robot.o OBJ/world.o OBJ/graphics.o OBJ/khep_serial.o OBJ/user.o -o sim

serial: serial.c
	$(cc1) USER/serial.c -o serial

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
header.h: types.h graphics.h sim.h robot.h world.h
	touch SRC/header.h

clean:
	rm -f sim serial OBJ/*.o
