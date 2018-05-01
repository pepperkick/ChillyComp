//============================================
//    CountPlayersInAnyTeam
//        Checks the number of players in server
//        Only countes players in a team
//         Sets counted players
//============================================

public int CountPlayersInAnyTeam() {
    int l_iPlayers = 0;

    for(new i = 1; i <= MaxClients; i++)
        if(IsClientInAnyTeam(i))
            l_iPlayers++;

    return l_iPlayers;
}

public int CountPlayersInTeam(int team) {
    int l_iPlayers = 0;

    for(new i = 1; i <= MaxClients; i++)
        if(IsClientInTeam(i, team))
            l_iPlayers++;

    return l_iPlayers;
}

public bool IsClientInAnyTeam(client) {
    return IsClientProper(client) ? (GetClientTeam(client) == TEAM_BLU || GetClientTeam(client) == TEAM_RED) : false;
}

public bool IsClientInTeam(client, team) {
    return IsClientProper(client) ? (GetClientTeam(client) == team) : false;
}

public bool IsClientProper(client) {
    #if defined ALLOWBOT
        return client != 0 ? IsClientInGame(client) : false;
    #endif

    return client != 0 ? IsClientInGame(client) && !IsFakeClient(client) : false;
}

public bool IsPlayerPlusOne(client) {
    return GetArrayCell(g_bMarkedPlusOnePlayers, client);
}

public bool SetPlayerPlusOne(client, boolean) {
    DebugLog("Setting Player %d PlusOne Status: %d", client, boolean);
    SetArrayCell(g_bMarkedPlusOnePlayers, client, boolean);
}

public bool IsPlayerRolled(client) {
    return GetArrayCell(g_bPlayerRollStatus, client);
}

public bool SetPlayerRolled(client, boolean) {
    DebugLog("Setting Player %d Roll Status: %d", client, boolean);
    SetArrayCell(g_bPlayerRollStatus, client, boolean);
}

public bool IsPlayerReady(client) {
    return GetArrayCell(g_bPlayerReadyStatus, client);
}

public bool SetPlayerReady(client, boolean) {
    DebugLog("Setting Player %d Ready Status: %d", client, boolean);
    SetArrayCell(g_bPlayerReadyStatus, client, boolean);
}

public int TotalPlayersReadyInTeam(team) {
    int count = 0;

    for (int i = 1; i < MaxClients; i++) {
        if (IsPlayerReady(i) && IsClientInTeam(i, team)) 
            count++;
    }

    return count;
}

public int TotalPlayersReady() {
    int count = 0;

    for (int i = 0; i < MaxClients; i++) {
        if (IsPlayerReady(i)) 
            count++;
    }

    return count;
}

public int TotalPlayersRolled() {
    int count = 0;

    for (int i = 1; i < MaxClients; i++) {
        if (IsPlayerRolled(i)) 
            count++;
    }

    return count;
}

public int GetArgTeam(char[] arg) {
    if (StrEqual(arg, "spectate", false))  return TEAM_SPC;
    if (StrEqual(arg, "spectator", false)) return TEAM_SPC;
    if (StrEqual(arg, "blue", false))      return TEAM_BLU;
    if (StrEqual(arg, "red", false))       return TEAM_RED;

    return 0;
}

public Handle:GetCountedPlayersArray() {
    Handle CountedPlayers = CreateArray();

    for(int i = 1; i < MaxClients; i++) {
        PushArrayCell(CountedPlayers, IsClientInAnyTeam(i));
    }

    return CountedPlayers;
}