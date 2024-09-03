// Supported with union (c) 2020 Union team

// User API for oCGame
// Add your methods here

bool m_infiniteRings{ false };
bool m_holdTime{ false };

// ----------------------------------

void setIgnoreFocusItems(bool);
const bool isIgnoreFocusItems();

void setInfiniteRings(bool);
const bool isInfiniteRings();

void setHoldTime(bool);
const bool isHoldTime();

void addSavedPosition(CStringA, oCNpc*);
void removeSavedPosition(CStringA);
zMAT4 getSavedPosition(CStringA);
std::map<CStringA, int> getSavedPositionsEntries();

template <typename T>
T findSavedPosition(CStringA);

void archiveSavedPositions(int);
void unarchiveSavedPositions(int);

void addHoldedNpc(oCNpc*);
void removeHoldedNpc(oCNpc*);
void archiveHoldedNpcs(int);
void unarchiveHoldedNpcs(int);

// ----------------------------------

void ShowDebugInfos_IVK();
void UpdatePlayerStatus_IVK();
void WriteSavegame_IVK(int, zBOOL);
void LoadSavegame_IVK(int, zBOOL);
void ChangeLevel_IVK(const zSTRING&, const zSTRING&);