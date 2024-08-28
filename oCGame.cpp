// Supported with union (c) 2020 Union team
// Union SOURCE file

namespace GOTHIC_ENGINE {
	
	void oCGame::setIgnoreFocusItems(bool toggle)	{ oCNpcFocus::focus->i_prio = toggle ? -1 : 1; }
	const bool oCGame::isIgnoreFocusItems()			{ return oCNpcFocus::focus->i_prio == -1; }

	void oCGame::setInfiniteRings(bool toggle)		{ m_infiniteRings = toggle; }
	const bool oCGame::isInfiniteRings()			{ return m_infiniteRings; }

	void oCGame::setHoldTime(bool toggle)			{ m_holdTime = toggle; }
	const bool oCGame::isHoldTime()					{ return m_holdTime; }

	HOOK Ivk_oCGame_ShowDebugInfos AS(&oCGame::ShowDebugInfos, &oCGame::ShowDebugInfos_IVK);
	void oCGame::ShowDebugInfos_IVK()
	{
		int savedShowTime = game_showtime;
		game_showtime = FALSE;

		THISCALL(Ivk_oCGame_ShowDebugInfos)();

		game_showtime = savedShowTime;
		
		if (game_showtime)
		{
			zSTRING timeString		= GetWorldTimer()->GetTimeString();
			int stringWidth			= screen->FontSize(timeString);

			if (isHoldTime())
				screen->SetFontColor(zCOLOR(255, 255, 0));

			screen->Print(8191 - stringWidth, 0, timeString);
			screen->SetFontColor(zCOLOR(255, 255, 255));
		}
	}
}