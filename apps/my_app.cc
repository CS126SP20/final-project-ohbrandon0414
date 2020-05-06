// Copyright (c) 2020 [Your Name]. All rights reserved.

#include "my_app.h"

#include <cinder/gl/gl.h>
#include <Box2D/Box2D.h>
#include <ObjectArray.h>
#include <cinder/audio/audio.h>


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
int sets = 1;
std::string r_set_score = "R";
std::string y_set_score = "Y";
bool is_using_bot;
int r_total_score = 0;
int y_total_score = 0;

ci::audio::SourceFileRef rock_sliding = ci::audio::load( ci::app::loadAsset( "rock_slide.mp3"));
ci::audio::SourceFileRef background_music = ci::audio::load( ci::app::loadAsset( "beat.mp3"));
ci::audio::VoiceRef sliding_sound;
ci::audio::VoiceRef music;


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
  is_using_bot = false;
  is_game_over = false;
//  sliding_sound = ci::audio::Voice::create(rock_sliding);
//  sliding_sound->start();
  sliding_sound = ci::audio::Voice::create(rock_sliding);
  music = ci::audio::Voice::create(background_music);
  std::cout<<music->getVolume();
//  music->start();
}

void MyApp::update() {
//  if(!music->isPlaying()) {
//    music->start();
//  }
  // update the state if the set if over or not
  if (is_start_screen) {return;}
  if (!is_set_over) {
    engine_->Step();
    is_set_over = engine_->GetIsSetOver();
    currentRock = engine_->GetCurrentRock();

    if(engine_->ShouldPlaySound()) {
        sliding_sound->start();
    } else {
      sliding_sound->stop();
    }

    if(is_using_bot && !is_set_over && !engine_->GetIsRedTurn()) {
      StepBot();
    }

    UpdateAttributes();
    // updates the current rock with in this file

    for( int i = 0; i < 10; ++i ){
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
    DrawSetOver();
    return;
  }
  if (is_game_over) {
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
  // nothing available if the last rock is launched and the set is not over
  if (engine_->IsLastRockLaunched() && !is_set_over) {
    return;
  }
  // when the set is over
  if(is_set_over && event.getY() > 550) {
    // user presses continue
    ApplyScore();
    engine_->Reset();
    is_set_over = false;
    if (sets > kSets) {
      is_game_over = true;
    }
    return;
  }
  // when its the bots turn user cannot use the mouse.
  if (is_using_bot && !engine_->GetIsRedTurn()) {return;}

  // users placement
  if (is_using_mouse && !engine_->GetIsYPointSelected() && !is_start_screen) {
    // if the current rock went out of bounds or they are all stable at their positions.
    if (currentRock == nullptr || engine_->AllRocksAreSlowed()) {
      if(event.getY() > 275 && event.getY() < 550){
        engine_->SetYPoint(event.getY());
        engine_->SetIsYPointSelected(true);
        should_show_placement = false;
        should_show_angle = true;
        return;
      }
    }
  }

  // user is in the game get to choose the power or force of the rock
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
    should_show_angle = false;
    should_show_placement = true;
    engine_->UpdateNumLaunches();
  }
}

/// creates a textbox for the power
void MyApp::PrintText(const std::string& text, int size, const glm::vec2& loc, bool changer, bool centered) {
  auto alignment = cinder::TextBox::LEFT;
  if(centered) {
   alignment = cinder::TextBox::CENTER;
  }
  cinder::gl::color(0.7, changer,0);
  auto box = cinder::TextBox()
      .alignment(alignment)
      .font(cinder::Font("Bauhaus 93", size))
      .text(text);

  const auto box_size = box.getSize();
  const cinder::vec2 locp = {loc.x - box_size.x / 2, loc.y - box_size.y / 2};
  const auto surface = box.render();
  const auto texture = cinder::gl::Texture::create(surface);
  cinder::gl::draw(texture, locp);
}


void MyApp::UpdateAttributes() {
  // range of the placement
  int range = board->GetLowerSideLine() - board->GetUpperSideLine() - (2 * kRange);
  // time managing of the placement when using keys
  y_position = static_cast<int>((ci::app::getElapsedSeconds()) * 100);
  y_position = y_position % (range * 2);

  if (y_position >= range) {
    y_position = (range* 2) - y_position;
  }
  y_position += board->GetUpperSideLine() + kRange;

  // takes care of the power gage when using keys
  power = static_cast<int>((ci::app::getElapsedSeconds()) * 3);
  power = power % 10;
}

void MyApp::DrawAttributes() {
  // not showing anything if the last rock is launched
  if (engine_->IsLastRockLaunched()) {return;}

  if (is_using_bot && !engine_->GetIsRedTurn()) {return;}


  // it is turn to show placement and the current rock is ob or slowed down.
  if(should_show_placement && (currentRock == nullptr || currentRock->IsSlowedDown())) {
    // when user is using mouse
    if (is_using_mouse) {
      cinder::gl::color(0,0,0);
      cinder::gl::drawSolidCircle(getMousePos() - getWindowPos(), 25);
      return;
    }
    // when user is using keys
    if(is_using_key) {
      cinder::gl::color(0,0,0);
      cinder::gl::drawSolidCircle({100, y_position}, 25);
      return;
    }
  }

  // angle should be shown
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

  // dont show anything if its bots turn
  if(is_using_bot && !engine_->GetIsRedTurn()) {return;}

  // the number of the power gage
  if (is_using_key && is_angle_set && !engine_->GetIsLaunched()) {
    std::string str = std::to_string(power + 1);
    PrintText(str, 100, {1400, 600}, !engine_->GetIsRedTurn(), false);
  }

  // dont show anything if its bots turn
  if(is_using_bot && engine_->GetIsRedTurn()) {return;}

  // selected power
  if (is_using_key && engine_->GetIsLaunched()) {
    std::string str2 = std::to_string(selected_power + 1);
    PrintText(str2, 100, {1400,600}, engine_->GetIsRedTurn(), false);
  }
}

void MyApp::DrawSetOver() {
  PrintText("SET OVER", 120, {800, 50}, false, true);
  std::string winner;
  std::string score;
  engine::WinnerState win_state = engine_->GetWinner();
  bool red_wins = true;
  if (win_state == engine::WinnerState::NoWinner) {
    winner = "TIE";
  } else {
    if (win_state == engine::WinnerState::YellowWins) {
      winner = "YELLOW\nWINS";
      red_wins = false;
    } else {
      winner = "RED\nWINS";
    }
  }
  PrintText(winner, 120, {900, 300} , !red_wins, true);
  PrintText("CONTINUE", 120, {800, 650}, true, true);
}

void MyApp::DrawStartScreen() {
  PrintText("CLICK ON YOUR OPTIONS", 120, {400, 60}, false, false);
  /// pvp or pvc
  PrintText("AGAINST: ", 100, {300, 300}, false, false);
  PrintText("PERSON", 100, {700, 300}, is_using_bot == false, false);
  PrintText("COMPUTER", 100, {1100, 300}, is_using_bot == true, false);
  // mouse or keys
  PrintText("USE: ", 100, {450, 400}, false, false);
  PrintText("MOUSE", 100, {700, 400}, is_using_mouse, false);
  PrintText("KEYS", 100, {1100, 400}, is_using_key, false);
  // out of bounds yes or no
  PrintText("OUT OF BOUNDS: ", 100, {100, 500}, false, false);
  PrintText("YES", 100, {750, 500}, is_using_ob == true, false);
  PrintText("NO", 100, {1100, 500}, is_using_ob == false, false);
  // to move on
  PrintText("NEXT", 110, {850, 650}, false, true);
}

void MyApp::ChooseOptions(const cinder::app::MouseEvent& event) {
  // against:
  if(event.getY() < 400 && event.getY() > 250) {
    if (event.getX() < 1000) {
      is_using_bot = false;
    }
    if(event.getX() > 1000) {
      is_using_bot = true;
    }
  }
  // using:
  if(event.getY() < 500 && event.getY() > 400) {
    if (event.getX() < 1000) {
      is_using_mouse = true;
      is_using_key = false;
    }
    if(event.getX() > 1000) {
      is_using_key = true;
      is_using_mouse = false;
    }
  }
  // out of bounds:
  if (event.getY() < 600 && event.getY() > 500) {
    if (event.getX() < 1000){
      is_using_ob = true;
    }
    if (event.getX() > 1000){
      is_using_ob = false;
    }
  }
  // next:
  if (event.getY() > 650) {
    board = new Board(m_world, is_using_ob);
    engine_ = new engine(board);
    bot_ = new bot(board, engine_);
    engine_->SetUseOB(is_using_ob);
    is_start_screen = false;
  }
}

void MyApp::DrawScore() {
  PrintText(std::to_string(engine_->GetRedLeft()), 100, {30, 600}, false, false);
  PrintText(r_set_score, 100, {90, 600}, false, false);
  PrintText(std::to_string(engine_->GetYellowLeft()), 100, {30, 700}, true, false);
  PrintText(y_set_score, 100, {90, 700}, true, false);
}

void MyApp::ApplyScore() {
  sets++;
  int score = engine_->GetWinnerScore();
  if (engine_->GetWinner() == engine::WinnerState::YellowWins) {
    y_set_score += " " +  std::to_string(score);
    r_set_score +=" 0";
    y_total_score += score;
  } else if(engine_->GetWinner() == engine::WinnerState::RedWins){
    r_set_score += " " +  std::to_string(score);
    y_set_score +=" 0";
    r_total_score += score;
  } else {
    // situation of tie
    r_set_score += " 0";
    y_set_score += " 0";
  }
}

void MyApp::StepBot() {
    if (!engine_->GetIsYPointSelected()) {
      engine_->SetYPoint(bot_->GetPlacement());
      engine_->SetIsYPointSelected(true);
      return;
    }
    if(engine_->GetIsYPointSelected() && !engine_->GetIsLaunched() && !currentRock->IsRed()) {
      currentRock->GetBody()->ApplyLinearImpulse(bot_->GetForce(), currentRock->GetBody()->GetWorldCenter());
      engine_->SetIsLaunched(true);
      engine_->SetIsYPointSelected(false);
      engine_->UpdateNumLaunches();
      return;
    }
}
void MyApp::DrawGameOver() {
  ci::gl::clear();
  std::string game_state = "";
  bool red_wins = true;
  if(r_total_score > y_total_score) {
    game_state = "RED\nWINS";
  } else if (r_total_score < y_total_score) {
    game_state = "YELLOW\nWINS";
    red_wins = false;
  } else {
    game_state = "TIE";
  }

  PrintText("GAME OVER", 150, {700, 100}, false, true);
  PrintText(game_state, 130, {850, 300}, !red_wins, true);
  PrintText(std::to_string(r_total_score), 150, {850, 600}, false, true);
  PrintText(std::to_string(y_total_score), 150, {1000, 600}, true, true);
}

}  // namespace myapp
