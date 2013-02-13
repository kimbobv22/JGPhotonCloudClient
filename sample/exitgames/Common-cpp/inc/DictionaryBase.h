/* Exit Games Common - C++ Client Lib
 * Copyright (C) 2004-2013 by Exit Games GmbH. All rights reserved.
 * http://www.exitgames.com
 * mailto:developer@exitgames.com
 */

#ifndef __DICTIONARY_BASE_H
#define __DICTIONARY_BASE_H

#include "Hashtable.h"

namespace ExitGames
{
	namespace Common
	{
		class DictionaryBase: public Base
		{
		public:
			using ToString::toString;

			virtual ~DictionaryBase(void);

			DictionaryBase(const DictionaryBase& toCopy);
			DictionaryBase& operator=(const DictionaryBase& toCopy);

			bool operator==(const DictionaryBase &toCompare) const;
			bool operator!=(const DictionaryBase &toCompare) const;

			void removeAllElements(void);
			JString& toString(JString& retStr, bool withTypes=false) const;
			const Hashtable& getHashtable(void) const;
			unsigned int getSize(void) const;
			virtual const nByte* getKeyTypes(void) const;
			virtual const nByte* getValueTypes(void) const;
			virtual const unsigned int* getValueDimensions(void) const;
		protected:
			DictionaryBase(void);

			virtual DictionaryBase& assign(const DictionaryBase& toCopy);

			void put(const DictionaryBase& src);
			template<typename FKeyType, typename FValueType> void put(const FKeyType& key, const FValueType& val);
			template<typename FKeyType> void put(const FKeyType& key);
			template<typename FKeyType, typename FValueType> void put(const FKeyType& key, FValueType pVal, short size);
			template<typename FKeyType, typename FValueType> void put(const FKeyType& key, FValueType pVal, const short* const sizes);
			template<typename FKeyType, typename FValueType, typename FDummyType> const FValueType* getValue(const FKeyType& key, const FDummyType& /*dummyDeducer*/) const;
			template<typename FKeyType, typename FDummyType> const Object* getValue(const FKeyType& key, const FDummyType& /*dummyDeducer*/) const;
			template<typename FValueType, typename FDummyType> const FValueType* getValue(const Object& key, const FDummyType& /*dummyDeducer*/) const;
			template<typename FDummyType> const Object* getValue(const Object& key, const FDummyType& /*dummyDeducer*/) const;
			template<typename FKeyType, typename FDummyType> JVector<FKeyType> getKeys(const FDummyType& /*dummyDeducer*/) const;
			template<typename FDummyType> JVector<Object> getKeys(const FDummyType& /*dummyDeducer*/) const;
			template<typename FValueType, typename FDummyType> const FValueType& getElementAt(unsigned int index, const FDummyType& /*dummyDeducer*/) const;
			template<typename FDummyType> const Object& getElementAt(unsigned int index, const FDummyType& /*dummyDeducer*/) const;
			template<typename FValueType, typename FDummyType> FValueType& getElementAt(unsigned int index, const FDummyType& /*dummyDeducer*/);
			template<typename FDummyType> const Object& getElementAt(unsigned int index, const FDummyType& /*dummyDeducer*/);
		private:
			DictionaryBase(nByte* pKeyTypes, nByte* pValueTypes, unsigned int* pValueDimensions);
			virtual bool compare(const DictionaryBase &toCompare) const;
			virtual DictionaryBase* copy(short amount) const;

			class TypeInfo : public Base
			{
			public:
				using ToString::toString;

				TypeInfo(nByte* pKeyTypes, nByte* pValueTypes, unsigned int* pValueDimensions);
				TypeInfo(const TypeInfo& toCopy);
				TypeInfo& operator=(const TypeInfo& toCopy);
				~TypeInfo(void);

				JString& toString(JString& retStr, bool withTypes=false) const;
				const nByte* getKeyTypes(void) const;
				const nByte* getValueTypes(void) const;
				const unsigned int* getValueDimensions(void) const;
			private:
				void init(nByte* pKeyTypes, nByte* pValueTypes, unsigned int* pValueDimensions);
				void cleanup(void);
				JString& toStringRecursor(JString& retStr, bool withTypes, unsigned int recursionDepth=0) const;

				nByte* mpKeyTypes;
				nByte* mpValueTypes;
				unsigned int* mpValueDimensions;
			};

			Hashtable mHashtable;
			TypeInfo* mpTypeInfo;

			friend class Helpers::Utils;
			friend class Object;
		};



		template<typename FKeyType, typename FValueType>
		void DictionaryBase::put(const FKeyType& key, const FValueType& val)
		{
			mHashtable.put(key, val);
		}

		template<typename FKeyType>
		void DictionaryBase::put(const FKeyType& key)
		{
			mHashtable.put(key);
		}

		template<typename FKeyType, typename FValueType>
		void DictionaryBase::put(const FKeyType& key, FValueType pVal, short size)
		{
			mHashtable.put(key, pVal, size);
		}

		template<typename FKeyType, typename FValueType>
		void DictionaryBase::put(const FKeyType& key, FValueType pVal, const short* const sizes)
		{
			mHashtable.put(key, pVal, sizes);
		}

		template<typename FKeyType, typename FValueType, typename FDummyType>
		const FValueType* DictionaryBase::getValue(const FKeyType& key, const FDummyType& /*dummyDeducer*/) const
		{
			ValueObject<FValueType>* resObject = (ValueObject<FValueType>*)getHashtable().getValue(KeyObject<FKeyType>(key));
			return resObject?resObject->getDataAddress():NULL;
		}

		template<typename FKeyType, typename FDummyType>
		const Object* DictionaryBase::getValue(const FKeyType& key, const FDummyType& /*dummyDeducer*/) const
		{
			return getHashtable().getValue(key);
		}

		template<typename FValueType, typename FDummyType>
		const FValueType* DictionaryBase::getValue(const Object& key, const FDummyType& /*dummyDeducer*/) const
		{
			return getHashtable().getValue(key);
		}

		template<typename FDummyType>
		const Object* DictionaryBase::getValue(const Object& key, const FDummyType& /*dummyDeducer*/) const
		{
			return getHashtable().getValue(key);
		}

		template<typename FKeyType, typename FDummyType>
		JVector<FKeyType> DictionaryBase::getKeys(const FDummyType& /*dummyDeducer*/) const
		{
			JVector<FKeyType> keys;
			JVector<Object> keyObjs = getHashtable().getKeys();
			for(unsigned int i=0 ; i<keyObjs.getSize() ; ++i)
				keys.addElement(static_cast<KeyObject<FKeyType> >(keyObjs[i]).getDataCopy());
			return keys;
		}

		template<typename FValueType, typename FDummyType>
		const FValueType& DictionaryBase::getElementAt(unsigned int index, const FDummyType& /*dummyDeducer*/) const
		{
			return *((ValueObject<FValueType>&)getHashtable()[index]).getDataAddress();
		}

		template<typename FDummyType>
		const Object& DictionaryBase::getElementAt(unsigned int index, const FDummyType& /*dummyDeducer*/) const
		{
			return getHashtable()[index];
		}

		template<typename FValueType, typename FDummyType>
		FValueType& DictionaryBase::getElementAt(unsigned int index, const FDummyType& /*dummyDeducer*/)
		{
			return *((ValueObject<FValueType>&)getHashtable()[index]).getDataAddress();
		}

		template<typename FDummyType>
		const Object& DictionaryBase::getElementAt(unsigned int index, const FDummyType& /*dummyDeducer*/)
		{
			return getHashtable()[index];
		}
	}
}

#endif