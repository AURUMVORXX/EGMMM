// Supported with union (c) 2020 Union team
// Union SOURCE file

namespace GOTHIC_ENGINE {
	
	HOOK Ivk_oCWorldTimer_Timer AS(&oCWorldTimer::Timer, &oCWorldTimer::Timer_IVK);
	void oCWorldTimer::Timer_IVK()
	{
		if (!ogame->isHoldTime())
			THISCALL(Ivk_oCWorldTimer_Timer)();
	}
}