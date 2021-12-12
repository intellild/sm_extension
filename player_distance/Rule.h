#pragma once

#include <array>
#include <cinttypes>

class Rule
{
private:
    int32_t m_owner;
    bool m_enabled;
    std::array<float, 64> m_setting;
    std::array<int32_t, 64> m_match;
    uint32_t m_flags;

public:
    Rule(const int32_t owner);

    int32_t Owner() const;
    void Setting(int32_t client, float value);
    void Setting(float value);
    const std::array<float, 64> & Setting();
    void Enable();
    void Disable();
    bool IsEnabled() const;
    void Run(const std::array<float, 64> &distances);
    const std::array<int32_t, 64> & Match() const;
    bool Match(const int32_t client) const;
    void Reset();
    uint32_t Flags() const;
    void Flags(const uint32_t flags);
};
