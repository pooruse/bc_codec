#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include <float.h>
#include "sample_table.h"

#define NUM_OF_SAMPLE 256
#define NUM_OF_PICKS 80

uint8_t x[NUM_OF_SAMPLE];

int main(int argc, char **argv)
{
    FILE *infile;
    FILE *outfile;
    uint8_t r_buf[NUM_OF_PICKS * 2];
    int rlen;
    int i, j;
    double d_tmp;
    double quantization;
    double base_fcy;

    // check parameter format
    if(argc < 3){
	printf("usage: decode [SRC] [DST]\n");
	return 0;
    }

    // open source file
    infile = fopen(argv[1],"rb");
    if(infile == NULL){
	fprintf(stderr, "open %s fail\n", argv[1]);
	return 1;
    }

    // open distination file
    outfile = fopen(argv[2],"wb");
    if(outfile == NULL){
	fclose(infile);
	fprintf(stderr, "open %s fail\n", argv[2]);
	return 1;
    }

    while(1){
	// read input file
	rlen  = fread(r_buf, 1, NUM_OF_PICKS * 2, infile);

	// clear x array
	for(i = 0; i < NUM_OF_SAMPLE; i++){
	    x[i] = 0;
	}
	
	// freqency domain -> time domain
	base_fcy = 1 / ((double)NUM_OF_SAMPLE / 8000.0);
	for(i = 0; i < NUM_OF_PICKS; i++){
	    
	    d_tmp = (double)sample_table[r_buf[i * 2]] / base_fcy;
	    d_tmp = 2.0 * M_PI * d_tmp / NUM_OF_SAMPLE;
	    
	    for(j = 0; j < NUM_OF_SAMPLE; j++){
		quantization = r_buf[i * 2 + 1] * (cos(d_tmp * (double)j));
		x[j] += (uint8_t)quantization;
	    }
	}

	fwrite(x, 1, NUM_OF_SAMPLE, outfile);
	
	// end condition
	if(rlen != NUM_OF_PICKS * 2){
	    break;
	}
    }

    fclose(outfile);
    fclose(infile);
    
    return 0;
}
