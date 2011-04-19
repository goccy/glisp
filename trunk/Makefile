CC = gcc
CFLAGS = -O0 -g3 -Wall -W  -I./include/ -I/opt/local/include/ #-pipe -fomit-frame-pointer -funroll-loops -fprefetch-loop-arrays -falign-functions=4 -fforce-addr -floop-optimize -fschedule-insns
LDLIBS = -L/opt/local/lib/ -lreadline

target = glisp

objs = build/glisp.o build/token.o build/parse.o build/eval.o build/hash.o build/func.o build/mem.o build/vm.o

.PHONY: all
all: $(target)

$(target): $(objs)
	$(CC) -o $@ $^ $(LDLIBS)

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

.PHONY: clean
clean:
	$(RM) -rf $(objs) $(target) *~
