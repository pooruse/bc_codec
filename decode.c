#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include <float.h>
#include "sample_table.h"

#define NUM_OF_SAMPLE 256
#define NUM_OF_BASE_BAND 256

double X[NUM_OF_BASE_BAND];
double x[NUM_OF_SAMPLE];
int8_t x_q[NUM_OF_SAMPLE];


int main(int argc, char **argv)
{
    FILE *infile;
    FILE *outfile;
    int rlen;
    int i, j;
    double cos_res;
    double d_tmp;

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
	rlen  = fread(X, sizeof(double), NUM_OF_BASE_BAND, infile);

	// clear x array
	for(i = 0; i < NUM_OF_SAMPLE; i++){
	    x[i] = 0;
	}
	
	// freqency domain -> time domain
	for(i = 0; i < NUM_OF_BASE_BAND; i++){

	    d_tmp = M_PI / NUM_OF_SAMPLE * (i + 0.5);
	    for(j = 0; j < NUM_OF_SAMPLE; j++){
		cos_res = cos(d_tmp * (double)j);
		x[i] += (double)X[j] * cos_res;
	    }
	}

	// quantilization
	for(i = 0; i < NUM_OF_SAMPLE; i++){
	    x_q[i] = (int8_t)x[i] * 2;
	    //printf("%d\n", x_q[i]);
	}
	
	fwrite(x_q, 1, NUM_OF_SAMPLE, outfile);
	
	// end condition
	if(rlen != NUM_OF_BASE_BAND){
	    break;
	}
    }

    fclose(outfile);
    fclose(infile);
    
    return 0;
}
