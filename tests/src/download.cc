// Tests for https://api.chimu.moe/v1/set/:set_id

#include "chimu/api.hh"
#include "chimu/structs.hh"

#include <gtest/gtest.h>

TEST(ChimuAPIv1, Download)
{
    Chimu::APIAccess access;

    // Make sure we're connected
    ASSERT_TRUE(access.isConnected());

    // Get the specific beatmap set
    std::vector<uint8_t> data = access.download(1);

    // We cant really test it other than hoping for more than one byte
    ASSERT_GT(data.size(), 0);
}
