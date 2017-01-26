LDFLAGS=-lm
CFLAGS=-g3

all: decode encode

reader: reader.o
	gcc $^ $(CFLAGS) -o $@ $(LDFLAGS)

encode: encode.o sample_table.o
	gcc $^ $(CFLAGS) -o $@ $(LDFLAGS)

decode: decode.o sample_table.o
	gcc $^ $(CFLAGS) -o $@ $(LDFLAGS)

%.o:%.c
	gcc -c $< $(CFLAGS) -o $@

.PHONY: clean

clean:
	-rm *.o decode encode
