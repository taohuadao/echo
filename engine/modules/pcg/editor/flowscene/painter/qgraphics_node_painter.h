#pragma once

#include "engine/core/editor/editor.h"

#ifdef ECHO_EDITOR_MODE

#include "engine/modules/pcg/pcg_flow_graph.h"
#include <QPen>
#include <QGraphicsScene>
#include <QtWidgets/QGraphicsItem>
#include "../item/qgraphics_round_item.h"
#include "../item/qgraphics_round_item_final.h"
#include "../item/qgraphics_connect_point_item.h"
#include "../item/qgraphics_connect_item.h"

namespace Procedural
{
	class PCGNodePainter
	{
	public:
		// Style
		struct Style
		{
			QColor m_normalBoundaryColor = QColor(132, 132, 132);
			QColor m_finalBoundaryColor = QColor(132, 132, 132, 0);
			QColor m_selectedBoundaryColor = QColor(255, 165, 0);
			QColor m_gradientColor0 = QColor(80, 80, 80);
			QColor m_gradientColor1 = QColor(80, 80, 80);
			QColor m_gradientColor2 = QColor(64, 64, 64);
			QColor m_gradientColor3 = QColor(58, 58, 58);
			QColor m_shadowColor = QColor(20, 20, 20);
			QColor m_fontColor = Qt::gray;
			QColor m_fontColorFaded = Qt::gray;
			QColor m_warningColor = QColor(128, 128, 0);
			QColor m_errorColor = Qt::red;
			QColor finalColor = QColor(54, 108, 179, 255);
			float  m_penWidth = 1.5f;
		};

	public:
		PCGNodePainter(QGraphicsView* view, QGraphicsScene* scene, Echo::PCGFlowGraph* flowGraph, Echo::PCGNode* pgNode);
		~PCGNodePainter();

		// reset
		void reset();

		// update
		void update();

	public:
		// pcg node
		Echo::PCGNode* getPCGNode() { return m_pcgNode; }

	protected:
		// build
		void buildInputConnectPoints();
		void buildOutputConnectPoints();

		// update display
		void updateInputConnectPoints();
		void updateOutputConnectPoints();

		// sync position
		void syncNodePosition();

	private:
		Style									m_style;
		Echo::PCGNode*							m_pcgNode = nullptr;
		QGraphicsView*							m_graphicsView = nullptr;
		class QGraphicsFlowScene*				m_graphicsScene = nullptr;
		QGraphicsRoundRectItem*					m_rect = nullptr;
		QGraphicsRoundRectItemFinal*			m_rectFinal = nullptr;
		float									m_rectFinalWidth = 15;
		std::vector<QGraphicsConnectPointItem*>	m_inputConnectionPoints;
		std::vector<QGraphicsConnectPointItem*>	m_outputConnectionPoints;
		float									m_width = 120;
		float									m_height = 40;
		float									m_connectPointRadius = 6.f;
		QGraphicsSimpleTextItem*				m_text = nullptr;
	};
	typedef Echo::vector<PCGNodePainter*>::type PCGNodePainters;
}

#endif