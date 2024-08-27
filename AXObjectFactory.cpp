// Supported with union (c) 2020 Union team
// Union SOURCE file

namespace GOTHIC_ENGINE {
	
	zCLASS_UNION_DEFINITION(AXObjectFactory, oCObjectFactory, 0, 0);

	oCNpc* AXObjectFactory::CreateNpc(int index)
	{
		oCNpc* npc = new AXNpc();

		if (index != zPAR_INDEX_UNDEF)
			npc->InitByScript(index, 0);

		return npc;
	}

}