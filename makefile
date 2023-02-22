CC=gcc
CFLAG=-Wall -ansi -pedantic -DNDEBUG
LDFLAGS=-lMLV -lm
OPT=-O3
ALL=$(BIN)main.o $(BIN)graphic.o\
    $(BIN)game_manager.o $(BIN)relic.o\
    $(BIN)guardian.o $(BIN)player.o\
    $(BIN)terrain.o $(BIN)sound.o $(BIN)collision.o\
    $(BIN)file_manager.o $(BIN)mana.o

# -DNDEBUG  -fsanitize=address

# .exe
EXE=game
# .c
SRC=./src/
# .h
INC=./include/
# .o
BIN=./bin/

$(EXE): $(ALL)
	$(CC) $(ALL) -o $(EXE) $(CFLAG) $(LDFLAGS)

file_manager.o: $(SRC)file_manager.c $(INC)file_manager.h

collision.o: $(SRC)collision.c $(INC)collision.h

guardian.o:  $(SRC)guardian.c $(INC)guardian.h $(INC)collision.h $(INC)terrain.h $(INC)player.h

graphic.o:   $(SRC)graphic.c $(INC)graphic.h $(INC)player.h $(INC)guardian.h $(INC)file_manager.h

player.o:    $(SRC)player.c    $(INC)player.h   $(INC)terrain.h

game_manager.o: $(SRC)game_manager.c $(INC)game_manager.h $(INC)terrain.h\
                $(INC)player.h $(INC)guardian.h $(INC)sound.h $(INC)mana.h

relic.o: $(SRC)relic.c $(INC)relic.h $(INC)terrain.h

sound.o: $(SRC)sound.c $(INC)sound.h

terrain.o: $(SRC)terrain.c $(INC)terrain.h 

main.o: $(SRC)main.c $(INC)graphic.h $(INC)game_manager.h $(INC)collision.h\
        $(INC)file_manager.h $(SRC)mana.c

$(BIN)%.o:  $(SRC)%.c
	$(CC) -c $< -o $@ $(CFLAG)

clean:
	rm -f $(BIN)*.o
	rm $(EXE)
