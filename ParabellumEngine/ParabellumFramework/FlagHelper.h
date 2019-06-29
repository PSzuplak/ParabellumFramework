//	------------------------------------------------------------------------------------------------
//
//	To manage bit flags
//	
//
//	------------------------------------------------------------------------------------------------

#ifndef _FLAGHELPER_H_
#define _FLAGHELPER_H_




#define PF_FLAG_ISSETNO(flags, no)		(flags & (1 << no))
#define PF_FLAG_ISSET(flags, no)		(flags & no)

#define PF_FLAG_SETNO(flags, no)		( flags |= (1 << no) )
#define PF_FLAG_SET(flags, val)			( flags |= val )

#define PF_FLAG_UNSETNO(flags, no)		( flags &= ~(1 << no) )
#define PF_FLAG_UNSET(flags, val)		( flags &= ~val )
#define PF_FLAG_UNSET_ALL(flags, val)	( flags = 0 )

#define PF_FLAG_CLEARYOUNG(flags, count) flags &= ~((1 << alignment) - 1)


#endif