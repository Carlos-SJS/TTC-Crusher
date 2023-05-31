#include "player_test.hpp"

Player::Player(int x){
    this->x = x;
}

int Player::getX(){
    return this->x;
}

void Player::setX(int x){
    this->x = x;
}