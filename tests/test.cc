// Copyright (c) 2020 CS126SP20. All rights reserved.

#define CATCH_CONFIG_MAIN

#include <catch2/catch.hpp>
#include <cinder/Rand.h>
#include <Location.h>

#include <mylibrary/example.h>


TEST_CASE("Random sanity test", "[random]") {
  const float random = cinder::randFloat();
  REQUIRE(0. <= random);
  REQUIRE(random <= 1.);
}
TEST_CASE("Location distance test", "[distance]") {
  Location loc = {0, 0};
  REQUIRE(loc.distance({3,4}) == 5);
}
