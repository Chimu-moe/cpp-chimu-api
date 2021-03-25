#include <chimu/api.hh>
#include <curl/curl.h>
#include <fmt/format.h>
#include <nlohmann/json.hpp>

#include <iostream>
#include <iterator>

using namespace Chimu;

template<typename T>
struct APIResult
{
public:
    uint32_t m_iCode{};
    std::string m_sMessage{};
    T m_tData{};

public:
    static APIResult<T> fromJson(const std::string_view& svData)
    {
        auto json = nlohmann::json::parse(svData);

        APIResult<T> result;
        result.m_iCode = json["code"];
        result.m_sMessage = json["message"];

        if (result.m_iCode == 0)
            result.m_tData = T::fromJson(json["data"].dump());

        return result;
    }

    static APIResult<std::vector<T>> fromJsonVector(
        const std::string_view& svData)
    {
        auto json = nlohmann::json::parse(svData);

        APIResult<std::vector<T>> result;
        result.m_iCode = json["code"];
        result.m_sMessage = json["message"];

        if (result.m_iCode == 0)
        {
            for (auto json_ : json["data"])
            {
                result.m_tData.push_back(T::fromJson(json_.dump()));
            }
        }

        return result;
    }
};

size_t _writer(void* contents, size_t size, size_t nmemb,
               std::vector<uint8_t>* s)
{
    size_t newLength = size * nmemb;
    s->reserve(newLength);

    {
        auto begin = (uint8_t*) contents;
        auto end = ((uint8_t*) contents) + newLength;

        std::copy(begin, end, std::back_inserter(*s));
    }

    return newLength;
}

APIAccess::APIAccess()
{
    m_hCurl = curl_easy_init();
    if (!m_hCurl)
        throw APIException(APIError::Unknown, "Failed to initialize cURL");

    // Allow HTTP/1, HTTP/2, HTTP/3
    curl_easy_setopt(m_hCurl, CURLOPT_ALTSVC, "altsvc.txt");
    curl_easy_setopt(m_hCurl, CURLOPT_ALTSVC_CTRL,
                     (long) CURLALTSVC_H1 | CURLALTSVC_H2 | CURLALTSVC_H3);

    // Follow redirects, really important for downloads!
    curl_easy_setopt(m_hCurl, CURLOPT_FOLLOWLOCATION, true);

    curl_easy_setopt(m_hCurl, CURLOPT_WRITEFUNCTION, _writer);

#if (CURLPIPE_MULTIPLEX > 0)
    /* Reuse existing connections */
    curl_easy_setopt(m_hCurl, CURLOPT_PIPEWAIT, true);
#endif
}
APIAccess::~APIAccess()
{
    curl_easy_cleanup(m_hCurl);
}

std::vector<uint8_t> APIAccess::requestRaw(std::string_view svURL,
                                           Method eMethod,
                                           std::vector<uint8_t> vBody)
{
    curl_easy_setopt(m_hCurl, CURLOPT_URL, svURL.data());

    std::vector<uint8_t> vResultingData{};

    curl_easy_setopt(m_hCurl, CURLOPT_WRITEDATA, &vResultingData);

    CURLcode hResult = curl_easy_perform(m_hCurl);
    if (hResult != CURLE_OK)
        throw new APIException(APIError::Unknown, curl_easy_strerror(hResult));

    return vResultingData;
}
std::string APIAccess::requestString(std::string_view svURL, Method eMethod,
                                     std::vector<uint8_t> vBody)
{
    auto data = requestRaw(svURL, eMethod, std::move(vBody));

    return std::string(data.begin(), data.end());
}

bool APIAccess::isConnected()
{
    // This will just throw if we failed to establish an connection
    try
    {
        APIAccess::requestRaw("https://api.chimu.moe/");

        return true;
    }
    catch (const APIException& exception)
    {
        std::cout << exception.what() << std::endl;
        return false;
    }
}

BeatmapSet APIAccess::getBeatmapSet(uint32_t iSetID)
{
    auto result = requestJSON<APIResult<BeatmapSet>>(
        fmt::format("{}/v1/set/{}", CHIMU_ENDPOINT, iSetID));

    if (result.m_iCode != 0)
        throw APIException((APIError) result.m_iCode, result.m_sMessage);

    return result.m_tData;
}

Beatmap APIAccess::getBeatmap(uint32_t iBeatmapID)
{
    auto result = requestJSON<APIResult<Beatmap>>(
        fmt::format("{}/v1/map/{}", CHIMU_ENDPOINT, iBeatmapID));

    if (result.m_iCode != 0)
        throw APIException((APIError) result.m_iCode, result.m_sMessage);

    return result.m_tData;
}

std::vector<BeatmapSet> APIAccess::Search(
    std::string_view sQuery, uint32_t iAmount, uint32_t iOffset,
    RankedStatus eStatus, PlayMode eMode, Genre eGenre, Language eLanguage,
    float fMinAR, float fMaxAR, float fMinOD, float fMaxOD, float fMinCS,
    float fMaxCS, float fMinHP, float fMaxHP, float fMinDiff, float fMaxDiff,
    float fMinBPM, float fMaxBPM, int32_t iMinLength, int32_t iMaxLength)
{
    std::string q = "?query=" + std::string(sQuery);

    q += fmt::format("&amount={}", iAmount);
    q += fmt::format("&offset={}", iOffset);

    if (eStatus != RankedStatus::Any)
        q += fmt::format("&status={}", (int) eStatus);

    if (eMode != PlayMode::Any) q += fmt::format("&mode={}", (int) eMode);

    if (eGenre != Genre::Any) q += fmt::format("&genre={}", (int) eGenre);

    if (eLanguage != Language::Any)
        q += fmt::format("&language={}", (int) eLanguage);

    q += fmt::format("&min_ar={}", fMinAR);
    q += fmt::format("&min_od={}", fMinOD);
    q += fmt::format("&min_cs={}", fMinCS);
    q += fmt::format("&min_hp={}", fMinHP);
    q += fmt::format("&min_diff={}", fMinDiff);
    q += fmt::format("&min_bpm={}", fMinBPM);
    q += fmt::format("&min_length={}", iMinLength);

    q += fmt::format("&max_ar={}", fMaxAR);
    q += fmt::format("&max_od={}", fMaxOD);
    q += fmt::format("&max_cs={}", fMaxCS);
    q += fmt::format("&max_hp={}", fMaxHP);
    q += fmt::format("&max_diff={}", fMaxDiff);
    q += fmt::format("&max_bpm={}", fMaxBPM);
    q += fmt::format("&max_length={}", iMaxLength);

    auto rawJson =
        requestString(fmt::format("{}/v1/search{}", CHIMU_ENDPOINT, q));
    auto result = APIResult<BeatmapSet>::fromJsonVector(rawJson);

    if (result.m_iCode != 0)
        throw APIException((APIError) result.m_iCode, result.m_sMessage);

    return result.m_tData;
}

std::vector<uint8_t> APIAccess::download(uint32_t iSetID, bool bNoVideo)
{
    auto result = requestRaw(fmt::format(
        "{}/v1/download/{}?n={}", CHIMU_ENDPOINT, iSetID, (int) bNoVideo));

    if (result.size() > 2 && result[0] == '{' && result[1] == '"')
    {
        auto _result =
            APIResult<Beatmap>::fromJson((const char*) result.data());

        if (_result.m_iCode != 0)
            throw APIException((APIError) _result.m_iCode, _result.m_sMessage);
    }

    return requestRaw(fmt::format("{}/v1/download/{}?n={}", CHIMU_ENDPOINT,
                                  iSetID, (int) bNoVideo));
}

Beatmap Beatmap::fromJson(const std::string_view& svData)
{
    auto json = nlohmann::json::parse(svData);

    Beatmap beatmap;
    beatmap.m_iBeatmapID = json["BeatmapId"];
    beatmap.m_iParentSetID = json["ParentSetId"];
    beatmap.m_sDiffName = json["DiffName"];
    beatmap.m_sFileMD5 = json["FileMD5"];
    beatmap.m_eMode = json["Mode"];
    beatmap.m_fBPM = json["BPM"];
    beatmap.m_fAR = json["AR"];
    beatmap.m_fOD = json["OD"];
    beatmap.m_fCS = json["CS"];
    beatmap.m_fHP = json["HP"];
    beatmap.m_iTotalLength = json["TotalLength"];
    beatmap.m_iHitLength = json["HitLength"];
    beatmap.m_iPlayCount = json["Playcount"];
    beatmap.m_iPassCount = json["Passcount"];
    beatmap.m_iMaxCombo = json["MaxCombo"];
    beatmap.m_fDifficultyRating = json["DifficultyRating"];
    beatmap.m_sOsuFile = json["OsuFile"];
    beatmap.m_sDownloadPath = json["DownloadPath"];

    return beatmap;
}

BeatmapSet BeatmapSet::fromJson(const std::string_view& svData)
{
    auto json = nlohmann::json::parse(svData);

    BeatmapSet beatmapSet;
    beatmapSet.m_uSetID = json["SetId"];
    beatmapSet.m_eRankedStatus = json["RankedStatus"];

    for (auto json_ : json["ChildrenBeatmaps"])
    {
        beatmapSet.m_vChildrenBeatmaps.push_back(
            Beatmap::fromJson(json_.dump()));
    }

    beatmapSet.m_sApprovedDate = json["ApprovedDate"];
    beatmapSet.m_sLastUpdate = json["LastUpdate"];
    beatmapSet.m_sLastChecked = json["LastChecked"];

    beatmapSet.m_sArtist = json["Artist"];
    beatmapSet.m_sTitle = json["Title"];
    beatmapSet.m_sCreator = json["Creator"];
    beatmapSet.m_sSource = json["Source"];
    beatmapSet.m_sTags = json["Tags"];
    beatmapSet.m_eGenre = json["Genre"];
    beatmapSet.m_eLanguage = json["Language"];
    beatmapSet.m_uFavourites = json["Favourites"];

    beatmapSet.m_bHasVideo = json["HasVideo"];
    beatmapSet.m_bIsDisabled = json["Disabled"];

    return beatmapSet;
}
