#include "binding.h"
#include "compute.h"
#include "extension.h"
#include "Rule.h"
#include "RuleManager.h"

extern IdentityToken_t *g_RuleIdentityToken;
extern HandleType_t g_RuleHandleType;
extern RuleManager g_RuleManager;

#define READ_HANDLE \
    Rule *pRule = nullptr;\
    do\
    {\
        Handle_t hndl = static_cast<Handle_t>(params[1]);\
        HandleError herr;\
        HandleSecurity sec;\
        sec.pOwner = pContext->GetIdentity();\
        sec.pIdentity = g_RuleIdentityToken;\
        herr = handlesys->ReadHandle(hndl, g_RuleHandleType, std::addressof(sec), reinterpret_cast<void **>(std::addressof(pRule)));\
        if (herr != HandleError_None)\
        {\
            return pContext->ThrowNativeError("Invalid player distance rule handle %x (error %d).", hndl, herr);\
        }\
    } while(0);

cell_t sp_GetClientDistanceAbsSquare(IPluginContext *pContext, const cell_t *params)
{
    cell_t client1 = params[1];
    cell_t client2 = params[2];
    float distance = GetDistance(client1, client2);
    return sp_ftoc(distance);
}

cell_t sp_CreateRule(IPluginContext *pContext, const cell_t *params)
{
    int32_t client = params[1];
    return PlayerDistance::CreateRule(pContext, client);
}

cell_t sp_MatchRule(IPluginContext *pContext, const cell_t *params)
{
    READ_HANDLE;
    const int32_t client = params[2];
    return pRule->Match(client);
}

cell_t sp_EnableRule(IPluginContext *pContext, const cell_t *params)
{
    READ_HANDLE;
    pRule->Enable();
    return 0;
}

cell_t sp_DisableRule(IPluginContext *pContext, const cell_t *params)
{
    READ_HANDLE;
    pRule->Disable();
    return 0;
}

cell_t sp_Setting(IPluginContext *pContext, const cell_t *params)
{
    READ_HANDLE;
    const int32_t client = params[2];
    const float value = sp_ctof(params[3]);
    pRule->Setting(client, value);
    return 0;
}

cell_t sp_SettingAll(IPluginContext *pContext, const cell_t *params)
{
    READ_HANDLE;
    const float value = sp_ctof(params[2]);
    pRule->Setting(value);
    return 0;
}

cell_t sp_ResetAllRules(IPluginContext *pContext, const cell_t *params)
{
    g_RuleManager.ResetAllRules();
    return 0;
}
