/* Exit Games Common - C++ Client Lib
 * Copyright (C) 2004-2013 by Exit Games GmbH. All rights reserved.
 * http://www.exitgames.com
 * mailto:developer@exitgames.com
 */

#ifndef __BASE_LISTENER_H
#define __BASE_LISTENER_H

#include "Helpers/TypeName.h"

namespace ExitGames
{
	namespace Common
	{
		class BaseListener
		{
		public:
			virtual void debugReturn(const JString& string) = 0;
		};

		/** @file */

		/** @class BaseListener
			This class defines the listener interface for the debug
			callback mechanism of all classes, that inherit from Base.
			@details
			If some error occurs while using a subclass of Base, there
			is a callback mechanism that will send error messages about
			what went wrong, to help you debugging. To receive these
			error messages you have to derive from BaseListener and
			implement the virtual function in your derived class. Then
			register your class by calling the static function Base::setListener().
	   
			Implementing this listener is optional. If you do not do it,
			you do not get debug returns from Common classes in
			error cases, but you can still use these utility classes.
			@sa
			Base , Base::setListener()                                     */

			/** @fn BaseListener::debugReturn(const JString& string)
				This is the callback function for debug-messages from all
				Common-classes except for JString.
				@details
				Please refer to BaseListener for more information and
				a code example.
				@sa
				BaseListener                                          */
	}
}

#endif