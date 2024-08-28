// Supported with union (c) 2020 Union team
// Union SOURCE file

namespace GOTHIC_ENGINE {
	
	HOOK Ivk_oCSpawnManager_CheckRemoveNpc AS(&oCSpawnManager::CheckRemoveNpc, &oCSpawnManager::CheckRemoveNpc_IVK);
	zBOOL oCSpawnManager::CheckRemoveNpc_IVK(oCNpc* npc)
	{
		if (npc->isHolded())
			return FALSE;

		return THISCALL(Ivk_oCSpawnManager_CheckRemoveNpc)(npc);
	}

}