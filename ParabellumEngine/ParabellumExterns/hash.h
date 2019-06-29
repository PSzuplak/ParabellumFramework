//-------------------------------------------------------------------------
//
// Hashes
//
//-------------------------------------------------------------------------



#ifndef _QWERTYU111_H_
#define _QWERTYU111_H_


constexpr unsigned long long GenerateHash(unsigned long long hash, const char* str)
{
	return (!*str ? hash : GenerateHash(((hash << 5) - hash) + *str, str + 1));
}

constexpr unsigned long long GenerateHash(const char* str)
{
	return (!str ? 0 : GenerateHash(0, str));
}

// Create 64bit hash from string. Be aware that sometimes compilers does NOT create it in compile time!
constexpr unsigned long long operator "" _hash(const char* str, size_t length)
{
	return GenerateHash(str);
}


// This struct forces to treat something as const - usefull to force compile time stuff
template<unsigned long long constUINT64>
struct XYZ_API Struct12
{
	static const unsigned long long value = constUINT64;
};

// macro to create compile time hash
#define C$(str) Struct12<GenerateHash(str)>::value


















#endif