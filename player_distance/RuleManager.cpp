#include <algorithm>
#include "RuleManager.h"

void RuleManager::OnHandleDestroy(HandleType_t type, void *object)
{
    auto rule = reinterpret_cast<Rule*>(object);
    auto pos = std::find(m_rules.begin(), m_rules.end(), rule);
    if (pos != m_rules.end())
    {
        m_rules.erase(pos);
    }
    delete rule;
}

void RuleManager::Run(float distanceMatrix[64][64])
{
    for (auto rule : m_rules)
    {
        auto owner = rule->Owner();
        rule->Run(distanceMatrix[owner - 1]);
    }
}
