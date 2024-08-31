// Supported with union (c) 2020 Union team
// Union HEADER file

namespace GOTHIC_ENGINE {

    struct positionEntry
    {
        CStringA worldName;
        CStringA name;

        zMAT4 trafoObjToWorld;

        bool operator<(const positionEntry& e) const
        {
            return worldName < e.worldName;
        };
    };

    std::vector<positionEntry> V_SavedPosition;

}