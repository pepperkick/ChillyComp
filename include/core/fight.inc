int g_iFightBluLeader = -1;
int g_iFightRedLeader = -1;

FightingStart() {
    DebugLog("Started Fighting");

    int l_iMode  = 1;
    
    g_iFightBluLeader = -1;
    g_iFightRedLeader = -1;

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

    g_iFightBluLeader = -1;
    g_iFightRedLeader = -1;

    PickingReset(true);
}

FightingComplete(winner, loser) {
    PickingStart(winner, loser);

    CPrintToChat(winner, "%s[%s] %t", COLOR_TAG, TAG, "Fight-Winner");
    CPrintToChat(loser, "%s[%s] %t", COLOR_TAG, TAG, "Fight-Loser");
}

SetLeaders() {
    bool l_bTeamCheck    = false;
    bool l_bTeamLeaders  = false;

    for(new i = 1; i <= MaxClients; i++) {
        if(IsClientInAnyTeam(i)) {
            if(l_bTeamLeaders) {
                ChangeClientTeam(i, TEAM_SPE);
            }

            if(l_bTeamCheck) {
                ChangeClientTeam(i, TEAM_RED);
                g_iFightRedLeader = i;

                DebugLog("[FIGHT] BLU Team Leader %d", i);y

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