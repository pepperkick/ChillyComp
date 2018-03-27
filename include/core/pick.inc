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
        HudSetColor(244, 67, 54);
        HudSetText("RED Pick!");
    } else if(g_iPickingTeam == TEAM_BLU) {
        HudSetColor(33, 150, 244);
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

    char name[1024], formattedName[1024];
    GetClientName(client, name, sizeof(name));

    CancelMenu(g_mPickMenu);

    if(g_iPickingTeam == TEAM_BLU) {
        ChangeClientTeam(client, TEAM_BLU);
        SetArrayCell(g_hPlayerPicked, client, TEAM_BLU);

        Format(formattedName, sizeof(formattedName), "{matBlue}%s", name);
        CPrintToChat(g_iFightBluLeader, "%s[%s] %t", COLOR_TAG, TAG, "Pick-Selected-ToLeader", formattedName)
        CPrintToChat(client, "%s[%s] %t", COLOR_TAG, TAG, "Pick-Selected-ToPlayer", "{matBlue}BLU");

        if(count == 0 || players >= (GetConVarInt(g_hcTeamSize) * 2)) {
            PickingComplete();

            return;
        }

        g_iPickingTeam = TEAM_RED;

        HudSetColor(255, 0, 0);
        HudSetText("RED Pick!");

        CPrintToChat(g_iFightRedLeader, "%s[%s] %t", COLOR_TAG, TAG, "Pick-Turn");
        PickingShowPlayerMenu(g_iFightRedLeader, g_hPickRolledPlayers);
        SetChatPickClient(g_iFightRedLeader);
    } else if(g_iPickingTeam == TEAM_RED) {
        ChangeClientTeam(client, TEAM_RED);
        SetArrayCell(g_hPlayerPicked, client, TEAM_RED);

        Format(formattedName, sizeof(formattedName), "{matRed}%s", name);
        CPrintToChat(g_iFightRedLeader, "%s[%s] %t", COLOR_TAG, TAG, "Pick-Selected-ToLeader", formattedName)
        CPrintToChat(client, "%s[%s] %t", COLOR_TAG, TAG, "Pick-Selected-ToPlayer", "{matRed}RED");

        if(count == 0 || players >= (GetConVarInt(g_hcTeamSize) * 2)) {
            PickingComplete();

            return;
        }

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

    g_iRollStatus = STATE_POST;

    CPrintToChatAll("%s[%s] %t", COLOR_TAG, TAG, "Pick-Complete");

    OnRollComplete();
}

PickingReset(complete) {
    DebugLog("Reset Picking - %d", complete);

    if (complete) {
        delete g_mPickMenu;

        g_iPickingTeam = TEAM_NIL;
        g_iPickingWinner = 0;
        g_iPickingLoser = 0;

        for(int i = 0; i < MAX_PLAYERS; i++) {
            if (g_hPickRolledPlayers != INVALID_HANDLE) {
                int size = GetArraySize(g_hPickRolledPlayers);

                if (size <= i) {
                    PushArrayCell(g_hPickRolledPlayers, false)
                } else {
                    SetArrayCell(g_hPickRolledPlayers, i, false);
                }
            }

            if (g_hIntitalPickRolledPlayers != INVALID_HANDLE) {
                int size = GetArraySize(g_hIntitalPickRolledPlayers);

                if (size <= i) {
                    PushArrayCell(g_hIntitalPickRolledPlayers, TEAM_NIL)
                } else {
                    SetArrayCell(g_hIntitalPickRolledPlayers, i, TEAM_NIL);
                }
            }

            if (g_hPlayerPicked != INVALID_HANDLE) {
                int size = GetArraySize(g_hPlayerPicked);

                if (size <= i) {
                    PushArrayCell(g_hPlayerPicked, TEAM_NIL)
                } else {
                    SetArrayCell(g_hPlayerPicked, i, TEAM_NIL);
                }
            }
        }
    } else {
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

                CPrintToChat(i, "%s[%s] %t", COLOR_TAG, TAG, "Pick-Restart-Moved");
            }
        }

        PickingShowPlayerMenu(g_iPickingWinner, g_hPickRolledPlayers);
    }
}

public Action:Command_RestartPicking(client, args) {
    DebugLog("Recieved Command to Restart Picking Process");

    if (g_iRollStatus != STATE_PICKING) {
        CPrintToChat(client, "%s[%s] {matPurple}Cannot reset picking until picking has started!");

        return Plugin_Handled;
    }

    PickingReset(false);

    return Plugin_Continue;
}