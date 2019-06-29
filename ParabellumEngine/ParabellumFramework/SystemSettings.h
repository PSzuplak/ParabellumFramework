#ifndef _SYSTEMSETTINGS_H_
#define _SYSTEMSETTINGS_H_


#define PF_COMPILER_TYPE_VISUALSTUDIO



// if set to 1 all calculations (VectorN and MatrixNxN) will be made inline (without function call)
// it will be possible by use other defines from VectorN and MatrixNxN headers
#define PF_MATH_FORCE_INLINE_CALCULATION 1


// if definied then all Vector and MAtrix calculation will be done using SIMD
#define PF_MATH_SIMD 







#endif