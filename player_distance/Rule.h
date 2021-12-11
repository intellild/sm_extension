#pragma once

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
    void Run(float distances[64]);
    bool Match(int32_t client) const;
};
