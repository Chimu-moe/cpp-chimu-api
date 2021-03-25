// Tests for https://api.chimu.moe/v1/set/:set_id

#include "chimu/api.hh"
#include "chimu/structs.hh"

#include <gtest/gtest.h>

TEST(ChimuAPIv1, GetSet)
{
    Chimu::APIAccess access;

    // Make sure we're connected
    ASSERT_TRUE(access.isConnected());

    // Get the specific beatmap set
    Chimu::BeatmapSet set = access.getBeatmapSet(1);

    ASSERT_EQ(set.m_uSetID, 1);
    ASSERT_EQ(set.m_vChildrenBeatmaps[0].m_iBeatmapID, 75);
    ASSERT_EQ(set.m_eGenre, Chimu::Genre::Game);
}
