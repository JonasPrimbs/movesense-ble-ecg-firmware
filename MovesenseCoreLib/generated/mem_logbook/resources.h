#pragma once
/***********************************************************************
* THIS FILE HAS BEEN GENERATED BY WBRES TOOL. DO NOT TRY TO CHANGE IT. *
***********************************************************************/
// Copyright (c) Suunto Oy 2014 - 2017. All rights reserved.

#include "whiteboard/Identifiers.h"
#include "whiteboard/ParameterList.h"
#include "whiteboard/Result.h"
#include "whiteboard/ResourceClient.h"

#include "whiteboard/builtinTypes/Array.h"
#include "whiteboard/builtinTypes/ByteStream.h"
#include "whiteboard/builtinTypes/Date.h"
#include "whiteboard/builtinTypes/DateTime.h"
#include "whiteboard/builtinTypes/Optional.h"
#include "whiteboard/builtinTypes/Structures.h"
#include "whiteboard/builtinTypes/Time.h"
#include "whiteboard/builtinTypes/Timestamp.h"
#include "whiteboard/builtinTypes/TypedEnum.h"
#include "whiteboard/builtinTypes/Vector2D.h"
#include "whiteboard/builtinTypes/Vector3D.h"
#include "whiteboard/builtinTypes/WrapperFor32BitPointer.h"

#define WB_RESOURCE_VALUE(whiteboardId, localResourceId, executionContextId) \
    static_cast<whiteboard::ResourceId::Value>( \
        (static_cast<uint32>(localResourceId) << 16) | \
        (static_cast<uint32>(whiteboardId) << 8) | \
        (static_cast<uint32>(executionContextId) << 4) | \
        (static_cast<uint32>(whiteboard::ID_INVALID_RESOURCE_INSTANCE)))

#define WB_CALLER_CONTEXT whiteboard::ID_INVALID_EXECUTION_CONTEXT


#include "../wb-resources/resources.h"
#include "../movesense_types/resources.h"

#define WB_EXEC_CTX_APPLICATION                  static_cast<whiteboard::ExecutionContextId>(0)
#define WB_EXEC_CTX_MEAS                         static_cast<whiteboard::ExecutionContextId>(1)

namespace WB_RES {

struct WB_ALIGN(8) LogEntry;
struct WB_ALIGN(4) LogEntries;

struct WB_ALIGN(8) LogEntry
{
	// Structure type identification and serialization
	typedef int Structure;
	static const whiteboard::LocalDataTypeId DATA_TYPE_ID = 13312;

	WB_ALIGN(4) uint32 id;
	WB_ALIGN(4) uint32 modificationTimestamp;
	WB_ALIGN(8) whiteboard::Optional< uint64 > size;
};

struct WB_ALIGN(4) LogEntries
{
	// Structure type identification and serialization
	typedef int Structure;
	static const whiteboard::LocalDataTypeId DATA_TYPE_ID = 13313;

	WB_ALIGN(4) whiteboard::Array< LogEntry > elements;
};

namespace LOCAL
{

struct ROOT;

struct MEM;

struct MEM_LOGBOOK;

struct MEM_LOGBOOK_ENTRIES
{
	static const whiteboard::ExecutionContextId EXECUTION_CONTEXT = WB_EXEC_CTX_APPLICATION;
	static const whiteboard::ResourceId::Value ID = WB_RESOURCE_VALUE(0, 13312, EXECUTION_CONTEXT);
	static const whiteboard::LocalResourceId LID = 13312;

	struct GET
	{
		typedef whiteboard::StronglyTypedResult<const LogEntries&, whiteboard::HTTP_CODE_CONTINUE> HTTP_CODE_CONTINUE;
		typedef whiteboard::StronglyTypedResult<const LogEntries&, whiteboard::HTTP_CODE_OK> HTTP_CODE_OK;
		typedef whiteboard::StronglyTypedResult<const whiteboard::NoType&, whiteboard::HTTP_CODE_NOT_FOUND> HTTP_CODE_NOT_FOUND;

		struct Parameters
		{
			struct STARTAFTERID
			{
				static const whiteboard::ParameterIndex Index = 0;

				typedef uint32 Type;
				typedef Type ConstReferenceType;
			};

			typedef STARTAFTERID Parameter1;

			static const whiteboard::ParameterIndex NUMBER_OF_PARAMETERS = 1;
		};

		/** Reference wrapper for strongly typed parameter list for /Mem/Logbook/Entries */
		class ParameterListRef
		{
		private:
			/** Prevent use of default constructor */
			ParameterListRef() DELETED;

			/** Prevent use of copy constructor */
			ParameterListRef(const ParameterListRef&) DELETED;

			/** Prevent use of assignment operator */
			const ParameterListRef& operator=(const ParameterListRef&) DELETED;

		public:
			/** Constructor that initializes this class from existing parameter list
			*
			* @param rParameterList Reference to parameter list that contains untyped parameters
			*/
			inline ParameterListRef(const whiteboard::ParameterList& rParameterList)
				: mrParameterList(rParameterList)
			{
			}

			/** Checks whether optional parameter STARTAFTERID has a value
			*
			* @return A value indicating whether the parameter has a value
			*/
			inline bool hasStartAfterId() const
			{
				if (mrParameterList.getNumberOfParameters() <= Parameters::STARTAFTERID::Index)
				{
					return false;
				}

				return mrParameterList[Parameters::STARTAFTERID::Index].getType() != whiteboard::WB_TYPE_NONE;
			}

			/** Gets STARTAFTERID parameter value
			*
			* @return Current parameter value
			*/
			inline Parameters::STARTAFTERID::ConstReferenceType getStartAfterId() const
			{
				return mrParameterList[Parameters::STARTAFTERID::Index].convertTo<Parameters::STARTAFTERID::ConstReferenceType>();
			}

		private:
			/** Reference to actual parameter list */
			const whiteboard::ParameterList& mrParameterList;
		};

		/** Compile time type checking */
		inline static void typeCheck(
			const whiteboard::Api::OptionalParameter<Parameters::STARTAFTERID::ConstReferenceType>& = whiteboard::NoType::NoValue)
		{
		}
	};

	struct POST
	{
		typedef whiteboard::StronglyTypedResult<uint32, whiteboard::HTTP_CODE_CREATED> HTTP_CODE_CREATED;

		struct Parameters
		{
			static const whiteboard::ParameterIndex NUMBER_OF_PARAMETERS = 0;
		};

		/** Compile time type checking */
		inline static void typeCheck()
		{
		}
	};

	struct DELETE
	{
		typedef whiteboard::StronglyTypedResult<const whiteboard::NoType&, whiteboard::HTTP_CODE_OK> HTTP_CODE_OK;

		struct Parameters
		{
			static const whiteboard::ParameterIndex NUMBER_OF_PARAMETERS = 0;
		};

		/** Compile time type checking */
		inline static void typeCheck()
		{
		}
	};
};

struct MEM_LOGBOOK_ISFULL
{
	static const whiteboard::ExecutionContextId EXECUTION_CONTEXT = WB_EXEC_CTX_APPLICATION;
	static const whiteboard::ResourceId::Value ID = WB_RESOURCE_VALUE(0, 13313, EXECUTION_CONTEXT);
	static const whiteboard::LocalResourceId LID = 13313;

	struct GET
	{
		typedef whiteboard::StronglyTypedResult<bool, whiteboard::HTTP_CODE_OK> HTTP_CODE_OK;

		struct Parameters
		{
			static const whiteboard::ParameterIndex NUMBER_OF_PARAMETERS = 0;
		};

		/** Compile time type checking */
		inline static void typeCheck()
		{
		}
	};

	struct SUBSCRIBE
	{
		typedef whiteboard::StronglyTypedResult<const whiteboard::NoType&, whiteboard::HTTP_CODE_OK> HTTP_CODE_OK;

		struct Parameters
		{
			static const whiteboard::ParameterIndex NUMBER_OF_PARAMETERS = 0;
		};

		/** Compile time type checking */
		inline static void typeCheck()
		{
		}
	};

	struct EVENT
	{
		typedef bool NotificationType;
		typedef NotificationType ConstReferenceNotificationType;

		struct Parameters
		{
			static const whiteboard::ParameterIndex NUMBER_OF_PARAMETERS = 0;
		};

		/** Compile time type checking */
		inline static void typeCheck(
			const whiteboard::Api::OptionalParameter<ConstReferenceNotificationType>&)
		{
		}
	};

	struct UNSUBSCRIBE
	{
		typedef whiteboard::StronglyTypedResult<const whiteboard::NoType&, whiteboard::HTTP_CODE_OK> HTTP_CODE_OK;

		struct Parameters
		{
			static const whiteboard::ParameterIndex NUMBER_OF_PARAMETERS = 0;
		};

		/** Compile time type checking */
		inline static void typeCheck()
		{
		}
	};
};

struct MEM_LOGBOOK_ISOPEN
{
	static const whiteboard::ExecutionContextId EXECUTION_CONTEXT = WB_EXEC_CTX_APPLICATION;
	static const whiteboard::ResourceId::Value ID = WB_RESOURCE_VALUE(0, 13314, EXECUTION_CONTEXT);
	static const whiteboard::LocalResourceId LID = 13314;

	struct GET
	{
		typedef whiteboard::StronglyTypedResult<bool, whiteboard::HTTP_CODE_OK> HTTP_CODE_OK;

		struct Parameters
		{
			static const whiteboard::ParameterIndex NUMBER_OF_PARAMETERS = 0;
		};

		/** Compile time type checking */
		inline static void typeCheck()
		{
		}
	};

	struct SUBSCRIBE
	{
		typedef whiteboard::StronglyTypedResult<bool, whiteboard::HTTP_CODE_OK> HTTP_CODE_OK;

		struct Parameters
		{
			static const whiteboard::ParameterIndex NUMBER_OF_PARAMETERS = 0;
		};

		/** Compile time type checking */
		inline static void typeCheck()
		{
		}
	};

	struct EVENT
	{
		typedef bool NotificationType;
		typedef NotificationType ConstReferenceNotificationType;

		struct Parameters
		{
			static const whiteboard::ParameterIndex NUMBER_OF_PARAMETERS = 0;
		};

		/** Compile time type checking */
		inline static void typeCheck(
			const whiteboard::Api::OptionalParameter<ConstReferenceNotificationType>&)
		{
		}
	};

	struct UNSUBSCRIBE
	{
		typedef whiteboard::StronglyTypedResult<const whiteboard::NoType&, whiteboard::HTTP_CODE_OK> HTTP_CODE_OK;

		struct Parameters
		{
			static const whiteboard::ParameterIndex NUMBER_OF_PARAMETERS = 0;
		};

		/** Compile time type checking */
		inline static void typeCheck()
		{
		}
	};
};

struct MEM_LOGBOOK_LOG;

struct MEM_LOGBOOK_LOG_LOGINDEX;

struct MEM_LOGBOOK_LOG_LOGINDEX_REMOVE
{
	static const whiteboard::ExecutionContextId EXECUTION_CONTEXT = WB_EXEC_CTX_APPLICATION;
	static const whiteboard::ResourceId::Value ID = WB_RESOURCE_VALUE(0, 13315, EXECUTION_CONTEXT);
	static const whiteboard::LocalResourceId LID = 13315;

	struct PUT
	{
		typedef whiteboard::StronglyTypedResult<const whiteboard::NoType&, whiteboard::HTTP_CODE_OK> HTTP_CODE_OK;
		typedef whiteboard::StronglyTypedResult<const whiteboard::NoType&, whiteboard::HTTP_CODE_RANGE_NOT_SATISFIABLE> HTTP_CODE_RANGE_NOT_SATISFIABLE;

		struct Parameters
		{
			struct LOGINDEX
			{
				static const whiteboard::ParameterIndex Index = 0;

				typedef int32 Type;
				typedef Type ConstReferenceType;
			};

			typedef LOGINDEX Parameter1;

			static const whiteboard::ParameterIndex NUMBER_OF_PARAMETERS = 1;
		};

		/** Reference wrapper for strongly typed parameter list for /Mem/Logbook/Log/{LogIndex}/Remove */
		class ParameterListRef
		{
		private:
			/** Prevent use of default constructor */
			ParameterListRef() DELETED;

			/** Prevent use of copy constructor */
			ParameterListRef(const ParameterListRef&) DELETED;

			/** Prevent use of assignment operator */
			const ParameterListRef& operator=(const ParameterListRef&) DELETED;

		public:
			/** Constructor that initializes this class from existing parameter list
			*
			* @param rParameterList Reference to parameter list that contains untyped parameters
			*/
			inline ParameterListRef(const whiteboard::ParameterList& rParameterList)
				: mrParameterList(rParameterList)
			{
			}

			/** Gets LOGINDEX parameter value
			*
			* @return Current parameter value
			*/
			inline Parameters::LOGINDEX::ConstReferenceType getLogIndex() const
			{
				return mrParameterList[Parameters::LOGINDEX::Index].convertTo<Parameters::LOGINDEX::ConstReferenceType>();
			}

		private:
			/** Reference to actual parameter list */
			const whiteboard::ParameterList& mrParameterList;
		};

		/** Compile time type checking */
		inline static void typeCheck(
			Parameters::LOGINDEX::ConstReferenceType)
		{
		}
	};
};

struct MEM_LOGBOOK_LOGGING
{
	static const whiteboard::ExecutionContextId EXECUTION_CONTEXT = WB_EXEC_CTX_APPLICATION;
	static const whiteboard::ResourceId::Value ID = WB_RESOURCE_VALUE(0, 13316, EXECUTION_CONTEXT);
	static const whiteboard::LocalResourceId LID = 13316;

	struct GET
	{
		typedef whiteboard::StronglyTypedResult<uint16, whiteboard::HTTP_CODE_OK> HTTP_CODE_OK;

		struct Parameters
		{
			static const whiteboard::ParameterIndex NUMBER_OF_PARAMETERS = 0;
		};

		/** Compile time type checking */
		inline static void typeCheck()
		{
		}
	};

	struct SUBSCRIBE
	{
		typedef whiteboard::StronglyTypedResult<const whiteboard::NoType&, whiteboard::HTTP_CODE_OK> HTTP_CODE_OK;
		typedef whiteboard::StronglyTypedResult<const whiteboard::NoType&, whiteboard::HTTP_CODE_LOCKED> HTTP_CODE_LOCKED;
		typedef whiteboard::StronglyTypedResult<const whiteboard::NoType&, whiteboard::HTTP_CODE_INTERNAL_SERVER_ERROR> HTTP_CODE_INTERNAL_SERVER_ERROR;
		typedef whiteboard::StronglyTypedResult<const whiteboard::NoType&, whiteboard::HTTP_CODE_INSUFFICIENT_STORAGE> HTTP_CODE_INSUFFICIENT_STORAGE;

		struct Parameters
		{
			static const whiteboard::ParameterIndex NUMBER_OF_PARAMETERS = 0;
		};

		/** Compile time type checking */
		inline static void typeCheck()
		{
		}
	};

	struct EVENT
	{
		typedef uint16 NotificationType;
		typedef NotificationType ConstReferenceNotificationType;

		struct Parameters
		{
			static const whiteboard::ParameterIndex NUMBER_OF_PARAMETERS = 0;
		};

		/** Compile time type checking */
		inline static void typeCheck(
			const whiteboard::Api::OptionalParameter<ConstReferenceNotificationType>&)
		{
		}
	};

	struct UNSUBSCRIBE
	{
		typedef whiteboard::StronglyTypedResult<const whiteboard::NoType&, whiteboard::HTTP_CODE_OK> HTTP_CODE_OK;

		struct Parameters
		{
			static const whiteboard::ParameterIndex NUMBER_OF_PARAMETERS = 0;
		};

		/** Compile time type checking */
		inline static void typeCheck()
		{
		}
	};
};

struct MEM_LOGBOOK_BYID;

struct MEM_LOGBOOK_BYID_LOGID;

struct MEM_LOGBOOK_BYID_LOGID_DATA
{
	static const whiteboard::ExecutionContextId EXECUTION_CONTEXT = WB_EXEC_CTX_APPLICATION;
	static const whiteboard::ResourceId::Value ID = WB_RESOURCE_VALUE(0, 13317, EXECUTION_CONTEXT);
	static const whiteboard::LocalResourceId LID = 13317;

	struct GET
	{
		typedef whiteboard::StronglyTypedResult<const whiteboard::ByteStream&, whiteboard::HTTP_CODE_CONTINUE> HTTP_CODE_CONTINUE;
		typedef whiteboard::StronglyTypedResult<const whiteboard::ByteStream&, whiteboard::HTTP_CODE_OK> HTTP_CODE_OK;
		typedef whiteboard::StronglyTypedResult<const whiteboard::NoType&, whiteboard::HTTP_CODE_NOT_FOUND> HTTP_CODE_NOT_FOUND;

		struct Parameters
		{
			struct LOGID
			{
				static const whiteboard::ParameterIndex Index = 0;

				typedef int32 Type;
				typedef Type ConstReferenceType;
			};

			typedef LOGID Parameter1;

			struct OFFSET
			{
				static const whiteboard::ParameterIndex Index = 1;

				typedef int32 Type;
				typedef Type ConstReferenceType;
			};

			typedef OFFSET Parameter2;

			static const whiteboard::ParameterIndex NUMBER_OF_PARAMETERS = 2;
		};

		/** Reference wrapper for strongly typed parameter list for /Mem/Logbook/byId/{LogId}/Data */
		class ParameterListRef
		{
		private:
			/** Prevent use of default constructor */
			ParameterListRef() DELETED;

			/** Prevent use of copy constructor */
			ParameterListRef(const ParameterListRef&) DELETED;

			/** Prevent use of assignment operator */
			const ParameterListRef& operator=(const ParameterListRef&) DELETED;

		public:
			/** Constructor that initializes this class from existing parameter list
			*
			* @param rParameterList Reference to parameter list that contains untyped parameters
			*/
			inline ParameterListRef(const whiteboard::ParameterList& rParameterList)
				: mrParameterList(rParameterList)
			{
			}

			/** Gets LOGID parameter value
			*
			* @return Current parameter value
			*/
			inline Parameters::LOGID::ConstReferenceType getLogId() const
			{
				return mrParameterList[Parameters::LOGID::Index].convertTo<Parameters::LOGID::ConstReferenceType>();
			}

			/** Checks whether optional parameter OFFSET has a value
			*
			* @return A value indicating whether the parameter has a value
			*/
			inline bool hasOffset() const
			{
				if (mrParameterList.getNumberOfParameters() <= Parameters::OFFSET::Index)
				{
					return false;
				}

				return mrParameterList[Parameters::OFFSET::Index].getType() != whiteboard::WB_TYPE_NONE;
			}

			/** Gets OFFSET parameter value
			*
			* @return Current parameter value
			*/
			inline Parameters::OFFSET::ConstReferenceType getOffset() const
			{
				return mrParameterList[Parameters::OFFSET::Index].convertTo<Parameters::OFFSET::ConstReferenceType>();
			}

		private:
			/** Reference to actual parameter list */
			const whiteboard::ParameterList& mrParameterList;
		};

		/** Compile time type checking */
		inline static void typeCheck(
			Parameters::LOGID::ConstReferenceType,
			const whiteboard::Api::OptionalParameter<Parameters::OFFSET::ConstReferenceType>& = whiteboard::NoType::NoValue)
		{
		}
	};
};

struct MEM_LOGBOOK_BYID_LOGID_DESCRIPTORS
{
	static const whiteboard::ExecutionContextId EXECUTION_CONTEXT = WB_EXEC_CTX_APPLICATION;
	static const whiteboard::ResourceId::Value ID = WB_RESOURCE_VALUE(0, 13318, EXECUTION_CONTEXT);
	static const whiteboard::LocalResourceId LID = 13318;

	struct GET
	{
		typedef whiteboard::StronglyTypedResult<const whiteboard::ByteStream&, whiteboard::HTTP_CODE_CONTINUE> HTTP_CODE_CONTINUE;
		typedef whiteboard::StronglyTypedResult<const whiteboard::ByteStream&, whiteboard::HTTP_CODE_OK> HTTP_CODE_OK;

		struct Parameters
		{
			struct LOGID
			{
				static const whiteboard::ParameterIndex Index = 0;

				typedef int32 Type;
				typedef Type ConstReferenceType;
			};

			typedef LOGID Parameter1;

			struct OFFSET
			{
				static const whiteboard::ParameterIndex Index = 1;

				typedef int32 Type;
				typedef Type ConstReferenceType;
			};

			typedef OFFSET Parameter2;

			static const whiteboard::ParameterIndex NUMBER_OF_PARAMETERS = 2;
		};

		/** Reference wrapper for strongly typed parameter list for /Mem/Logbook/byId/{LogId}/Descriptors */
		class ParameterListRef
		{
		private:
			/** Prevent use of default constructor */
			ParameterListRef() DELETED;

			/** Prevent use of copy constructor */
			ParameterListRef(const ParameterListRef&) DELETED;

			/** Prevent use of assignment operator */
			const ParameterListRef& operator=(const ParameterListRef&) DELETED;

		public:
			/** Constructor that initializes this class from existing parameter list
			*
			* @param rParameterList Reference to parameter list that contains untyped parameters
			*/
			inline ParameterListRef(const whiteboard::ParameterList& rParameterList)
				: mrParameterList(rParameterList)
			{
			}

			/** Gets LOGID parameter value
			*
			* @return Current parameter value
			*/
			inline Parameters::LOGID::ConstReferenceType getLogId() const
			{
				return mrParameterList[Parameters::LOGID::Index].convertTo<Parameters::LOGID::ConstReferenceType>();
			}

			/** Checks whether optional parameter OFFSET has a value
			*
			* @return A value indicating whether the parameter has a value
			*/
			inline bool hasOffset() const
			{
				if (mrParameterList.getNumberOfParameters() <= Parameters::OFFSET::Index)
				{
					return false;
				}

				return mrParameterList[Parameters::OFFSET::Index].getType() != whiteboard::WB_TYPE_NONE;
			}

			/** Gets OFFSET parameter value
			*
			* @return Current parameter value
			*/
			inline Parameters::OFFSET::ConstReferenceType getOffset() const
			{
				return mrParameterList[Parameters::OFFSET::Index].convertTo<Parameters::OFFSET::ConstReferenceType>();
			}

		private:
			/** Reference to actual parameter list */
			const whiteboard::ParameterList& mrParameterList;
		};

		/** Compile time type checking */
		inline static void typeCheck(
			Parameters::LOGID::ConstReferenceType,
			const whiteboard::Api::OptionalParameter<Parameters::OFFSET::ConstReferenceType>& = whiteboard::NoType::NoValue)
		{
		}
	};
};


} // namespace LOCAL

} // namespace WB_RES
