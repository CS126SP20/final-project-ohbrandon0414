// Copyright (c) 2020 [Your Name]. All rights reserved.

#include "my_app.h"

#include <cinder/app/App.h>
#include <cinder/gl/gl.h>
#include <Box2D/Box2D.h>
#include <ObjectArray.h>


#include <vector>
#include <Rock.h>
#include <Board.h>
#include <engine.h>
namespace myapp {

using cinder::app::KeyEvent;

int power;
int y_position;
int selected_power;
b2World* m_world;
Board* board;
engine* engine_;
Rock* currentRock;
bool should_show_placement = true;

MyApp::MyApp() { }

void MyApp::setup() {
  // basic set up
  b2Vec2 gravity(0.0f, 0);
  m_world = new b2World(gravity);
  engine_ = new engine(m_world);
  board = new Board(m_world);
}

void MyApp::update() {
  engine_->Step();

  y_position = static_cast<int>((ci::app::getElapsedSeconds()) * 300);
  y_position = y_position % 350;
  y_position += 230;

  // updates the current rock with in this file
  currentRock = engine_->GetCurrentRock();

   for( int i = 0; i < 5; ++i ){
     m_world->Step( 1 / 100.0f, 10, 10 );
   }
}

void MyApp::draw() {
  cinder::gl::enableAlphaBlending();
  cinder::gl::clear();

  board->Display();


  // takes care of the power gage.
  power = static_cast<int>((ci::app::getElapsedSeconds()) * 3);
  power = power % 5;

  if (should_show_placement) {
    cinder::gl::color(0,0,0);
    cinder::gl::drawSolidCircle({50, y_position}, 25);
  }
  if (!engine_->GetIsLaunched()) {
    std::string str = std::to_string(power + 1);
    PrintText(str, {500, 500}, {100,70});
  } else {
    // prints out the selected power.
    std::string str2 = std::to_string(selected_power + 1);
    PrintText(str2, {500, 500}, {200,70});
  }

  for(Rock* temp: engine_->GetRocks()) {
    temp->Display();
  }
}


void MyApp::keyDown(KeyEvent event) {
  switch (event.getCode()) {
    case KeyEvent::KEY_p: {
      if (engine_->GetIsYPointSelected() && !engine_->GetIsLaunched()) {
        selected_power = power;
        currentRock->GetBody()->ApplyLinearImpulse({static_cast<float32>((power + 1) * 100000) + 750000 , 0},
                                                   currentRock->GetBody()->GetWorldCenter());
        engine_->SetIsLaunched(true);
        engine_->SetIsYPointSelected(false);
        should_show_placement = true;
      }
      break;
    }
    case KeyEvent::KEY_d: {
      if (!engine_->GetIsYPointSelected()) {
        engine_->SetYPoint(y_position);
        engine_->SetIsYPointSelected(true);
        should_show_placement = false;
      }
    }
  }


  }

/// mouse click controls the power gage.
void MyApp::mouseDown(cinder::app::MouseEvent event) {
}

/// creates a textbox for the power
void MyApp::PrintText(const std::string& text, const glm::ivec2& size, const glm::vec2& loc) {
  cinder::gl::color(1,1,0.5);

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
}  // namespace myapp
