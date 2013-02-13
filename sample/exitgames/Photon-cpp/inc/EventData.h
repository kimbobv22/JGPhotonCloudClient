/* Exit Games Photon - C++ Client Lib
* Copyright (C) 2004-2013 by Exit Games GmbH. All rights reserved.
* http://www.exitgames.com
* mailto:developer@exitgames.com
*/

#ifndef __EVENT_DATA_H
#define __EVENT_DATA_H

#include "CEventData.h"
#include "Common.h"

namespace ExitGames
{
	namespace Photon
	{
		class EventData
		{
		public:
			~EventData(void);

			EventData(const EventData& toCopy);
			EventData& operator=(const EventData& toCopy);

			const Common::Object& operator[](unsigned int index) const;

			Common::JString toString(bool withParameters=false, bool withParameterTypes=false) const;
			Common::Object getParameterForCode(nByte parameterCode) const;

			nByte getCode(void) const;
			const Common::Hashtable& getParameters(void) const;
		private:
			friend class PhotonPeer;

			EventData(const CEventData& cEventData);

			nByte mCode;
			Common::Hashtable mParameters;
		};
	}
}

#endif