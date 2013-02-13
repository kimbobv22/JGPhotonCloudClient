/* Exit Games Common - C++ Client Lib
 * Copyright (C) 2004-2013 by Exit Games GmbH. All rights reserved.
 * http://www.exitgames.com
 * mailto:developer@exitgames.com
 */

#ifndef __ANSI_STRING_H
#define __ANSI_STRING_H

#include "BaseCharString.h"

namespace ExitGames
{
	namespace Common
	{
		class ANSIString : public BaseCharString
		{
		public:
			ANSIString(void);
			ANSIString(const ANSIString& str);
			ANSIString(const JString& str);
			ANSIString(const char* str);
			ANSIString(const EG_CHAR* str);
			~ANSIString(void);

			ANSIString& operator=(const ANSIString& Rhs);
			ANSIString& operator=(const JString& Rhs);
			ANSIString& operator=(const char* Rhs);
			ANSIString& operator=(const EG_CHAR* Rhs);

			operator const char* (void) const;
			operator const EG_CHAR* (void) const;
			operator JString (void) const;

			const char*	cstr(void) const;
			JString JStringRepresentation(void) const;
		};
	}
}

#endif