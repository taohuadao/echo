#include "rvo_agent.h"
#include "rvo_module.h"
#include "engine/core/main/engine.h"

namespace Echo
{
	RvoAgent::RvoAgent()
	{

	}

	RvoAgent::~RvoAgent()
	{
	}

	void RvoAgent::bindMethods()
	{
		CLASS_BIND_METHOD(RvoAgent, getRadius);
		CLASS_BIND_METHOD(RvoAgent, setRadius);
		CLASS_BIND_METHOD(RvoAgent, getSpeed);
		CLASS_BIND_METHOD(RvoAgent, setSpeed);
		CLASS_BIND_METHOD(RvoAgent, getGoal);
		CLASS_BIND_METHOD(RvoAgent, setGoal);

		CLASS_REGISTER_PROPERTY(RvoAgent, "Radius", Variant::Type::Real, getRadius, setRadius);
		CLASS_REGISTER_PROPERTY(RvoAgent, "Speed", Variant::Type::Real, getSpeed, setSpeed);
		CLASS_REGISTER_PROPERTY(RvoAgent, "Goal", Variant::Type::Vector3, getGoal, setGoal);
	}

	void RvoAgent::setRadius(float radius)
	{
		if (m_radius != radius)
		{
			m_radius = radius;

			if (m_rvoAgent)
				m_rvoAgent->setRadius(m_radius);
		}
	}

	void RvoAgent::setGoal(const Vector3& goal)
	{
		if (m_goal != goal)
		{
			m_goal = goal;
		}
	}

	Vector3 RvoAgent::getVelocity() const
	{
		if (m_rvoAgent)
		{
			Vector3 velocity(m_rvoAgent->velocity_.x(), 0.f, m_rvoAgent->velocity_.y());
			return velocity;
		}

		return Vector3::ZERO;
	}

	void RvoAgent::updateInternal(float elapsedTime)
	{
		if (!m_rvoAgent)
		{
			Vector3 wpos = getWorldPosition();
			m_rvoAgent = RvoModule::instance()->getRvoSimulator()->addAgent(RVO::Vector2(wpos.x, wpos.z));
			m_rvoAgent->setUserData(this);
			m_rvoAgent->setRadius(m_radius);
		}

		if (m_rvoAgent && IsGame)
		{
			Vector3 goalDir = m_goal - getWorldPosition(); 
					goalDir.y = 0.f;
			float   goalLen = goalDir.len();
			if (goalLen > m_radius)
			{
				Vector3 dir = m_speed * goalDir / goalLen;
				m_rvoAgent->setPrefVelocity(RVO::Vector2(dir.x, dir.z));
			}
			else
			{
				Vector3 dir = m_speed * goalDir;
				m_rvoAgent->setPrefVelocity(RVO::Vector2(dir.x, dir.z));
			}

			const RVO::Vector2& pos = m_rvoAgent->position_;
			this->setWorldPosition(Vector3(pos.x(), getWorldPosition().y, pos.y()));
		}
	}
}