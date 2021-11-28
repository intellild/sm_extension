/**
 * vim: set ts=4 :
 * =============================================================================
 * SourceMod Sample Extension
 * Copyright (C) 2004-2008 AlliedModders LLC.  All rights reserved.
 * =============================================================================
 *
 * This program is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License, version 3.0, as published by the
 * Free Software Foundation.
 * 
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * As a special exception, AlliedModders LLC gives you permission to link the
 * code of this program (as well as its derivative works) to "Half-Life 2," the
 * "Source Engine," the "SourcePawn JIT," and any Game MODs that run on software
 * by the Valve Corporation.  You must obey the GNU General Public License in
 * all respects for all other code used.  Additionally, AlliedModders LLC grants
 * this exception to all derivative works.  AlliedModders LLC defines further
 * exceptions, found in LICENSE.txt (as of this writing, version JULY-31-2007),
 * or <http://www.sourcemod.net/license.php>.
 *
 * Version: $Id$
 */

#include <cmath>

#ifdef __PERF__
#include <ctime>
#endif

#include <mathlib/vector.h>
#include <IPlayerHelpers.h>
#include <edict.h>
#include <iplayerinfo.h>

#include "extension.h"

extern IPlayerManager *playerhelpers;

PlayerDistance g_PlayerDistance;		/**< Global singleton for extension's main interface */

SMEXT_LINK(&g_PlayerDistance);

typedef struct point_struct {
    float x;
    float y;
    float z;
    float __padding;
} point_t;

static float g_Distance[64][64];
static point_t g_Buffer[64];

static inline float square(float value)
{
    return value * value;
}

static void init()
{
    for (int i = 0; i < 64; i++)
    {
        point_t &point = g_Buffer[i];
        point.x = NAN;
        point.y = NAN;
        point.z = NAN;
        g_Buffer[i].__padding = 0.0;
    }
}

static void calculate(point_t points[64])
{
    for (int i = 0; i < 64; i++)
    {
        point_t p1 = points[i];
        for (int j = 0; j < 64; j++)
        {
            point_t p2 = points[j];
            // padding is always zero, to align SIMD length
            float distance = square(p1.x - p2.x) + square(p1.y - p2.y) + square(p1.z - p2.z) + square(p1.__padding - p2.__padding);
            g_Distance[i][j] = distance;
        }
    }
}

static void GameFrameHook(bool simulating)
{
    g_PlayerDistance.FrameAction();
}

bool PlayerDistance::SDK_OnLoad(char *error, size_t maxlen, bool late)
{
    g_pSM->AddGameFrameHook(GameFrameHook);
    return true;
}

void PlayerDistance::SDK_OnUnload()
{
    g_pSM->RemoveGameFrameHook(GameFrameHook);
}

void PlayerDistance::FrameAction()
{
    for (int i = 0; i < 64; i++)
    {
        point_t &point = g_Buffer[i];
        int client = i + 1;
        IGamePlayer *pPlayer = playerhelpers->GetGamePlayer(client);
        if (!pPlayer || !pPlayer->IsInGame())
        {
            point.x = NAN;
            point.y = NAN;
            point.z = NAN;
            continue;
        }

        IPlayerInfo *pInfo = pPlayer->GetPlayerInfo();
        if (!pInfo)
        {
            point.x = NAN;
            point.y = NAN;
            point.z = NAN;
            continue;
        }

        Vector vec = pInfo->GetAbsOrigin();
        point.x = vec.x;
		point.y = vec.y;
		point.z = vec.z;
    }
}
