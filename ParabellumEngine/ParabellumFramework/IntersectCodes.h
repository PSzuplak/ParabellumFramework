#ifndef _INTERSECTCODES_H_
#define _INTERSECTCODES_H_

namespace ParabellumFramework
{	

	enum class IntersectCodes : ParabellumFramework::EUINT32
	{
		DISJOINT = 1,
		CONTAIN = 2,
		INTERSECT = 3,
	};

}

#endif