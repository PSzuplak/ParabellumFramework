#ifndef _DLLIE_H_
#define _DLLIE_H_

#ifdef DLLIMPORT
#define XYZ_API __declspec(dllimport)
#else
#define XYZ_API __declspec(dllexport)
#endif

#endif