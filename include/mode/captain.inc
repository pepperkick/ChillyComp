/*
    Include file for Spec Roll functions
    In this mode all players are moved to spec and 2 who join back are captains
        Callers
            CapRollStart

        Timers
            Timer_PrintToHud
            Timer_CheckCounter
*/

bool g_iCapRollPreRollDone             = false;
bool g_bCapRollPlayer[MAX_PLAYERS]     = false;

CapRollStart(Handle:CountedPlayers) {
    DebugLog("Started Captain Roll Sequence");

    CapMoveAllToSpec();

    CreateTimer(0.1, Timer_SpecRollCheckCounter,  _, TIMER_REPEAT);

    HudSetText("Moved everyone to Spectators!\nJoin in to become captain");
}

CapRollReset() {
    g_iCapRollPreRollDone     = false;

    for(int i = 0; i < MAX_PLAYERS; i++) {
        g_bCapRollPlayer[i] = false;
    }

    DebugLog("Reset Cap Roll");
}

CapMoveAllToSpec() {
    for(int i = 0; i < MAX_PLAYERS; i++) {
        if(IsClientProper(i)) {
            ChangeClientTeam(i, TEAM_SPC);
            g_bCapRollPlayer[i] = true;
        }
    }

    g_iCapRollPreRollDone = true;

    DebugLog("Moved all Players to Spec");
}

public Action:CapRollTeamConditions(client, team) {
    DebugLog("[CAPROLL] Handling %d joining team %d", client, team);

    if (g_iCapRollPreRollDone) {
        if (CountPlayersInTeam() >= 2) {
            CPrintToChat(client, "%s[%s] %t", COLOR_TAG, TAG, "Rolling-Condition-CaptainDecided");
            PrintToChat(client, "[ChillyRoll] The leaders are decided!");

            return Plugin_Handled;
        } else if (team == TEAM_BLU && GetArrayCell(g_hPlayerPicked, client) == TEAM_RED) {
            CPrintToChat(client, "%s[%s] %t", COLOR_TAG, TAG, "Rolling-Condition-PickedByOtherTeam", "{red}RED", "{blue}BLU");

            return Plugin_Handled;
        } else if (team == TEAM_RED && GetArrayCell(g_hPlayerPicked, client) == TEAM_BLU) {
            CPrintToChat(client, "%s[%s] %t", COLOR_TAG, TAG, "Rolling-Condition-PickedByOtherTeam", "{blue}BLU", "{red}RED");

            return Plugin_Handled;
        } else if ((team == TEAM_BLU || team == TEAM_RED) && GetArrayCell(g_hPlayerPicked, client) == 0) {
            CPrintToChat(client, "%s[%s] %t", COLOR_TAG, TAG, "Rolling-Condition-WaitForPick")

            return Plugin_Handled;
        }
    }

    return Plugin_Continue;
}

CapRollCheckPlaye(client) {
    return g_bCapRollPlayer[client];
}

public Action:Timer_CapRollCheckCounter(Handle timer) {
    if(CountPlayersInTeam() >= 2) {
        DebugLog("[CAPROLL] Rolling Complete");

        Handle RolledPlayers = CreateArray();

        for(int i = 0; i < MAX_PLAYERS; i++) {
            PushArrayCell(RolledPlayers, g_bSpecRollPlayer[i]);
        }

        OnRollingComplete(RolledPlayers);

        return Plugin_Stop;
    }

    return Plugin_Continue;
}