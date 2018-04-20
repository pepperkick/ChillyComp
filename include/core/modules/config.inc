public CheckStatusChange(int status) {
    char path[128], config[256];
    GetConVarString(g_hcConfigPath, path, sizeof(path));

    switch(status) {
        case STATE_INITIAL:
            Format(config, sizeof(config), "%s/%s.cfg", path, "status_initial");
        case STATE_ROLLING:
            Format(config, sizeof(config), "%s/%s.cfg", path, "status_rolling");
        case STATE_FIGHTING:
            Format(config, sizeof(config), "%s/%s.cfg", path, "status_fighting");
        case STATE_PICKING:
            Format(config, sizeof(config), "%s/%s.cfg", path, "status_picking");
        case STATE_SETUP:
            Format(config, sizeof(config), "%s/%s.cfg", path, "status_setup");
        case STATE_WARMUP:
            Format(config, sizeof(config), "%s/%s.cfg", path, "status_warmup");
        case STATE_LIVE:
            Format(config, sizeof(config), "%s/%s.cfg", path, "status_live");
        case STATE_POST:
            Format(config, sizeof(config), "%s/%s.cfg", path, "status_post");
        default:
            DebugLog("Unknown status");
    }

    ServerCommand("exec %s", config);
}