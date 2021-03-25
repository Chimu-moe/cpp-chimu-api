// Tests for https://api.chimu.moe/v1/set/:set_id

#include "chimu/api.hh"
#include "chimu/structs.hh"

#include <gtest/gtest.h>

TEST(ChimuAPIv1, Search)
{
    Chimu::APIAccess access;

    // Make sure we're connected
    ASSERT_TRUE(access.isConnected());

    // Get the specific beatmap set
    std::vector<Chimu::BeatmapSet> set = access.Search("peppy", 100, 0);

    // We cant really test it other than hoping for more than one result
    ASSERT_GT(set.size(), 0);
}
