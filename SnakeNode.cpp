#include "SnakeNode.h"

#include<cmath>
#include<string>
#include <SFML/Graphics.hpp>

using namespace sfSnake;

const float SnakeNode::Width = 20.f;
const float SnakeNode::Height = 20.f;
sf::Texture texture1;
sf::Texture texture0;

//蛇块大小参数设置
const std::string s1="Pic/body.png",s2="Pic/head.png";


SnakeNode::SnakeNode(sf::Vector2f position,double dx,double dy) : position_(position) {
    texture1.loadFromFile(s1);
    texture0.loadFromFile(s2);
    shape_.setOrigin(sf::Vector2f(10.f, 10.f));
    shape_0.setOrigin(sf::Vector2f(10.f, 10.f));
    shape_.setTexture(texture1);
    shape_0.setTexture(texture0);
    shape_.setPosition(position_);
    shape_0.setPosition(position_);
    if(fabs(dx)<0.1){
        if(dy>0)shape_.setRotation(180.f),shape_0.setRotation(180.f);
    }
    else if(fabs(dy)<0.1){
        if(dx>0)shape_.setRotation(90.f),shape_0.setRotation(90.f);
        else if(dx<0)shape_.setRotation(-90.f),shape_0.setRotation(-90.f);
    }
    else{
        if(dx<0&&dy<0)shape_.setRotation(-std::atan((float)(dx/dy))/3.14*180),
        shape_0.setRotation(-std::atan((float)(dx/dy))/3.14*180);
        else if(dx>0&&dy<0)shape_.setRotation(std::atan((float)(-dx/dy))/3.14*180),
        shape_0.setRotation(std::atan((float)(-dx/dy))/3.14*180);
        else if(dx<0&&dy>0)shape_.setRotation(-180.f+std::atan((float)(-dx/dy))/3.14*180),
        shape_0.setRotation(-180.f+std::atan((float)(-dx/dy))/3.14*180);
        else if(dx>0&&dy>0)shape_.setRotation(180.f-std::atan((float)(dx/dy))/3.14*180),
        shape_0.setRotation(180.f-std::atan((float)(dx/dy))/3.14*180);
    }
    /*shape_.setSize(sf::Vector2f(SnakeNode::Width, SnakeNode::Height));
    
    shape_.setFillColor(sf::Color::Green);
    
    shape_.setOutlineColor(sf::Color::White);
    shape_.setOutlineThickness(-1.f);*/

    // shape的参数设置
}

void SnakeNode::setPosition(sf::Vector2f position) {
    position_ = position;
    shape_.setPosition(position_);
}
//由下面重载到上面
void SnakeNode::setPosition(float x, float y) {
    position_.x = x;
    position_.y = y;
    shape_.setPosition(position_);
}

void SnakeNode::move(float xOffset, float yOffset) {
    position_.x += xOffset;
    position_.y += yOffset;
    shape_.setPosition(position_);
}

sf::FloatRect SnakeNode::getBounds() const {
    return shape_.getGlobalBounds();
    //不太清楚
}

sf::Vector2f SnakeNode::getPosition() const { return position_; }

void SnakeNode::render(sf::RenderWindow& window,int i) { 
    if(i==0)window.draw(shape_0);
    else window.draw(shape_);    
}