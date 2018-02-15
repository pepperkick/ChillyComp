Menu   g_mPickMenu;

char   g_sPickMenuTitle[128];

Handle g_hPickRolledPlayers;
Handle g_hIntitalPickRolledPlayers;

int    g_iPickingTeam   = TEAM_NIL;
int    g_iPickingWinner = 0;
int    g_iPickingLoser  = 0;

PickingStart(winner, loser, RolledPlayers) {
    DebugLog("Started Picking");

    g_sPickMenuTitle = "Pick a Player";

    g_hPickRolledPlayers = RolledPlayers;
    g_hIntitalPickRolledPlayers = RolledPlayers;

    g_iPickingWinner = winner;
    g_iPickingLoser  = loser;

    g_iPickingTeam = GetClientTeam(g_iPickingWinner);

    RegAdminCmd("restartpicking", Command_RestartPicking, ADMFLAG_BAN, "Restart the picking stage of rolling");
    RegAdminCmd("rspicking",      Command_RestartPicking, ADMFLAG_BAN, "Restart the picking stage of rolling");

    if(g_iPickingTeam == TEAM_RED) {
        HudSetColor(255, 127, 127);
        HudSetText("RED Pick!");
    } else if(g_iPickingTeam == TEAM_BLU) {
        HudSetColor(127, 127, 255);
        HudSetText("BLU Pick!");
    }

    PickingShowPlayerMenu(g_iPickingWinner, g_hPickRolledPlayers);
}

OnPick(client) {
    SetArrayCell(g_hPickRolledPlayers, client, false);

    int count = GetArrayTrueCount(g_hPickRolledPlayers);
    int players = CountPlayersInTeam();

    DebugLog("Roll Count: %d, Players in team: %d", count, players);

    if(count == 0 || players >= (GetConVarInt(g_hcTeamSize) * 2)) return PickingComplete();

    if(g_iPickingTeam == TEAM_BLU) {
        ChangeClientTeam(client, TEAM_BLU);
        SetArrayCell(g_hPlayerPicked, client, TEAM_BLU);

        CPrintToChat(client, "%s[%s] %t", COLOR_TAG, TAG, "Picking-Picked", "{blue}BLU");

        g_iPickingTeam = TEAM_RED;

        HudSetColor(255, 127, 127);
        HudSetText("RED Pick!");

        PickingShowPlayerMenu(g_iFightRedLeader, g_hPickRolledPlayers);
    } else if(g_iPickingTeam == TEAM_RED) {
        ChangeClientTeam(client, TEAM_RED);
        SetArrayCell(g_hPlayerPicked, client, TEAM_RED);

        CPrintToChat(client, "%s[%s] %t", COLOR_TAG, TAG, "Picking-Picked", "{blue}RED");

        g_iPickingTeam = TEAM_BLU;

        HudSetColor(127, 127, 255);
        HudSetText("BLU Pick!");

        PickingShowPlayerMenu(g_iFightBluLeader, g_hPickRolledPlayers)
    }
}

PickingComplete() {
    DebugLog("Picking Complete");

    HudSetColor(0, 255, 0);
    HudSetText("Rolling Complete!");
}

PickingReset() {
    DebugLog("Reset Picking");

    g_iPickingTeam = GetClientTeam(g_iPickingWinner);

    if(g_iPickingTeam == TEAM_RED) {
        HudSetColor(255, 127, 127);
        HudSetText("RED Pick!");
    } else if(g_iPickingTeam == TEAM_BLU) {
        HudSetColor(127, 127, 255);
        HudSetText("BLU Pick!");
    }

    for (int i = 0; i < MaxClients; i++) {
        bool rolled = GetArrayCell(g_hIntitalPickRolledPlayers, i);

        SetArrayCell(g_hPickRolledPlayers, i, rolled);

        if (rolled) {
            ChangeClientTeam(i, TEAM_SPC);

            PrintToChat(i, "[CHILLYROLL] Picking was restarted, you were moved to spec");
        }
    }

    PickingShowPlayerMenu(g_iPickingWinner, g_hPickRolledPlayers);
}

public Action:Command_RestartPicking(client, args) {
    DebugLog("Recieved Command to Restart Picking Process");

    PickingReset();
}