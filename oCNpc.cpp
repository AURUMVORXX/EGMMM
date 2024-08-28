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

	HOOK Ivk_oCNpc_Disable AS(&oCNpc::Disable, &oCNpc::Disable_IVK);
	void oCNpc::Disable_IVK()
	{
		if (isHolded())
			return;

		THISCALL(Ivk_oCNpc_Disable)();
	}
}