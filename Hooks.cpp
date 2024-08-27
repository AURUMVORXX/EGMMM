// Supported with union (c) 2020 Union team
// Union SOURCE file

namespace GOTHIC_ENGINE {
	
	//-----------------------------------------------
	// oCSpawnManager::CheckRemoveNpc - for Hold NPC feature

	HOOK Ivk_oCSpawnManager_CheckRemoveNpc AS(&oCSpawnManager::CheckRemoveNpc, &oCSpawnManager::AX_CheckRemoveNpc);
	zBOOL oCSpawnManager::AX_CheckRemoveNpc(oCNpc* npc)
	{
		AXNpc* castedNpc = zDYNAMIC_CAST<AXNpc>(npc);

		if (castedNpc && castedNpc->isHolded())
			return FALSE;
		else
			return THISCALL(Ivk_oCSpawnManager_CheckRemoveNpc)(npc);
	}
}