/*
    Include file for Standerd Rolling functions
        Functions
            RollingStart
            RolliComplete
*/

int    g_iLastPlayerCount   = 0;        //Last count of Players before recheck for new Players

Handle g_hcPlayerCountTimer  = INVALID_HANDLE;
Handle g_bPlayerRollStatus   = INVALID_HANDLE;

RollingStart() {
    DebugLog("Started Rolling");

    int l_iMode = GetConVarInt(g_hcRollMode);

    if (GetConVarInt(g_hcEnabled) == 0) return;

    g_bRollingSequence = true;

    Handle CountedPlayers = GetCountedPlayersArray();

    SetStatus(STATE_ROLLING);

    DebugLog("Rolling Mode is: %d", l_iMode);

    if (l_iMode == 1) {
        SpecRollStart(CountedPlayers);
    } else if (l_iMode == 2) {
        // CapRollStart(CountedPlayers);
        CapRollStart();
    } else {
        DebugLog("Unknown Rolling Mode! Please check if mode variable is set properly.");
    }
}

RollingReset() {
    SetStatus(STATE_INITIAL);

    SpecRollReset();
    CapRollReset();

    g_iLastPlayerCount = 0;

    g_hcPlayerCountTimer = CreateTimer(2.00, Timer_CheckPlayerCount, _, TIMER_REPEAT);

    FightReset();
}

ClearRollStatus() {
    for (int i = 1; i < MaxClients; i++) {
        SetPlayerRolled(i, false);
    }
}

public Action:RollingConditions(client, String:arg[]) {
    int l_iMode     = GetConVarInt(g_hcRollMode);
    int l_iTeam     = GetArgTeam(arg);

    DebugLog("Client %d joining team %d", client, l_iTeam);

    if (GetConVarInt(g_hcEnabled) == 0) return Plugin_Continue;

    if(GetStatus() >= STATE_ROLLING && GetStatus() < STATE_SETUP) {
        if(l_iMode == 1) {
            return SpecRollTeamConditions(client, l_iTeam);
        } else if(l_iMode == 2) {
            return CapRollTeamConditions(client, l_iTeam);
        } else {
            return Plugin_Continue;
        }
    }

    return Plugin_Continue;
}

RollingCheckPlayer(client) {
    int l_iMode = GetConVarInt(g_hcRollMode);

    if (l_iMode == 1) {
        SpecRollCheckPlayer(client);
    } else if (l_iMode == 2) {
        CapRollCheckPlayer(client);
    } else {
        DebugLog("Unknown Rolling Mode! Please check if mode variable is set properly.");
    }
}

OnRollingComplete() {
    DebugLog("Rolling Complete");

    g_hcPlayerCountTimer = INVALID_HANDLE;

    #if defined DEBUG
        DebugLog("Rolled Player List");
        for(int i = 0; i < MaxClients; i++) {
            bool rolled = IsPlayerRolled(i);
            DebugLog("%d - %d", i, rolled);
        }
    #endif

    FightingStart();
}

public Action:Timer_CheckPlayerCount(Handle timer) {
    if (GetConVarInt(g_hcEnabled) == 0) return Plugin_Continue;

    if (GetConVarInt(g_hcAutoStart) == 0) return Plugin_Continue;

    int l_iPlayerCounter = CountPlayersInAnyTeam();

    #if defined DEBUG
        if(g_iLastPlayerCount != l_iPlayerCounter) {
            DebugLog("Counter: %d Required: %d", l_iPlayerCounter, GetConVarInt(g_hcTeamSize)*2);
            g_iLastPlayerCount = l_iPlayerCounter;
        }
    #endif

    if (GetStatus() == STATE_INITIAL) {
        if(l_iPlayerCounter >= (GetConVarInt(g_hcTeamSize) * 2)) {
            RollingStart();

            return Plugin_Stop;
        }
    }

    return Plugin_Continue;
}

public Handle_RollModeChange(Handle:convar, const String:oldValue[], const String:newValue[]) {
    DebugLog("Rolling mode has changed!");

    RollingReset();
}

public Action:Command_StartRolling(client, args) {
    int l_iAutoStartMode = GetConVarInt(g_hcAutoStart);

    if (l_iAutoStartMode == 1) {
        CPrintToChat(client, "%s[%s] %t", COLOR_TAG, TAG, "Roll-Command-ErrorAutoEnabled");

        return Plugin_Continue;
    }

    if (GetStatus() != STATE_INITIAL) {
        CPrintToChat(client, "%s[%s] %t", COLOR_TAG, TAG, "Roll-Command-ErrorAlreadyStarted");

        return Plugin_Continue;
    }

    if (CountPlayersInAnyTeam() < 2) {
        CPrintToChat(client, "%s[%s] %t", COLOR_TAG, TAG, "Roll-Command-ErrorNotEnoughPlayers");

        return Plugin_Continue;
    }

    DebugLog("Recieved Command to Start Rolling Process");

    RollingStart();

    return Plugin_Continue;
}

public Action:Command_JoinSpec(client, const String:command[], argc) {
    return RollingConditions(client, "spectate");
}