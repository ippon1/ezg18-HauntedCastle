/*
 * Copyright (c) 2008-2015, NVIDIA CORPORATION.  All rights reserved.
 *
 * NVIDIA CORPORATION and its licensors retain all intellectual property
 * and proprietary rights in and to this software, related documentation
 * and any modifications thereto.  Any use, reproduction, disclosure or
 * distribution of this software and related documentation without an express
 * license agreement from NVIDIA CORPORATION is strictly prohibited.
 */
// Copyright (c) 2004-2008 AGEIA Technologies, Inc. All rights reserved.
// Copyright (c) 2001-2004 NovodeX AG. All rights reserved.  

#ifndef PX_PHYSICS_COMMON_PX_TYPEINFO
#define PX_PHYSICS_COMMON_PX_TYPEINFO

/** \addtogroup common
@{
*/

#include "common/PxPhysXCommonConfig.h"

#ifndef PX_DOXYGEN
namespace physx
{
#endif

/**
\brief an enumeration of concrete classes inheriting from PxBase

Enumeration space is reserved for future PhysX core types, PhysXExtensions, 
PhysXVehicle and Custom application types.

@see PxBase, PxTypeInfo
*/

struct PxConcreteType
{
	enum Enum
	{
		eUNDEFINED,

		eHEIGHTFIELD,
		eCONVEX_MESH,
		eTRIANGLE_MESH,
		eCLOTH_FABRIC,

		eRIGID_DYNAMIC,
		eRIGID_STATIC,
		eSHAPE,
		eMATERIAL,
		eCONSTRAINT,
		eCLOTH,
		ePARTICLE_SYSTEM,
		ePARTICLE_FLUID,
		eAGGREGATE,
		eARTICULATION,
		eARTICULATION_LINK,
		eARTICULATION_JOINT,
		
		ePHYSX_CORE_COUNT,
        eFIRST_PHYSX_EXTENSION = 256,
		eFIRST_VEHICLE_EXTENSION = 512,
        eFIRST_USER_EXTENSION = 1024
	};
};

/** 
\brief a structure containing per-type information for types inheriting from PxBase

@see PxBase, PxConcreteType
*/

template<typename T> struct PxTypeInfo {};

#define PX_DEFINE_TYPEINFO(_name, _fastType) \
	class _name; \
	template <> struct PxTypeInfo<_name>	{	static const char* name() { return #_name;	}	enum { eFastTypeId = _fastType };	};

/* the semantics of the fastType are as follows: an object A can be cast to a type B if B's fastType is defined, and A has the same fastType.
 * This implies that B has no concrete subclasses or superclasses.
 */

PX_DEFINE_TYPEINFO(PxBase,				PxConcreteType::eUNDEFINED)
PX_DEFINE_TYPEINFO(PxMaterial,			PxConcreteType::eMATERIAL)
PX_DEFINE_TYPEINFO(PxConvexMesh,		PxConcreteType::eCONVEX_MESH)
PX_DEFINE_TYPEINFO(PxTriangleMesh,		PxConcreteType::eTRIANGLE_MESH)
PX_DEFINE_TYPEINFO(PxHeightField,		PxConcreteType::eHEIGHTFIELD)
PX_DEFINE_TYPEINFO(PxActor,				PxConcreteType::eUNDEFINED)
PX_DEFINE_TYPEINFO(PxRigidActor,		PxConcreteType::eUNDEFINED)
PX_DEFINE_TYPEINFO(PxRigidBody,			PxConcreteType::eUNDEFINED)
PX_DEFINE_TYPEINFO(PxRigidDynamic,		PxConcreteType::eRIGID_DYNAMIC)
PX_DEFINE_TYPEINFO(PxRigidStatic,		PxConcreteType::eRIGID_STATIC)
PX_DEFINE_TYPEINFO(PxArticulationLink,	PxConcreteType::eARTICULATION_LINK)
PX_DEFINE_TYPEINFO(PxArticulationJoint, PxConcreteType::eARTICULATION_JOINT)
PX_DEFINE_TYPEINFO(PxArticulation,		PxConcreteType::eARTICULATION)
PX_DEFINE_TYPEINFO(PxAggregate,			PxConcreteType::eAGGREGATE)
PX_DEFINE_TYPEINFO(PxConstraint,		PxConcreteType::eCONSTRAINT)
PX_DEFINE_TYPEINFO(PxShape,				PxConcreteType::eSHAPE)
PX_DEFINE_TYPEINFO(PxClothFabric,		PxConcreteType::eCLOTH_FABRIC)
PX_DEFINE_TYPEINFO(PxCloth,				PxConcreteType::eCLOTH)
PX_DEFINE_TYPEINFO(PxParticleBase,		PxConcreteType::eUNDEFINED)
PX_DEFINE_TYPEINFO(PxParticleFluid,		PxConcreteType::ePARTICLE_FLUID)
PX_DEFINE_TYPEINFO(PxParticleSystem,	PxConcreteType::ePARTICLE_SYSTEM)

#ifndef PX_DOXYGEN
} // namespace physx
#endif

/** @} */
#endif
