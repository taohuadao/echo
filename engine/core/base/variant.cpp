#include "variant.h"
#include "object.h"

namespace Echo
{
	const Variant Variant::INVALID;

	Variant::Variant(bool value)
		: m_type(Type::Bool)
	{
		m_bool = value;
	}

	Variant::Variant(float value)
		: m_type(Type::Real)
	{
		m_double = value;
	}

	Variant::Variant(double value)
		: m_type(Type::Real)
	{
		m_double = value;
	}

	Variant::Variant(int value)
		: m_type(Type::Int)
	{
		m_int = value;
	}

	Variant::Variant(ui32 value)
		: m_type(Type::UInt)
	{
		m_uint = value;
	}

	Variant::Variant(const Echo::String& str)
		: m_type(Type::String)
	{
		m_any = str;
	}

	Variant::Variant(const Vector2& value)
		: m_type(Type::Vector2)
	{
		m_any = value;
	}

	Variant::Variant(const Vector3& value)
		: m_type(Type::Vector3)
	{
		m_any = value;
	}

	Variant::Variant(const Vector4& value)
		: m_type(Type::Vector4)
	{
		m_any = value;
	}

	Variant::Variant(const Quaternion& value)
		: m_type(Type::Quaternion)
	{
		m_any = value;
	}

	Variant::Variant(const Color& value)
		: m_type(Type::Color)
	{
		m_any = value;
	}

	Variant::Variant(const Matrix& value)
		: m_type(Type::MatrixN)
	{
		m_any = value;
	}

	Variant::Variant(const Base64String& value)
		: m_type(Type::Base64String)
	{
		m_any = value;
	}

	Variant::Variant(const StringOption& value)
		: m_type(Type::StringOption)
	{
		m_any = value;
	}
    
    Variant::Variant(Signal* value)
        : m_type(Type::Signal)
    {
        m_signal = value;
    }

	Variant::Variant(Object* value)
		: m_type(Type::Object)
	{
		m_obj = value;
	}

	Variant::Variant(const RealVector& value)
		: m_type(Type::VectorN)
	{
		m_any = value;
	}

	Variant::Variant(const ResourcePath& value)
		: m_type(Type::ResourcePath)
	{
		m_any = value;
	}

	Variant::Variant(const NodePath& value)
		: m_type(Type::NodePath)
	{
		m_any = value;
	}

	Variant::~Variant()
	{

	}

	Variant::Variant(const Variant &orig)
	{
		m_type = orig.m_type;
		m_any  = orig.m_any;
		m_obj = orig.m_obj;
	}

	// operator "="
	Variant&  Variant::operator=(const Variant& orig)
	{
		m_type = orig.m_type;
		m_any = orig.m_any;
		m_obj = orig.m_obj;

		return *this;
	}

	Echo::String Variant::toString() const
	{
		switch (m_type)
		{
		case Type::Bool:			return StringUtil::ToString(m_bool);
		case Type::Int:				return StringUtil::ToString(m_int);
		case Type::UInt:			return StringUtil::ToString(m_uint);
		case Type::Real:			return StringUtil::ToString(m_double);
		case Type::Vector2:			return StringUtil::ToString(any_cast<Vector2>(m_any));
		case Type::Vector3:			return StringUtil::ToString(any_cast<Vector3>(m_any));
		case Type::Vector4:			return StringUtil::ToString(any_cast<Vector4>(m_any));
		case Type::Quaternion:		return StringUtil::ToString(toQuaternion());
		case Type::VectorN:			return StringUtil::ToString(any_cast<RealVector>(m_any));
		case Type::Color:			return StringUtil::ToString(any_cast<Color>(m_any));
		case Type::String:			return any_cast<String>(m_any);
		case Type::ResourcePath:	return (any_cast<ResourcePath>(m_any)).getPath();
		case Type::NodePath:		return (any_cast<NodePath>(m_any)).getPath();
		case Type::Base64String:	return (any_cast<Base64String>(m_any)).getData();
		case Type::StringOption:	return (any_cast<StringOption>(m_any)).getValue();
		case Type::Object:			return StringUtil::ToString(toObj() ? toObj()->getId():-1);
        default:					return StringUtil::BLANK;
		}
	}

	// from string
	bool Variant::fromString(Type type, const String& str)
	{
		switch (type)
		{
		case Type::Bool: { m_type = Type::Bool; m_bool = StringUtil::ParseBool(str); } return true;
		case Type::Int: { m_type = Type::Int; m_int = StringUtil::ParseI32(str); } return true;
		case Type::Real: { m_type = Type::Real; m_double = StringUtil::ParseDouble(str); } return true;
		case Type::String: { m_type = Type::String; m_any = str; } return true;
		case Type::Vector2: { m_type = Type::Vector2; m_any = StringUtil::ParseVec2(str); } return true;
		case Type::Vector3: { m_type = Type::Vector3; m_any = StringUtil::ParseVec3(str); }return true;
		case Type::Vector4: { m_type = Type::Vector4; m_any = StringUtil::ParseVec4(str); } return true;
		case Type::Quaternion: { m_type = Type::Quaternion; m_any = StringUtil::ParseQuaternion(str); } return true;
		case Type::Color: { m_type = Type::Color; m_any = StringUtil::ParseColor(str); } return true;
		case Type::ResourcePath: { m_type = Type::ResourcePath; m_any = ResourcePath(str, nullptr); }return true;
		case Type::NodePath: { m_type = Type::NodePath; m_any = NodePath(str, nullptr); } return true;
		case Type::StringOption: { m_type = Type::StringOption; m_any = StringOption(str); } return true;
		case Type::Object: { m_type = Type::Object; m_obj = Object::getById(StringUtil::ParseI32(str)); } return true;
		case Type::VectorN: { m_type = Type::VectorN; m_any = StringUtil::ParseRealVector(str); } return true;
		case Type::Base64String: { m_type = Type::Base64String; m_any = Base64String(str.c_str()); } return true;
        default:    return false;
		}
	}
}
