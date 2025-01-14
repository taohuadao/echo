#pragma once

#include "engine/core/util/Any.hpp"
#include "engine/core/math/color.h"
#include "engine/core/render/base/image/pixel_format.h"
#include "engine/core/render/base/texture/texture.h"

#ifdef ECHO_EDITOR_MODE

namespace Echo
{
    class TextureAtlasPackage
    {
	public:
		// TRect
		template< typename T>
		struct TRect
		{
			T left, top, width, height;

			// construct
			TRect()
				: left(0), top(0), width(0), height(0)
			{}

			// construct
			TRect(T const& _left, T const& _top, T const& _width, T const& _height)
				: left(_left), top(_top), width(_width), height(_height)
			{}

			// calculate area
			T getArea() const { return width * height; }
		};
		typedef TRect<int> IRect;

		// Node
		struct Node
		{
			int			m_id;
			int			m_child[2];
			IRect		m_rc;
			any			m_userData;

			Node();

			// is leaf node
			bool IsLeaf() const;
		};

	public:
		TextureAtlasPackage(int width, int height);
		~TextureAtlasPackage();

		// insert data, return node idx
		int insert(Color* data, int width, int height, const any& userData);

		// overwrite data
		int overWrite(int nodeIdx, Color* data, int width, int height);

		// get node viewport
		const Vector4 getViewport(int nodeIdx) const;

		// get node info
		const Node& getNode(int nodeIdx) const { return m_nodes[nodeIdx]; }

		// get all nodes
		const vector<Node>::type& getAllNodes() const { return m_nodes; }

		// width & height
		int getWidth() const { return m_width; }
		int getHeight() const { return m_height; }

		// save image
		void saveImage(const String& path);

	private:
		// modify data
		int insert(int nodeIdx, Color* data, int width, int height);

	private:
		int					m_width = 0;
		int					m_height = 0;
		vector<Node>::type	m_nodes;
		void*				m_textureData = nullptr;
		PixelFormat			m_format = PF_RGBA8_UNORM;
    };
}

#endif
