#include <typeinfo>
#include "component.h"

Component::Component()
{
    m_ObjectName = typeid(*this).name();
}
