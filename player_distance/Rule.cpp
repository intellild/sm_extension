#include <cmath>
#include <smsdk_ext.h>
#include <edict.h>
#include <iplayerinfo.h>
#include "Rule.h"
#include "ext_math.h"

constexpr uint32_t EXCLUDE_SELF = 1;
constexpr uint32_t EXCLUDE_ENERMY = (1 << 1);
constexpr uint32_t EXCLUDE_TEAMMATES = (1 << 2);
constexpr uint32_t EXCLUDE_BOT = (1 << 3);
constexpr uint32_t EXCLUDE_ALL = EXCLUDE_SELF | EXCLUDE_ENERMY | EXCLUDE_TEAMMATES | EXCLUDE_BOT;

static IPlayerInfo* GetPlayerInfo(int32_t client)
{
    IGamePlayer *pPlayer = playerhelpers->GetGamePlayer(client);
	if (!pPlayer || !pPlayer->IsInGame())
	{
		return nullptr;
	}

	return pPlayer->GetPlayerInfo();
}

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

	auto pInfo = GetPlayerInfo(m_owner);
	if (!pInfo)
	{
		return;
	}

    auto ownerTeam = pInfo->GetTeamIndex();

    std::array<uint32_t, 64> flags;
    for (auto i = 0; i < 64; i++)
    {
        int32_t target = i + 1;
        if (m_owner == target && (m_flags & EXCLUDE_SELF))
        {
            flags[i] = 0;
            continue;
        }
        auto targetPlayerInfo = GetPlayerInfo(target);
        if (!targetPlayerInfo)
        {
            flags[i] = 0;
            continue;
        }
        if (targetPlayerInfo->IsFakeClient() && (m_flags & EXCLUDE_BOT))
        {
            flags[i] = 0;
            continue;
        }
        auto targetTeam = targetPlayerInfo->GetTeamIndex();
        if (targetTeam == ownerTeam)
        {
            if (m_flags & EXCLUDE_TEAMMATES)
            {
                flags[i] = 0;
                continue;
            }
        }
        else if (m_flags & EXCLUDE_ENERMY)
        {
            flags[i] = 0;
            continue;
        }
        flags[i] = 1;
    }

    for (auto i = 0; i < 64; i++)
    {
        float setting = m_setting[i];
        float distance = distances[i];
        if (!std::isnan(distance) && distances[i] < setting)
        {
            m_match[i] = flags[i];
        }
        else
        {
            m_match[i] = 0;
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

void Rule::Flags(const uint32_t flags)
{
    m_flags = flags;
}

uint32_t Rule::Flags() const
{
    return m_flags;
}
