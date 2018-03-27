int g_iFightBluLeader = 0;
int g_iFightRedLeader = 0;

Handle g_iFightRolledPlayers = INVALID_HANDLE;

FightingStart(Handle RolledPlayers) {
    DebugLog("Started Fighting");

    int l_iMode  = 1;

    g_iFightRolledPlayers = RolledPlayers;

    SetStatus(STATE_FIGHTING);

    SetLeaders();

    if (l_iMode == 1) {
        MeleeFightStart(g_iFightBluLeader, g_iFightRedLeader);
    } else {
        DebugLog("Unknown Fighting Mode! Please check if fight mode variable is set properly.");
    }
}

FightReset() {
    MeleeFightReset();

    g_iFightBluLeader = 0;
    g_iFightRedLeader = 0;

    PickingReset(true);
}

FightingComplete(winner, loser) {
    PickingStart(winner, loser, g_iFightRolledPlayers);

    CPrintToChat(winner, "%s[%s] %t", COLOR_TAG, TAG, "Fight-Winner");
    CPrintToChat(loser, "%s[%s] %t", COLOR_TAG, TAG, "Fight-Loser");
}

SetLeaders() {
    bool l_bTeamCheck    = false;
    bool l_bTeamLeaders  = false;

    for(new i = 1; i <= MaxClients; i++) {
        if(IsClientInAnyTeam(i)) {
            if(l_bTeamLeaders) {
                ChangeClientTeam(i, TEAM_SPC);
            }

            if(l_bTeamCheck) {
                ChangeClientTeam(i, TEAM_RED);
                g_iFightRedLeader = i;

                DebugLog("[FIGHT] BLU Team Leader %d", i);

                l_bTeamLeaders = true;
            } else {
                ChangeClientTeam(i, TEAM_BLU);
                g_iFightBluLeader = i;

                DebugLog("[FIGHT] RED Team Leader %d", i);

                l_bTeamCheck = true;
            }
        }
    }
}