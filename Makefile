#Begimay KONUSHBAEVA
#Joel KONJOLO
CC=gcc
CFLAGS=-Iinclude
BIN=exec
EXEC=$(BIN)/projet $(BIN)/testTable $(BIN)/projet_0 $(BIN)/projet_1

OBJDIR=obj
SRCDIR=src


all : $(EXEC)

$(BIN)/projet.o : $(SRCDIR)/projet.c
	$(CC) $(SRCDIR)/projet.c
$(BIN)/projet : $(OBJDIR)/projet.o $(OBJDIR)/table.o
	$(CC) -o $(BIN)/projet $(OBJDIR)/projet.o $(OBJDIR)/table.o $(CFLAGS)
$(BIN)/testTable.o : $(SRCDIR)/testTable.c
	$(CC) $(SRCDIR)/testTable.c
$(BIN)/testTable : $(OBJDIR)/testTable.o $(OBJDIR)/table.o
	$(CC) -o $(BIN)/testTable $(OBJDIR)/testTable.o $(OBJDIR)/table.o  $(CFLAGS)
$(BIN)/projet_0.o : $(SRCDIR)/projet_0.c
	$(CC) $(SRCDIR)/projet_0.c
$(BIN)/projet_0 : $(OBJDIR)/projet_0.o
	$(CC) $(SRCDIR)/projet_0.c -o $(BIN)/projet_0
$(BIN)/projet_1.o : $(SRCDIR)/projet_1.c
	$(CC) $(SRCDIR)/projet_1.c
$(BIN)/projet_1 : $(OBJDIR)/projet_1.o
	$(CC) $(SRCDIR)/projet_1.c -o $(BIN)/projet_1
$(BIN)/table.o : $(SRCDIR)/table.c $(SRCDIR)/table.h
	$(CC) -o $(OBJDIR)/table.o -c $(SRCDIR)/table.c $(CFLAGS)

$(OBJDIR)/%.o : $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c -o $@ $<
clean :
	rm  $(OBJDIR)/* $(BIN)/*