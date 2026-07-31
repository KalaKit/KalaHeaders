//---------------------------------------------------------------------------
// string_utils.hpp
//
// Copyright (C) 2026 Lost Empire Entertainment
//
// This is free source code, and you are welcome to redistribute it under certain conditions.
// Read LICENSE.md for more information.
//
// Provides:
//   - various string conversions and functions to improve workflow with string operations
//---------------------------------------------------------------------------

#pragma once

#include <vector>
#include <string>
#include <cstring>
#include <algorithm>

#ifndef KDEBUG
	#if defined(_MSC_VER) && defined(_DEBUG)
		#define KDEBUG
	#elif (defined(__GNUC__) || defined(__clang__)) && !defined(NDEBUG)
		#define KDEBUG
	#endif
#endif

#ifndef rcast
	#define rcast reinterpret_cast
#endif
#ifndef scast
	#define scast static_cast
#endif
#ifndef ccast
	#define ccast const_cast
#endif

namespace KalaHeaders::KalaString
{
	using std::stoi;
	using std::stoll;
	using std::stoul;
	using std::stoull;
	using std::stod;
	using std::stod;
	using std::stold;

	using std::vector;
	using std::string;
	using std::string_view;
	using std::to_string;
	using std::search;
	using std::transform;
	using std::toupper;
	using std::tolower;
	using std::isalpha;
	using std::isdigit;
	using std::isspace;
	using std::memcpy;
	using std::memset;

	//
	// GENERAL FUNCTIONS
	//

	inline bool ContainsAlpha(string_view value)
	{
		for (char c : value)
		{
			if (isalpha(scast<unsigned char>(c))) return true;
		}

		return false;
	}
	inline bool ContainsNumber(string_view value)
	{
		for (char c : value)
		{
			if (isdigit(scast<unsigned char>(c))) return true;
		}

		return false;
	}
	inline bool ContainsSymbol(string_view value)
	{
		for (char c : value)
		{
			unsigned char uc = scast<unsigned char>(c);
			if (!isalpha(uc)
				&& !isdigit(uc)
				&& !isspace(uc))
			{
				return true;
			}
		}

		return false;
	}
	inline bool ContainsSpace(string_view value)
	{
		for (char c : value)
		{
			if (isspace(scast<unsigned char>(c))) return true;
		}

		return false;
	}
	//Returns true if string contains any unsafe file characters,
	//Safe: 'A-Z', 'a-z', '0-9', '_', '-', '.'
	inline bool ContainsUnsafeFileChar(string_view origin)
	{
		for (unsigned char c : origin)
		{
			if (!(isalnum(c)
				|| c == '_'
				|| c == '-'
				|| c == '.'))
			{
				return true;
			}
		}
		return false;
	}
	
	//Copies the value of the origin string_view within the bounds of the target char array,
	//does not fill empty chars after \0,
	//does not mutate output on empty input
	template<size_t N>
	inline constexpr void StringToCharArray(
		string_view inValue, 
		char (&outValue)[N])
	{
		//skip if there is no input data
		if (inValue.empty()) return;
		
		size_t len = inValue.size();
		if (len >= N) len = N - 1;
		
		memcpy(outValue, inValue.data(), len);
		outValue[len] = '\0';
	}
	
	//Fills in remaining space of char array after '\0',
	//does not mutate output if no null terminator exists
	template<size_t N>
	inline constexpr void ZeroPadCharArray(char(&outValue)[N])
	{
		//find the null terminator
		size_t i = 0;
		for (; i < N; i++) { if (outValue[i] == '\0') break; }
		
		//skip if no null terminator was found
		if (i == N) return;
		
		//zero-pad everything after the null-terminator
		memset(&outValue[i + 1], 0, N - (i + 1));
	}

	//Check if origin is the same as target, with optional case sensitivity flag
	inline constexpr bool StringsMatch(
		string_view origin,
		string_view target,
		bool ignoreCase = true)
	{
		//return false if origin or target is empty
		if (origin.empty()
			|| target.empty())
		{
			return false;
		}

		//case-sensitive compare
		if (!ignoreCase) return origin == target;

		//case-insensitive compare
		if (origin.size() != target.size()) return false;

		for (size_t i = 0; i < origin.size(); ++i)
		{
			if (scast<char>(tolower(scast<unsigned char>(origin[i])))
				!= scast<char>(tolower(scast<unsigned char>(target[i]))))
			{
				return false;
			}
		}

		return true;
	}
	
	//Split origin into a vector of chunks between each splitter,
	//keep strings between two tokens as a single string with preserved tokens
	inline vector<string> TokenizeString(
		string_view origin,
		char token,
		string_view splitter)
	{
		//return nothing if origin is empty
		if (origin.empty()) return{};
		
		vector<string> result{};
		string current{};
		bool inQuotes{};
		size_t i = 0;
		
		while (i < origin.size())
		{
			char c = origin[i];
			
			if (c == token)
			{
				inQuotes = !inQuotes;
				current += c; //keep the token itself
				++i;
				continue;
			}
			
			if (!inQuotes
				&& origin.compare(i, splitter.size(), splitter) == 0)
			{
				if (!current.empty())
				{
					result.emplace_back(current);
					current.clear();
				}
				
				i += splitter.size();
				continue;
			}
			
			current += c;
			++i;
		}
		
		if (!current.empty()) result.emplace_back(current);
		
		return result;
	}
	

	//Split origin into a vector of chunks between each splitter
	inline vector<string> SplitString(
		string_view origin,
		string_view splitter)
	{
		//return nothing if origin is empty
		if (origin.empty()) return{};

		vector<string> result{};
		size_t start{};
		size_t pos{};

		while ((pos = origin.find(splitter, start)) != string::npos)
		{
			result.emplace_back(origin.substr(start, pos - start));
			start = pos + splitter.length();
		}

		//push the remainder (or whole string if no delimiter was found)
		result.emplace_back(origin.substr(start));

		return result;
	}

	//Join all chunks in parts vector together into a single string
	//and add delimiter after each chunk except the last one
	inline string JoinString(
		const vector<string_view>& parts,
		string_view delimiter)
	{
		//return nothing if parts vector is empty
		if (parts.empty()) return "";

		size_t totalSize{};

		//reserve space to avoid many reallocations
		for (const auto& part : parts) totalSize += part.size();
		totalSize += delimiter.size() * (parts.size() - 1);

		string result{};
		result.reserve(totalSize);

		for (size_t i = 0; i < parts.size(); ++i)
		{
			result += parts[i];
			if (i + 1 < parts.size()) result += delimiter;
		}

		return result;
	}

	//Remove leading and trailing whitespace characters from origin
	inline string TrimString(string_view origin)
	{
		const char* whitespace = " \t\n\r\f\v";
		size_t start = origin.find_first_not_of(whitespace);
		if (start == string::npos) return ""; // all whitespace

		size_t end = origin.find_last_not_of(whitespace);
		return string(origin.substr(start, end - start + 1));
	}

	//Remove occurrences of target from origin,
	//if removeAll is true then all found occurences will be removed
	inline string RemoveFromString(
		string_view origin,
		string_view target,
		bool removeAll = false)
	{
		//return origin if target is empty
		if (target.empty()) return string(origin);

		string result(origin);
		size_t pos = result.find(target);

		//nothing was found, skip further actions
		if (pos == string::npos) return result;

		//remove first occurence
		result.erase(pos, target.length());

		//remove all remaining occurences if requested
		if (removeAll)
		{
			while ((pos = result.find(target, pos)) != string::npos)
			{
				result.erase(pos, target.length());
			}
		}

		return result;
	}

	//Replace occurences of target from origin with replacement,
	//if replaceAll is true then all found occurences will be replaced
	inline string ReplaceFromString(
		string_view origin,
		string_view target,
		string_view replacement,
		bool replaceAll = false)
	{
		//return origin if target is empty
		if (target.empty()) return string(origin);

		string result(origin);
		size_t pos = result.find(target);

		//nothing was found, skip further actions
		if (pos == string::npos) return result;

		//replace first occurence
		result.replace(pos, target.length(), replacement);

		//replace all remaining occurences if requested
		if (replaceAll)
		{
			pos += replacement.length();

			while ((pos = result.find(target, pos)) != string::npos)
			{
				result.replace(pos, target.length(), replacement);
				pos += replacement.length();
			}
		}

		return result;
	}

	//Replaces everything after the start of target with replacer and returns the result
	inline string ReplaceAfter(
		string_view origin, 
		string_view target, 
		string_view replacer = {})
	{
		if (origin.empty()) return {};
		if (target.empty()) return string(origin);

		size_t pos = origin.find(target);
		if (pos == string_view::npos) return string(origin);

		string result{};
		result.reserve(pos + replacer.size());

		result.append(origin.substr(0, pos));
		result.append(replacer);

		return result;
	}

	//Replaces everything before the end of target with replacer and returns the result
	inline string ReplaceBefore(
		string_view origin, 
		string_view target,
		string_view replacer = {})
	{
		if (origin.empty()) return {};
		if (target.empty()) return string(origin);

		size_t pos = origin.find(target);
		if (pos == string_view::npos) return string(origin);

		string result{};
		result.reserve(replacer.size() + origin.size() - pos - target.size());

		result.append(replacer);
		result.append(origin.substr(pos + target.size()));

		return result;
	}

	//Returns everything after the start of target
	inline string GetAfter(
		string_view origin, 
		string_view target)
	{
		if (origin.empty()) return {};
		if (target.empty()) return string(origin);

		size_t pos = origin.find(target);
		if (pos == string_view::npos) return string(origin);

		return string(origin.substr(pos + target.size()));
	}

	//Returns everything before the end of target
	inline string GetBefore(
		string_view origin, 
		string_view target)
	{
		if (origin.empty()) return {};
		if (target.empty()) return string(origin);

		size_t pos = origin.find(target);
		if (pos == string_view::npos) return string(origin);

		return string(origin.substr(0, pos));
	}

	//Set all letters of this string to uppercase letters
	inline string ToUpperString(string_view origin)
	{
		//return origin if target is empty
		if (origin.empty()) return "";

		string result = string(origin);

		transform(result.begin(), result.end(), result.begin(),
			[](unsigned char c) 
			{
				return scast<char>(toupper(c));
			});

		return result;
	}

	//Set all letters of this string to lowercase letters
	inline string ToLowerString(string_view origin)
	{
		//return origin if target is empty
		if (origin.empty()) return "";

		string result = string(origin);

		transform(result.begin(), result.end(), result.begin(),
			[](unsigned char c)
			{
				return scast<char>(tolower(c));
			});

		return result;
	}
}