/*
 * SigprocFilterbankOutputUtility.h
 *
 *  Created on: 5 May 2016
 *      Author: vincentvillani
 */

#ifndef SIGPROCFILTERBANKOUTPUTUTILITY_H_
#define SIGPROCFILTERBANKOUTPUTUTILITY_H_

#include "SigprocFilterbankOutput.h"

#define TWO_LOW_BITS 3


//TODO: Add a routine to pack data back into 1, 2 or 4 bit arrays
//TODO: Can probably optimise this, some operations may not be needed
//nbits = number of bits to use per byte
inline void PackUnpackedData(unsigned char* unpackedData, unsigned char* packedData, int nbits, uint64_t unpackedDataLength)
{
	switch(nbits)
	{
		//Pack 1 byte into 2 bits
		case 2:
			//For each unpacked byte, pack it into a packed data byte as two bits
			for(uint64_t i = 0; i < unpackedDataLength; ++i)
			{
				//Get the two lowest bits of the unpackedData byte, shift it the number of times appropriate, OR it with the current output byte
				packedData[i / 4] = (unpackedData[i] & TWO_LOW_BITS) << (6 - ((i % 4) * 2)) | packedData[i / 4];
			}
			break;

		default:
			fprintf(stderr, "PackRawDataBlock: Invalid nbits value!\n");
			exit(1);
			break;
	}
}

//Write data to an output file
inline void WriteSigprocOutputFile(SigprocFilterbankOutput* SPFO, unsigned char* packedData, uint64_t byteSize)
{
	SPFO->outputFileStream.write((char*)packedData, byteSize);
}

#endif /* SIGPROCFILTERBANKOUTPUTUTILITY_H_ */
