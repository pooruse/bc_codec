#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <float.h>
#include "sample_table.h"

#define out_t int16_t

#define NUM_OF_SAMPLE 256
#define NUM_OF_BASE_BAND 256
#define NUM_OF_PICK 256

struct pick_buf_t {
    double X;
    int band;
}pick_buf[NUM_OF_PICK];

int8_t x[NUM_OF_SAMPLE];
double X[NUM_OF_BASE_BAND];
out_t X_q[NUM_OF_BASE_BAND];
out_t output_buf[NUM_OF_PICK * 2];

int main(int argc, char **argv)
{
    FILE *infile;
    FILE *outfile;
    int rlen;
    int i, j;
    double d_tmp;
    double cos_res;
    int pick_index;
    double out_tmp;
    int out_index;
    
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
	for(i = 0; i < NUM_OF_BASE_BAND; i++){

	    d_tmp = i * M_PI / NUM_OF_SAMPLE;
	    for(j = 0; j < rlen; j++){
		
		cos_res = cos(d_tmp * ((double)j + 0.5));
		X[i] += (double)x[j] * cos_res;
	    }
	    
	    X[i] /= NUM_OF_SAMPLE;
	}

	// pick important freqencies
	for(i = 0; i < NUM_OF_PICK; i++){
	    out_tmp = 0;
	    for(j = 1; j < NUM_OF_BASE_BAND; j++){
		if(out_tmp < fabs(X[j])){
		    out_tmp = fabs(X[j]);
		    out_index = j;
		}
	    }
	    pick_buf[i].band = out_index;
	    pick_buf[i].X = X[out_index];
	    X[out_index] = 0;
	}

	// quantization
	for(i = 0; i < NUM_OF_PICK; i++){
	    output_buf[i * 2] = pick_buf[i].band;
	    out_tmp = pick_buf[i].X * 100;
	    output_buf[i * 2 + 1] = (out_t)out_tmp;
	}
	
	// write file
	fwrite(output_buf, sizeof(out_t), NUM_OF_PICK * 2, outfile);
	
	// end condition
	if(rlen != NUM_OF_SAMPLE){
	    break;
	}
    }

    fclose(outfile);
    fclose(infile);
    
    return 0;
}
