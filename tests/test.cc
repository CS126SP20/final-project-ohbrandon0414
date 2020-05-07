// Copyright (c) 2020 CS126SP20. All rights reserved.

#define CATCH_CONFIG_MAIN

#include <catch2/catch.hpp>
#include <cinder/Rand.h>
#include <Location.h>
#include <Rock.h>
#include <engine.h>
#include <Board.h>
#include <bot.h>

///Test for Location Object
TEST_CASE("Location test") {
  Location loc = {0, 0};
  REQUIRE(loc.distance({3,4}) == 5);
  REQUIRE(loc.operator==(Location(0,0)));
  REQUIRE(loc.GetX() == 0);
  REQUIRE(loc.GetY() == 0);
}

///Test for Rock Object
TEST_CASE("Rock test") {
  b2Vec2 gravity(0.0f, 0);
  b2World* m_world = new b2World(gravity);
  Rock* rock = new Rock(m_world, {0, 0}, true);
  REQUIRE(rock->IsRed());
  REQUIRE(!rock->IsLaunched());

  REQUIRE(rock->IsCompletelyStopped());
  REQUIRE(rock->GetBody()->GetLinearVelocity() == b2Vec2_zero);
}

/// Test for the board
TEST_CASE("Board test") {
  b2Vec2 gravity(0.0f, 0);
  b2World* m_world = new b2World(gravity);

  Board* board = new Board(m_world, true);
  REQUIRE(board->GetBackLine() >= board->GetTeePoint().GetX());
}

/// Test the engines
TEST_CASE("Engine test-Before Set Ends") {
  b2Vec2 gravity(0.0f, 0);
  b2World* m_world = new b2World(gravity);
  Board* board = new Board(m_world, true);

  engine* engine_ = new engine(board);

  SECTION("Remove and Create Rock") {
    Rock* rock = new Rock(m_world, {100, 1000}, true);
    engine_->CreateRock(rock);
    REQUIRE(engine_->GetRocks().size() == 1);
    REQUIRE(engine_->GetCurrentRock() == rock);

    engine_->RemoveRock(rock);
    REQUIRE(engine_->GetCurrentRock() == nullptr);
    REQUIRE(engine_->GetRocks().empty());
  }

  SECTION("Check for Out of Bounds") {
    engine_->CreateRock(new Rock(m_world, {0, 0}, true));
    engine_->CheckOutOfBoundsVertical();
    engine_->CreateRock(new Rock(m_world, {100, 1000}, true));
    engine_->CheckOutOfBoundsVertical();
    REQUIRE(engine_->GetRocks().empty());

    engine_->CreateRock(new Rock(m_world, {2050, 500}, false));
    engine_->CheckOutOfBoundsHorizontal();
    REQUIRE(engine_->GetRocks().empty());

    engine_->CreateRock(new Rock(m_world, {1700, 500}, false));
    engine_->CheckOutOfBoundsVertical();
    engine_->CheckOutOfBoundsHorizontal();
    REQUIRE(!engine_->GetRocks().empty());
  }
}

TEST_CASE("Engine Test- After Set Ends") {
  b2Vec2 gravity(0.0f, 0);
  b2World* m_world = new b2World(gravity);
  Board* board = new Board(m_world, true);

  engine* engine_ = new engine(board);

  SECTION("Yellow Wins by 2") {
    engine_->CreateRock(new Rock(m_world, {1500, 300}, true));
    engine_->CreateRock(new Rock(m_world, {1600, 400}, false));
    engine_->CreateRock(new Rock(m_world, {1600, 450}, false));
    engine_->CreateRock(new Rock(m_world, {1700, 500}, true));

    engine_->UpdateRocksInHouse();
    engine_->SetWinner();
    REQUIRE(engine_->GetWinner() == engine::WinnerState::YellowWins);
    REQUIRE(engine_->GetWinnerScore() == 2);
  }

  SECTION("Yellow Wins by 1") {
    engine_->CreateRock(new Rock(m_world, {1500, 300}, true));
    engine_->CreateRock(new Rock(m_world, {1600, 400}, false));
    engine_->CreateRock(new Rock(m_world, {1600, 450}, true));
    engine_->CreateRock(new Rock(m_world, {1700, 500}, false));

    engine_->UpdateRocksInHouse();
    engine_->SetWinner();
    REQUIRE(engine_->GetWinner() == engine::WinnerState::YellowWins);
    REQUIRE(engine_->GetWinnerScore() == 1);
  }

  SECTION("Tie") {
    engine_->CreateRock(new Rock(m_world, {1400, 300}, true));
    engine_->CreateRock(new Rock(m_world, {1400, 400}, false));
    engine_->CreateRock(new Rock(m_world, {1400, 450}, true));
    engine_->CreateRock(new Rock(m_world, {1400, 500}, false));

    engine_->UpdateRocksInHouse();
    engine_->SetWinner();
    REQUIRE(engine_->GetWinner() == engine::WinnerState::NoWinner);
    REQUIRE(engine_->GetWinnerScore() == 0);
  }

  SECTION("Reset") {
    engine_->CreateRock(new Rock(m_world, {1400, 300}, true));
    engine_->CreateRock(new Rock(m_world, {1400, 400}, false));
    engine_->CreateRock(new Rock(m_world, {1400, 450}, true));
    engine_->CreateRock(new Rock(m_world, {1400, 500}, false));
    engine_->Reset();
    REQUIRE(engine_->GetCurrentRock() == nullptr);
    REQUIRE(engine_->GetRocks().empty());
    REQUIRE(engine_->GetRedRocksInHouse().empty());
    REQUIRE(engine_->GetYellowRocksInHouse().empty());
    REQUIRE(engine_->GetRedLeft() == 5);
    REQUIRE(engine_->AreAllRocksAreSlowed());
    REQUIRE(!engine_->GetIsYPointSelected());
  }
}

///Test for cpu strategy
TEST_CASE("Bot test") {
  b2Vec2 gravity(0.0f, 0);
  b2World* m_world = new b2World(gravity);
  Board* board = new Board(m_world, true);

  engine* engine_ = new engine(board);
  bot* bot_ = new bot(board, engine_);

  SECTION("On tee") {
    engine_->CreateRock(new Rock(m_world, {1600, 400}, true));

    REQUIRE(bot_->GetPlacement() < 500);
    REQUIRE(bot_->GetPlacement() > 200);
    REQUIRE(bot_->GetForce().x == 1300000.0f);
    REQUIRE(bot_->GetForce().y == 59000.0f);
  }

  SECTION("Empty") {
    REQUIRE(bot_->GetForce().x == 1300000.0f);
    REQUIRE(bot_->GetForce().y == 33000.0f);
  }

}




