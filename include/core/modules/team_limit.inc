public Action:TeamLimitConditions(client, String:arg[]) {
    if (GetConVarInt(g_hcTeamLimitEnable) == 1) return Plugin_Continue;

    int limit = GetConVarInt(g_hcTeamSize);
    int redCounter = CountPlayersInTeam(TEAM_RED);
    int bluCounter = CountPlayersInTeam(TEAM_BLU);

    DebugLog("Number of players, RED: %d, BLU: %d", redCounter, bluCounter);

    if(StrEqual(arg, "red", false) && redCounter < limit) {
        return Plugin_Continue;
    } else if(StrEqual(arg, "blue", false) && bluCounter < limit) {
        return Plugin_Continue;
    } else if(StrEqual(arg, "spectate", false)) {
        return Plugin_Continue;
    } else if(StrEqual(arg, "spectator", false)) {
        return Plugin_Continue;
    } else {
        CPrintToChat(client, "%s[%s] %t", COLOR_TAG, TAG, "TeamLimit-Full", StrEqual(arg, "red", false) ? "{matRed}RED" : "{matBlue}BLU");
        return Plugin_Handled;
    }
}