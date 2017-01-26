#include <stdint.h>
#include <stdio.h>

#define in_t int8_t

int main(int argc, char **argv)
{
    FILE *f;
    in_t value;
    int rlen;
    
    if(argc != 2){
	printf("usage: reader [FILE]\n");
	return 0;
    }

    // open input file
    f = fopen(argv[1], "rb");
    if(f == NULL){
	fprintf(stderr, "open input file error\n");
	return 1;
    }
    while(1){
	rlen = fread(&value, sizeof(in_t), 1, f);
	
	if(rlen != sizeof(in_t)){
	    break;
	}

	printf("%d\n", (int)value);
    }

    fclose(f);
    return 0;
}
