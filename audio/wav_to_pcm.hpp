//------------------------------------------------------------------------------
// wav_to_pcm.hpp
//
// Copyright (C) 2025 Lost Empire Entertainment
//
// This is free source code, and you are welcome to redistribute it under certain conditions.
// Read LICENSE.md for more information.
//
// Provides:
//   - Conversion from WAV files to raw pcm data as vector<u8>
//------------------------------------------------------------------------------

/*------------------------------------------------------------------------------
>>> WAV core layout (PCM only) <<<

Offset | Size | Field
-------|------|--------------------------------------------
0      | 4    | ChunkID = "RIFF"
4      | 4    | ChunkSize = 36 + Subchunk2Size
8      | 4    | Format = "WAVE"

12     | 4    | Subchunk1ID = "fmt "
16     | 4    | Subchunk1Size = 16 (for PCM)
20     | 2    | AudioFormat = 1 (PCM), 3 (IEEE float)
22     | 2    | NumChannels
24     | 4    | SampleRate
28     | 4    | ByteRate
32     | 2    | BlockAlign
34     | 2    | BitsPerSample

-- if Subchunk1Size > 16, extra format bytes follow --

??     | 4    | Subchunk2ID = "data"
??+4   | 4    | Subchunk2Size
??+8   | *    | PCM sample data
------------------------------------------------------------------------------*/

#pragma once

#include <vector>
#include <array>
#include <string>
#include <filesystem>
#include <fstream>
#include <cerrno>
#include <algorithm>
#include <utility>
#include <cstring>
#include <cstdint>
#include <cstddef>

namespace KalaHeaders
{
	using std::vector;
	using std::array;
	using std::string;
	using std::filesystem::path;
	using std::filesystem::current_path;
	using std::filesystem::weakly_canonical;
	using std::filesystem::exists;
	using std::filesystem::is_regular_file;
	using std::filesystem::perms;
	using std::filesystem::status;
	using std::ifstream;
	using std::ios;
	using std::size_t;
	using std::streamoff;
	using std::streamsize;
	using std::uint8_t;
	using std::uint16_t;
	using std::uint32_t;
	using std::move;
	using std::memcmp;
	using std::memcpy;
	using std::min;
	using std::find;
	
	using u8 = uint8_t;
	using u16 = uint16_t;
	using u32 = uint32_t;
	
	//File should be bigger than this + look for 'data' after this
	constexpr u32 EXPECTED_DATA_POS_START = 12;
	
	constexpr array<u32, 4> ALLOWED_SAMPLE_RATES =
	{
		44100,  //music, CD
		48000,  //film, games, default
		96000,  //high-res
		192000  //mastering-grade
	};
	constexpr array<u8, 2> ALLOWED_CHANNELS =
	{
		1, //mono
		2  //stereo
	};
	constexpr array<u8, 3> ALLOWED_BPS =
	{
		16, //16-bit integer
		24, //24-bit integer
		32  //32-bit float
	};
	
	enum class ConvertResult_WAV : u8
	{
		RESULT_SUCCESS                     = 0, //No errors, succeeded with compression
		
		//
		// FILE OPERATIONS
		//
		
		RESULT_FILE_NOT_FOUND              = 1,  //File does not exist
		RESULT_INVALID_EXTENSION           = 2,  //File is not '.wav'
		RESULT_UNAUTHORIZED_READ           = 3,  //Not authorized to read this file
		RESULT_FILE_LOCKED                 = 4,  //Cannot read this file, file is in use
		RESULT_UNKNOWN_READ_ERROR          = 5,  //Unknown file error when reading file
		RESULT_FILE_EMPTY                  = 6,  //There is no content inside this file
		
		//
		// WAV TO PCM CONVERSION
		//
		
		RESULT_UNSUPPORTED_FILE_SIZE       = 7,  //Always assume 'data' block is placed after 12 bytes
		
		RESULT_INVALID_RIFF_MAGIC          = 8,  //0-4 must be 'RIFF'
		RESULT_INVALID_WAVE_MAGIC          = 9,  //5-8 must be 'WAVE'
		RESULT_INVALID_FMT_CHUNK           = 10, //13-16 must be 'fmt '
		RESULT_INVALID_FORMAT_TYPE         = 11, //21-22 must be '1'

		RESULT_UNSUPPORTED_WAV_FORMAT      = 12, //21-22 must be '1'
		RESULT_UNSUPPORTED_CHANNELS        = 13, //23-24 must be within ALLOWED_CHANNELS
		RESULT_UNSUPPORTED_SAMPLE_RATE     = 14, //25-28 must be within ALLOWED_SAMPLE_RATES
		RESULT_UNSUPPORTED_BITS_PER_SAMPLE = 15, //35-36 must be within ALLOWED_BPS
		
		RESULT_MISSING_DATA_CHUNK          = 16  //'data' chunk must exist somewhere
	};
	
	struct PCMData_WAV
	{
		vector<u8> pcmData;  //PCM data in bytes
		
		u32 sampleRate;   //the sample rate of the original file, usually 44100hz or 48000hz
		u8 bitsPerSample; //the bps of the original file, usually 16-bit int, 24-bit int or 32-bit float (unused in mp3 and opus, defaults to 16)
		u8 channels;      //usually 1 channel (mono) or 2 channels (stereo)
	};
	
	inline bool ContainsSampleRate(u32 sr)
	{
		return find(
			ALLOWED_SAMPLE_RATES.begin(), 
			ALLOWED_SAMPLE_RATES.end(), 
			sr) 
			!= ALLOWED_SAMPLE_RATES.end();
	}
	inline bool ContainsChannel(u8 c)
	{
		return find(
			ALLOWED_CHANNELS.begin(), 
			ALLOWED_CHANNELS.end(), 
			c) 
			!= ALLOWED_CHANNELS.end();
	}
	inline bool ContainsBPS(u8 bps)
	{
		return find(
			ALLOWED_BPS.begin(), 
			ALLOWED_BPS.end(), 
			bps) 
			!= ALLOWED_BPS.end();
	}
	
	inline string ResultToString(ConvertResult_WAV result)
	{
		switch (result)
		{
		default: return "RESULT_UNKNOWN";
			
		case ConvertResult_WAV::RESULT_SUCCESS:
			return "RESULT_SUCCESS";
		
		case ConvertResult_WAV::RESULT_FILE_NOT_FOUND:
			return "RESULT_FILE_NOT_FOUND";
		case ConvertResult_WAV::RESULT_INVALID_EXTENSION:
			return "RESULT_INVALID_EXTENSION";
		case ConvertResult_WAV::RESULT_UNAUTHORIZED_READ:
			return "RESULT_UNAUTHORIZED_READ";
		case ConvertResult_WAV::RESULT_FILE_LOCKED:
			return "RESULT_FILE_LOCKED";
		case ConvertResult_WAV::RESULT_UNKNOWN_READ_ERROR:
			return "RESULT_UNKNOWN_READ_ERROR";
		case ConvertResult_WAV::RESULT_FILE_EMPTY:
			return "RESULT_FILE_EMPTY";
			
		case ConvertResult_WAV::RESULT_UNSUPPORTED_FILE_SIZE:
			return "RESULT_UNSUPPORTED_FILE_SIZE";
			
		case ConvertResult_WAV::RESULT_INVALID_RIFF_MAGIC:
			return "RESULT_INVALID_RIFF_MAGIC";
		case ConvertResult_WAV::RESULT_INVALID_WAVE_MAGIC:
			return "RESULT_INVALID_WAVE_MAGIC";
		case ConvertResult_WAV::RESULT_INVALID_FMT_CHUNK:
			return "RESULT_INVALID_FMT_CHUNK";
		case ConvertResult_WAV::RESULT_INVALID_FORMAT_TYPE:
			return "RESULT_INVALID_FORMAT_TYPE";
			
		case ConvertResult_WAV::RESULT_UNSUPPORTED_WAV_FORMAT:
			return "RESULT_UNSUPPORTED_WAV_FORMAT";
		case ConvertResult_WAV::RESULT_UNSUPPORTED_CHANNELS:
			return "RESULT_UNSUPPORTED_CHANNELS";
		case ConvertResult_WAV::RESULT_UNSUPPORTED_SAMPLE_RATE:
			return "RESULT_UNSUPPORTED_SAMPLE_RATE";
		case ConvertResult_WAV::RESULT_UNSUPPORTED_BITS_PER_SAMPLE:
			return "RESULT_UNSUPPORTED_BITS_PER_SAMPLE";
			
		case ConvertResult_WAV::RESULT_MISSING_DATA_CHUNK:
			return "RESULT_MISSING_DATA_CHUNK";
		}
		
		return "RESULT_UNKNOWN";
	}
	
	//Takes in a path to the .wav file and returns pcm data with a result enum
	inline ConvertResult_WAV Convert_WAV(
		const path& inFile,
		PCMData_WAV& outData)
	{
		//
		// PRE-READ CHECKS
		//
		
		if (!exists(inFile)) return ConvertResult_WAV::RESULT_FILE_NOT_FOUND;
		if (!is_regular_file(inFile)
			|| !inFile.has_extension()
			|| inFile.extension() != ".wav")
		{
			return ConvertResult_WAV::RESULT_INVALID_EXTENSION;
		}
		
		auto fileStatus = status(inFile);
		auto filePerms = fileStatus.permissions();
		
		bool canRead = (filePerms & (
			perms::owner_read
			| perms::group_read
			| perms::others_read))  
			!= perms::none;
		
		if (!canRead)
		{
			return ConvertResult_WAV::RESULT_UNAUTHORIZED_READ;
		}
				
		try
		{
			//
			// TRY TO OPEN AND READ
			//
			
			errno = 0;
			ifstream in(inFile, ios::in | ios::binary);
			if (in.fail()
				&& errno != 0)
			{
				if (errno == EBUSY
					|| errno == ETXTBSY)
				{
					return ConvertResult_WAV::RESULT_FILE_LOCKED;
				}
				else return ConvertResult_WAV::RESULT_UNKNOWN_READ_ERROR;
			}
			
			in.seekg(0, ios::end);
			size_t fileSize = static_cast<size_t>(in.tellg());
			
			if (fileSize == 0) return ConvertResult_WAV::RESULT_FILE_EMPTY;
			if (fileSize <= EXPECTED_DATA_POS_START) return ConvertResult_WAV::RESULT_UNSUPPORTED_FILE_SIZE;
			
			in.seekg(static_cast<streamoff>(0), ios::beg);
			
			//
			// PARSE FOUND DATA
			//
			
			vector<u8> rawData(fileSize);
			
			in.read(
				reinterpret_cast<char*>(rawData.data()),
				static_cast<streamsize>(fileSize));
				
			if (memcmp(rawData.data() + 0, "RIFF", 4) != 0) return ConvertResult_WAV::RESULT_INVALID_RIFF_MAGIC;
			if (memcmp(rawData.data() + 8, "WAVE", 4) != 0) return ConvertResult_WAV::RESULT_INVALID_WAVE_MAGIC;
			if (memcmp(rawData.data() + 12, "fmt ", 4) != 0) return ConvertResult_WAV::RESULT_INVALID_FMT_CHUNK;
			
			u16 audioFormat{};
			memcpy(&audioFormat, rawData.data() + 20, sizeof(u16));
			
			//only raw pcm for now, maybe support for IEEE (3) later
			if (audioFormat != 1) return ConvertResult_WAV::RESULT_INVALID_FORMAT_TYPE;
			
			u16 channels{};
			u32 correctSampleRate{};
			u16 bitsPerSample{};
			
			memcpy(&channels,          rawData.data() + 22, sizeof(u16));
			memcpy(&correctSampleRate, rawData.data() + 24, sizeof(u32));
			memcpy(&bitsPerSample,     rawData.data() + 34, sizeof(u16));
			
			u8 correctChannels = static_cast<u8>(channels);
			u8 correctBPS      = static_cast<u8>(bitsPerSample);
			
			if (!ContainsSampleRate(correctSampleRate)) return ConvertResult_WAV::RESULT_UNSUPPORTED_SAMPLE_RATE;
			if (!ContainsChannel(correctChannels))      return ConvertResult_WAV::RESULT_UNSUPPORTED_CHANNELS;
			if (!ContainsBPS(correctBPS))               return ConvertResult_WAV::RESULT_UNSUPPORTED_BITS_PER_SAMPLE;
			
			size_t dataOffset{}; //data chunk start
			size_t dataSize{};   //header claims to have this amount of bytes of data
			size_t dataEnd{};    //safety check to avoid EOF
			
			for (size_t i = EXPECTED_DATA_POS_START; i + 8 < rawData.size(); ++i)
			{
				if (memcmp(rawData.data() + i, "data", 4) == 0)
				{
					memcpy(&dataSize, rawData.data() + i + 4, 4);
					dataOffset = i + 8;
					break;
				}
			}
			
			dataEnd = min(rawData.size(), dataOffset + dataSize);
			
			if (dataOffset == 0
				|| dataOffset >= rawData.size())
			{
				return ConvertResult_WAV::RESULT_MISSING_DATA_CHUNK;
			}
			
			//
			// FINISHED PARSE, SEND DATA OUT
			//
				
			PCMData_WAV pData{};
			pData.pcmData.reserve(dataSize);
			
			pData.pcmData.assign(
				rawData.begin() + dataOffset,
				rawData.begin() + dataEnd);
				
			pData.sampleRate    = correctSampleRate;
			pData.channels      = correctChannels;
			pData.bitsPerSample = correctBPS;

			in.close();	
			
			outData = move(pData);
		}
		catch (...)
		{
			return ConvertResult_WAV::RESULT_UNKNOWN_READ_ERROR;
		}
		
		return ConvertResult_WAV::RESULT_SUCCESS;
	}
}