#include <sourcemod>

#define INCLUDE_SELF 1
#define INCLUDE_ENERMY (1 << 1)
#define INCLUDE_TEAMMATES (1 << 2)
#define INCLUDE_BOT (1 << 3)

native float PlayerDistance_GetClientDistanceAbsSquare(int client1, int client2);
native Handle PlayerDistance_CreateRule(int client);
native bool PlayerDistance_MatchRule(Handle rule, int client);
native void PlayerDistance_EnableRule(Handle rule);
native void PlayerDistance_DisableRule(Handle rule);
native void PlayerDistance_Setting(Handle rule, int client, float value);
native void PlayerDistance_SettingAll(Handle rule, float value);
native void PlayerDistance_ResetRule(Handle rule);
native void PlayerDistance_IsRuleEnabled(Handle rule);
native void PlayerDistance_SetRuleFlags(Handle rule, int flags);
native int PlayerDistance_GetRuleFlags(Handle rule);
