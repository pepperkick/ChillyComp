#if defined _tf2_hud_included
 #endinput
#endif
#define _tf2_hud_included

#define MAX_MESSAGE_LENGTH 256

char minhudBuffer[MAX_MESSAGE_LENGTH];

stock PrintToMinHudAll(const String:szMessage[], any:...) {
	decl String:szBuffer[MAX_MESSAGE_LENGTH];

	for (new i = 1; i <= MaxClients; i++) {
        SetGlobalTransTarget(i);
        VFormat(szBuffer, sizeof(szBuffer), szMessage, 2);
        PrintToMinHud(i, szBuffer);
	}
}

stock PrintToMinHud(client, const String:msg[], any:...) {
	VFormat(minhudBuffer, sizeof(minhudBuffer), msg, 3);

    if (!IsClientValid(client)) {
	    QueryClientConVar(client, "cl_hud_minmode", ConVarQuery_MinMode);
    }
}

public void ConVarQuery_MinMode(QueryCookie cookie, int client,
	ConVarQueryResult result, const char[] cvarName, const char[] cvarValue) {
	DebugLog("[HUD] Client %d has minmode %d", client, StringToInt(cvarValue));
	if (StringToInt(cvarValue) == 0) {
		new entity = CreateEntityByName("game_text_tf");
		DispatchKeyValue(entity, "message", minhudBuffer);
		DispatchKeyValue(entity, "display_to_team", "0");
		DispatchKeyValue(entity, "targetname", "game_text1");
		DispatchKeyValue(entity, "background", "0");
		DispatchSpawn(entity);
		AcceptEntityInput(entity, "Display", entity, entity);
		CreateTimer(10.0, KillGameText, entity);
	} else {
		PrintToHud(client, minhudBuffer);
	}
}

public Action:KillGameText(Handle:hTimer, any:entity) {
	if ((entity > 0) && IsValidEntity(entity))	{
		AcceptEntityInput(entity, "kill");
	}

	return Plugin_Stop;
}