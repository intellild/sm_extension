#include "Rule.h"
#include "ext_math.h"
#include <cmath>

Rule::Rule(int32_t owner) : m_owner(owner)
{

}

int32_t Rule::Owner() const
{
    return m_owner;
}

void Rule::Setting(int32_t client, float value)
{
    m_setting[client - 1] = square(value);
}

void Rule::Setting(float value)
{
    float v = square(value);
    for (auto i = 0; i < 64; i++)
    {
        m_setting[i] = square(v);
    }
}

void Rule::Enable()
{
    m_enabled = true;
}

void Rule::Disable()
{
    m_enabled = false;
}

bool Rule::IsEnabled() const
{
    return m_enabled;
}

void Rule::Run(const std::array<float, 64> &distances)
{
    if (!m_enabled)
    {
        return;
    }
    for (auto i = 0; i < 64; i++)
    {
        float setting = m_setting[i];
        float distance = distances[i];
        if (!std::isnan(distance) && setting < distances[i])
        {
            m_setting[i] = true;
        }
        else
        {
            m_setting[i] = false;
        }
    }
}

bool Rule::Match(const int32_t client) const
{
    if (!m_enabled)
    {
        return false;
    }
    return m_match[client - 1];
}