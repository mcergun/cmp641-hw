CC=gcc -Wall
SRCDIR=src
BUILDDIR=build
EXECUTABLE=imganalyze

all: imganalyze
	@echo
	@echo compile completed
	@echo

imganalyze: create_builddir $(BUILDDIR)/imganalyze.o $(BUILDDIR)/imagehelper.o
	@echo linking $(EXECUTABLE)
	@$(CC) -fopenmp $(BUILDDIR)/imganalyze.o $(BUILDDIR)/imagehelper.o -o $(EXECUTABLE)

$(BUILDDIR)/imagehelper.o: $(SRCDIR)/imagehelper.c
	@echo compiling imagehelper
	@$(CC) $(SRCDIR)/imagehelper.c -c -o $(BUILDDIR)/imagehelper.o

$(BUILDDIR)/imganalyze.o: $(SRCDIR)/imganalyze.c
	@echo compiling $(EXECUTABLE)
	@$(CC) -fopenmp $(SRCDIR)/imganalyze.c -c -o $(BUILDDIR)/imganalyze.o

create_builddir:
	@mkdir -p $(BUILDDIR)	

clean:
	@echo cleaning objects
	@rm -f $(BUILDDIR)/*.o
	@rm -f $(EXECUTABLE)