/*
 * SigprocFilterbank.h
 *
 *  Created on: 5 May 2016
 *      Author: vincentvillani
 */




#ifndef SIGPROCFILTERBANK_H_
#define SIGPROCFILTERBANK_H_


/*
  Copyright 2014 Ewan Barr

  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at

  http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.
*/
/*
  filterbank.hpp

  By Ewan Barr (2013)
  ewan.d.barr@gmail.com


  Modified by Vincent Villani (2016)
  villanivincent@gmail.com

  This file contians classes and methods for the reading, storage
  and manipulation of filterbank format data. Filterbank format
  can be any time-frequency data block. Time must be the slowest
  changing dimension.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include "SigprocHeader.h"

/*!
  \brief Base class for handling filterbank data.

  All time and frequency resolved data types should inherit
  from this class. Class presents virtual set and get methods
  for various requrired meta data. The filterbank data itself
  is stred in the *data pointer as unsigend chars.
*/
class Filterbank
{
protected:

	//Filterbank metadata
	//unsigned char* data; /*!< Pointer to filterbank data.*/
	unsigned int nsamps; /*!< Number of time samples. */
	unsigned int nchans; /*!< Number of frequecy channels. */
	unsigned char nbits; /*!< Bits per time sample. */
	float fch1; /*!< Frequency of top channel (MHz) */
	float foff; /*!< Channel bandwidth (MHz) */
	float tsamp; /*!< Sampling time (seconds) */

	/*!
	\brief Instantiate a new Filterbank object with metadata.

	Instantiate a new Filterbank object from an existing data
	pointer and metadata.

	\param data_ptr A pointer to a memory location containing filterbank data.
	\param nsamps The number of time samples in the data.
	\param nchans The number of frequency channels in that data.
	\param nbins The size of a single data point in bits.
	\param fch1 The centre frequency of the first data channel.
	\param foff The bandwidth of a frequency channel.
	\param tsamp The sampling time of the data.
	*/
	inline Filterbank(unsigned int nsamps,
		 unsigned int nchans, unsigned char nbits,
		 float fch1, float foff, float tsamp)
	:nsamps(nsamps),nchans(nchans),
	 nbits(nbits),fch1(fch1),foff(foff),tsamp(tsamp){}



	/*!
	\brief Instantiate a new default Filterbank object.

	Create a new Filterbank object with the data pointer and
	all metadata set to zero.
	*/
	inline Filterbank(void)
	:nsamps(0),nchans(0),
	 nbits(0),fch1(0.0),foff(0.0),tsamp(0.0){}


	//Free all allocated data
	inline virtual ~Filterbank()
	{
	}

public:

	/*!
	\brief Get the currently set sampling time.

	\return The currently set sampling time.
	*/
	inline virtual float get_tsamp(void){return tsamp;}

	/*!
	\brief Set the sampling time.

	\param tsamp The sampling time of the data (in seconds).
	*/
	inline virtual void set_tsamp(float tsamp){this->tsamp = tsamp;}

	/*!
	\brief Get the currently set channel bandwidth.

	\return The channel bandwidth (in MHz).
	*/
	inline virtual float get_foff(void){return foff;}

	/*!
	\brief Set the channel bandwidth.

	\param foff The channel bandwidth (in MHz).
	*/
	inline virtual void set_foff(float foff){this->foff = foff;}

	/*!
	\brief Get the frequency of the top channel.

	\return The frequency of channel 0 (in MHz)
	*/
	inline virtual float get_fch1(void){return fch1;}

	/*!
	\brief Set the frequency of the top channel.

	\param fch1 The frequency of channel 0 (in MHz).
	*/
	inline virtual void set_fch1(float fch1){this->fch1 = fch1;}

	/*!
	\brief Get the number of frequency channels.

	\return The number of frequency channels.
	*/
	inline virtual float get_nchans(void){return nchans;}

	/*!
	\brief Set the number of frequency channels.

	\param nchans The number of frequency channels in the data.
	*/
	inline virtual void set_nchans(unsigned int nchans){this->nchans = nchans;}

	/*!
	\brief Get the number of time samples in the data.

	\return The number of time samples.
	*/
	inline virtual unsigned int get_nsamps(void){return nsamps;}

	/*!
	\brief Set the number of time samples in data.

	\param nsamps The number of time samples.
	*/
	inline virtual void set_nsamps(unsigned int nsamps){this->nsamps = nsamps;}

	/*!
	\brief Get the number of bits per sample.

	\return The number of bits per sample.
	*/
	inline virtual float get_nbits(void){return nbits;}

	/*!
	\brief Set the number of bits per sample.

	\param nbits The number of bits per sample.
	*/
	inline virtual void set_nbits(unsigned char nbits){this->nbits = nbits;}

	/*!
	\brief Get the pointer to the filterbank data.

	\return The pointer to the filterbank data.
	*/
	//virtual unsigned char* get_data(void){return this->data;}

	/*!
	\brief Set the filterbank data pointer.

	\param data A pointer to a block of filterbank data.
	*/
	//virtual void set_data(unsigned char *data){this->data = data;}

	/*!
	\brief Get the centre frequency of the data block.

	\return The centre frequency of the filterbank data.
	*/

	inline virtual float get_cfreq(void)
	{
		if (foff < 0)
		  return fch1 + foff * nchans / 2;
		else
		  return fch1 - foff * nchans / 2;
	}
};





/*!
  \brief A class for handling Sigproc format filterbanks.

  A subclass of the Filterbank class for handling filterbank
  in Sigproc style/format from file. Filterbank memory buffer
  is allocated in constructor and deallocated in destructor.
*/

class SigprocFilterbank: public Filterbank
{

private:

public:

	//Total data size in bytes (excluding the header)
	size_t packedDataByteSize;
	std::ifstream infile; //Input file stream
	SigprocHeader hdr;
	std::string filename;

	/*!
	\brief Create a new SigprocFilterbank object from a file.

	Constructor opens a filterbank file reads the header
	It does not read in any of the actual data (non header data)
	Metadata is set from the filterbank header values.

	\param filename Path to a valid sigproc filterbank file.
	*/
	inline SigprocFilterbank(std::string filename)
	{

		//Copy the file name
		this->filename = filename;

		this->infile.open(filename.c_str(), std::ifstream::in | std::ifstream::binary);

		//Did the file open sucessfully?
		if(!infile.is_open())
		{
			fprintf(stderr, "SigprocFilterbank::SigprocFilterbank(): Unable to open %s filterbank file", filename.c_str());
			exit(1);
		}

		// Read the header
		read_header(this->infile, hdr);

		//Data size in bytes
		this->packedDataByteSize = (size_t) hdr.nsamples * hdr.nbits * hdr.nchans / 8;


		//Seek to the end of the header
		infile.seekg(hdr.size, std::ios::beg);

		/*
		//printf("Input size: %lu\n", input_size);
		this->data = new unsigned char [input_size];
		infile.seekg(hdr.size, std::ios::beg);
		// Read the data
		infile.read(reinterpret_cast<char*>(this->data), input_size);
		*/

		// Set the metadata
		this->nsamps = hdr.nsamples;
		this->nchans = hdr.nchans;
		this->tsamp = hdr.tsamp;
		this->nbits = hdr.nbits;
		this->fch1 = hdr.fch1;
		this->foff  = hdr.foff;

	}



	inline uint64_t getCurrentBytePosition()
	{
		uint64_t currentBytePos = infile.tellg();

		return currentBytePos - hdr.size;
	}


	inline bool hasReachedEOF()
	{
		return infile.eof();
	}


	inline uint64_t calcUnpackedByteSize()
	{
		return nsamps * nchans;
	}


};

#endif /* SIGPROCFILTERBANK_H_ */
