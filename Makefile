CC=gcc -Wall
SRCDIR=src
BUILDDIR=build

single: $(BUILDDIR)/single.o $(BUILDDIR)/imagehelper.o
	$(CC) $(BUILDDIR)/single.o $(BUILDDIR)/imagehelper.o -o single

$(BUILDDIR)/imagehelper.o: $(SRCDIR)/imagehelper.c
	$(CC) $(SRCDIR)/imagehelper.c -c -o $(BUILDDIR)/imagehelper.o


$(BUILDDIR)/single.o: $(SRCDIR)/single.c
	$(CC) $(SRCDIR)/single.c -c -o $(BUILDDIR)/single.o

clean:
	rm -f $(BUILDDIR)/*.o