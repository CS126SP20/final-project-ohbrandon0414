// Copyright (c) 2020 [Your Name]. All rights reserved.

#include "my_app.h"

#include <cinder/gl/gl.h>
#include <Box2D/Box2D.h>
#include <ObjectArray.h>


#include <vector>
#include <Rock.h>
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
Rock* currentRock;



MyApp::MyApp() { }

void MyApp::setup() {
  // basic set up
  b2Vec2 gravity(0.0f, 0);
  m_world = new b2World(gravity);
  board = new Board(m_world);
  engine_ = new engine(m_world, board);
  is_angle_set = false;
  should_show_angle = false;
  should_show_placement = true;
  is_game_over = false;
  use_key = false;
  use_mouse = true;
  use_ob = true;
}

void MyApp::update() {
  if (!is_game_over) {
    engine_->Step();
    if(engine_->GetIsGameOver()) {
      is_game_over = true;
    }
    UpdateAttributes();
    // updates the current rock with in this file
    currentRock = engine_->GetCurrentRock();

    for( int i = 0; i < 5; ++i ){
      m_world->Step( 1 / 100.0f, 10, 10 );
    }
  }
}

void MyApp::draw() {
  if (is_start_screen) {
    DrawStartScreen();
    return;
  }
  if (is_game_over) {
    DrawGameOver();
    return;
  }
  cinder::gl::enableAlphaBlending();
  cinder::gl::clear();

  board->Display();

  DrawAttributes();

  for(Rock* temp: engine_->GetRocks()) {
    temp->Display();
  }
}


void MyApp::keyDown(KeyEvent event){
  switch (event.getCode()) {
    case KeyEvent::KEY_1: {
      if(is_start_screen) {
        use_mouse = true;
        is_start_screen = false;
      }
      break;
    }
    case KeyEvent::KEY_2: {
      if(is_start_screen) {
        use_key = true;
        is_start_screen = false;
      }
      break;
    }
      case KeyEvent::KEY_p: {
      if (use_key && is_angle_set && !engine_->GetIsLaunched() && !engine_->GetIsGameOver()) {
        selected_power = power;
        currentRock->GetBody()->ApplyLinearImpulse(
            {static_cast<float32>((power + 1) * 75000) + 1100000,
             angle_y_point * (power + 1)  * 500},
            currentRock->GetBody()->GetWorldCenter());
        engine_->SetIsLaunched(true);
        engine_->SetIsYPointSelected(false);
        should_show_placement = true;
        is_angle_set = false;
        engine_->UpdateNumLaunches();
      }
      break;
    }
    case KeyEvent::KEY_d: {
      if (!engine_->GetIsYPointSelected()  && !engine_->GetIsGameOver()) {
        if (currentRock == nullptr || currentRock->IsSlowedDown()) {
          engine_->SetYPoint(y_position);
          engine_->SetIsYPointSelected(true);
          should_show_placement = false;
          should_show_angle = true;
        }

      }
      break;
    }
    case KeyEvent::KEY_a: {
      if (use_key && engine_->GetIsYPointSelected()
          && !engine_->GetIsLaunched()  && !engine_->GetIsGameOver()) {
        angle_y_point = y_position - currentRock->GetPosition().GetY();
        is_angle_set = true;
        should_show_angle = false;
      }
      break;
    }
  }
  }

/// mouse click controls the power gage.
void MyApp::mouseDown(cinder::app::MouseEvent event) {
  if(is_start_screen) {
      if(event.getY() < 450 && event.getY() > 350) {
        if (event.getX() < 1000) {
          use_mouse = true;
          use_key = false;
        }
        if(event.getX() > 1000) {
          use_key = true;
          use_mouse = false;
        }
      }
      if (event.getY() < 550 && event.getY() > 450) {
        if (event.getX() < 1000){
          use_ob = true;
        }
        if (event.getX() > 1000){
          use_ob = false;
        }
        engine_->SetUseOB(use_ob);
      }
      if (event.getY() > 550) {
        is_start_screen = false;
      }
  }
  if (engine_->GetIsYPointSelected() && !engine_->GetIsLaunched()) {
    selected_power = power;
    currentRock->GetBody()->ApplyLinearImpulse({static_cast<float32>(abs(currentRock->GetPosition().GetX() * 1000 - event.getPos().x * 1000)) ,
                                                static_cast<float32>((event.getPos().y * 1000
                                                                      - currentRock->GetPosition().GetY() * 1000))},
                                               currentRock->GetBody()->GetWorldCenter());
    engine_->SetIsLaunched(true);
    engine_->SetIsYPointSelected(false);
    is_angle_set = true;
    should_show_angle = false;
    should_show_placement = true;
    engine_->UpdateNumLaunches();
  }
}

/// creates a textbox for the power
void MyApp::PrintText(const std::string& text, const glm::ivec2& size, const glm::vec2& loc, bool changer) {
  cinder::gl::color(changer,0,0.7);
  auto box = cinder::TextBox()
      .alignment(cinder::TextBox::CENTER)
      .font(cinder::Font("Arial", 100))
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
  if (should_show_placement && (currentRock == nullptr || currentRock->IsSlowedDown())) {
    cinder::gl::color(0,0,0);
    cinder::gl::drawSolidCircle({100, y_position}, 25);
  } else {
    if (should_show_angle) {
      if (use_key) {
        cinder::gl::color(0,0,0);
        cinder::gl::drawLine({currentRock->GetPosition().GetX(), currentRock->GetPosition().GetY()},
                             {currentRock->GetPosition().GetX() + 400, y_position});
      }
      if (use_mouse) {
        cinder::gl::color(0,0,0);
        cinder::gl::drawLine({currentRock->GetPosition().GetX(), currentRock->GetPosition().GetY()},
                             getMousePos());
      }
    }
  }
  if (use_key && is_angle_set) {
    if (!engine_->GetIsLaunched()) {
      std::string str = std::to_string(power + 1);
      PrintText(str, {1000, 1000}, {1000,70}, false);
    } else {
      // prints out the selected power.
      std::string str2 = std::to_string(selected_power + 1);
      PrintText(str2, {1000, 1000}, {1000,70}, false);
    }
  }
}
void MyApp::DrawGameOver() {
  PrintText("Set Over", {1000, 1000} , {1000, 300}, false);
  std::string winner;
  std::string score;
  if (engine_->GetWinner() == engine::WinnerState::NoWinner) {
    winner = "Tie";
  } else {
     score = std::to_string(engine_->GetWinnerScore());
    if (engine_->GetWinner() == engine::WinnerState::YellowWins) {
      winner = "Yellow:";
    } else {
      winner = "Red:";
    }
  }
  PrintText(winner, {1000, 1000} , {800, 500} , false);
  PrintText(score, {1000, 1000} , {1000, 500}, false);

}
void MyApp::DrawStartScreen() {
  PrintText("CLICK ON YOUR OPTION", {1800, 500}, {1000, 100}, false);
  PrintText("USE: ", {1500, 500}, {600, 400}, false);
  PrintText("MOUSE", {1500, 500}, {900, 400}, use_mouse);
  PrintText("KEYS", {1500, 500}, {1200, 400}, use_key);
  PrintText("OUT OF BOUNDS: ", {1500, 500}, {600, 500}, false);
  PrintText("YES", {1500, 500}, {1000, 500}, use_ob == true);
  PrintText("NO", {1500, 500}, {1200, 500}, use_ob == false);

  PrintText("NEXT", {1500, 500}, {1000, 600}, false);


}
}  // namespace myapp
