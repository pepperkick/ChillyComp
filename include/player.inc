//============================================
//    CountPlayersInTeam
//        Checks the number of players in server
//        Only countes players in a team
//         Sets counted players
//============================================

public int CountPlayersInTeam() {
    int l_iPlayers = 0;

    for(new i = 1; i <= MaxClients; i++)
        if(IsClientInTeam(i))
            l_iPlayers++;

    return l_iPlayers;
}

public bool IsClientInTeam(client) {
    return IsClientProper(client) ? (GetClientTeam(client) == TEAM_BLU || GetClientTeam(client) == TEAM_RED) : false;
}

public bool IsClientProper(client) {
    return client != 0 ? IsClientInGame(client) && !IsFakeClient(client) : false;
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

    for(int i = 0; i < MaxClients; i++) {
        PushArrayCell(CountedPlayers, IsClientInTeam(i));
    }

    return CountedPlayers;
}