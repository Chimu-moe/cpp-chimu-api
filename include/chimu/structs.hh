#pragma once

#include <chrono>
#include <string>
#include <vector>

namespace Chimu
{
    enum class PlayMode
    {
        Any = -1,

        Osu = 0,
        Taiko = 1,
        CatchTheBeat = 2,
        Mania = 3,
    };

    enum class RankedStatus
    {
        Any = -1,

        Graveyard = -2,
        WIP = -1,
        Pending = 0,
        Ranked = 1,
        Approved = 2,
        Qualified = 3,
        Loved = 4
    };

    enum class Genre
    {
        Any = -1,
        Unspecified = 1,
        Game = 2,
        Anime = 3,
        Rock = 4,
        Pop = 5,
        Other = 6,
        Novelty = 7,
        HipHop = 9,
        Electronic = 10,
    };

    enum class Language
    {
        Any = 0,
        Other = 1,
        English = 2,
        Japanese = 3,
        Chinese = 4,
        Instrumental = 5,
        Korean = 6,
        French = 7,
        German = 8,
        Swedish = 9,
        Spanish = 10,
        Italian = 11
    };

    struct Beatmap
    {
    public:
        uint32_t m_iBeatmapID;
        uint32_t m_iParentSetID;
        std::string m_sDiffName;
        std::string m_sFileMD5;
        PlayMode m_eMode;
        float m_fBPM;
        float m_fAR;
        float m_fOD;
        float m_fCS;
        float m_fHP;
        uint32_t m_iTotalLength;
        uint32_t m_iHitLength;
        uint32_t m_iPlayCount;
        uint32_t m_iPassCount;
        uint32_t m_iMaxCombo;
        float m_fDifficultyRating;
        std::string m_sOsuFile;
        std::string m_sDownloadPath;

    public:
        static Beatmap fromJson(const std::string_view& svData);
    };

    struct BeatmapSet
    {
    public:
        uint32_t m_uSetID;
        RankedStatus m_eRankedStatus;
        std::vector<Beatmap> m_vChildrenBeatmaps;
        std::string m_sApprovedDate;
        std::string m_sLastUpdate;
        std::string m_sLastChecked;
        std::string m_sArtist;
        std::string m_sTitle;
        std::string m_sCreator;
        std::string m_sSource;
        std::string m_sTags;
        Genre m_eGenre;
        Language m_eLanguage;
        int64_t m_uFavourites;
        bool m_bHasVideo;
        bool m_bIsDisabled;

    public:
        static BeatmapSet fromJson(const std::string_view& svData);
    };
}  // namespace Chimu
