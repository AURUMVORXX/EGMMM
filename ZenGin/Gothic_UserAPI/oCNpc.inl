// Supported with union (c) 2020 Union team

// User API for oCNpc
// Add your methods here

bool m_holded{ false };
bool m_dontArchive{ false };

// ----------------------------

void setHolded(bool);
const bool isHolded();

void setDontArchive(bool);
const bool isDontArchive();

// ----------------------------

oCNpc* oCNpc_IVK();
void Disable_IVK();
void Equip_IVK(oCItem*);
void Archive_IVK(zCArchiver&);
void Unarchive_IVK(zCArchiver&);