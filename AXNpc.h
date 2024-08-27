// Supported with union (c) 2020 Union team
// Union HEADER file

namespace GOTHIC_ENGINE {

	class AXNpc : public oCNpc
	{
	private:

		bool m_holded{ false };

	public:
		zCLASS_UNION_DECLARATION(AXNpc);

		void setHolded(bool);
		bool isHolded() const;

		void Disable();
	};

}