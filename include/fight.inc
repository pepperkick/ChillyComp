int g_iFightBluLeader = 0;
int g_iFightRedLeader = 0;

Handle g_iFightRolledPlayers = INVALID_HANDLE;

FightingStart(RolledPlayers) {
    DebugLog("Started Fighting");

    int l_iFight = 1;
    int l_iMode  = 1;

    g_bRollingSequence = 2;
    g_iFightRolledPlayers = RolledPlayers;

    g_iRollStatus = 2;

    SetLeaders();

    if (l_iMode == 1) {
        MeleeFightStart(g_iFightBluLeader, g_iFightRedLeader);
    } else {
        DebugLog("Unknown Fighting Mode! Please check if fight mode variable is set properly.");
    }
}

FightingComplete(winner, loser) {
    PickingStart(winner, loser, g_iFightRolledPlayers);
}

SetLeaders() {
    bool l_bTeamCheck    = false;
    bool l_bTeamLeaders  = false;

    for(new i = 1; i <= MaxClients; i++) {
        if(IsClientInTeam(i)) {
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