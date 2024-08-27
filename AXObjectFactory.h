// Supported with union (c) 2020 Union team
// Union HEADER file

namespace GOTHIC_ENGINE {
	
	class AXObjectFactory : public oCObjectFactory
	{
	public:
		zCLASS_UNION_DECLARATION(AXObjectFactory);

		virtual oCNpc* CreateNpc(int);
	};

}