#include "stage_node_painter.h"
#include "engine/core/base/class_method_bind.h"
#include "engine/core/log/Log.h"
#include "engine/core/render/base/pipeline/render_pipeline.h"

#ifdef ECHO_EDITOR_MODE

#include <QtCore/QObject>

namespace Pipeline
{
	StageNodePainter::StageNodePainter(QGraphicsView* view, QGraphicsScene* scene, Echo::RenderStage* stage)
	{
		m_stage = stage;
		m_graphicsView = view;
		m_graphicsScene = scene;

		m_stage->onRenderBegin.connectClassMethod(this, Echo::createMethodBind(&StageNodePainter::onCaptureFrame));

		if (!m_rect)
		{
			initBoundary();

			Echo::Vector2 textPos(15.f, 15.f);
			m_text = m_graphicsScene->addSimpleText(m_stage->getName().c_str());
			m_text->setBrush(QBrush(m_style.m_fontColor));
			m_text->setParentItem(m_rect);
			m_text->setPos(textPos.x - getHalfWidth(), textPos.y);

			QPixmap addNew((Echo::Engine::instance()->getRootPath() + "engine/core/render/base/pipeline/editor/icon/import_dark.png").c_str());
			m_addAction = new QGraphicsPixmapItemCustom();
			m_addAction->setPixmap(addNew.scaled(QSize(16, 16)));
			m_addAction->setParentItem(m_rect);
			m_addAction->setAcceptHoverEvents(true);
			m_graphicsScene->addItem(m_addAction);

			m_addAction->setHoverEnterEventCb([](QGraphicsPixmapItem* item)
			{
				QPixmap addNew((Echo::Engine::instance()->getRootPath() + "engine/core/render/base/pipeline/editor/icon/import.png").c_str());
				item->setPixmap(addNew.scaled(QSize(16, 16)));
			});

			m_addAction->setHoverEnterLeaveCb([](QGraphicsPixmapItem* item)
			{
				QPixmap addNew((Echo::Engine::instance()->getRootPath() + "engine/core/render/base/pipeline/editor/icon/import_dark.png").c_str());
				item->setPixmap(addNew.scaled(QSize(16, 16)));
			});

			m_addAction->setMousePressEventCb([this](QGraphicsPixmapItem* item)
			{
				showAddQueueMenu();
			});
		}
	}

	StageNodePainter::~StageNodePainter()
	{
		reset();
	}

	void StageNodePainter::initBoundary()
	{
		m_rect = new QGraphicsRenderStageItem(nullptr, m_stage->getId());
		m_rect->setZValue(-1.f);
		m_rect->setPen(QPen(m_style.m_normalBoundaryColor, m_style.m_penWidth));
		m_rect->setFlag(QGraphicsItem::ItemIsFocusable, true);
		m_rect->setPos(QPointF(0.f, 0.f));
		m_graphicsScene->addItem(m_rect);

		// mouse press event
		m_rect->setMousePressEventCb([this](QGraphicsItem* item)
		{
			EditorApi.showObjectProperty(m_stage);
			m_needCaptureFrame = true;
		});

		m_rect->setKeyPressEventCb([this](QKeyEvent* event)
		{
			if (event->key() == Qt::Key_Delete)
				onDeleteThisRenderStage();
		});
	}

	void StageNodePainter::showAddQueueMenu()
	{
		if (!m_addMenu)
		{
			m_addMenu = EchoNew(QMenu(m_graphicsView));

			Echo::StringArray childClasses;
			Echo::Class::getChildClasses(childClasses, "IRenderQueue", true);
			for (const Echo::String& className : childClasses)
			{
				if (!Echo::Class::isVirtual(className))
				{
					QAction* renderQueueAction = new QAction(className.c_str());
					m_addMenu->addAction(renderQueueAction);

					EditorApi.qConnectAction(renderQueueAction, QSIGNAL(triggered()), this, Echo::createMethodBind(&StageNodePainter::onNewRenderQueue));
				}
			}
		}

		m_addMenu->exec(QCursor::pos());
	}

	void StageNodePainter::onNewRenderQueue()
	{
		if (m_stage)
		{
			QAction* action = qobject_cast<QAction*>(EditorApi.qSender());
			if (action)
			{
				Echo::String text = action->text().toStdString().c_str();

				Echo::IRenderQueue* renderQueue = dynamic_cast<Echo::IRenderQueue*>(Echo::Class::create(text));
				if (renderQueue)
				{
					renderQueue->setName("New " + text);
					m_stage->addRenderQueue(renderQueue);

					EditorApi.showObjectProperty(renderQueue);
				}
			}
		}
	}

	void StageNodePainter::onDeleteThisRenderStage()
	{
		Echo::RenderPipeline* pipeline = m_stage ? m_stage->getPipeline() : nullptr;
		if (pipeline)
		{
			if (QMessageBox::Yes == QMessageBox(QMessageBox::Warning, "Warning", "Do you really want to delete the selected render stage ?", QMessageBox::Yes | QMessageBox::No).exec())
			{
				EditorApi.showObjectProperty(pipeline);
				pipeline->deleteStage(m_stage);
			}
		}
	}

	void StageNodePainter::reset()
	{
		EchoSafeDeleteContainer(m_renderQueueNodePainters, RenderQueueNodePainter);

		if (m_rect)
			m_graphicsScene->removeItem(m_rect);

		m_stage = nullptr;
		m_graphicsView = nullptr;
		m_graphicsScene = nullptr;
		m_rect = nullptr;
		m_text = nullptr;
	}

	RenderQueueNodePainter* StageNodePainter::newRenderQueueNodePainter(Echo::IRenderQueue* renderQueue)
	{
		RenderQueueNodePainter* painter = EchoNew(RenderQueueNodePainter(m_graphicsView, m_graphicsScene, renderQueue));
		if (painter)
		{
			painter->setCaptureFrameCb([this](Echo::FrameBuffer* fb)
			{
				if (m_captureFrameCb)
					m_captureFrameCb(fb);
			});
		}

		return painter;
	}

	void StageNodePainter::updateRenderQueues(Echo::i32 x)
	{
		Echo::vector<Echo::Vector2>::type		poses;
		Echo::vector<Echo::IRenderQueue*>::type renderqueues;
		for (size_t y = 0; y < m_stage->getRenderQueues().size(); y++)
		{
			renderqueues.push_back(m_stage->getRenderQueues()[y]);
			poses.push_back(Echo::Vector2(x, y));
		}

		while (m_renderQueueNodePainters.size() > renderqueues.size())
		{
			EchoSafeDelete(m_renderQueueNodePainters.back(), RenderQueueNodePainter);
			m_renderQueueNodePainters.pop_back();
		}

		if (m_renderQueueNodePainters.size() < renderqueues.size())
		{
			for (size_t i = m_renderQueueNodePainters.size(); i < renderqueues.size(); ++i)
				m_renderQueueNodePainters.emplace_back(newRenderQueueNodePainter( renderqueues[i]));
		}

		for (size_t i = 0; i < renderqueues.size(); i++)
		{
			if (!m_renderQueueNodePainters[i] || m_renderQueueNodePainters[i]->getRenderQueue() != renderqueues[i])
			{
				EchoSafeDelete(m_renderQueueNodePainters[i], RenderQueueNodePainter);
				m_renderQueueNodePainters[i] = newRenderQueueNodePainter( renderqueues[i]);
			}
		}

		for (size_t i = 0; i < renderqueues.size(); i++)
		{
			m_renderQueueNodePainters[i]->update(poses[i].x, poses[i].y);
		}
	}

	void StageNodePainter::updateRenderQueueAddButtons(Echo::i32 xPos)
	{
		Echo::i32 count = m_stage->getRenderQueues().size() + 1;
		while (m_renderQueueAddButtons.size() > count)
		{
			EchoSafeDelete(m_renderQueueAddButtons.back(), RenderQueueAddButton);
			m_renderQueueAddButtons.pop_back();
		}

		if (m_renderQueueAddButtons.size() < count)
		{
			for (size_t i = m_renderQueueAddButtons.size(); i < count; ++i)
				m_renderQueueAddButtons.emplace_back(EchoNew(Pipeline::RenderQueueAddButton(m_graphicsScene, m_stage->getPipeline())));
		}

		for (size_t i = 0; i < count; i++)
		{
			m_renderQueueAddButtons[i]->update(xPos, i);
		}
	}

	void StageNodePainter::update(Echo::i32 xPos, bool isFinal)
	{
		if (m_rect)
		{
			bool enable = m_stage->isEnable();

			if (m_renderQueueSize != m_stage->getRenderQueues().size() || m_enable!=enable)
			{
				m_renderQueueSize = m_stage->getRenderQueues().size();
				m_height = 40.f + m_renderQueueSize * 56.f + 16.f;

				float halfHeight = m_height * 0.5f;

				QPainterPath path;
				path.addRoundedRect(QRectF(-getHalfWidth(), 0.f, getWidth(), m_height), m_style.m_cornerRadius, m_style.m_cornerRadius);
				m_rect->setPath(path);

				if (enable)
				{
					QLinearGradient gradient(QPointF(0.0, -halfHeight), QPointF(0.0, halfHeight));
					gradient.setColorAt(0.0, m_style.m_gradientColor0);
					gradient.setColorAt(0.03, m_style.m_gradientColor1);
					gradient.setColorAt(0.97, m_style.m_gradientColor2);
					gradient.setColorAt(1.0, m_style.m_gradientColor3);
					m_rect->setBrush(gradient);
				}
				else
				{
					m_rect->setBrush(QBrush(m_style.m_gradientColor1));
				}

				m_addAction->setPos(QPointF(-8.f, m_height - 24.f));
				m_enable = enable;
			}

			m_text->setText(m_stage->getName().c_str());

			// update position
			m_rect->setFlag(QGraphicsItem::ItemIsMovable, false);
			m_rect->setPos(xPos * (getWidth() + getSpace()), 0.f);

			// pen width
			float penWidth = enable ? m_style.m_penWidth : m_style.m_penWidth - 1;
			m_rect->setPen(QPen(m_rect->isFocused() ? m_style.m_selectedBoundaryColor : m_style.m_normalBoundaryColor, penWidth));
		}

		updateRenderQueues(xPos);
		updateRenderQueueAddButtons(xPos);
	}

	void StageNodePainter::onCaptureFrame()
	{
		if (m_needCaptureFrame)
		{
			if (m_stage)
			{
				Echo::FrameBuffer* fb = m_stage->getFrameBuffer();
				if (fb && m_captureFrameCb)
				{
					m_captureFrameCb(fb);
				}
			}

			m_needCaptureFrame = false;
		}
	}
}

#endif