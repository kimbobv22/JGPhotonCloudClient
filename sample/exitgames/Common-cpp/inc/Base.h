/* Exit Games Common - C++ Client Lib
 * Copyright (C) 2004-2013 by Exit Games GmbH. All rights reserved.
 * http://www.exitgames.com
 * mailto:developer@exitgames.com
 */

#ifndef __BASE_H
#define __BASE_H

#include "BaseListener.h"

namespace ExitGames
{
	namespace Common
	{
		class Base : public ToString
		{
		public:
			static void setListener(const BaseListener* const baseListener);
		protected:
			static void debugReturn(const JString& string);
			static BaseListener* mpBaseListener;
		};
	}
}

#endif