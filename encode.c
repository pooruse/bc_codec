#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include <float.h>
#include "sample_table.h"

#define NUM_OF_SAMPLE 256
#define NUM_OF_BASE_BAND 256

int8_t x[NUM_OF_SAMPLE];
double X[NUM_OF_BASE_BAND];

int main(int argc, char **argv)
{
    FILE *infile;
    FILE *outfile;
    int rlen;
    int i, j;
    double d_tmp;
    double cos_res;
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
	for(i = 0; i < NUM_OF_BASE_BAND; i++){

	    d_tmp = i * M_PI / NUM_OF_SAMPLE;
	    for(j = 0; j < rlen; j++){
		
		cos_res = cos(d_tmp * ((double)j + 0.5));
		X[i] += (double)x[j] * cos_res;
		/*
		if(i == 64){
		    printf("%d %04d * %f = %f\n",
			   i,
			   x[j],
			   cos_res,
			   (double)x[j] * cos_res);
		}
		*/
	    }
	    
	    X[i] /= NUM_OF_SAMPLE;
	    //printf("%3d %f\n",i , X[i]);
	}
	
	fwrite(X, sizeof(double), NUM_OF_BASE_BAND, outfile);
	
	// end condition
	if(rlen != NUM_OF_SAMPLE){
	    break;
	}
    }

    fclose(outfile);
    fclose(infile);
    
    return 0;
}
