/* Exit Games Common - C++ Client Lib
 * Copyright (C) 2004-2013 by Exit Games GmbH. All rights reserved.
 * http://www.exitgames.com
 * mailto:developer@exitgames.com
 */

#ifndef __UTILS_H
#define __UTILS_H

#include "Base.h"

namespace ExitGames
{
	namespace Photon
	{
		class PhotonPeer;
		class LitePeer;
		class EventData;
		class OperationResponse;
		class OperationRequest;
	};

	namespace Common
	{
		class DictionaryBase;
		class Object;
		template<typename EType> class JVector;
		class Hashtable;

		namespace Helpers
		{
			class Utils : public Base
			{
			protected:
				static EG_Dictionary* convertToEGDictionary(const DictionaryBase* const origDict, EG_Dictionary* convDict=NULL);
				static DictionaryBase* convertToOOPDictionary(const EG_Dictionary* const origDict, DictionaryBase* convDict=NULL);
				static EG_HashTable* convertToEGHash(const Hashtable* const origHash, EG_HashTable* convHash=NULL);
				static EG_Vector* convertToEGVector(const JVector<Object>* const origVec);
				static EG_Object* convertToEGObject(const Object* const origObj, EG_Object* convObj=NULL);
				static Hashtable* convertToOOPHash(const EG_HashTable* const origHash, Hashtable* convHash=NULL);
				static JVector<Object>* convertToOOPVector(const EG_Vector* const origVec);
				static Object* convertToOOPObject(const EG_Object* const origObj, Object* convObj=NULL);
				static void convertObjectsInArrayToC(const Object& cppObj, void** cData, unsigned int recursionDepth=0);
				static void convertObjectsInCArrayToCPP(const EG_Object& cObj, void** cppData);
			private:
				friend class Photon::PhotonPeer;
				friend class Photon::LitePeer;
				friend class Photon::EventData;
				friend class Photon::OperationResponse;
				friend class Photon::OperationRequest;
			};
		}
	}
}

#endif