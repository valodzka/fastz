#ifndef XTIME_H

#define XTIME_H

/*
** This file is in the public domain, so clarified as of
** 1996-06-05 by Arthur David Olson.
*/

/*
** ID
*/

#ifndef lint
#ifndef NOID
static char	xtimehid[] = "@(#)xtime.h	1.0";
#endif /* !defined NOID */
#endif /* !defined lint */

/*
** The structure that holds information for a time zone is opaque to
** all the APIs we provide; a zoneinfo_t is a pointer to that structure.
*/
struct state;

typedef struct state * zoneinfo_t;

zoneinfo_t	loadzone(const char * zonename);
void		freezone(zoneinfo_t zoneinfo);

struct tm *	localtime_zr(const time_t * timep, struct tm * tmp,
		const zoneinfo_t zoneinfo);

time_t		mktime_z(struct tm * tmp, const zoneinfo_t zoneinfo);

#endif /* !defined XTIME_H */
