#pragma once

#include "engine/core/scene/node.h"
#include "physx_vehicle_chassis.h"
#include "physx_vehicle_wheel.h"

namespace Echo
{
	// https://docs.nvidia.com/gameworks/content/gameworkslibrary/physx/guide/Manual/Vehicles.html
	class PhysxVehicleDrive4W : public Node 
	{
		ECHO_CLASS(PhysxVehicleDrive4W, Node)

	public:
		typedef vector<PhysxVehicleWheel*>::type WheelList;

		// Surface Type
		enum SurfaceType
		{
			DrivableSurface = 0xffff0000,
			UndriveableSurface = 0x0000ffff
		};

	public:
		PhysxVehicleDrive4W();
		virtual ~PhysxVehicleDrive4W();

		// Get chassis
		PhysxVehicleChassis* getChassis();

		// Get wheel by index
		vector<PhysxVehicleWheel*>::type getAllWheels();

		// Reset
		void reset();

	public:
		// set the car back to its rest state
		void setToRestState();

		// set accel
		void setAccel(float accel);
		
		// set steer
		void setSteer(float steer);

		// brake
		void setBrake(float brake);

		// hand brake
		void setHandBrake(float handBrake);

		// set the car will use auto-gears
		void setUseAutoGears(bool useAutoGears);
		bool isUseAutoGears() const { return m_isUseAutoGears; }

	protected:
		// update
		virtual void updateInternal(float elapsedTime) override;

		// update simulation
		void updateSimulation(float elapsedTime);

		// setting up the vehicle
		void settingUp();
		void setupWheelsSimulationData(physx::PxVehicleWheelsSimData* wheelsSimData);
		void setupDriveSimData(physx::PxVehicleDriveSimData4W& driveSimData);
		void setupVehicleActor();
		void setupNonDrivableSurface(physx::PxFilterData& filterData);

	private:
		PhysxVehicleChassis*			m_chassis = nullptr;
		WheelList						m_wheels;
		physx::PxVehicleWheelsSimData*	m_wheelsSimData = nullptr;
		physx::PxRigidDynamic*			m_vehicleActor = nullptr;
		physx::PxVehicleDrive4W*		m_vehicleDrive4W = nullptr;
		bool							m_isUseAutoGears = true;
		float							m_throttleInput = 0.f;
		float							m_steeringInput = 0.f;
	};
}
