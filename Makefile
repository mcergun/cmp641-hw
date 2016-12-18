CC=gcc -Wall
SRCDIR=src
BUILDDIR=build
EXECUTABLE_S=single
EXECUTABLE_M1=multiple1
EXECUTABLE_M2=multiple2

all: single multiple1 multiple2

single: create_builddir $(BUILDDIR)/single.o $(BUILDDIR)/imagehelper.o
	$(CC) $(BUILDDIR)/single.o $(BUILDDIR)/imagehelper.o -o $(EXECUTABLE_S)

multiple1: create_builddir $(BUILDDIR)/multiple1.o $(BUILDDIR)/imagehelper.o
	$(CC) -fopenmp $(BUILDDIR)/multiple1.o $(BUILDDIR)/imagehelper.o -o $(EXECUTABLE_M1)

multiple2: create_builddir $(BUILDDIR)/multiple2.o $(BUILDDIR)/imagehelper.o
	$(CC) -fopenmp $(BUILDDIR)/multiple2.o $(BUILDDIR)/imagehelper.o -o $(EXECUTABLE_M2)

$(BUILDDIR)/imagehelper.o: $(SRCDIR)/imagehelper.c
	$(CC) $(SRCDIR)/imagehelper.c -c -o $(BUILDDIR)/imagehelper.o


$(BUILDDIR)/single.o: $(SRCDIR)/single.c
	$(CC) $(SRCDIR)/single.c -c -o $(BUILDDIR)/single.o

$(BUILDDIR)/multiple1.o: $(SRCDIR)/multiple1.c
	$(CC) -fopenmp $(SRCDIR)/multiple1.c -c -o $(BUILDDIR)/multiple1.o

$(BUILDDIR)/multiple2.o: $(SRCDIR)/multiple2.c
	$(CC) -fopenmp $(SRCDIR)/multiple2.c -c -o $(BUILDDIR)/multiple2.o

create_builddir:
	mkdir -p $(BUILDDIR)	

clean:
	rm -f $(BUILDDIR)/*.o
	rm -f $(EXECUTABLE_S)
	rm -f $(EXECUTABLE_M1)
	rm -f $(EXECUTABLE_M2)