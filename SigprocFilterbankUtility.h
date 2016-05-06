/*
 * SigprocFilterbankUtility.h
 *
 *  Created on: 5 May 2016
 *      Author: vincentvillani
 */

#ifndef SIGPROCFILTERBANKUTILITY_H_
#define SIGPROCFILTERBANKUTILITY_H_

#define HI4BITS 240
#define LO4BITS   15
#define HI2BITS 192
#define UPMED2BITS 48
#define LOMED2BITS 12
#define LO2BITS 3

/*----------------------------------------------------------------------------*/



#include <stdint.h>
#include <stdio.h>

#include "SigprocFilterbank.h"
#include "RawDataBlock.h"


//Fills up a rawDataBlock with data from a filterbank
void ReadFilterbankData(SigprocFilterbank* SPF, RawDataBlock* rawDataBlock)
{
	if(SPF->infile.eof() || SPF->infile.bad())
	{
		fprintf(stderr, "ReadFilterbankData: There is a problem with reading the filterbank file\n");
		rawDataBlock->usedDataLength = 0;
		return;
	}

	//Attempt to read in a whole block worth of data
	SPF->infile.read((char*)rawDataBlock->packedRawData, rawDataBlock->totalDataLength);

	//Set how much data was read in to the raw data block
	rawDataBlock->usedDataLength = SPF->infile.gcount();
}


void ReadFilterbankData(SigprocFilterbank* SPF, unsigned char* packedDataOutput, uint64_t bytesToRead, uint64_t& bytesRead)
{

	if(SPF->infile.eof() || SPF->infile.bad())
	{
		fprintf(stderr, "ReadFilterbankData: There is a problem with reading the filterbank file\n");
		bytesRead = 0;
		return;
	}

	//Attempt to read in the requested amout of data
	SPF->infile.read((char*)packedDataOutput, bytesToRead);

	//Note how many bytes were actually read
	bytesRead = SPF->infile.gcount();
}


/*Function to unpack 1,2 and 4 bit data
data is unpacked into an empty buffer
Note: Only unpacks big endian bit ordering
Originally written by Dr Ewan Barr
Modified by Vincent Villani (2016)
*/
void UnpackRawDataBlock(RawDataBlock* rawDataBlock, unsigned char* unpackedData, int nbits, int nbytes)
{
	int ii,jj;

	switch(nbits)
	{
		case 1:
		for(ii = 0; ii < nbytes; ++ii)
		{
			for(jj = 0; jj < 8; ++jj)
			{
				unpackedData[(ii * 8) + jj] = (rawDataBlock->packedRawData[ii] >> jj) & 1;
			}
		}
		break;

		case 2:
		for(ii = 0; ii < nbytes; ++ii)
		{
			unpackedData[(ii * 4) + 3] = rawDataBlock->packedRawData[ii] & LO2BITS;
			unpackedData[(ii * 4) + 2] = (rawDataBlock->packedRawData[ii] & LOMED2BITS) >> 2;
			unpackedData[(ii * 4) + 1] = (rawDataBlock->packedRawData[ii] & UPMED2BITS) >> 4;
			unpackedData[(ii * 4) + 0] = (rawDataBlock->packedRawData[ii] & HI2BITS) >> 6;
		}
		break;

		case 4:
		for(ii = 0; ii < nbytes; ++ii)
		{
			unpackedData[(ii * 2) + 1] = rawDataBlock->packedRawData[ii] & LO4BITS;
			unpackedData[(ii * 2) + 0] = (rawDataBlock->packedRawData[ii] & HI4BITS) >> 4;
		}
		break;
	}
}



void PrintFilterbankHeader(SigprocFilterbank* SPF)
{

	printf("\nNumber of Samples: %u\n", SPF->get_nsamps());
	printf("Number of Channels: %f\n", SPF->get_nchans());
	printf("Bits per Sample: %f\n", SPF->get_nbits());
	//printf("Polarisation channels: %lu", sigProc->get_)
	printf("Sample time: %f\n", SPF->get_tsamp());
	printf("First channel Frequency: %f\n", SPF->get_fch1());
	printf("Per channel bandwidth: %f\n\n", SPF->get_foff());
}


#endif /* SIGPROCFILTERBANKUTILITY_H_ */
