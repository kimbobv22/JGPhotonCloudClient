/* Exit Games Common - C++ Client Lib
 * Copyright (C) 2004-2013 by Exit Games GmbH. All rights reserved.
 * http://www.exitgames.com
 * mailto:developer@exitgames.com
 */

#ifndef __BASE_CHAR_STRING_H
#define __BASE_CHAR_STRING_H

#include "Base.h"

namespace ExitGames
{
	namespace Common
	{
		class JString;

		class BaseCharString : public Base
		{
		public:
			virtual ~BaseCharString(void);
			virtual operator const char* (void) const = 0;
			virtual operator const EG_CHAR* (void) const = 0;
			virtual operator JString (void) const = 0;

			virtual const char*	cstr(void) const = 0;
			virtual JString JStringRepresentation(void) const = 0;
			virtual JString& toString(JString& retStr, bool withTypes=false) const;
		protected:
			char* mBuffer;
		};
	}
}

#endif