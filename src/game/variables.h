
/* If you are missing that file, acquire a complete release at teeworlds.com.                */
#ifndef GAME_VARIABLES_H
#define GAME_VARIABLES_H
#undef GAME_VARIABLES_H // this file will be included several times

MACRO_CONFIG_STR(SvMotd, sv_motd, 900, "", CFGFLAG_SERVER, "Message of the day to display for the clients")
MACRO_CONFIG_INT(SvTournamentMode, sv_tournament_mode, 0, 0, 1, CFGFLAG_SERVER, "Tournament mode. When enabled, players joins the server as spectator")
MACRO_CONFIG_INT(SvTournamentChat, sv_tournament_chat, 0, 0, 2, CFGFLAG_SERVER, "Tournament chat mode, 0 = disabled, 1 = spectator can't send global chat, 2 = all players can only send team chat")
MACRO_CONFIG_INT(SvSpamprotection, sv_spamprotection, 1, 0, 1, CFGFLAG_SERVER, "Spam protection")

MACRO_CONFIG_INT(SvInactiveKickTime, sv_inactivekick_time, 3, 0, 1000, CFGFLAG_SERVER, "How many minutes to wait before taking care of inactive players")
MACRO_CONFIG_INT(SvInactiveKick, sv_inactivekick, 1, 0, 2, CFGFLAG_SERVER, "How to deal with inactive players (0 & 1=move to spectator, 2=kick)")
MACRO_CONFIG_INT(SvInactiveKickSpec, sv_inactivekick_spec, 0, 0, 1, CFGFLAG_SERVER, "Kick inactive spectators")

MACRO_CONFIG_INT(SvStrictSpectateMode, sv_strict_spectate_mode, 0, 0, 1, CFGFLAG_SERVER, "Restricts information in spectator mode")
MACRO_CONFIG_INT(SvVoteSpectate, sv_vote_spectate, 1, 0, 1, CFGFLAG_SERVER, "Allow voting to move players to spectators")
MACRO_CONFIG_INT(SvVoteSpectateRejoindelay, sv_vote_spectate_rejoindelay, 3, 0, 1000, CFGFLAG_SERVER, "How many minutes to wait before a player can rejoin after being moved to spectators by vote")
MACRO_CONFIG_INT(SvVoteKick, sv_vote_kick, 1, 0, 2, CFGFLAG_SERVER, "Allow voting to kick players (2 = only allow in rooms)")
MACRO_CONFIG_INT(SvVoteKickMin, sv_vote_kick_min, 0, 0, MAX_CLIENTS, CFGFLAG_SERVER, "Minimum number of players required to start a kick vote")
MACRO_CONFIG_INT(SvVoteKickBantime, sv_vote_kick_bantime, 5, 0, 1440, CFGFLAG_SERVER, "The time in seconds to ban a player if kicked by vote. 0 makes it just use kick")
MACRO_CONFIG_INT(SvJoinVoteDelay, sv_join_vote_delay, 60, 0, 1000, CFGFLAG_SERVER, "Add a delay before recently joined players can call any vote or participate in a kick/spec vote (in seconds)")
MACRO_CONFIG_INT(SvOldTeleportWeapons, sv_old_teleport_weapons, 0, 0, 1, CFGFLAG_SERVER | CFGFLAG_GAME, "Teleporting of all weapons (deprecated, use special entities instead)")
MACRO_CONFIG_INT(SvOldTeleportHook, sv_old_teleport_hook, 0, 0, 1, CFGFLAG_SERVER | CFGFLAG_GAME, "Hook through teleporter (deprecated, use special entities instead)")
MACRO_CONFIG_INT(SvTeleportHoldHook, sv_teleport_hold_hook, 0, 0, 1, CFGFLAG_SERVER | CFGFLAG_GAME, "Hold hook when teleported")
MACRO_CONFIG_INT(SvTeleportLoseWeapons, sv_teleport_lose_weapons, 0, 0, 1, CFGFLAG_SERVER | CFGFLAG_GAME, "Lose weapons when teleported (useful for some race maps)")
MACRO_CONFIG_INT(SvDestroyBulletsOnDeath, sv_destroy_bullets_on_death, 0, 0, 1, CFGFLAG_SERVER | CFGFLAG_GAME, "Destroy bullets when their owner dies")
MACRO_CONFIG_INT(SvDestroyLasersOnDeath, sv_destroy_lasers_on_death, 0, 0, 1, CFGFLAG_SERVER | CFGFLAG_GAME, "Destroy lasers when their owner dies")

MACRO_CONFIG_INT(SvMapUpdateRate, sv_mapupdaterate, 5, 1, 100, CFGFLAG_SERVER, "64 player id <-> vanilla id players map update rate")

MACRO_CONFIG_STR(SvServerType, sv_server_type, 64, "none", CFGFLAG_SERVER, "Type of the server (novice, moderate, ...)")

MACRO_CONFIG_INT(SvSendVotesPerTick, sv_send_votes_per_tick, 5, 1, 15, CFGFLAG_SERVER, "Number of vote options being send per tick")

// SoloFng
MACRO_CONFIG_INT(SvHammerMelt, sv_hammer_melt, 3, 0, 60, CFGFLAG_SERVER, "how many ticks to subtract from freezetime, when hammering frozen teammates")
MACRO_CONFIG_INT(SvMeltSafeticks, sv_melt_safeticks, 20, 0, 300, CFGFLAG_SERVER, "how many seconds to be unfreezable after melting (not when being molten per hammer)")

MACRO_CONFIG_INT(SvFreezeScore, sv_freeze_score, 1, 0, 10, CFGFLAG_SERVER, "player score for freezing")
MACRO_CONFIG_INT(SvFreezeTeamscore, sv_freeze_teamscore, 1, 0, 10, CFGFLAG_SERVER, "team score for freezing")
MACRO_CONFIG_INT(SvSacrScore, sv_sacr_score, 3, 0, 10, CFGFLAG_SERVER, "player score for sacrificing")
MACRO_CONFIG_INT(SvSacrTeamscore, sv_sacr_teamscore, 5, 0, 10, CFGFLAG_SERVER, "team score for sacrificing")
MACRO_CONFIG_INT(SvWrongSacrScore, sv_wrong_sacr_score, -5, -10, 0, CFGFLAG_SERVER, "player score for sacrificing in wrong shrine")
MACRO_CONFIG_INT(SvWrongSacrTeamscore, sv_wrong_sacr_teamscore, -1, -10, 0, CFGFLAG_SERVER, "team score for sacrificing in wrong shrine")
MACRO_CONFIG_INT(SvRightSacrScore, sv_right_sacr_score, 5, 0, 20, CFGFLAG_SERVER, "player score for sacrificing in wrong shrine")
MACRO_CONFIG_INT(SvRightSacrTeamscore, sv_right_sacr_teamscore, 10, 0, 20, CFGFLAG_SERVER, "team score for sacrificing in wrong shrine")
MACRO_CONFIG_INT(SvMeltScore, sv_melt_score, 1, 0, 10, CFGFLAG_SERVER, "player score for melting")
MACRO_CONFIG_INT(SvMeltTeamscore, sv_melt_teamscore, 0, 0, 10, CFGFLAG_SERVER, "team score for melting")

MACRO_CONFIG_INT(SvSuppressEntities, sv_suppress_entities, 1, 0, 1, CFGFLAG_SERVER, "for non fng maps, disables spawning of pickups")
MACRO_CONFIG_INT(SvBroadcasts, sv_broadcasts, 1, 0, 1, CFGFLAG_SERVER, "allow broadcasts")
MACRO_CONFIG_STR(SvDefBroadcast, sv_def_broadcast, 128, "", CFGFLAG_SERVER, "default broadcast to display")
MACRO_CONFIG_INT(SvBroadcastTime, sv_broadcast_time, 3, 1, 10, CFGFLAG_SERVER, "default time for broadcasts to appear")

MACRO_CONFIG_INT(SvHammerScaleX, sv_hammer_scale_x, 320, 1, 1000, CFGFLAG_SERVER, "linearly scale up hammer x power, percentage, for hammering enemies and unfrozen teammates")
MACRO_CONFIG_INT(SvHammerScaleY, sv_hammer_scale_y, 120, 1, 1000, CFGFLAG_SERVER, "linearly scale up hammer y power, percentage, for hammering enemies and unfrozen teammates")

MACRO_CONFIG_INT(SvMeltHammerScaleX, sv_melt_hammer_scale_x, 50, 1, 1000, CFGFLAG_SERVER, "linearly scale up hammer x power, percentage, for hammering frozen teammates")
MACRO_CONFIG_INT(SvMeltHammerScaleY, sv_melt_hammer_scale_y, 50, 1, 1000, CFGFLAG_SERVER, "linearly scale up hammer y power, percentage, for hammering frozen teammates")

MACRO_CONFIG_INT(SvLoltextHspace, sv_loltext_hspace, 14, 10, 25, CFGFLAG_SERVER, "horizontal offset between loltext 'pixels'")
MACRO_CONFIG_INT(SvLoltextVspace, sv_loltext_vspace, 14, 10, 25, CFGFLAG_SERVER, "vertical offset between loltext 'pixels'")

MACRO_CONFIG_INT(SvSacrLoltext, sv_sacr_loltext, 0, 0, 1, CFGFLAG_SERVER, "display a loltext when sacrificing")
MACRO_CONFIG_INT(SvFreezeLoltext, sv_freeze_loltext, 0, 0, 1, CFGFLAG_SERVER, "display a loltext when freezing")
MACRO_CONFIG_INT(SvMeltLoltext, sv_melt_loltext, 0, 0, 1, CFGFLAG_SERVER, "display a loltext when melting")

MACRO_CONFIG_INT(SvSacrBroadcast, sv_sacr_broadcast, 1, 0, 1, CFGFLAG_SERVER, "display a broadcast when sacrificing")
MACRO_CONFIG_INT(SvFreezeBroadcast, sv_freeze_broadcast, 0, 0, 1, CFGFLAG_SERVER, "display a broadcast when freezing")
MACRO_CONFIG_INT(SvMeltBroadcast, sv_melt_broadcast, 0, 0, 1, CFGFLAG_SERVER, "display a broadcast when melting by hammer")

MACRO_CONFIG_INT(SvEmoticonDelay, sv_emoticon_delay, 2, 0, 5, CFGFLAG_SERVER, "be careful with 0 as it allows for emoticon spam")

MACRO_CONFIG_INT(SvLaserSkipFrozen, sv_laser_skip_frozen, 0, 0, 1, CFGFLAG_SERVER, "allow/disallow shooting through frozen tees")
MACRO_CONFIG_INT(SvLaserSkipTeammates, sv_laser_skip_teammates, 0, 0, 1, CFGFLAG_SERVER, "allow/disallow shooting through teammates")

MACRO_CONFIG_INT(SvSacrSound, sv_sacr_sound, 1, 0, 2, CFGFLAG_SERVER, "play ctf capture sound on sacrification (0 = off, 1 = global, 2 = local")

MACRO_CONFIG_INT(SvAllYourBase, sv_all_your_base, 50, 0, 200, CFGFLAG_SERVER, "display AYB if one team does only need this many score in order to win")

MACRO_CONFIG_INT(SvHookRegisterDelay, sv_hook_register_delay, 10, 0, 100, CFGFLAG_SERVER, "require this many ticks to keep a player hooked until it counts as an interaction (sacr)")

MACRO_CONFIG_INT(SvBloodInterval, sv_blood_interval, 1, 1, 300, CFGFLAG_SERVER, "should stay at 1 for openfng (as we bleed for only 1 tick)")
MACRO_CONFIG_INT(SvBleedOnFreeze, sv_bleed_on_freeze, 1, 0, 1, CFGFLAG_SERVER, "'blood' splash + sound on freezing someone")

MACRO_CONFIG_INT(SvPunishRagequit, sv_punish_ragequit, 30, 0, 120, CFGFLAG_SERVER, "number of seconds to ban for forcefully leaving the game while frozen. 0 = off.")
MACRO_CONFIG_INT(SvPunishWrongSacr, sv_punish_wrong_sacr, 6, 0, 30, CFGFLAG_SERVER, "number of seconds to freeze who is sacrificing in a wrong-colored shrine.")

MACRO_CONFIG_INT(SvHammerFreeze, sv_hammer_freeze, 0, 0, 10, CFGFLAG_SERVER, "number of seconds to freeze when hit with a hammer")

MACRO_CONFIG_INT(SvEndvoteTime, sv_endvote_time, 60, 0, 180, CFGFLAG_SERVER, "cause vote on extending the map as soon as there are this many seconds left to hit the timelimit")
MACRO_CONFIG_INT(SvEndvoteScore, sv_endvote_score, 50, 0, 100, CFGFLAG_SERVER, "cause vote on extending the map as soon as there is this much score left to hit the scorelimit")
MACRO_CONFIG_INT(SvExtend, sv_extend, 0, 0, 0, CFGFLAG_SERVER, "when set to 1 before round ends, another round on the same map is enforced and this is reset to 0")

//Old-School feature
MACRO_CONFIG_INT(SvTickSound, sv_tick_sound, 0, 0, 2, CFGFLAG_SERVER, "Ticking sound 0-disable 1-local 2-global")
MACRO_CONFIG_INT(SvSuperHammer, sv_super_hammer, 0, 0, 1, CFGFLAG_SERVER, "Super Hammer")
MACRO_CONFIG_INT(SvVivid, sv_vivid, 0, 0, 1, CFGFLAG_SERVER, "Jumpup and vivid animation when unfreeze")

// debug
#ifdef CONF_DEBUG // this one can crash the server if not used correctly
MACRO_CONFIG_INT(DbgDummies, dbg_dummies, 0, 0, 15, CFGFLAG_SERVER, "")
#endif

MACRO_CONFIG_INT(DbgFocus, dbg_focus, 0, 0, 1, CFGFLAG_CLIENT, "")
MACRO_CONFIG_INT(DbgTuning, dbg_tuning, 0, 0, 1, CFGFLAG_CLIENT, "")
#endif
