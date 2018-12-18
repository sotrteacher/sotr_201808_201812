DIROBJ := obj/
DIREXE := exec/
DIRHEA := include/
DIRSRC := src/

CFLAGS := -I$(DIRHEA) -c -Wall -std=c99
LDLIBS := -pthread -lrt -lm
CC := gcc

all : dirs manager decoder symbol_decoder

dirs:
	mkdir -p $(DIROBJ) $(DIREXE)

manager: $(DIROBJ)manager.o $(DIROBJ)semaphoreI.o 
	$(CC) -lm -o $(DIREXE)$@ $^ $(LDLIBS)

decoder: $(DIROBJ)decoder.o $(DIROBJ)semaphoreI.o 
	$(CC) -o $(DIREXE)$@ $^ $(LDLIBS)

symbol_decoder: $(DIROBJ)symbol_decoder.o $(DIROBJ)semaphoreI.o 
	$(CC) -o $(DIREXE)$@ $^ $(LDLIBS)

$(DIROBJ)%.o: $(DIRSRC)%.c
	$(CC) $(CFLAGS) $^ -o $@

test:
	./exec/manager 34.5.12.12.15.55 2 2

solution:
	./exec/manager 35.57.18.5.1.12.12.25.57.12.9.11.5.57.20.8.9.19.57.20.5.19.20.55 5 4

clean : 
	rm -rf *~ core $(DIROBJ) $(DIREXE) $(DIRHEA)*~ $(DIRSRC)*~
