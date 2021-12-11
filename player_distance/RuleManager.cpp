#include <algorithm>
#include "compute.h"
#include "RuleManager.h"

void RuleManager::OnHandleDestroy(HandleType_t type, void *object)
{
    auto pRule = reinterpret_cast<Rule*>(object);
    auto pos = std::find(m_rules.begin(), m_rules.end(), pRule);
    if (pos != m_rules.end())
    {
        m_rules.erase(pos);
    }
    delete pRule;
}

Rule* RuleManager::CreateRule(const int32_t client)
{
    auto pRule = new Rule(client);
    m_rules.push_back(pRule);
    return pRule;
}

void RuleManager::Run(const distance_matrix_t &distanceMatrix)
{
    for (auto pRule : m_rules)
    {
        auto owner = pRule->Owner();
        pRule->Run(distanceMatrix[owner - 1]);
    }
}
