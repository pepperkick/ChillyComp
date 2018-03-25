Menu   g_mPickMenu;

char   g_sPickMenuTitle[128];

Handle g_hPickRolledPlayers;
Handle g_hIntitalPickRolledPlayers;

int    g_iPickingTeam   = TEAM_NIL;
int    g_iPickingWinner = 0;
int    g_iPickingLoser  = 0;

void PickingStart(winner, loser, Handle RolledPlayers) {
    DebugLog("Started Picking");

    g_sPickMenuTitle = "Pick a Player";

    g_hPickRolledPlayers = RolledPlayers;
    g_hIntitalPickRolledPlayers = RolledPlayers;

    g_iPickingWinner = winner;
    g_iPickingLoser  = loser;

    g_iPickingTeam = GetClientTeam(g_iPickingWinner);

    g_iRollStatus = STATE_PICKING;

    if(g_iPickingTeam == TEAM_RED) {
        HudSetColor(255, 127, 127);
        HudSetText("RED Pick!");
    } else if(g_iPickingTeam == TEAM_BLU) {
        HudSetColor(127, 127, 255);
        HudSetText("BLU Pick!");
    }

    int count = GetArrayTrueCount(g_hPickRolledPlayers);
    int players = CountPlayersInTeam();

    DebugLog("Roll Count: %d, Players in team: %d", count, players);

    if(count == 0 || players >= (GetConVarInt(g_hcTeamSize) * 2)) {
        PickingComplete();

        return;
    }

    PickingShowPlayerMenu(g_iPickingWinner, g_hPickRolledPlayers);
    SetChatPickClient(g_iPickingWinner);
}

void OnPick(client) {
    SetArrayCell(g_hPickRolledPlayers, client, false);

    int count = GetArrayTrueCount(g_hPickRolledPlayers);
    int players = CountPlayersInTeam();

    DebugLog("Roll Count: %d, Players in team: %d", count, players);

    if(count == 0 || players >= (GetConVarInt(g_hcTeamSize) * 2)) {
        PickingComplete();

        return;
    }

    char name[1024], formattedName[1024];
    GetClientName(client, name, sizeof(name));

    if(g_iPickingTeam == TEAM_BLU) {
        ChangeClientTeam(client, TEAM_BLU);
        SetArrayCell(g_hPlayerPicked, client, TEAM_BLU);

        Format(formattedName, sizeof(formattedName), "{blue}%s", name);
        CPrintToChat(g_iFightBluLeader, "%s[%s] %t", COLOR_TAG, TAG, "Pick-Selected-ToLeader", formattedName)
        CPrintToChat(client, "%s[%s] %t", COLOR_TAG, TAG, "Pick-Selected-ToPlayer", "{blue}BLU");

        g_iPickingTeam = TEAM_RED;

        HudSetColor(255, 0, 0);
        HudSetText("RED Pick!");

        CPrintToChat(g_iFightRedLeader, "%s[%s] %t", COLOR_TAG, TAG, "Pick-Turn");
        PickingShowPlayerMenu(g_iFightRedLeader, g_hPickRolledPlayers);
        SetChatPickClient(g_iFightRedLeader);
    } else if(g_iPickingTeam == TEAM_RED) {
        ChangeClientTeam(client, TEAM_RED);
        SetArrayCell(g_hPlayerPicked, client, TEAM_RED);

        Format(formattedName, sizeof(formattedName), "{red}%s", name);
        CPrintToChat(g_iFightRedLeader, "%s[%s] %t", COLOR_TAG, TAG, "Pick-Selected-ToLeader", formattedName)
        CPrintToChat(client, "%s[%s] %t", COLOR_TAG, TAG, "Pick-Selected-ToPlayer", "{red}BLU");

        g_iPickingTeam = TEAM_BLU;

        HudSetColor(0, 0, 255);
        HudSetText("BLU Pick!");

        CPrintToChat(g_iFightBluLeader, "%s[%s] %t", COLOR_TAG, TAG, "Pick-Turn");
        PickingShowPlayerMenu(g_iFightBluLeader, g_hPickRolledPlayers);
        SetChatPickClient(g_iFightBluLeader);
    }
}

void PickingComplete() {
    DebugLog("Picking Complete");

    g_hPickRolledPlayers = INVALID_HANDLE;
    g_hIntitalPickRolledPlayers = INVALID_HANDLE;

    g_iPickingTeam   = TEAM_NIL;
    g_iPickingWinner = 0;
    g_iPickingLoser  = 0;
    g_iRollStatus    = 0;

    ResetPickMenuRetries();
    ClearPlusOne();
    RollingReset();

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