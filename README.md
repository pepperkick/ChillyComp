# ChillyComp

A sourcemod plugin to handle all the tasks for a tf2 competitive match.

## Introduction
This plugin was made to be modular and easily configurable, to help it the plugin is filled with configurable convars and commands. To keep all the commands and convars in one place, "cc_" prefix is used instead of "sm_".

## Features
- **Match Status**: The plugin fully manages the status of the match which can be used to execute configs according to the status. Also the status can be used by other plugins.
- **Modular**: The plugin is really modular, each stage can have multiple features or a stage can be done in different ways by just changing a convar.
- **Automated**: The aim of the plugin is to make the matches completely automated without any admin involvement.
- **Easily Configurable**: The plugin is filled with convars and commands to allow full control over the plugin to make the perfect match for you.
- **Warmup Restart**: The plugin restarts the match during warmup period to help players warmup.
- **Team Limit**: The plugin enforces team size restrictions so that other players cannot join a team for unfair match.

## Status and Configs
Plugin will automatically execute configs when the status of the match changes, so use the config however you like to control the match.<br>
The config path is configurable with cc_config_path.<br>
<br>
**Example**: If cc_config_path is conpconfig then config path is<br>
"cfg/compconfig/status_live.cfg|

| Status | File Name | Description |
| --- | --- | --- |
| Initial | status_initial | Initial period of the match |
| Rolling | status_rolling | Rolling period of the match |
| Fighting | status_fighting | Fighting period of the match |
| Picking | status_picking | Picking period of the match |
| Setup | status_setup | After the teams are picked and teams are doing preparation |
| Warmup | status_warmup | Match is being restarted for player warmup |
| Live | status_live | Match is live! |
| Post | status_post | Post match completion | 

## Convars
Use this variables in config files to control your match how you want.
| Name                      | Default     | Description                                                                                                                                                                                     |
| ------------------------- | ----------- | ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| cc_enable                 | 1           | Enables or disables the whole plugin<br>0 = Disabled<br>1 = Enabled                                                                                                                             |
| cc_rolling_autostart      | 0           | Should the plugin automatically start when enough players present<br>0 = Disabled<br>1 = Enabled                                                                                                |
| cc_rolling_mode           | 1           | Which rolling mode should the plugin use<br>1 = Spectator Roll: All the players have to roll to spectator team and the last 2 left become captains<br>**NOTE**:*More modes will come in future* |
| cc_config_path            | compconfigs | Which path should plugin use to execute per status configs                                                                                                                                      |
| cc_rolling_penalty_enable | 0           | Should a player be penalized for trying to roll early<br>0 = Disabled<br>1 = Enabled                                                                                                            |
| cc_rolling_penalty_time   | 2           | How many seconds should the penalty last for (in seconds)                                                                                                                                       |
| cc_rolling_fight_health   | 150         | How much health should the captain have during the fight                                                                                                                                        |
| cc_match_status           | 0           | The current status of the match, this is for other plugins to know the status. Do not change this by yourself                                                                                   |
| cc_match_teamsize         | 6           | How many players should each team have for the match                                                                                                                                            |
| cc_match_teamlimit        | 1           | Should the teams be limited by the team size (value from cc_match_teamsize)<br>0 = Disabled<br>1 = Enabled                                                                                       |
| cc_liverestart            | 1           | How many times should the match restart during the warmup process                                                                                                                               |

## Commands
Use these commands to have full control over the plugin.
| Name             | Alias            | Permission   | Stage              | Description                                                                                                                                |
| ---------------- | ----------------- | ------------ | ------------------ | ------------------------------------------------------------------------------------------------------------------------------------------ |
| pick             | p                 | Team Captain | Picking            | Allows the team captain to pick a player.<br>Usage: pick {player name / player ID}<br>**NOTE**: *Player ID can be found with list command* |
| list             | -                 | Any Player   | Picking            | Allows the player to see the list of players that have rolled for the match                                                                |
| startroll        | -                 | Admin (Ban)  | Initial            | Allows admin to manually start a roll<br>**NOTE**: *This command will not work if auto start feature is enabled*                           |
| mark             | -                 | Admin (Ban)  | Initial - Picking   | Allows admin to mark a player is plus one                                                                                                  |
| restartpicking   | rspicking         | Admin (Ban)  | Picking            | Allows admin to restart the picking stage                                                                                                  |
| changecaptainred | changecapred, ccr | Admin (Ban)  | Fighting           | Allows admin to switch the captain for RED team with another rolled player                                                                 |
| changecaptainblu | changecapblu, ccb | Admin (Ban)  | Fighting           | Allows admin to switch the captain for BLU team with another rolled player                                                                 |
| changeroll       | -                 | Admin (Ban)  | Fighting - Picking | Allows admin to change the roll status of a player                                                                                         |