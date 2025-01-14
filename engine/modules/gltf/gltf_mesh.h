#pragma once

#include "engine/core/scene/render_node.h"
#include "engine/core/scene/node_path.h"
#include "engine/core/render/base/mesh/mesh.h"
#include "engine/core/render/base/shader/material.h"
#include "engine/core/render/base/proxy/render_proxy.h"
#include "gltf_res.h"
#include "gltf_skeleton.h"

namespace Echo
{
	class GltfMesh : public Render
	{
		ECHO_CLASS(GltfMesh, Render)

	public:
		GltfMesh();
		virtual ~GltfMesh();

		// set gltf resource
		const ResourcePath& getGltfRes() { return m_assetPath; }
		void setGltfRes(const ResourcePath& path);

		// set mesh index
		int getMeshIdx() const { return m_meshIdx; }
		void setMeshIdx(int meshIdx);

		// set primitive index
		int getPrimitiveIdx() { return m_primitiveIdx; }
		void setPrimitiveIdx(int primitiveIdx);

		// material
		Material* getMaterial() const { return m_material; }
		void setMaterial( Object* material);

		// Cast shadow
		bool isCastShadow() const { return m_castShadow; }
		void setCastShadow(bool castShadow);

		// skeleton
		const NodePath& getSkeletonPath() { return m_skeletonPath; }
		void setSkeletonPath(const NodePath& skeletonPath);

	protected:
		// build drawable
		void buildRenderable();

		// update
		virtual void updateInternal(float elapsedTime) override;

		// get global uniforms
		virtual void* getGlobalUniformValue(const String& name) override;

		// clear
		void clear();
		void clearRenderable();

		// gltf anim
		void syncGltfNodeAnim();
		void syncGltfSkinAnim();

		// light data
		void syncLightData();

	private:
		bool					m_renderableDirty = true;
		RenderProxyPtr			m_renderable;
		Matrix4					m_matWVP;
		ResourcePath			m_assetPath = ResourcePath("", ".gltf");
		GltfResPtr				m_asset;			                        // gltf asset ptr
		i32						m_nodeIdx = -1;			                       // node index in the asset, used by skeleton
		i32						m_meshIdx;			                        // mesh index in the asset
		i32						m_skinIdx;
		int						m_primitiveIdx;		                        // sub mesh index
		MaterialPtr				m_material;			                        // custom material
		bool					m_castShadow = true;
		NodePath				m_skeletonPath;
		bool					m_skeletonDirty;	                        // dirty flag
		GltfSkeleton*			m_skeleton;
		vector<Matrix4>::type	m_jointMatrixs;
		i32						m_iblDiffuseSlot;
		i32						m_iblSpecularSlot;
		i32						m_iblBrdfSlot;
	};
}
