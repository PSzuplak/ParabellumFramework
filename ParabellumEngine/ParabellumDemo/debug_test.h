#ifndef _DEBUG_TEST_H_
#define _DEBUG_TEST_H_


typedef char yes;      // sizeof(yes) == 1
typedef char(&no)[2]; // sizeof(no)  == 2

template <class T>
struct is_class_or_union
{
	// SFINAE eliminates this when the type of arg is invalid
	template <class U>
	static double tester(int U::*p);

	// overload resolution prefers anything at all over "..."
	template <class U>
	static char tester(...);

	// see which overload is chosen when U == T
	static int const value = sizeof(tester<T>(0));

	//typedef mpl::bool_<value> type;
};

#endif