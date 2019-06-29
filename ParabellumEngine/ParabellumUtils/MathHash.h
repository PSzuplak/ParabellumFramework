//-------------------------------------------------------------------------
//
// Hashes
//
//-------------------------------------------------------------------------



#ifndef __PRAGMAONCE_PARABELLUMUTILS_MATHHASH_H__
#define __PRAGMAONCE_PARABELLUMUTILS_MATHHASH_H__


namespace ParabellumUtils
{
	namespace Math
	{

		constexpr unsigned long long GenerateHash(unsigned long long hash, const char* str)
		{
			return (!*str ? hash : GenerateHash(((hash << 5) - hash) + *str, str + 1));
		}

		constexpr unsigned long long GenerateHash(const char* str)
		{
			return (!str ? 0 : GenerateHash(0, str));
		}




		// This struct forces to treat something as const - usefull to force compile time stuff
		template<unsigned long long constUINT64>
		struct Struct12
		{
			static const unsigned long long value = constUINT64;
		};


		// macro to create compile time hash
		#define C$(str) ParabellumUtils::Math::Struct12<ParabellumUtils::Math::GenerateHash(str)>::value


		// Create 64bit hash from string. Be aware that sometimes compilers does NOT create it in compile time! even if its constexpr
		constexpr unsigned long long operator "" _hash(const char* str, size_t length)
		{
			return GenerateHash(str);
		}

		constexpr char TakeChar(const char* str, unsigned int index)
		{
			return *(str + index);
		}

		constexpr char TakeChar(char cha)
		{
			return cha;
		}


	}
}
















#endif