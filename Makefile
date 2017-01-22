LDFLAGS=-lm
CFLAGS=-g3

encode: encode.o sample_table.o
	gcc $^ -o $@ $(LDFLAGS)


%.o:%.c
	gcc -c $< -o $@

.PHONY: clean

clean:
	-rm *.o decode.exe
