//-------------------------------------------------------------------------
//	Creates variable with unique name
//	The purpose of it is to use C feature to call object destructor 
//	when its being removing out of scope 
//-------------------------------------------------------------------------


#define CONCATENATE_DIRECT(str1, str2) str1##str2
#define CONCATENATE(str1, str2) CONCATENATE_DIRECT(str1, str2)

#define DECLARE_ANONYMOUS(variableName) CONCATENATE(variableName, __LINE__)

// create struct which have definied enum
#define FIXED_ENUM(name, suffix) struct CONCATENATE(name, suffix) { enum : unsigned long long { value = suffix,  }; };

/*
for Visual Studio where edit and continue
#ifdef _MSC_VER 
# define DECLARE_ANONYMOUS(variableName) CONCATENATE(variableName, __COUNTER__)
*/