CC=gcc -Wall
CFLAGS=-fopenmp
SRCDIR=src
BUILDDIR=build
EXECUTABLE=imganalyze

all: imganalyze
	@echo
	@echo compile completed
	@echo

imganalyze: create_builddir $(BUILDDIR)/imganalyze.o $(BUILDDIR)/imagehelper.o
	@echo linking $(EXECUTABLE)
	@$(CC) $(CFLAGS) $(BUILDDIR)/imganalyze.o $(BUILDDIR)/imagehelper.o -o $(EXECUTABLE) -lm

$(BUILDDIR)/imagehelper.o: $(SRCDIR)/imagehelper.c
	@echo compiling imagehelper
	@$(CC) $(CFLAGS) $(SRCDIR)/imagehelper.c -c -o $(BUILDDIR)/imagehelper.o -lm

$(BUILDDIR)/imganalyze.o: $(SRCDIR)/imganalyze.c
	@echo compiling $(EXECUTABLE)
	@$(CC) $(CFLAGS) $(SRCDIR)/imganalyze.c -c -o $(BUILDDIR)/imganalyze.o -lm

create_builddir:
	@mkdir -p $(BUILDDIR)	

clean:
	@echo cleaning objects
	@rm -f $(BUILDDIR)/*.o
	@rm -f $(EXECUTABLE)