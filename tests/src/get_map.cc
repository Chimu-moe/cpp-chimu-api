// Tests for https://api.chimu.moe/v1/map/:set_id

#include "chimu/api.hh"

#include <gtest/gtest.h>

TEST(ChimuAPIv1, GetMap)
{
    Chimu::APIAccess access;

    // Make sure we're connected
    ASSERT_TRUE(access.isConnected());

    // Get the specific beatmap
    Chimu::Beatmap map = access.getBeatmap(75);

    ASSERT_EQ(map.m_iBeatmapID, 75);
    ASSERT_EQ(map.m_iParentSetID, 1);
}
