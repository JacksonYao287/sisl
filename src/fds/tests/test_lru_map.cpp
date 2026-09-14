/*********************************************************************************
 * Modifications Copyright 2017-2019 eBay Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *    https://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software distributed
 * under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
 * CONDITIONS OF ANY KIND, either express or implied. See the License for the
 * specific language governing permissions and limitations under the License.
 *
 *********************************************************************************/

#include <gtest/gtest.h>

#include "sisl/fds/lru_map.hpp"

namespace {

TEST(LruMapTest, AccessesMostRecentlyUsedEntriesFirst) {
    sisl::LruMap< int, int > cache(2);

    cache.set(1, 10);
    cache.set(2, 20);

    EXPECT_EQ(cache.get(1), 10);
    EXPECT_EQ(cache.get(2), 20);

    cache.set(3, 30);
    EXPECT_EQ(cache.get(1), 0);
    EXPECT_EQ(cache.get(2), 20);
    EXPECT_EQ(cache.get(3), 30);
}

TEST(LruMapTest, UpdatesExistingKeyAndRefreshesRecency) {
    sisl::LruMap< int, std::string > cache(2);

    cache.set(1, "old");
    cache.set(2, "new");
    cache.set(1, "fresh");
    cache.set(3, "latest");

    EXPECT_EQ(cache.get(1), "fresh");
    EXPECT_EQ(cache.get(2), "");
    EXPECT_EQ(cache.get(3), "latest");
}

TEST(LruMapTest, IteratorVisitsMostRecentlyUsedFirst) {
    sisl::LruMap< int, int > cache(3);
    cache.set(1, 11);
    cache.set(2, 22);
    cache.set(3, 33);

    auto it = cache.begin();
    ASSERT_NE(it, cache.end());
    EXPECT_EQ(it->first, 3);
    EXPECT_EQ(it->second, 33);

    ++it;
    ASSERT_NE(it, cache.end());
    EXPECT_EQ(it->first, 2);
    EXPECT_EQ(it->second, 22);

    ++it;
    ASSERT_NE(it, cache.end());
    EXPECT_EQ(it->first, 1);
    EXPECT_EQ(it->second, 11);

    ++it;
    EXPECT_EQ(it, cache.end());
}

} // namespace

int main(int argc, char* argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
