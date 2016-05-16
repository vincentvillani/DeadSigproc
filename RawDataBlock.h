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
#include <stdio.h>
#include <new>


//Raw data block for storing filterbank data loaded from disk in the RAM.
class RawDataBlock
{
public:

	uint64_t rawDataBlockID;
	uint32_t workerThreadsCompletedProcessing;

	unsigned char* packedRawData;

	//Info about how much this data block can store & other meta data
	unsigned char nBits; //Number of bits per atomic sample (copied from the filterbank header)
	uint64_t totalDataLength; //Number of elements in the 'packed raw data' array
	uint64_t usedDataLength; //How many elements in the packedRawData array are storing some actual data?

	bool isLastBlock;

	inline RawDataBlock(uint64_t id, uint64_t arrayLength, unsigned char nBits)
	{
		this->totalDataLength = arrayLength;
		this->usedDataLength = 0;
		this->nBits = nBits;

		//Allocate memory
		this->packedRawData = new (std::nothrow) unsigned char[arrayLength];

		if(this->packedRawData == NULL)
		{
			fprintf(stderr, "RawDataBlock::RawDataBlock(): Unable to allocate %llu bytes for packedRawData\n");
			exit(1);
		}

		this->rawDataBlockID = id;

		this->workerThreadsCompletedProcessing = 0;

		this->isLastBlock = false;
	}
	/*
	RawDataBlock(unsigned char* rawData, uint64_t rawDataLength, unsigned char nBits, uint64_t usedDataLength) : packedRawData(rawData),
			nBits(nBits), totalDataLength(rawDataLength), usedDataLength(usedDataLength)
	{
	}
	*/
	inline ~RawDataBlock()
	{
		if(this->packedRawData != NULL)
			delete [] this->packedRawData;
	}
};


#endif /* RAWDATABLOCK_H_ */
