/*
 * UnitTests.h
 *
 *  Created on: 5 May 2016
 *      Author: vincentvillani
 */

#ifndef UNITTESTS_H_
#define UNITTESTS_H_

#include "RawDataBlock.h"

#include "SigprocFilterbank.h"
#include "SigprocFilterbankUtility.h"

#include "SigprocFilterbankOutput.h"
#include "SigprocFilterbankOutputUtility.h"

void UnitTest_FilterbankReadInRawDataBlocks()
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
}


void UnitTest_ReadUnpackPackWrite()
{
	{
		//Open the SPF file
		SigprocFilterbank SPF("/lustre/projects/p002_swin/surveys/SUPERB/2016-01-05-12:07:06/01/2016-01-05-12:07:06.fil");

		PrintFilterbankHeader(&SPF);

		uint64_t packedStorageByteSize = SPF.packedDataByteSize; //size that can hold everything in this file

		//Allocate space for 10 seconds worth of data at a time
		RawDataBlock rawDataBlock(packedStorageByteSize, (unsigned char)SPF.get_nbits());

		uint64_t iteration = 0;

		//Read it in a data block at a time
		while(SPF.hasReachedEOF() == false)
		{
			ReadFilterbankData(&SPF, &rawDataBlock);

			std::cout << iteration << ": " << "just read " << rawDataBlock.usedDataLength << " bytes!" << std::endl;
			iteration += 1;
		}


		std::cout << "Finished reading filterbank file!" << std::endl;


		//Unpack the packed data
		unsigned char* unpackedData = new unsigned char[SPF.calcUnpackedByteSize()];

		UnpackRawDataBlock(&rawDataBlock, unpackedData, (unsigned char)SPF.get_nbits(), packedStorageByteSize);



		//Pack the unpacked data again
		PackUnpackedData(unpackedData, rawDataBlock.packedRawData, 2, SPF.calcUnpackedByteSize());


		//Write the repacked data out to a new filterbank file
		SigprocFilterbankOutput SPFO("/lustre/projects/p002_swin/surveys/SUPERB/2016-01-05-12:07:06/01/2016-01-05-12:07:06.fil", "UnitTest_ReadUnpackPackWrite.fil");

		//Write the repacked data out to the file
		WriteSigprocOutputFile(&SPFO, rawDataBlock.packedRawData, packedStorageByteSize);

		delete [] unpackedData;
	}


	//Read in both filterbank files to memory
	SigprocFilterbank SPFIn("/lustre/projects/p002_swin/surveys/SUPERB/2016-01-05-12:07:06/01/2016-01-05-12:07:06.fil");
	SigprocFilterbank SPFOut("UnitTest_ReadUnpackPackWrite.fil");

	//Print both header files
	PrintFilterbankHeader(&SPFIn);
	PrintFilterbankHeader(&SPFOut);

	//Read all data from each filterbank file in and make sure they are the same
	RawDataBlock rawDataBlockIn(SPFIn.packedDataByteSize, (unsigned char)SPFIn.get_nbits());
	RawDataBlock rawDataBlockOut(SPFOut.packedDataByteSize, (unsigned char)SPFOut.get_nbits());

	//Make sure the input byte sizes match
	if(SPFIn.packedDataByteSize != SPFOut.packedDataByteSize)
	{
		fprintf(stderr, "UnitTest_ReadUnpackPackWrite: Filterbank packed byte sizes do not match!\n");
		exit(1);
	}


	//Read byte by byte and make sure each byte is the same
	//If it is it shows that our pack/unpack functions work as intended
	for(uint64_t i = 0; i < SPFIn.packedDataByteSize; ++i)
	{
		if(rawDataBlockIn.packedRawData[i] != rawDataBlockOut.packedRawData[i])
		{
			fprintf(stderr, "UnitTest_ReadUnpackPackWrite: Bytes %llu are different!\n%u != %u\n", i, rawDataBlockIn.packedRawData[i], rawDataBlockOut.packedRawData[i]);
			exit(1);
		}
	}

}


#endif /* UNITTESTS_H_ */
