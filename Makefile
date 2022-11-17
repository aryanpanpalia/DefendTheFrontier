CC = g++
LD = $(CC)

TARGET = game

CPPFLAGS = -MMD -MP -Os -DOBJC_OLD_DISPATCH_PROTOTYPES -g

IGNORED_WARNINGS = -Wall

LIB_DIR = Libraries

INC_DIRS = -I$(LIB_DIR) -I.

OBJS = $(LIB_DIR)/FEHLCD.o $(LIB_DIR)/FEHRandom.o $(LIB_DIR)/FEHSD.o $(LIB_DIR)/tigr.o

ifeq ($(OS),Windows_NT)
	LDFLAGS = -lopengl32 -lgdi32
	EXEC = game.exe
else
	LDFLAGS = -framework OpenGL -framework Cocoa
	EXEC = game.out
endif

main: main.o $(OBJS)
	$(CC) $(CPPFLAGS) $(INC_DIRS) $(OBJS) main.o -o $(EXEC) $(LDFLAGS) $(IGNORED_WARNINGS)

main.o: main.cpp
	$(CC) $(IGNORED_WARNINGS) $(INC_DIRS) -c main.cpp

FEHLCD.o: $(LIB_DIR)/FEHLCD.cpp $(LIB_DIR)/FEHLCD.h $(LIB_DIR)/FEHUtility.o
	$(CC) $(IGNORED_WARNINGS) $(INC_DIRS) -c $(LIB_DIR)/FEHLCD.cpp

FEHUtility.o: $(LIB_DIR)/FEHUtility.cpp $(LIB_DIR)/FEHUtility.h
	$(CC) $(IGNORED_WARNINGS) $(INC_DIRS) -c $(LIB_DIR)/FEHUtility.cpp

FEHRandom.o: $(LIB_DIR)/FEHRandom.cpp $(LIB_DIR)/FEHRandom.h
	$(CC) $(IGNORED_WARNINGS) $(INC_DIRS) -c $(LIB_DIR)/FEHRandom.cpp

FEHSD.o: $(LIB_DIR)/FEHSD.cpp $(LIB_DIR)/FEHSD.h
	$(CC) $(IGNORED_WARNINGS) $(INC_DIRS) -c $(LIB_DIR)/FEHSD.cpp

tigr.o: $(LIB_DIR)/tigr.c $(LIB_DIR)/tigr.h
	$(CC) $(IGNORED_WARNINGS) $(INC_DIRS) -c $(LIB_DIR)/tigr.c

clean:
	rm $(LIB_DIR)/*.o $(LIB_DIR)/*.d
	rm *.o $(EXEC)