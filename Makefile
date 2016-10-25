# Cat Felts -- Mini Shell Makefile
# Oct 10, 2016
# Assignment 3

#   $Id: Makefile,v 1.7 2016/10/24 00:49:47 feltsc Exp $

CFLAGS = -g -Wall

OFILES = msh.o arg_parse.o builtin.o expand.o



msh: $(OFILES)
	$(CC) $(CFLAGS) -o msh $(OFILES)

clean:
	rm -r msh $(OFILES)

#dependecy list
$(OFILES): proto.h globals.h
