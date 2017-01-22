LDFLAGS=-lm
CFLAGS=-g3

all: decode encode

encode: encode.o sample_table.o
	gcc $^ -o $@ $(LDFLAGS)

decode: decode.o sample_table.o
	gcc $^ -o $@ $(LDFLAGS)

%.o:%.c
	gcc -c $< -o $@

.PHONY: clean

clean:
	-rm *.o decode.exe
