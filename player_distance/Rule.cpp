#include "Rule.h"
#include "ext_math.h"
#include <cmath>

Rule::Rule(const int32_t owner)
    : m_owner{owner}, m_enabled{true},  m_setting{0}, m_match{0}
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
        m_setting[i] = v;
    }
}

const std::array<float, 64> & Rule::Setting()
{
    return m_setting;
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
        if (!std::isnan(distance) && distances[i] < setting)
        {
            m_match[i] = true;
        }
        else
        {
            m_match[i] = false;
        }
    }
}

const std::array<int32_t, 64> & Rule::Match() const
{
    return m_match;
}

bool Rule::Match(const int32_t client) const
{
    if (!m_enabled)
    {
        return false;
    }
    return m_match[client - 1];
}

void Rule::Reset()
{
    for (auto i = 0; i < 64; i++)
    {
        m_match[i] = false;
    }
}
