#include "physx_vehicle_wheel.h"
#include "../physx_module.h"

namespace Echo
{
	PhysxVehicleWheel::PhysxVehicleWheel()
		: Node()
	{

	}

	PhysxVehicleWheel::~PhysxVehicleWheel()
	{
	}

	void PhysxVehicleWheel::bindMethods()
	{
	}

	physx::PxConvexMesh* PhysxVehicleWheel::getPxWheelMesh()
	{
		if (!m_pxWheelMesh)
		{
			physx::PxVec3 points[2 * 16];
			for (physx::PxU32 i = 0; i < 16; i++)
			{
				const physx::PxF32 cosTheta = physx::PxCos(i * physx::PxPi * 2.0f / 16.0f);
				const physx::PxF32 sinTheta = physx::PxSin(i * physx::PxPi * 2.0f / 16.0f);
				const physx::PxF32 x = m_radius * cosTheta;
				const physx::PxF32 y = m_radius * sinTheta;
				points[2 * i + 0] = physx::PxVec3(x, y, -m_width / 2.0f);
				points[2 * i + 1] = physx::PxVec3(x, y, +m_width / 2.0f);
			}

			m_pxWheelMesh = createConvexMesh(points, 32);
		}

		return m_pxWheelMesh;
	}

	physx::PxConvexMesh* PhysxVehicleWheel::createConvexMesh(const physx::PxVec3* verts, const physx::PxU32 numVerts)
	{
		physx::PxConvexMeshDesc convexDesc;
		convexDesc.points.count = numVerts;
		convexDesc.points.stride = sizeof(physx::PxVec3);
		convexDesc.points.data = verts;
		convexDesc.flags = physx::PxConvexFlag::eCOMPUTE_CONVEX;

		physx::PxConvexMesh* convexMesh = NULL;
		physx::PxDefaultMemoryOutputStream buf;
		if (PhysxModule::instance()->getPxCooking()->cookConvexMesh(convexDesc, buf))
		{
			physx::PxDefaultMemoryInputData id(buf.getData(), buf.getSize());
			convexMesh = PhysxModule::instance()->getPxPhysics()->createConvexMesh(id);
		}

		return convexMesh;
	}

	physx::PxMaterial* PhysxVehicleWheel::getPxMaterial()
	{ 
		if (!m_material)
		{
			m_material = PhysxModule::instance()->getPxPhysics()->createMaterial(0.5f, 0.5f, 0.5f);
		}

		return m_material;
	}
}
