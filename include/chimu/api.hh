#pragma once

#include "structs.hh"

#include <exception>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

namespace Chimu
{
    constexpr const char* CHIMU_ENDPOINT = "https://api.chimu.moe";

    enum APIError
    {
        Unknown = 0,
        IntError = 101,
        KeyRequired = 102,
        StateNotSet = 103,
        BeatmapNotFound = 104,
        BeatmapUnavailable = 105,
        NoSearchResults = 106,
    };

    class APIException : public std::exception
    {
    private:
        APIError m_eCode;
        std::string m_sReason;

    public:
        APIException(APIError eCode, std::string_view svReason)
        {
            m_eCode = eCode;
            m_sReason = svReason;
        }

        uint32_t code() { return m_eCode; }

        const char* what() const noexcept override { return m_sReason.c_str(); }
    };

    class APIAccess
    {
    private:
        enum class Method
        {
            GET,
            POST,
        };

        void* m_hCurl;

        std::vector<uint8_t> requestRaw(std::string_view svURL,
                                        Method eMethod = Method::GET,
                                        std::vector<uint8_t> vBody = {});

        std::string requestString(std::string_view svURL,
                                  Method eMethod = Method::GET,
                                  std::vector<uint8_t> vBody = {});

        template<typename T>
        T requestJSON(std::string_view svURL, Method eMethod = Method::GET,
                      std::vector<uint8_t> vBody = {})
        {
            auto data = requestString(svURL, eMethod, std::move(vBody));

            return T::fromJson(data);
        }

    public:
        APIAccess();
        ~APIAccess();

        bool isConnected();

        BeatmapSet getBeatmapSet(uint32_t iSetID);
        Beatmap getBeatmap(uint32_t iBeatmapID);

        std::vector<BeatmapSet> Search(
            std::string_view sQuery, uint32_t iAmount, uint32_t iOffset,
            RankedStatus eStatus = RankedStatus::Any,
            PlayMode eMode = PlayMode::Any, Genre eGenre = Genre::Any,
            Language eLanguage = Language::Any, float fMinAR = -1,
            float fMaxAR = -1, float fMinOD = -1, float fMaxOD = -1,
            float fMinCS = -1, float fMaxCS = -1, float fMinHP = -1,
            float fMaxHP = -1, float fMinDiff = -1, float fMaxDiff = -1,
            float fMinBPM = -1, float fMaxBPM = -1, int32_t iMinLength = -1,
            int32_t iMaxLength = -1);

        std::vector<uint8_t> download(uint32_t iSetID, bool bNoVideo = false);

    private:
    };
}  // namespace Chimu
