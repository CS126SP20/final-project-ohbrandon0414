// Copyright (c) 2020 [Your Name]. All rights reserved.

#include "my_app.h"

#include <cinder/gl/gl.h>
#include <Box2D/Box2D.h>
#include <ObjectArray.h>


#include <vector>
#include <Rock.h>
#include <bot.h>
#include <Board.h>
#include <engine.h>
#include <Location.h>
namespace myapp {

using cinder::app::KeyEvent;

int power;
int y_position;
int selected_power;
b2World* m_world;
Board* board;
engine* engine_;
bot* bot_;
Rock* currentRock;
int sets = 0;
std::string r_score = "R";
std::string y_score = "Y";
bool is_red_turn = false;
bool is_y_point_selected;
bool is_using_bot;


MyApp::MyApp() { }

void MyApp::setup() {
  // basic set up
  b2Vec2 gravity(0.0f, 0.0f);
  m_world = new b2World(gravity);
  is_angle_set = false;
  should_show_angle = false;
  should_show_placement = true;
  is_set_over = false;
  is_using_key = false;
  is_using_mouse = true;
  is_using_ob = true;
  is_y_point_selected = false;
  is_using_bot = false;
}

void MyApp::update() {
  // update the state if the set if over or not
  if (is_start_screen) {return;}
  if (!is_set_over) {
    engine_->Step();
    currentRock = engine_->GetCurrentRock();

    if(is_using_bot) {
      StepBot();
    }

    is_set_over = engine_->GetIsSetOver();
    UpdateAttributes();
    // updates the current rock with in this file

    for( int i = 0; i < 30; ++i ){
      m_world->Step( 1 / 100.0f, 7, 10 );
    }
  }
}

void MyApp::draw() {
  if (is_start_screen) {
    DrawStartScreen();
    return;
  }
  if (is_set_over) {
    DrawGameOver();
    return;
  }
  cinder::gl::enableAlphaBlending();
  cinder::gl::clear();

  board->Display();

  // attributes are drawn in different situations
  DrawAttributes();
  // draw the score at the bottom
  DrawScore();

  // display all the rocks at the moment
  for(Rock* temp: engine_->GetRocks()) {
    temp->Display();
  }
}


void MyApp::keyDown(KeyEvent event){
  switch (event.getCode()) {
      // power of the rock
      case KeyEvent::KEY_p: {
      if (is_using_key && is_angle_set && !engine_->GetIsLaunched() && !engine_->GetIsSetOver()) {
        // save the power selected to show it
        selected_power = power;
        // apply the force selected
        currentRock->GetBody()->ApplyLinearImpulse(
            {static_cast<float32>((power + 1) * 75000) + 1100000,
             angle_y_point * (power + 1)  * 500},
            currentRock->GetBody()->GetWorldCenter());
        // update the bool variables
        engine_->SetIsLaunched(true);
        engine_->SetIsYPointSelected(false);
        should_show_placement = true;
        is_angle_set = false;
        engine_->UpdateNumLaunches();
        is_red_turn = !is_red_turn;

      }
      break;
    }
      // placement of the rock
    case KeyEvent::KEY_d: {
      if (is_using_key && !engine_->GetIsYPointSelected()  && !engine_->GetIsSetOver()) {
        // if the current rock went out of bounds or they are all stable at their positions.
        if (currentRock == nullptr || engine_->AllRocksAreSlowed()) {
          engine_->SetYPoint(y_position);
          engine_->SetIsYPointSelected(true);
          should_show_placement = false;
          should_show_angle = true;
        }
      }
      break;
    }
      // angle
    case KeyEvent::KEY_a: {
      if (is_using_key && engine_->GetIsYPointSelected()
          && !engine_->GetIsLaunched()  && !engine_->GetIsSetOver()) {
        angle_y_point = y_position - currentRock->GetPosition().GetY();
        is_angle_set = true;
        should_show_angle = false;
      }
      break;
    }
  }
  }

void MyApp::mouseDown(cinder::app::MouseEvent event) {
  if(is_start_screen) {
    // choose options, default options are already chosen
    ChooseOptions(event);
    return;
  }
  if (engine_->IsLastRockLaunched() && !is_set_over) {
    return;
  }
  if(is_set_over && sets < 5) {
    // user presses continue
    if (event.getY() > 550) {
      ApplyScore();
      engine_->Reset();
      is_set_over = false;
      return;
    }
  }

  if (is_using_bot && !engine_->GetIsRedTurn()) {return;}

  if (is_using_mouse && !engine_->GetIsYPointSelected() && !is_start_screen) {
    // if the current rock went out of bounds or they are all stable at their positions.
    if (currentRock == nullptr || engine_->AllRocksAreSlowed()) {
      if(event.getY() > 275 && event.getY() < 550){
        engine_->SetYPoint(event.getY());
        engine_->SetIsYPointSelected(true);
        is_y_point_selected = true;
        should_show_placement = false;
        should_show_angle = true;
        return;
      }
    }
  }
  // user is in the game
  if (is_using_mouse && engine_->GetIsYPointSelected() && !engine_->GetIsLaunched()
      && event.getX() > board->GetFrontLine() + 200) {
    currentRock->GetBody()->ApplyLinearImpulse({static_cast<float32>(abs(currentRock->GetPosition().GetX() * 1000 - event.getPos().x * 1000)) ,
                                                static_cast<float32>((event.getPos().y * 1000
                                                                      - currentRock->GetPosition().GetY() * 1000))},
                                               currentRock->GetBody()->GetWorldCenter());

    // update variables
    engine_->SetIsLaunched(true);
    engine_->SetIsYPointSelected(false);
    is_angle_set = true;
    is_y_point_selected = false;
    should_show_angle = false;
    should_show_placement = true;
    engine_->UpdateNumLaunches();
  }
}

/// creates a textbox for the power
void MyApp::PrintText(const std::string& text, const glm::ivec2& size, const glm::vec2& loc, bool changer) {
  cinder::gl::color(0.7, changer,0);
  auto box = cinder::TextBox()
      .alignment(cinder::TextBox::LEFT)
      .font(cinder::Font("Bauhaus 93", 100))
      .size(size)
      .text(text);

  const auto box_size = box.getSize();
  const cinder::vec2 locp = {loc.x - box_size.x / 2, loc.y - box_size.y / 2};
  const auto surface = box.render();
  const auto texture = cinder::gl::Texture::create(surface);
  cinder::gl::draw(texture, locp);
}


void MyApp::UpdateAttributes() {
  int range = board->GetLowerSideLine() - board->GetUpperSideLine() - (2 * kRange);
  y_position = static_cast<int>((ci::app::getElapsedSeconds()) * 100);
  y_position = y_position % (range * 2);

  if (y_position >= range) {
    y_position = (range* 2) - y_position;
  }
  y_position += board->GetUpperSideLine() + kRange;

  // takes care of the power gage.
  power = static_cast<int>((ci::app::getElapsedSeconds()) * 3);
  power = power % 10;
}

void MyApp::DrawAttributes() {
  if (engine_->IsLastRockLaunched()) {return;}
  if (is_using_key && should_show_placement && (currentRock == nullptr || currentRock->IsSlowedDown())) {
    cinder::gl::color(0,0,0);
    cinder::gl::drawSolidCircle({100, y_position}, 25);
    return;
  }
  if(is_using_mouse && should_show_placement && (currentRock == nullptr || currentRock->IsSlowedDown())) {
    cinder::gl::color(0,0,0);
    cinder::gl::drawSolidCircle(getMousePos() - getWindowPos(), 25);
    return;
  }

  if (should_show_angle) {
    if (is_using_key) {
      cinder::gl::color(0,0,0);
      cinder::gl::drawLine({currentRock->GetPosition().GetX(), currentRock->GetPosition().GetY()},
                           {currentRock->GetPosition().GetX() + 400, y_position});
    }
    if (is_using_mouse) {
      cinder::gl::color(0,0,0);
      cinder::gl::drawLine({currentRock->GetPosition().GetX(), currentRock->GetPosition().GetY()},
                           getMousePos() - getWindowPos());
    }
  }

  if (is_using_key && is_angle_set && !engine_->GetIsLaunched()) {
    std::string str = std::to_string(power + 1);
    PrintText(str, {1000, 1000}, {1400, 600}, is_red_turn);
  }
  if (is_using_key && engine_->GetIsLaunched()) {
    std::string str2 = std::to_string(selected_power + 1);
    PrintText(str2, {1000, 1000}, {1400,600}, !is_red_turn);
  }
}

void MyApp::DrawGameOver() {
  PrintText("SET OVER", {1000, 1000} , {1200, 700}, false);
  std::string winner;
  std::string score;
  if (engine_->GetWinner() == engine::WinnerState::NoWinner) {
    winner = "TIE";
  } else {
     score = std::to_string(engine_->GetWinnerScore());
    if (engine_->GetWinner() == engine::WinnerState::YellowWins) {
      winner = "YELLOW WINS ";

    } else {
      winner = "RED WINS ";
    }
  }
  PrintText(winner, {1000, 1000} , {1200, 1000} , false);
  PrintText("CONTINUE", {1000, 1000} , {1200, 1200}, false);
}

void MyApp::DrawStartScreen() {
  PrintText("CLICK ON YOUR OPTION", {1600, 500}, {1300, 300}, false);
  /// pvp or pvc
  PrintText("AGAINST: ", {1500, 500}, {1050, 500}, false);
  PrintText("PERSON", {1500, 500}, {1500, 500}, is_using_bot == false);
  PrintText("COMPUTER", {1500, 500}, {1900, 500}, is_using_bot == true);
  // mouse or keys
  PrintText("USE: ", {1500, 500}, {1300, 600}, false);
  PrintText("MOUSE", {1500, 500}, {1500, 600}, is_using_mouse);
  PrintText("KEYS", {1500, 500}, {1900, 600}, is_using_key);
  // out of bounds yes or no
  PrintText("OUT OF BOUNDS: ", {1300, 500}, {900, 700}, false);
  PrintText("YES", {1500, 500}, {1600, 700}, is_using_ob == true);
  PrintText("NO", {1500, 500}, {1900, 700}, is_using_ob == false);
  // to move on
  PrintText("NEXT", {1500, 500}, {1500, 900}, false);
}

void MyApp::ChooseOptions(const cinder::app::MouseEvent& event) {
  if(event.getY() < 350 && event.getY() > 250) {
    if (event.getX() < 1000) {
      is_using_bot = false;
    }
    if(event.getX() > 1000) {
      is_using_bot = true;
    }
  }
  if(event.getY() < 450 && event.getY() > 350) {
    if (event.getX() < 1000) {
      is_using_mouse = true;
      is_using_key = false;
    }
    if(event.getX() > 1000) {
      is_using_key = true;
      is_using_mouse = false;
    }
  }
  if (event.getY() < 550 && event.getY() > 450) {
    if (event.getX() < 1000){
      is_using_ob = true;
    }
    if (event.getX() > 1000){
      is_using_ob = false;
    }
  }
  if (event.getY() > 550) {
    board = new Board(m_world, is_using_ob);
    engine_ = new engine(board);
    bot_ = new bot(board, engine_);
    engine_->SetUseOB(is_using_ob);
    is_start_screen = false;
  }
}

void MyApp::DrawScore() {
  PrintText(r_score, {1500, 500}, {900, 850}, false);
  PrintText(std::to_string(engine_->GetRedLeft()), {1500, 500}, {800, 850}, false);
  PrintText(std::to_string(engine_->GetYellowLeft()), {1500, 500}, {800, 950}, true);
  PrintText(y_score, {1500, 500}, {900, 950}, true);
}

void MyApp::ApplyScore() {
  sets++;
  if (engine_->GetWinner() == engine::WinnerState::YellowWins) {
    y_score+= " " +  std::to_string(engine_->GetWinnerScore());
    r_score+=" 0";

  } else if(engine_->GetWinner() == engine::WinnerState::RedWins){
    r_score+= " " +  std::to_string(engine_->GetWinnerScore());
    y_score+=" 0";
  } else {
    // situation of tie
    r_score+= " 0";
    y_score+= " 0";
  }
}
void MyApp::StepBot() {
  if(!engine_->GetIsRedTurn()) {
    if (!engine_->GetIsYPointSelected()) {
      engine_->SetYPoint(bot_->GetPlacement());
      engine_->SetIsYPointSelected(true);
      return;
    }
    if(engine_->GetIsYPointSelected() && !engine_->GetIsLaunched() && !currentRock->IsRed()) {
      currentRock->GetBody()->ApplyLinearImpulse(bot_->GetForce(), currentRock->GetBody()->GetWorldCenter());
      std::cout<<currentRock->IsLaunched();
      engine_->SetIsLaunched(true);
      engine_->SetIsYPointSelected(false);
      engine_->UpdateNumLaunches();
      return;
    }
  }
}
}  // namespace myapp
