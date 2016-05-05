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
	SigprocFilterbank SPF = SigprocFilterbank("/lustre/projects/p002_swin/surveys/SUPERB/2016-01-05-12:07:06/01/2016-01-05-12:07:06.fil");

	uint64_t storageByteSize = 15625 * 10;

	//Allocate space for 10 seconds worth of data at a time
	RawDataBlock rawDataBlock = RawDataBlock(storageByteSize, SPF.nbits);

	//Read it in a data block at a time
	while(SPF.hasReachedEOF() == false)
	{
		std::cout << "Just read " << rawDataBlock.usedDataLength << " bytes!" << std::endl;
	}


	std::cout << "Finished reading filterbank file!" << std::endl;


	return 0;
}
