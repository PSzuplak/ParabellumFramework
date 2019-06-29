//	------------------------------------------------------------------------------------------------
//
//
//
//
//	------------------------------------------------------------------------------------------------


#ifndef __PFSTRING_H_
#define __PFSTRING_H_

#include <string>



namespace ParabellumFramework
{

	//
	// 
	//
	typedef std::u32string STDStringType;
	//template<typename STDStringType = std::u32string>
	class PFString : public STDStringType
	{

		//
		// Constructors and destructors
		//
		public:
		PFString()
		{
		}


		~PFString()
		{
			STDStringType::~basic_string();
		}

		//
		// Method
		//
		public:

		//
		// Cast
		//
		public:

		operator const STDStringType::value_type*() const { return this->c_str(); }		
	
	};

}






#endif