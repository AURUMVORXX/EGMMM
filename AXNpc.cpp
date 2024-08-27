// Supported with union (c) 2020 Union team
// Union SOURCE file

namespace GOTHIC_ENGINE {
	
	zCLASS_UNION_DEFINITION(AXNpc, oCNpc, 0, 0);

	void AXNpc::setHolded(bool holded)
	{
		m_holded = holded;
	}

	bool AXNpc::isHolded() const
	{
		return m_holded;
	}

	void AXNpc::Disable()
	{
		if (isHolded())
			return;
	}
}