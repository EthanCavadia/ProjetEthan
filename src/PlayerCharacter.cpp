#include <PlayerCharacter.h>
#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Window/Keyboard.hpp"
#include <physics.h>
#include <SFML/Graphics/Texture.hpp>
#include <iostream>

PlayerCharacter::PlayerCharacter(b2World& world, sf::Vector2f position, sf::Vector2f size) : position(position)
{
	//definition of the play size, position and texture
	playerShape.setPosition(position);
	playerShape.setSize(size);
	playerShape.setOrigin(size / 2.0f);
	playerShape.setFillColor(sf::Color::Green);

	//definition of the player movement behavior
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.fixedRotation = true;
	bodyDef.position = pixel2meter(position);
	bodyDef.userData = this;
	body = world.CreateBody(&bodyDef);

	//definition of the player shape and physic comportment
	b2FixtureDef rectFixtureDef;
	b2PolygonShape rectShape;
	rectShape.SetAsBox(pixel2meter(size.x / 2.0f), pixel2meter(size.y / 2.0f));
	rectFixtureDef.shape = &rectShape;
	rectFixtureDef.userData = this;
	rectFixtureDef.density = 1;
	rectFixtureDef.friction = 0;
	body->CreateFixture(&rectFixtureDef);
}

void PlayerCharacter::InputManager()
{
	float horizontalInput = 0.0f;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
	{
		horizontalInput = -1.0f;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
	{
		horizontalInput = 1.0f;
	}

	float verticalJump = body->GetLinearVelocity().y;
	if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) && 
		!wasJumpKeyPressed && isOnGround)
	{
		//Jump
		verticalJump = -80.0f;
	}
	body->SetLinearVelocity(b2Vec2(pixel2meter(playerSpeed) * horizontalInput, verticalJump));

	wasJumpKeyPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::Space);
}

void PlayerCharacter::Update(float dt)
{
	//Update player position with body
	position = meter2pixel(body->GetPosition());
	playerShape.setPosition(position);
}

void PlayerCharacter::Draw(sf::RenderWindow& window)
{
	window.draw(playerShape);
}

sf::Vector2f PlayerCharacter::GetPosition()
{
	return position;
}

void PlayerCharacter::SetPosition(sf::Vector2f v)
{
	position = v;
}

void PlayerCharacter::SetOnGround(bool contact)
{
	isOnGround = contact;
}

Ground::Ground(b2World& world, sf::Vector2f position, sf::Vector2f size)
{
	rect.setPosition(position);
	rect.setSize(size);
	rect.setOrigin(size / 2.0f);
	rect.setFillColor(sf::Color::Red);

	b2BodyDef bodyDef;
	bodyDef.position = pixel2meter(position);

	body = world.CreateBody(&bodyDef);
	b2FixtureDef fixtureDef;

	b2PolygonShape boxShape;
	boxShape.SetAsBox(pixel2meter(size.x / 2.0f), pixel2meter(size.y / 2.0f));
	fixtureDef.shape = &boxShape;
	fixture = body->CreateFixture(&fixtureDef);
}

void Ground::Draw(sf::RenderWindow& window)
{
	window.draw(rect);
}