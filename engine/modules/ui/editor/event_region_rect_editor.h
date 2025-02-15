#pragma once

#include "../base/image.h"
#include "engine/core/gizmos/Gizmos.h"

namespace Echo
{
#ifdef ECHO_EDITOR_MODE
    class UiEventRegionRectEditor : public ObjectEditor
    {
    public:
        UiEventRegionRectEditor(Object* object);
        virtual ~UiEventRegionRectEditor();
        
        // get camera2d icon, used for editor
        virtual ImagePtr getThumbnail() const override;

		// on editor update this
		virtual void editor_update_self() override;
        
    private:
		Gizmos*		m_gizmo = nullptr;
    };
    
#endif
}
