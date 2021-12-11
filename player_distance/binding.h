#pragma once

#include <smsdk_ext.h>

cell_t sp_GetClientDistanceAbsSquare(IPluginContext *pContext, const cell_t *params);
cell_t sp_CreateRule(IPluginContext *pContext, const cell_t *params);
cell_t sp_MatchRule(IPluginContext *pContext, const cell_t *params);
cell_t sp_EnableRule(IPluginContext *pContext, const cell_t *params);
cell_t sp_DisableRule(IPluginContext *pContext, const cell_t *params);
cell_t sp_Setting(IPluginContext *pContext, const cell_t *params);
cell_t sp_SettingAll(IPluginContext *pContext, const cell_t *params);
cell_t sp_ResetRule(IPluginContext *pContext, const cell_t *params);
