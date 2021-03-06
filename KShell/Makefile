# Makefile for the CS:APP Shell Lab

TEAM = NOBODY
VERSION = 1
HANDINDIR = /afs/cs/academic/class/15213-f02/L5/handin
DRIVER = ./sdriver.pl
TSH = ./tsh
TSHREF = ./tshref
TSHARGS = "-p"
CC = gcc
CFLAGS = -Wall -O2 -g


FILES =  ./myspin ./mysplit ./mystop ./myint

all: tsh $(FILES)

tsh: tsh.c funwitherr.c funwitherr.h jobs.c jobs.h mystring.c mystring.h
	$(CC) $(CFLAGS)  -o tsh tsh.c funwitherr.c jobs.c mystring.c /usr/lib64/libreadline.so

addition: myspin.c mysplit.c mystop.c myint.c
	$(CC) $(CFLAGS) -o myspin myspin.c
	$(CC) $(CFLAGS) -o mysplit mysplit.c
	$(CC) $(CFLAGS) -o mystop mystop.c
	$(CC) $(CFLAGS) -o myint myint.c 


path: path.c mystring.c mystring.h
	$(CC) $(CFLAGS) -o path path.c mystring.c /usr/lib64/libreadline.so


##################
# Handin your work
##################
handin:
	cp tsh.c $(HANDINDIR)/$(TEAM)-$(VERSION)-tsh.c


##################
# Regression tests
##################

# Run tests using the student's shell program
test01:
	$(DRIVER) -t ./test/trace01.txt -s $(TSH) -a $(TSHARGS)
test02:
	$(DRIVER) -t ./test/trace02.txt -s $(TSH) -a $(TSHARGS)
test03:
	$(DRIVER) -t ./test/trace03.txt -s $(TSH) -a $(TSHARGS)
test04:
	$(DRIVER) -t ./test/trace04.txt -s $(TSH) -a $(TSHARGS)
test05:
	$(DRIVER) -t ./test/trace05.txt -s $(TSH) -a $(TSHARGS)
test06:
	$(DRIVER) -t ./test/trace06.txt -s $(TSH) -a $(TSHARGS)
test07:
	$(DRIVER) -t ./test/trace07.txt -s $(TSH) -a $(TSHARGS)
test08:
	$(DRIVER) -t ./test/trace08.txt -s $(TSH) -a $(TSHARGS)
test09:
	$(DRIVER) -t ./test/trace09.txt -s $(TSH) -a $(TSHARGS)
test10:
	$(DRIVER) -t ./test/trace10.txt -s $(TSH) -a $(TSHARGS)
test11:
	$(DRIVER) -t ./test/trace11.txt -s $(TSH) -a $(TSHARGS)
test12:
	$(DRIVER) -t ./test/trace12.txt -s $(TSH) -a $(TSHARGS)
test13:
	$(DRIVER) -t ./test/trace13.txt -s $(TSH) -a $(TSHARGS)
test14:
	$(DRIVER) -t ./test/trace14.txt -s $(TSH) -a $(TSHARGS)
test15:
	$(DRIVER) -t ./test/trace15.txt -s $(TSH) -a $(TSHARGS)
test16:
	$(DRIVER) -t ./test/trace16.txt -s $(TSH) -a $(TSHARGS)

testall:
	$(DRIVER) -t ./test/trace01.txt -s $(TSH) -a $(TSHARGS)
	$(DRIVER) -t ./test/trace02.txt -s $(TSH) -a $(TSHARGS)
	$(DRIVER) -t ./test/trace03.txt -s $(TSH) -a $(TSHARGS)
	$(DRIVER) -t ./test/trace04.txt -s $(TSH) -a $(TSHARGS)
	$(DRIVER) -t ./test/trace05.txt -s $(TSH) -a $(TSHARGS)
	$(DRIVER) -t ./test/trace06.txt -s $(TSH) -a $(TSHARGS)
	$(DRIVER) -t ./test/trace07.txt -s $(TSH) -a $(TSHARGS)
	$(DRIVER) -t ./test/trace08.txt -s $(TSH) -a $(TSHARGS)
	$(DRIVER) -t ./test/trace09.txt -s $(TSH) -a $(TSHARGS)
	$(DRIVER) -t ./test/trace10.txt -s $(TSH) -a $(TSHARGS)
	$(DRIVER) -t ./test/trace11.txt -s $(TSH) -a $(TSHARGS)
	$(DRIVER) -t ./test/trace12.txt -s $(TSH) -a $(TSHARGS)
	$(DRIVER) -t ./test/trace13.txt -s $(TSH) -a $(TSHARGS)
	$(DRIVER) -t ./test/trace14.txt -s $(TSH) -a $(TSHARGS)
	$(DRIVER) -t ./test/trace15.txt -s $(TSH) -a $(TSHARGS)
	$(DRIVER) -t ./test/trace16.txt -s $(TSH) -a $(TSHARGS)

# Run the tests using the reference shell program
rtest01:
	$(DRIVER) -t trace01.txt -s $(TSHREF) -a $(TSHARGS)
rtest02:
	$(DRIVER) -t trace02.txt -s $(TSHREF) -a $(TSHARGS)
rtest03:
	$(DRIVER) -t trace03.txt -s $(TSHREF) -a $(TSHARGS)
rtest04:
	$(DRIVER) -t trace04.txt -s $(TSHREF) -a $(TSHARGS)
rtest05:
	$(DRIVER) -t trace05.txt -s $(TSHREF) -a $(TSHARGS)
rtest06:
	$(DRIVER) -t trace06.txt -s $(TSHREF) -a $(TSHARGS)
rtest07:
	$(DRIVER) -t trace07.txt -s $(TSHREF) -a $(TSHARGS)
rtest08:
	$(DRIVER) -t trace08.txt -s $(TSHREF) -a $(TSHARGS)
rtest09:
	$(DRIVER) -t trace09.txt -s $(TSHREF) -a $(TSHARGS)
rtest10:
	$(DRIVER) -t trace10.txt -s $(TSHREF) -a $(TSHARGS)
rtest11:
	$(DRIVER) -t trace11.txt -s $(TSHREF) -a $(TSHARGS)
rtest12:
	$(DRIVER) -t trace12.txt -s $(TSHREF) -a $(TSHARGS)
rtest13:
	$(DRIVER) -t trace13.txt -s $(TSHREF) -a $(TSHARGS)
rtest14:
	$(DRIVER) -t trace14.txt -s $(TSHREF) -a $(TSHARGS)
rtest15:
	$(DRIVER) -t trace15.txt -s $(TSHREF) -a $(TSHARGS)
rtest16:
	$(DRIVER) -t trace16.txt -s $(TSHREF) -a $(TSHARGS)


# clean up
clean:
	rm -f $(FILES) $(TSH)  *.o *~


