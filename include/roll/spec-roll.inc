/*
    Include file for Spec Roll functions
    In this mode all players roll to spec and last 2 are chosen as capt6ains.
        Callers
            SpecRollStart

        Timers
            Timer_PrintToHud
            Timer_CheckCounter
*/

int    g_iSpecRollPlayerCount         = 0;
int    g_iSpecRollSeconds             = 0;
int    g_iSpecRollRedLeader           = 0;
int    g_iSpecRollBluLeader           = 0;

bool   g_iSpecRollPreRollDone         = false;
bool   g_bSpecRollPlayer[MAX_PLAYERS] = false;

Handle g_bSpecRollCount               = INVALID_HANDLE;

//============================================
//    SpecRollStart
//        Starts the spec roll sequence
//============================================

public SpecRollStart(Handle:CountedPlayers) {
    DebugLog("Started Spec Roll Sequence");

    g_iSpecRollPreRollDone = false;

    g_bSpecRollCount = CloneArray(CountedPlayers);

    //Sets the timer to say the "ROLL!" message in HUD
    CreateTimer(GetRandomInt(4, 9) + 0.0, Timer_PrintToHud);

    //Message roll ready
    new String:hudmsg[128];
    Format(hudmsg, sizeof(hudmsg), "%T", "Rolling-Ready-HUD", LANG_SERVER);

    HudSetColor(139, 195, 74);
    HudSetText(hudmsg);

    CPrintToChatAll("%s[%s] %t", COLOR_TAG, TAG, "Rolling-Ready-MSG")
}

SpecRollReset() {
    g_iSpecRollPlayerCount = 0;
    g_iSpecRollSeconds     = 0;
    g_iSpecRollRedLeader   = 0;
    g_iSpecRollBluLeader   = 0;
    g_iSpecRollPreRollDone = false;

    for(int i = 0; i < MAX_PLAYERS; i++) {
        g_bSpecRollPlayer[i] = false;

        if (g_bSpecRollCount != INVALID_HANDLE) {
            int size = GetArraySize(g_bSpecRollCount);

            if (size <= i) {
                PushArrayCell(g_bSpecRollCount, false)
            } else {
                SetArrayCell(g_bSpecRollCount, i, false);
            }
        }
    }

    DebugLog("Reset Spec Roll");
}

SpecRollCheckPlayer(client) {
    return g_bSpecRollPlayer[client];
}

//============================================
//    SpecRollTeamConditions
//        Checks the team join conditions
//     Parameters
//        int     Client to check conditions for
//        string     Which team is the client trying
//============================================

public Action:SpecRollTeamConditions(client, team) {
    DebugLog("[SPECROLL] Handling %d joining team %d", client, team);

    if (g_iSpecRollPreRollDone) {
        if (team == TEAM_SPC) {
            if (CountPlayersInAnyTeam() > 2) {
                g_bSpecRollPlayer[client] = true;

                CPrintToChat(client, "%s[%s] %t", COLOR_TAG, TAG, "Rolling-Condition-Success");

                return Plugin_Continue;
            } else {
                CPrintToChat(client, "%s[%s] %t", COLOR_TAG, TAG, "Rolling-Condition-Last2Blocked");

                return Plugin_Handled;
            }
        } else if (team == TEAM_NIL) {
            CPrintToChat(client, "%s[%s] %t", COLOR_TAG, TAG, "Rolling-Condition-WaitForPick");

            return Plugin_Handled;
        } else if (team == TEAM_BLU && GetArrayCell(g_hPlayerPicked, client) == TEAM_RED) {
            CPrintToChat(client, "%s[%s] %t", COLOR_TAG, TAG, "Rolling-Condition-PickedByOtherTeam", "{red}RED", "{blue}BLU");

            return Plugin_Handled;
        } else if (team == TEAM_RED && GetArrayCell(g_hPlayerPicked, client) == TEAM_BLU) {
            CPrintToChat(client, "%s[%s] %t", COLOR_TAG, TAG, "Rolling-Condition-PickedByOtherTeam", "{blue}BLU", "{red}RED");

            return Plugin_Handled;
        } else if ((team == TEAM_BLU || team == TEAM_RED) && GetArrayCell(g_hPlayerPicked, client) == TEAM_NIL) {
            CPrintToChat(client, "%s[%s] %t", COLOR_TAG, TAG, "Rolling-Condition-WaitForPick");

            return Plugin_Handled;
        }
    } else {
        if(GetArrayCell(g_bSpecRollCount, client)) {
            if (team == TEAM_SPC) {
                CPrintToChat(client, "%s[%s] %t", COLOR_TAG, TAG, "Rolling-Condition-CannotJoinSpec");

                return Plugin_Handled;
            }
        }
    }

    return Plugin_Continue;
}

public Action:Timer_PrintToHud(Handle timer) {
    g_iSpecRollPreRollDone = true;
    CreateTimer(0.1, Timer_SpecRollCheckCounter,  _, TIMER_REPEAT);
    new String:hudmsg[128];
    Format(hudmsg, sizeof(hudmsg), "%T", "Rolling-Start-HUD", LANG_SERVER);

    HudSetColor(139, 195, 74);
    HudSetText(hudmsg);

    CPrintToChatAll("%s[%s] %t", COLOR_TAG, TAG, "Rolling-Start-MSG")
}

public Action:Timer_SpecRollCheckCounter(Handle timer) {
    if(CountPlayersInAnyTeam() <= 2) {
        DebugLog("[SPECROLL] Rolling Complete");

        Handle RolledPlayers = CreateArray();

        for(int i = 0; i < MAX_PLAYERS; i++) {
            PushArrayCell(RolledPlayers, g_bSpecRollPlayer[i]);
        }

        OnRollingComplete(RolledPlayers);

        return Plugin_Stop;
    }

    return Plugin_Continue;
}