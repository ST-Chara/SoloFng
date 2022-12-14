/* (c) Timo Buhrmester. See licence.txt in the root of the distribution   */
/* for more information. If you are missing that file, acquire a complete */
/* release at teeworlds.com.                                              */

#include <base/system.h>

#include <engine/shared/config.h>
//#include <engine/server/server.h>

#include <game/server/gamecontext.h>
#include <game/server/entities/character.h>
#include <game/server/weapons.h>
#include <game/server/entities/textentity.h>

#include "solofng.h"

#define TS Server()->TickSpeed()
#define TICK Server()->Tick()
#define GS GameServer()
#define GW GameWorld()
#define CHAR(C) (((C) < 0 || (C) >= MAX_CLIENTS) ? 0 : GS->GetPlayerChar(C))
#define PLAYER(C) (((C) < 0 || (C) >= MAX_CLIENTS) ? 0 : GS->m_apPlayers[C])
#define TPLAYER(C) (((C) < 0 || (C) >= MAX_CLIENTS) ? 0 : (GS->m_apPlayers[C] ? GS->m_apPlayers[C] : 0))
#define CFG(A) g_Config.m_Sv ## A
#define FORTEAMS(T) for(int T = TEAM_RED; T != -1; T = (T==TEAM_RED?TEAM_BLUE:-1))

#if defined(CONF_FAMILY_WINDOWS)
 #define D(F, ...) dbg_msg("SoloFNG", "%s:%i:%s(): " F, __FILE__, __LINE__, \
                                                            __FUNCTION__, __VA_ARGS__)
#elif defined(CONF_FAMILY_UNIX)
 #define D(F, ARGS...) dbg_msg("SoloFNG", "%s:%i:%s(): " F, __FILE__, __LINE__, \
                                                            __func__,##ARGS)
#endif


CGameControllerSoloFNG::CGameControllerSoloFNG()
: IGameController(), m_Broadcast(GS)
{
	m_pGameType = "solofng";
	m_GameFlags = IGF_SUDDENDEATH;
	m_aCltMask[0] = m_aCltMask[1] = 0;

	Reset();
}

CGameControllerSoloFNG::~CGameControllerSoloFNG()
{
	Reset(true);
}

void CGameControllerSoloFNG::Reset(bool Destruct)
{
	for(int i = 0; i < MAX_CLIENTS; i++)
		m_aFrozenBy[i] = m_aMoltenBy[i] = m_aLastInteraction[i] = -1;

	m_Broadcast.Reset();

	*m_aRagequitAddr = '\0';

//	m_ScoreDisplay.Reset(Destruct);
	m_aCltMask[0] = m_aCltMask[1] = 0;
}

void CGameControllerSoloFNG::OnPreTick()
{
	if ((IsEndRound() || IsEndMatch()) || m_Warmup)
		return;

	if (DoEmpty())
		return;

	//DoInteractions();

	//DoScoreDisplays();

	DoBroadcasts();

	DoRagequit();
}

bool CGameControllerSoloFNG::DoEmpty()
{
	bool Empty = true;

	for(int i = 0; Empty && i < MAX_CLIENTS; i++)
		if (TPLAYER(i))
			Empty = false;
	/*
	if (Empty)
		m_aTeamscore[0] = m_aTeamscore[1] = 0;
	*/
	
	return Empty;
}

void CGameControllerSoloFNG::DoHookers()
{
	for(int i = 0; i < MAX_CLIENTS; i++)
	{
		CCharacter *pChr = CHAR(i);
		if (!pChr)
			continue;
		
		int Hooking = pChr->GetHookedPlayer();

		if (Hooking >= 0)
		{
			CCharacter *pVic = CHAR(Hooking);
			
			if (!pVic || (/*pVic->GetPlayer()->GetTeam() != pChr->GetPlayer()->GetTeam() && */pChr->GetHookTick() < CFG(HookRegisterDelay)))
				Hooking = -1;
			
		}

		int HammeredBy = pChr->LastHammeredBy();

		if (Hooking >= 0)
		{
			CCharacter *pVic = CHAR(Hooking);
			if (pVic)
			{
				//bool SameTeam = pChr->GetPlayer()->GetTeam() == pVic->GetPlayer()->GetTeam();
				m_aLastInteraction[Hooking] = /*SameTeam ? -1 : */ i;
			}
		}

		if (HammeredBy >= 0)
		{	
			CCharacter *pHam = CHAR(HammeredBy);
			if (pHam)
			{
				//bool SameTeam = pChr->GetPlayer()->GetTeam() == pHam->GetPlayer()->GetTeam();
				m_aLastInteraction[i] = /*SameTeam ? -1 : */ HammeredBy;
				if (/* !SameTeam &&*/ CFG(HammerFreeze) && pChr->m_FreezeTick <= 0)
				{
					pChr->Freeze(CFG(HammerFreeze) * TS, HammeredBy);
					m_aFrozenBy[i] = HammeredBy; //suppress kill event being generated
				}
			}
			else
				m_aLastInteraction[i] = -1;
		}
	}
}

bool CGameControllerSoloFNG::OnCharacterTile(class CCharacter *pChr, int MapIndex)
{
	DoHookers();

	if (!pChr)
		return false;
	int Col = GS->Collision()->GetTileIndex(MapIndex);
	if(pChr->IsFrozen())
	{
		if(Col == 8)
		{
			if(CHAR(pChr->LastHammeredBy()))
				new CTextEntity(GW, CHAR(pChr->LastHammeredBy())->GetPos(), CTextEntity::TYPE_LASER, CTextEntity::SIZE_NORMAL, CTextEntity::ALIGN_LEFT, "+3", 3.0F);
			if(PLAYER(pChr->LastHammeredBy()))
			{
				GW->CreateSound(CHAR(pChr->LastHammeredBy())->GetPos(), SOUND_CTF_CAPTURE);
				PLAYER(pChr->LastHammeredBy())->m_Score+=3;
			}
			pChr->Die(pChr->LastHammeredBy(), WEAPON_NINJA);
		}

		if(Col == 9 || Col == 10)
		{
			if(CHAR(pChr->LastHammeredBy()))
				new CTextEntity(GW, CHAR(pChr->LastHammeredBy())->GetPos(), CTextEntity::TYPE_LASER, CTextEntity::SIZE_NORMAL, CTextEntity::ALIGN_LEFT, "+5", 3.0F);
			if(PLAYER(pChr->LastHammeredBy()))
			{
				GW->CreateSound(CHAR(pChr->LastHammeredBy())->GetPos(), SOUND_CTF_CAPTURE);
				PLAYER(pChr->LastHammeredBy())->m_Score+=5;
			}
			pChr->Die(pChr->LastHammeredBy(), WEAPON_NINJA);
		}
	}
	else
		pChr->Die(-1, WEAPON_WORLD);
}

void CGameControllerSoloFNG::DoScoreDisplays()
{
	/*FORTEAMS(i)
		m_ScoreDisplay.Update(i, m_aTeamscore[i]);

	m_ScoreDisplay.Operate(); */
}

void CGameControllerSoloFNG::DoBroadcasts(bool ForceSend)
{
	if (!(IsEndRound() || IsEndMatch()))
		return;

	//if (max(m_aTeamscore[0], m_aTeamscore[1]) + CFG(AllYourBase) >= CFG(Scorelimit))
	//	m_Broadcast.Update(-1, "ALL YOUR BASE ARE BELONG TO US.", -1);

	m_Broadcast.SetDef(CFG(DefBroadcast));

	m_Broadcast.Operate();
}

void CGameControllerSoloFNG::DoRagequit()
{
	if (*m_aRagequitAddr)
	{
		char aBuf[128];
		str_format(aBuf, sizeof(aBuf), "ban %s %d %s", m_aRagequitAddr, g_Config.m_SvPunishRagequit, "Forcefully left the server while being frozen.");
		GameServer()->Console()->ExecuteLine(aBuf);
		dbg_msg("GAME",aBuf);
		*m_aRagequitAddr = '\0';
		/*
		NETADDR Addr;
		if (net_addr_from_str(&Addr, m_aRagequitAddr) == 0)
		{
			Addr.port = 0;
			((CServer*)Server())->m_ServerBan.BanAddr(Addr, CFG(PunishRagequit), "Forcefully left the server while being frozen.");
		}
		*m_aRagequitAddr = '\0';
		*/
	}
}

void CGameControllerSoloFNG::HandleFreeze(int Killer, int Victim)
{
	CCharacter *pVictim = CHAR(Victim);
	if (!pVictim) // for odd reasons, this can happen (confirmed by segfault). didn't yet track down why 
	{
		D("no pVictim in HandleFreeze(%d, %d)", Killer, Victim);
		return;
	}


	//int FailTeam = pVictim->GetPlayer()->GetTeam() & 1;
	//m_aTeamscore[1 - FailTeam] += CFG(FreezeTeamscore);

	if (CFG(FreezeScore) && CFG(FreezeBroadcast)) //probably of no real use but for completeness...
	{
		char aBuf[64];
		str_format(aBuf, sizeof aBuf, "%s froze (%+d)", Server()->ClientName(Killer), CFG(FreezeScore));
		m_Broadcast.Update(-1, aBuf, CFG(BroadcastTime) * TS);
	}

	CPlayer *pPlKiller = TPLAYER(Killer);

	if (!pPlKiller)
		return;

	//freezing counts as a hostile interaction
	m_aLastInteraction[pVictim->GetPlayer()->GetCID()] = pPlKiller->GetCID();

	pPlKiller->m_Score += CFG(FreezeScore);
	SendFreezeKill(Killer, Victim, WEAPON_LASER);

	if (pPlKiller->GetCharacter())
	{
		GW->CreateSound(pPlKiller->GetCharacter()->m_Pos, SOUND_HIT, (1<<pPlKiller->GetCID()));
		if (CFG(FreezeLoltext) && CFG(FreezeScore))
		{
			char aBuf[64];
			str_format(aBuf, sizeof aBuf, "%+d", CFG(FreezeScore));
			new CTextEntity(GW, pPlKiller->GetCharacter()->GetPos(), CTextEntity::TYPE_LASER, CTextEntity::SIZE_NORMAL, CTextEntity::ALIGN_MIDDLE, aBuf, 1.0f);
		}
	}
}

void CGameControllerSoloFNG::HandleMelt(int Melter, int Meltee)
{
	CCharacter *pMeltee = CHAR(Meltee);
	if (!pMeltee) //due to HandleFreeze, i suspect this COULD also possibly happen. 
	{
		D("no pMeltee in HandleMelt(%d, %d)", Melter, Meltee);
		return;
	}

	int MeltTeam = pMeltee->GetPlayer()->GetTeam()&1;
	m_aTeamscore[MeltTeam] += CFG(MeltTeamscore);

	if (CFG(MeltTeamscore) && CFG(MeltBroadcast))
	{
		char aBuf[64];
		str_format(aBuf, sizeof aBuf, "%s melted (%+d)", GetTeamName(MeltTeam), CFG(MeltTeamscore));
		m_Broadcast.Update(-1, aBuf, CFG(BroadcastTime) * TS);
	}

	CPlayer *pPlMelter = TPLAYER(Melter);

	if (!pPlMelter)
		return;

	pPlMelter->m_Score += CFG(MeltScore);
	SendFreezeKill(Melter, Meltee, WEAPON_HAMMER);

	if (pPlMelter->GetCharacter() && CFG(MeltLoltext) && CFG(MeltScore))
	{
		char aBuf[64];
		str_format(aBuf, sizeof aBuf, "%+d", CFG(MeltScore));
		new CTextEntity(GW, pPlMelter->GetCharacter()->GetPos(), CTextEntity::TYPE_LASER, CTextEntity::SIZE_NORMAL, CTextEntity::ALIGN_MIDDLE, aBuf, 1.0f);
	}
}

void CGameControllerSoloFNG::HandleSacr(int Killer, int Victim, int ShrineTeam)
{//assertion: Killer >= 0, victim anyways
	CCharacter *pVictim = CHAR(Victim);

	if (!pVictim) //due to HandleFreeze, i suspect this COULD also possibly happen. 
	{
		D("no pVictim in HandleSacr(%d, %d, %d)", Killer, Victim, ShrineTeam);
		return;
	}

	//int FailTeam = pVictim->GetPlayer()->GetTeam();
	//bool Wrong = ShrineTeam != -1 && FailTeam == ShrineTeam;

	//m_aTeamscore[1-FailTeam] += Wrong?CFG(WrongSacrTeamscore):(ShrineTeam == -1 ? CFG(SacrTeamscore) : CFG(RightSacrTeamscore));

	/*if (!Wrong)
	{ */
		if (CFG(SacrSound) == 1)
			GW->CreateSoundGlobal(SOUND_CTF_CAPTURE);
		else if (CFG(SacrSound) == 2)
			GW->CreateSound(pVictim->m_Pos, SOUND_CTF_CAPTURE);
	//}

	CPlayer *pPlKiller = TPLAYER(Killer);
	if (!pPlKiller)
		return;

	pPlKiller->m_Score += /*Wrong?CFG(WrongSacrScore):*/ (ShrineTeam == -1 ?CFG(SacrScore) : CFG(RightSacrScore));
	SendFreezeKill(Killer, Victim, WEAPON_NINJA);

	/*
	if (Wrong && pPlKiller->GetCharacter() && CFG(PunishWrongSacr))
	{
		pPlKiller->GetCharacter()->Freeze(CFG(PunishWrongSacr) * TS);
		GS->CreateSound(pPlKiller->GetCharacter()->m_Pos, SOUND_PLAYER_PAIN_LONG);
		GS->SendChatTarget(pPlKiller->GetCID(), "The gods are not pleased with this sacrifice!");
	}
*/
	if (/* !Wrong &&*/ pPlKiller->GetCharacter())
		pPlKiller->GetCharacter()->SetEmote(EMOTE_HAPPY, TICK + TS * 2);

	if (pPlKiller->GetCharacter() && CFG(SacrLoltext) && /*(*/(/* !Wrong && */ CFG(SacrScore)) /*|| (Wrong && CFG(WrongSacrScore)))*/)
	{
		char aBuf[64];
		str_format(aBuf, sizeof aBuf, "%+d", /*Wrong?CFG(WrongSacrScore):*/(ShrineTeam == -1 ? CFG(SacrScore) : CFG(RightSacrScore)));
		new CTextEntity(GW, pPlKiller->GetCharacter()->GetPos(), CTextEntity::TYPE_LASER, CTextEntity::SIZE_NORMAL, CTextEntity::ALIGN_MIDDLE, aBuf, 1.0f);
	}
}

void CGameControllerSoloFNG::SendFreezeKill(int Killer, int Victim, int Weapon)
{
	char aBuf[256];
	str_format(aBuf, sizeof(aBuf), "frzkill k:%d:'%s' v:%d:'%s' w:%d",
	                      Killer, Server()->ClientName(Killer),
	                      Victim, Server()->ClientName(Victim), Weapon);

	GS->Console()->Print(IConsole::OUTPUT_LEVEL_DEBUG, "game", aBuf);

	// send the kill message
	CNetMsg_Sv_KillMsg Msg;
	Msg.m_Killer = Killer;
	Msg.m_Victim = Victim;
	Msg.m_Weapon = Weapon;
	Msg.m_ModeSpecial = 0;
	Server()->SendPackMsg(&Msg, MSGFLAG_VITAL, -1);
}
/*
bool CGameControllerSoloFNG::CanBeMovedOnBalance(int ClientID)
{

	if (!IGameController::CanBeMovedOnBalance(ClientID))
		return false;
	if (CHAR(ClientID) && CHAR(ClientID)->m_FreezeTick > 0)
		return false;
	return true;
	
}
*/
int CGameControllerSoloFNG::OnCharacterDeath(class CCharacter *pVictim, class CPlayer *pUnusedKiller, int Weapon)
{
	m_aCltMask[pVictim->GetPlayer()->GetTeam()&1] &= ~(1<<pVictim->GetPlayer()->GetCID());

	//IGameController::OnCharacterDeath(pVictim, pKiller, Weapon);

	int Cid = pVictim->GetPlayer()->GetCID();
	if (pVictim->m_FreezeTick > 0 && m_aLastInteraction[Cid] != -1  && Weapon == WEAPON_GAME && CFG(PunishRagequit)) //ragequit
		Server()->GetClientAddr(Cid, m_aRagequitAddr, sizeof m_aRagequitAddr); //directly adding the ban here causes deadly trouble

	return 0;
}


void CGameControllerSoloFNG::OnCharacterSpawn(class CCharacter *pChr)
{
	m_aCltMask[pChr->GetPlayer()->GetTeam()&1] |= (1<<pChr->GetPlayer()->GetCID());
	
	IGameController::OnCharacterSpawn(pChr);

	pChr->GiveWeapon(WEAPON_HAMMER, WEAPON_ID_HAMMER);
	pChr->GiveWeapon(WEAPON_LASER, WEAPON_ID_FREEZELASER);
	pChr->SetWeaponSlot(WEAPON_LASER, true);

	int Cid = pChr->GetPlayer()->GetCID();
	for(int i = 0; i < MAX_CLIENTS; ++i)
		if (m_aLastInteraction[i] == Cid)
			m_aLastInteraction[i] = -1;

	m_aLastInteraction[pChr->GetPlayer()->GetCID()] = -1;
}

void CGameControllerSoloFNG::ResetMatch()
{
	Reset();
}

void CGameControllerSoloFNG::Snap(int SnappingClient)
{
	IGameController::Snap(SnappingClient);

	CNetObj_GameData *pGameDataObj = (CNetObj_GameData*)Server()->
	      SnapNewItem(NETOBJTYPE_GAMEDATA, 0, sizeof(CNetObj_GameData));

	if(!pGameDataObj)
		return;

	//pGameDataObj->m_TeamscoreRed = m_aTeamscore[TEAM_RED];
	//pGameDataObj->m_TeamscoreBlue = m_aTeamscore[TEAM_BLUE];

	//pGameDataObj->m_FlagCarrierRed = 0;
	//pGameDataObj->m_FlagCarrierBlue = 0;
}

bool CGameControllerSoloFNG::OnEntity(int Index, vec2 Pos, int Layer, int Flags, int Number)
{
	switch(Index)
	{
	case ENTITY_SPAWN:
	case ENTITY_SPAWN_RED:
	case ENTITY_SPAWN_BLUE:
		return IGameController::OnEntity(Index, Pos, Layer, Flags, Number);

	default:
		if (!CFG(SuppressEntities))
			return IGameController::OnEntity(Index, Pos, Layer, Flags, Number);
	}

	return false;
}

bool CGameControllerSoloFNG::CanJoinTeam(int Team, int NotThisID, bool S)
{
	int Can = IGameController::CanJoinTeam(Team, NotThisID, S);
	if (!Can)
		return false;

	CCharacter *pChr = CHAR(NotThisID);
	return !pChr || pChr->m_FreezeTick <= 0;
}

#undef TS
#undef TICK
#undef GS

#define TS m_pGS->Server()->TickSpeed()
#define TICK m_pGS->Server()->Tick()
#define GS m_pGS
/*
CScoreDisplay::CScoreDisplay(class CGameContext *pGameServer)
: m_pGS(pGameServer)
{
	FORTEAMS(i)
		for(int j = 0; j < MAX_SCOREDISPLAYS; ++j)
			m_aScoreDisplayTextIDs[i][j] = -1;
	Reset();
}

CScoreDisplay::~CScoreDisplay()
{
	Reset(true);
}

void CScoreDisplay::Reset(bool Destruct)
{
	FORTEAMS(i)
	{
		for(int j = 0; j < MAX_SCOREDISPLAYS; ++j)
		{
			if (m_aScoreDisplayTextIDs[i][j] != -1)
				GS->DestroyLolText(m_aScoreDisplayTextIDs[i][j]);
			m_aScoreDisplayTextIDs[i][j] = -1;
		}
		m_aScoreDisplayCount[i] = 0;
		m_aScoreDisplayValue[i] = -1;
	}
	m_Changed = 0;
	if (!Destruct)
		FindMarkers();
}

void CScoreDisplay::FindMarkers()
{
	CMapItemLayerTilemap *pTMap = GS->Collision()->Layers()->GameLayer();
	CTile *pTiles = (CTile *)GS->Collision()->Layers()->Map()->GetData(pTMap->m_Data);
	for(int y = 0; y < pTMap->m_Height; y++)
	{
		for(int x = 0; x < pTMap->m_Width; x++)
		{
			int Index = pTiles[y * pTMap->m_Width + x].m_Index;
			if (Index == TILE_REDSCORE || Index == TILE_BLUESCORE)
			{
				int Team = Index - TILE_REDSCORE;
				Add(Team, vec2(x*32.f, y*32.f));
			}
				
		}
	}
}

void CScoreDisplay::Add(int Team, vec2 Pos)
{
	if (m_aScoreDisplayCount[Team&1] >= MAX_SCOREDISPLAYS)
		return;
	m_aScoreDisplays[Team&1][m_aScoreDisplayCount[Team&1]++] = Pos;
}

void CScoreDisplay::Update(int Team, int Score)
{
	if (m_aScoreDisplayValue[Team&1] == Score)
		return;

	m_aScoreDisplayValue[Team&1] = Score;
	m_Changed |= (1<<(Team&1));
}

void CScoreDisplay::Operate()
{
	FORTEAMS(Team)
	{
		if (m_Changed & (1<<Team))
		{
			char aBuf[16];
			str_format(aBuf, sizeof aBuf, "%d", m_aScoreDisplayValue[Team]);
			for(int i = 0; i < m_aScoreDisplayCount[Team]; i++)
			{
				if (m_aScoreDisplayTextIDs[Team][i] != -1)
					GS->DestroyLolText(m_aScoreDisplayTextIDs[Team][i]);
				m_aScoreDisplayTextIDs[Team][i] = GS->CreateLolText(0, false, m_aScoreDisplays[Team][i], vec2(0.f, 0.f), 3600 * TS, aBuf);
			}
		}
	}
	
	m_Changed = 0;
}

*/

CBroadcasterSolo::CBroadcasterSolo(class CGameContext *pGameServer)
: m_pGS(pGameServer)
{
	Reset();
}

CBroadcasterSolo::~CBroadcasterSolo()
{
	Reset();
}

void CBroadcasterSolo::SetDef(const char *pText)
{
	if (str_comp(m_aDefBroadcast, pText) != 0)
	{
		str_copy(m_aDefBroadcast, pText, sizeof m_aDefBroadcast);
		for(int i = 0; i < MAX_CLIENTS; ++i)
			if (m_aBroadcastStop[i] < 0)
				str_copy(m_aBroadcast[i], pText, sizeof m_aBroadcast[i]); //this is unfortunately required
		m_Changed = ~0;
	}
}

void CBroadcasterSolo::Update(int Cid, const char *pText, int Lifespan)
{
	if (Cid < 0) // all
	{
		for(int i = 0; i < MAX_CLIENTS; ++i)
			Update(i, pText, Lifespan);
		return;
	}

	m_aBroadcastStop[Cid] = Lifespan < 0 ? -1 : (TICK + Lifespan);
	bool Changed = str_comp(m_aBroadcast[Cid], pText) != 0;
	if (Changed)
	{
		str_copy(m_aBroadcast[Cid], pText, sizeof m_aBroadcast[Cid]);
		m_Changed |= (1<<Cid);
	}
}

void CBroadcasterSolo::Reset()
{
	for(int i = 0; i < MAX_CLIENTS; ++i)
	{
		m_aBroadcast[i][0] = '\0';
		m_aNextBroadcast[i] = m_aBroadcastStop[i] = -1;
		m_Changed = ~0;
	}
	m_aDefBroadcast[0] = '\0';
}

void CBroadcasterSolo::Operate()
{
	for(int i = 0; i < MAX_CLIENTS; ++i)
	{
		if (!GS->IsClientReadyToEnter(i))
			continue;
		
		if (m_aBroadcastStop[i] >= 0 && m_aBroadcastStop[i] < TICK)
		{
			str_copy(m_aBroadcast[i], m_aDefBroadcast, sizeof m_aBroadcast[i]);
			if (!*m_aBroadcast[i])
			{
				GS->SendBroadcast(" ", i);
				m_Changed &= ~(1<<i);
			}
			else
			{
				m_Changed |= (1<<i);
			}
			m_aBroadcastStop[i] = -1;
		}

		if (((m_Changed & (1<<i)) || m_aNextBroadcast[i] < TICK) && *m_aBroadcast[i])
		{
			GS->SendBroadcast(m_aBroadcast[i], i);
			m_aNextBroadcast[i] = TICK + TS * 3;
		}
	}
	m_Changed = 0;
}

