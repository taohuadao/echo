#pragma once

#include "../physx_body.h"
#include "engine/core/gizmos/Gizmos.h"

namespace Echo
{
#ifdef ECHO_EDITOR_MODE
	class PhysxVehicleDrive4WEditor : public ObjectEditor
	{
	public:
		PhysxVehicleDrive4WEditor(Object* object);
		virtual ~PhysxVehicleDrive4WEditor();

		// get thumbnail
		virtual ImagePtr getThumbnail() const override;

		// on editor select this node
		virtual void onEditorSelectThisNode() override;

		// update self
		virtual void editor_update_self() override;

	private:
		Gizmos* m_gizmo = nullptr;
	};
#endif
}
