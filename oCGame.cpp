// Supported with union (c) 2020 Union team
// Union SOURCE file

namespace GOTHIC_ENGINE {
	
	void oCGame::setIgnoreFocusItems(bool toggle)	{ oCNpcFocus::focus->i_prio = toggle ? -1 : 1; }
	const bool oCGame::isIgnoreFocusItems()			{ return oCNpcFocus::focus->i_prio == -1; }

	void oCGame::setInfiniteRings(bool toggle)		{ m_infiniteRings = toggle; }
	const bool oCGame::isInfiniteRings()			{ return m_infiniteRings; }
}