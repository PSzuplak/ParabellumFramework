//	------------------------------------------------------------------------------------------------
//
//	common function to handle all string conversions : strings 8 bit, string 16 bit etc.
//	making it ala boost but simpler interface using C++11 codecvt
//
//	Using :
//
//		output = cast< CAST_TYPE > ( input );
//
//		int output = cast<int>("3456");
//
//	------------------------------------------------------------------------------------------------

#ifndef _CONVERSIONS_H_
#define _CONVERSIONS_H_

#include <string>
#include <sstream>
#include <locale>
#include <codecvt>

#include "TypesC.h"
#include "Vector2D.h"
#include "Vector3D.h"
#include "Vector4D.h"

namespace ParabellumFramework
{

	//
	// Common signature
	//
	template<typename TDst, typename TSrc>
	TDst cast(TSrc src)
	{
		TDst dst;
		std::istringstream(src) >> dst;
		return dst;
	}

	//
	// Conversin between different string types
	//

	template<> std::string cast<std::string, const char*>(const char* src)
	{
		return std::string(src);
	}

	template<> std::u16string cast<std::u16string, const char16_t*>(const char16_t* src)
	{
		return std::u16string(src);
	}

	template<> std::u32string cast<std::u32string, const char32_t*>(const char32_t* src)
	{
		return std::u32string(src);
	}




	// Strings 8bit to X 
	
	template<> int cast<int, const char*>(const char* src)
	{
		return std::stoi(src);
	}

	template<> float cast<float, const char*>(const char* src)
	{
		return std::stof(src);
	}

	template<> double cast<double, const char*>(const char* src)
	{
		return std::stod(src);
	}

	template<> int cast<int, char*>(char* src)
	{
		return std::stoi(src);
	}

	template<> float cast<float, char*>(char* src)
	{
		return std::stof(src);
	}

	template<> double cast<double, char*>(char* src)
	{
		return std::stod(src);
	}
	
	
	template<> int cast<int, std::string&>(std::string& src)
	{
		return std::stoi(src);
	}

	template<> float cast<float, std::string&>(std::string& src)
	{
		return std::stof(src);
	}

	template<> double cast<double, std::string&>(std::string& src)
	{
		return std::stod(src);
	}


	// Strings 16bit to X


	// Strings 32bit to X


	// X to String8

	template<> std::string cast<std::string, int>(int src)
	{
		return std::move(std::to_string(src));
	}

	template<> std::string cast<std::string, float>(float src)
	{
		return std::move(std::to_string(src));
	}

	template<> std::string cast<std::string, double>(double src)
	{
		return std::move(std::to_string(src));
	}



}



#endif