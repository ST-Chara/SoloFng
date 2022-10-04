
#include "freezegun.h"
#include <game/generated/server_data.h>
#include <game/server/entities/laser.h>

bool CFreezeGun::LaserHit(CLaser *pLaser, vec2 HitPoint, CCharacter *pHit, bool OutOfEnergy)
{
    if (pHit && pHit->GameServer()->m_apPlayers[pLaser->GetOwner()]->GetCharacter() && pHit != pHit->GameServer()->m_apPlayers[pLaser->GetOwner()]->GetCharacter() && pHit->m_FreezeTick <= 0 && pHit->GetMeltTick() + g_Config.m_SvMeltSafeticks < pHit->Server()->Tick())
	{
		if(pLaser->Controller()->IsFriendlyFire(pHit->GetPlayer()->GetCID(), pLaser->GetOwner()))
			return true;
        pHit->Freeze(9, pLaser->GetOwner());
		pHit->m_HammeredBy = pLaser->GetOwner();
		pLaser->Controller()->SendKillMsg(pLaser->GetOwner(), pHit->GetPlayer()->GetCID(), WEAPON_LASER, 0);
		return true;
	}
	return false;
}

void CFreezeGun::Fire(vec2 Direction)
{
	int ClientID = Character()->GetPlayer()->GetCID();

	new CLaser(
		GameWorld(),
		WEAPON_GUN, //Type
		GetWeaponID(), //WeaponID
		ClientID, //Owner
		Pos(), //Pos
		Direction, //Dir
		g_pData->m_Weapons.m_Laser.m_Reach, // StartEnergy
		CFreezeGun::LaserHit);

	GameWorld()->CreateSound(Character()->m_Pos, SOUND_LASER_FIRE);
}