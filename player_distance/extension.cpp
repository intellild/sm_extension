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

class SMGlobalClass;

#include "extension.h"
#include <edict.h>
#include <iplayerinfo.h>
#include <mathlib/vector.h>
#include "calculation.h"

PlayerDistance g_PlayerDistance;		/**< Global singleton for extension's main interface */

SMEXT_LINK(&g_PlayerDistance);

static point_t g_Buffer[64];

static void GameFrameHook(bool simulating)
{
    g_PlayerDistance.FrameAction();
}

bool PlayerDistance::SDK_OnLoad(char *error, size_t maxlen, bool late)
{
    init_calculation();
    for (int i = 0; i < 64; i++)
    {
        point_t &point = g_Buffer[i];
        point.x = NAN;
        point.y = NAN;
        point.z = NAN;
        g_Buffer[i].__padding = 0.0;
    }
    g_pSM->AddGameFrameHook(GameFrameHook);
    return true;
}

void PlayerDistance::SDK_OnUnload()
{
    g_pSM->RemoveGameFrameHook(GameFrameHook);
}

void PlayerDistance::FrameAction()
{
    #ifdef __PERF__
    timespec start, end;
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start);
    int player_count = 0;
    #endif

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

        #ifdef __PERF__
        player_count++;
        #endif
    }
    calculate(g_Buffer);

    #ifdef __PERF__
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end);
    long duration = end.tv_nsec - start.tv_nsec;
    g_pSM->LogMessage(myself, "%d players, %ld ns \n", player_count, duration);
    #endif
}
