bool g_bWarmupRestartChecked = false;
bool g_bWarmupRestartRequired = false;

int g_bWarmupRestartCounter = 0;

public CheckWarmupRestart() {
    int count = GetConVarInt(g_hcRestartGameCvar);

    DebugLog("Number of restarts to do: %d", count);

    if (count > 0) {
        SetStatus(STATE_WARMUP);

        g_bWarmupRestartRequired = true;
        g_bWarmupRestartCounter = count;
    }

    g_bWarmupRestartChecked = true;
}

public DoWarmupRestart() {
    char text[64];

    if(g_bWarmupRestartCounter == 1) {
        Format(text, sizeof(text), "Restart");
    } else {
        Format(text, sizeof(text), "%d Restarts", g_bWarmupRestartCounter);
    }

    PrintToMinHudAll("-- Match Begins on Next %s --", text);

    SetConVarInt(g_hcRestartGameCvar, 10, false, false);

    g_bWarmupRestartCounter--;
}

public ResetWarmupResatrt() {
    g_bWarmupRestartChecked = false;
    g_bWarmupRestartRequired = false;
    g_bWarmupRestartCounter = 0;
}