/*
 * SigprocFilterbankOutput.h
 *
 *  Created on: 5 May 2016
 *      Author: vincentvillani
 */

#ifndef SIGPROCFILTERBANKOUTPUT_H_
#define SIGPROCFILTERBANKOUTPUT_H_

#include <fstream>

#include "SigprocHeader.h"

//Class to write out a Sigproc Filterbank
//Limitations:
//Requires a filterbank to copy its header information from
class SigprocFilterbankOutput
{


public:

	SigprocHeader header;
	std::string filename;

	std::ifstream inputFileStream;
	std::ofstream outputFileStream;


	SigprocFilterbankOutput(std::string headerStealFilepath, std::string outputFilename)
	{
		//Open the filterbank to steal the header from
		inputFileStream.open(headerStealFilepath, std::ifstream::in | std::ifstream::binary);

		if(inputFileStream.is_open() == false)
		{
			fprintf(stderr, "SigprocFilterbankOutput: unable to open input file %s\n", headerStealFilepath.c_str());
			exit(1);
		}

		//Read the header to find out how big it is
		//TODO: Add error checking
		read_header(inputFileStream, header);

		//Allocate enough space to read in the whole header
		unsigned char* rawHeaderData = new unsigned char[header.size];

		//Seek back to the beginning of the file
		inputFileStream.seekg(0, std::ios::beg);

		//Read the raw header data into the buffer
		inputFileStream.read((char*)rawHeaderData, header.size);

		//Open the output file and write the raw header data to it
		outputFileStream.open(outputFilename, std::ofstream::out | std::ofstream::binary);

		if(outputFileStream.is_open() == false)
		{
			fprintf(stderr, "SigprocFilterbankOutput: unable to open output file %s\n", outputFilename.c_str());
			exit(1);
		}

		outputFileStream.write((char*)rawHeaderData, header.size);

		delete [] rawHeaderData;


		//close the input filestream
		inputFileStream.close();

		//Leave the output one open, you can now write raw packed data to it
		//Note: std::ofstream should close the file on destruction of this object

	}


};



#endif /* SIGPROCFILTERBANKOUTPUT_H_ */
