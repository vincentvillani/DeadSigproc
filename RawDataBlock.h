/*
 * RawDataBlock.h
 *
 *  Created on: 5 May 2016
 *      Author: vincentvillani
 */

#ifndef RAWDATABLOCK_H_
#define RAWDATABLOCK_H_

#include <stdint.h>
#include <stdlib.h>


//Raw data block for storing filterbank data loaded from disk in the RAM.
class RawDataBlock
{
public:

	unsigned char* packedRawData;

	//Info about how much this data block can store & other meta data
	unsigned char nBits; //Number of bits per atomic sample (copied from the filterbank header)
	uint64_t totalDataLength; //Number of elements in the 'packed raw data' array
	uint64_t usedDataLength; //How many elements in the packedRawData array are storing some actual data?

	RawDataBlock() : packedRawData(NULL), nBits(0), totalDataLength(0), usedDataLength(0)
	{
	}

	RawDataBlock(uint64_t arrayLength, unsigned char nBits)
	{
		totalDataLength = arrayLength;
		usedDataLength = 0;
		this->nBits = nBits;

		//Allocate memory
		packedRawData = new unsigned char[arrayLength];
	}
	/*
	RawDataBlock(unsigned char* rawData, uint64_t rawDataLength, unsigned char nBits, uint64_t usedDataLength) : packedRawData(rawData),
			nBits(nBits), totalDataLength(rawDataLength), usedDataLength(usedDataLength)
	{
	}
	*/
	~RawDataBlock()
	{
		if(packedRawData != NULL)
			delete [] packedRawData;
	}
};


#endif /* RAWDATABLOCK_H_ */
