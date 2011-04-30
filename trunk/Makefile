CC = gcc -m64
READLINE_DIR = lib/greadline
CFLAGS = -O2 -Wall -W -fomit-frame-pointer -I./include/ -I$(READLINE_DIR)/include/
#CFLAGS = -O0 -g3 -DDEBUG_MODE -Wall -W -I./include/ -I$(READLINE_DIR)/include
READLINE_CFLAGS= -Os -Wall -W -I$(READLINE_DIR)/include/
LDLIBS = 
target = glisp

objs = build/main.o build/glisp.o build/token.o build/parse.o build/eval.o build/hash.o build/func.o build/mem.o build/vm.o build/array.o\
	$(READLINE_DIR)/build/greadline.o $(READLINE_DIR)/build/greadline.o $(READLINE_DIR)/build/keyword.o $(READLINE_DIR)/build/term.o $(READLINE_DIR)/build/complete.o\
	$(READLINE_DIR)/build/history.o

.PHONY: all
all: $(target)

$(target): $(objs)
	$(CC) -o $@ $^ $(LDLIBS)

build/main.o : src/main.c
	$(CC) $(CFLAGS) -o $@ -c $^

build/glisp.o : src/glisp.c
	$(CC) $(CFLAGS) -o $@ -c $^

build/token.o : src/token.c
	$(CC) $(CFLAGS) -o $@ -c $^

build/parse.o : src/parse.c
	$(CC) $(CFLAGS) -o $@ -c $^

build/eval.o : src/eval.c
	$(CC) $(CFLAGS) -o $@ -c $^

build/hash.o : src/hash.c
	$(CC) $(CFLAGS) -o $@ -c $^

build/func.o : src/func.c
	$(CC) $(CFLAGS) -o $@ -c $^

build/mem.o : src/mem.c
	$(CC) $(CFLAGS) -o $@ -c $^

build/vm.o : src/vm.c
	$(CC) $(CFLAGS) -o $@ -c $^

build/array.o : src/array.c
	$(CC) $(CFLAGS) -o $@ -c $^

$(READLINE_DIR)/build/greadline.o : $(READLINE_DIR)/src/greadline.c
	$(CC) $(READLINE_CFLAGS) -o $@ -c $^

$(READLINE_DIR)/build/complete.o : $(READLINE_DIR)/src/complete.c
	$(CC) $(READLINE_CFLAGS) -o $@ -c $^

$(READLINE_DIR)/build/keyword.o : $(READLINE_DIR)/src/keyword.c
	$(CC) $(READLINE_CFLAGS) -o $@ -c $^

$(READLINE_DIR)/build/term.o : $(READLINE_DIR)/src/term.c
	$(CC) $(READLINE_CFLAGS) -o $@ -c $^

$(READLINE_DIR)/build/history.o : $(READLINE_DIR)/src/history.c
	$(CC) $(READLINE_CFLAGS) -o $@ -c $^

.PHONY: clean
clean:
	$(RM) -rf $(objs) $(target) *~
