#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include <float.h>
#include "sample_table.h"

#define NUM_OF_SAMPLE 256
#define NUM_OF_PICKS 80

uint8_t x[NUM_OF_SAMPLE];
double X[SIZE_OF_SAMPLE_TABLE];

int main(int argc, char **argv)
{
    FILE *infile;
    FILE *outfile;
    uint8_t w_buf[NUM_OF_PICKS * 2];
    int rlen;
    int i, j;
    double d_tmp;
    double base_fcy;
    int pick_index;

    // check parameter format
    if(argc < 3){
	printf("usage: encode [SRC] [DST]\n");
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
	rlen  = fread(x, 1, NUM_OF_SAMPLE, infile);

	// clear X sample table
	for(i = 0; i < SIZE_OF_SAMPLE_TABLE; i++){
	    X[i] = 0;
	}
	
	// time domain -> freqency domain
	base_fcy = 1.0 / ((double)NUM_OF_SAMPLE / 8000.0);
	for(i = 1; i < SIZE_OF_SAMPLE_TABLE; i++){
	    
	    d_tmp = (double)sample_table[i] / base_fcy;
	    d_tmp = 2.0 * M_PI * d_tmp / NUM_OF_SAMPLE;
	    
	    for(j = 0; j < rlen; j++){
		X[i] += (x[j] * cos(d_tmp * (double)j)) / SIZE_OF_SAMPLE_TABLE;
	    }
	}

	// pick the most top freqencies
	for(i = 0; i < NUM_OF_PICKS; i++){
	    d_tmp = -DBL_MAX;

	    // j start with 1, because we ignore frequency 0
	    for(j = 1; j < SIZE_OF_SAMPLE_TABLE; j++){
		if(d_tmp < X[j]){
		    d_tmp = X[j];
		    pick_index = j;
		}
	    }
	    w_buf[i * 2] = (uint8_t)pick_index;
	    w_buf[i * 2 + 1] = (uint8_t)d_tmp;
	    X[pick_index] = 0;
	}

	fwrite(w_buf, 1, NUM_OF_PICKS * 2, outfile);
	
	// end condition
	if(rlen != NUM_OF_SAMPLE){
	    break;
	}
    }

    fclose(outfile);
    fclose(infile);
    
    return 0;
}
