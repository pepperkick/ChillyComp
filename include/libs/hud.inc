int g_iHudR = 0;
int g_iHudG = 0;
int g_iHudB = 0;

Handle HudInit(l_iR, l_iG, l_iB) {
    HudSetColor(l_iR, l_iG, l_iB);

    return CreateHudSynchronizer();
}

HudSetColor(l_iR, l_iG, l_iB) {
    g_iHudR = l_iR;
    g_iHudG = l_iG;
    g_iHudB = l_iB;
}

PrintToHudAll(String:str[]) {
    for (new i = 1; i < MaxClients; i++) {
        PrintToHud(i, str);
    }
}

PrintToHud(client, String:str[]) {
    SetHudTextParams(-1.0, -1.0, 20.0, g_iHudR, g_iHudG, g_iHudB, 255);

    if (IsClientValid(client)) {
        ShowSyncHudText(client, g_hoHud, "%s", str);
    }
}

HudSetText(String:str[]) {
    SetHudTextParams(-1.0, -1.0, 20.0, g_iHudR, g_iHudG, g_iHudB, 255);

    for (new i = 1; i < MaxClients; i++) {
        if (IsClientValid(i)) {
            ShowSyncHudText(i, g_hoHud, "%s", str);
        }
    }
}