/*
 * main.cpp
 *
 *  Created on: 5 May 2016
 *      Author: vincentvillani
 */


#include <iostream>

#include "DeadSigproc.h"

int main()
{
	//Open the SPF file
	SigprocFilterbank SPF("/lustre/projects/p002_swin/surveys/SUPERB/2016-01-05-12:07:06/01/2016-01-05-12:07:06.fil");

	PrintFilterbankHeader(&SPF);

	uint64_t storageByteSize = 15625 * 1024 * 10;

	//Allocate space for 10 seconds worth of data at a time
	RawDataBlock rawDataBlock(storageByteSize, (unsigned char)SPF.get_nbits());

	uint64_t iteration = 0;

	//Read it in a data block at a time
	while(SPF.hasReachedEOF() == false)
	{
		ReadFilterbankData(&SPF, &rawDataBlock);

		std::cout << iteration << ": " << "just read " << rawDataBlock.usedDataLength << " bytes!" << std::endl;
		iteration += 1;
	}


	std::cout << "Finished reading filterbank file!" << std::endl;


	return 0;
}
