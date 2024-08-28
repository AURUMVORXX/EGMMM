// Supported with union (c) 2020 Union team
// Union SOURCE file

namespace GOTHIC_ENGINE {
	
	void oCNpc::setHolded(bool holded)
	{
		m_holded = holded;
	}

	const bool oCNpc::isHolded()
	{
		return m_holded;
	}

	// ----------------------------------------------------

	HOOK Ivk_oCNpc AS(0x0072D950, &oCNpc::oCNpc_IVK);
	oCNpc* oCNpc::oCNpc_IVK()
	{
		m_holded = false;

		return THISCALL(Ivk_oCNpc)();
	}

	HOOK Ivk_oCNpc_Disable AS(&oCNpc::Disable, &oCNpc::Disable_IVK);
	void oCNpc::Disable_IVK()
	{
		if (isHolded())
			return;

		THISCALL(Ivk_oCNpc_Disable)();
	}

	HOOK Ivk_oCNpc_Equip AS(&oCNpc::Equip, &oCNpc::Equip_IVK);
	void oCNpc::Equip_IVK(oCItem* item)
	{
		if (ogame->isInfiniteRings() && item)
		{
			int category = inventory2.GetCategory(item);

			if (category == INV_MAGIC && item->HasFlag(ITM_FLAG_RING))
			{
				if (!item->HasFlag(ITM_FLAG_ACTIVE))
				{
					AddItemEffects(item);
					item->SetFlag(ITM_FLAG_ACTIVE);
				}
				else
				{
					RemoveItemEffects(item);
					item->ClearFlag(ITM_FLAG_ACTIVE);
				}

				return;
			}
		}

		THISCALL(Ivk_oCNpc_Equip)(item);
	}
}