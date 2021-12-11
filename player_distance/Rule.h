#pragma once

#include <array>
#include <cinttypes>

class Rule
{
private:
    int32_t m_owner;
    bool m_enabled;
    float m_setting[64];
    int32_t m_match[64];

public:
    Rule(int32_t owner);

    int32_t Owner() const;
    void Setting(int32_t client, float value);
    void Setting(float value);
    void Enable();
    void Disable();
    bool IsEnabled() const;
    void Run(const std::array<float, 64> &distances);
    bool Match(const int32_t client) const;
};
