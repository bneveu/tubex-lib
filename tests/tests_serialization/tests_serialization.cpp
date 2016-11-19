/* ============================================================================
 *  tube-lib - Tube class tests
 * ============================================================================
 *  Copyright : Copyright 2016 Simon Rohou
 *  License   : This program can be distributed under the terms of
 *              the Apache License, Version 2.0. See the file LICENSE.
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2015
 * ---------------------------------------------------------------------------- */

#define CATCH_CONFIG_MAIN
#include "../catch/catch.hpp"
#include "../catch/catch_interval.hpp"
#include "../tests_cases.hpp"
#include <stdio.h>

using namespace Catch;
using namespace Detail;

bool testSerialization(const Tube& tube1)
{
  string filename = "test_serialization.tube";

  tube1.serialize(filename); // serialization
  Tube tube2(filename); // deserialization
  remove(filename.c_str());

  return tube1 == tube2;
}

TEST_CASE("(de)serializations on bounded tubes", "[core]")
{
  SECTION("Test tube1")
  {
    REQUIRE(testSerialization(tubeTest1()));
  }

  SECTION("Test tube1(01)")
  {
    REQUIRE(testSerialization(tubeTest1_01()));    
  }

  SECTION("Test tube2")
  {
    REQUIRE(testSerialization(tubeTest2()));
  }

  SECTION("Test tube3")
  {
    REQUIRE(testSerialization(tubeTest3()));
  }

  SECTION("Test tube4")
  {
    REQUIRE(testSerialization(tubeTest4()));
  }

  SECTION("Test tube4(05)")
  {
    REQUIRE(testSerialization(tubeTest4_05()));
  }
}

TEST_CASE("(de)serializations on unbounded tubes", "[core]")
{
  SECTION("Test POS_REALS")
  {
    Tube tube = tubeTest1();
    tube.set(Interval::POS_REALS, tube.size() / 2);
    REQUIRE(testSerialization(tube));
    tube.set(Interval::POS_REALS);
    REQUIRE(testSerialization(tube));
  }

  SECTION("Test NEG_REALS")
  {
    Tube tube = tubeTest2();
    tube.set(Interval::NEG_REALS, 5);
    REQUIRE(testSerialization(tube));
    tube.set(Interval::NEG_REALS);
    REQUIRE(testSerialization(tube));
  }

  SECTION("Test ALL_REALS")
  {
    Tube tube = tubeTest3();
    tube.set(Interval::ALL_REALS, 1);
    REQUIRE(testSerialization(tube));
    tube.set(Interval::ALL_REALS);
    REQUIRE(testSerialization(tube));
  }

  SECTION("Test EMPTY_SET")
  {
    Tube tube = tubeTest4();
    tube.set(Interval::EMPTY_SET, 0);
    tube.set(Interval::EMPTY_SET, 8);
    REQUIRE(testSerialization(tube));
    tube.set(Interval::EMPTY_SET);
    REQUIRE(testSerialization(tube));
  }
}