#include "compute.h"
#include "extension.h"
#include "Rule.h"
#include "RuleManager.h"
#include "binding.h"

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

cell_t sp_IsRuleEnabled(IPluginContext *pContext, const cell_t *params)
{
    READ_HANDLE;
    return pRule->IsEnabled();
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

cell_t sp_ResetRule(IPluginContext *pContext, const cell_t *params)
{
    READ_HANDLE;
    pRule->Reset();
    return 0;
}

cell_t sp_SetRuleFlags(IPluginContext *pContext, const cell_t *params)
{
    READ_HANDLE;
    auto flags = static_cast<uint32_t>(params[2]);
    pRule->Flags(flags);
    return 0;
}

cell_t sp_GetRuleFlags(IPluginContext *pContext, const cell_t *params)
{
    READ_HANDLE;
    return static_cast<cell_t>(pRule->Flags());
}

sp_nativeinfo_t* GetBindings()
{
    static sp_nativeinfo_t Bindings[] =
    {
        {"PlayerDistance_GetClientDistanceAbsSquare", sp_GetClientDistanceAbsSquare},
        {"PlayerDistance_CreateRule", sp_CreateRule},
        {"PlayerDistance_MatchRule", sp_MatchRule},
        {"PlayerDistance_EnableRule", sp_EnableRule},
        {"PlayerDistance_DisableRule", sp_DisableRule},
        {"PlayerDistance_Setting", sp_Setting},
        {"PlayerDistance_SettingAll", sp_SettingAll},
        {"PlayerDistance_ResetRule", sp_ResetRule},
        {"PlayerDistance_IsRuleEnabled", sp_IsRuleEnabled},
        {"PlayerDistance_SetRuleFlags", sp_SetRuleFlags},
        {"PlayerDistance_GetRuleFlags", sp_GetRuleFlags},
        {NULL, NULL},
    };
    return Bindings;
}
