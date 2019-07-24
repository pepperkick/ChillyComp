new String:ChatColorCodes[][] = {"\x01", "\x02", "\x03", "\x04", "\x05", "\x06", "\x07", "\x08"};

public Action:Event_ServerCvar(Handle:event, const String:name[], bool:dontBroadcast) {
    decl String:cvarName[64];
    GetEventString(event, "cvarname", cvarName, sizeof(cvarName));
    DebugLog("Checking cvar '%s'", cvarName);

    if (StrContains(cvarName, "cc_", false) == 0 && GetConVarInt(g_hcBlockCvarChangeChat) == 1) {
        DebugLog("Blocked '%s' change chat", cvarName);
        return Plugin_Handled;
    }

    return Plugin_Continue;
}

public FilterColorChat(client, String:text[]) {
    int enabled = GetConVarInt(g_hcBlockColorChat);

    for (new i = 0; i < sizeof(ChatColorCodes); i++) {
		if (StrContains(text, ChatColorCodes[i], false) > -1) {
            if (enabled == 0) {
                return false;
            } else if (enabled == 1) {
                if (IsPlayerAdmin(client)) {
                    return false;
                }
            }

            DebugLog("Blocked color chat: %s", text);

            return true;
		}
	}

    return false;
}

public bool IsPlayerAdmin(client) {
    return CheckCommandAccess(client, "sm_ban", ADMFLAG_BAN);
}