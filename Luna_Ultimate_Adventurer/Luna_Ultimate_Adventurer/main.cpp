#include "lua.hpp"
#include <iostream>
#include <SFML\Graphics.hpp>

#define TILESIZE 20.0f
#define PIXELWIDTH 800
#define PIXELHEIGHT 640

using namespace std;
using namespace sf;

/*
Win condition:
Reach the goal (blue circle)

Lose condition:
Deplete time or thirst

Controls:
WASD or arrow keys to move
E to replenish thirst
*/

enum Object
{
	Water  = 0,
	Bridge = 1,
	Tree   = 2,
	Stone  = 3,
	Field  = 4,
	Player = 5,
	Goal   = 6
};

CircleShape player = CircleShape(TILESIZE / 2); 
CircleShape goal = CircleShape(TILESIZE / 2);
Vector2f baseTile = Vector2f(TILESIZE, TILESIZE);
RenderWindow window = RenderWindow(VideoMode(PIXELWIDTH, PIXELHEIGHT), "Luna the Ultimate Adventurer");

Text text;

Color getOC(Object id)
{
	switch (id)
	{
	case Water:
		return Color(3, 86, 155, 255);
	case Bridge:
		return Color(79, 46, 43, 255);
	case Tree:
		return Color(18, 83, 65, 255);
	case Stone:
		return Color(86, 86, 86, 255);
	case Field:
		return Color(140, 195, 110, 255);
	case Player:
		return Color(231, 47, 39, 255);
	case Goal:
		return Color(25, 25, 255, 255);
	}
	return Color(255, 255, 255, 255);
}

float corPos(float f)
{
	return (f - 1) * TILESIZE;
}

int displayWindow(lua_State * L)
{
	window.display();
	return 0;
}

int clearWindow(lua_State * L)
{
	window.clear();
	return 0;
}

int drawTile(lua_State * L)
{
	float posX = corPos(lua_tonumber(L, 1));
	float posY = corPos(lua_tonumber(L, 2));
	Object object = (Object)lua_tointeger(L, 3);
	lua_pop(L, 3);

	RectangleShape shape = RectangleShape(baseTile);
	shape.setPosition(Vector2f(posX, posY));
	shape.setFillColor(getOC(object));

	window.draw(shape);
	return 0;
}

int drawPlayer(lua_State * L)
{
	window.draw(player);
	return 0;
}

int drawGoal(lua_State * L)
{
	window.draw(goal);
	return 0;
}

int drawText(lua_State * L)
{
	string t = lua_tostring(L, 1);
	float posX = lua_tonumber(L, 2);
	float posY = lua_tonumber(L, 3);

	text.setString(t);
	float width = text.getLocalBounds().width;
	float height = text.getLocalBounds().height;
	text.setPosition(posX - width, posY - height / 2);

	float padding = 30;

	window.draw(text);
	lua_pop(L, 3);
	return 0;
}

int moveEntity(lua_State* L)
{
	float posX = corPos(lua_tonumber(L, 1));
	float posY = corPos(lua_tonumber(L, 2));
	Object o = (Object)lua_tointeger(L, 3);
	lua_pop(L, 2);

	if (o == 5)
	{
		player.setPosition(posX, posY);
	}
	else if (o == 6)
	{
		goal.setPosition(posX, posY);
	}
	
	return 0;
}

int sleep(lua_State * L)
{
	sf::sleep(sf::seconds(lua_tonumber(L, 1)));
	lua_pop(L, 1);
	return 0;
}

int main()
{
	/***************************************************** "Load Lua" *****************************************************/
	lua_State* L = luaL_newstate();
	luaL_openlibs(L);

	int error = luaL_loadfile(L, "gamelogic.lua") || lua_pcall(L, 0, 0, 0);
	if (error)
	{
		cerr << "unable to run: load: " << lua_tostring(L, -1) << endl;
		lua_pop(L, 1);
	}

	/*********************************************** "Add functions to Lua" ***********************************************/
	lua_pushcfunction(L, displayWindow);
	lua_setglobal(L, "displayWindowC");
	
	lua_pushcfunction(L, clearWindow);
	lua_setglobal(L, "clearWindowC");

	lua_pushcfunction(L, drawTile);
	lua_setglobal(L, "drawTileC");

	lua_pushcfunction(L, drawPlayer);
	lua_setglobal(L, "drawPlayerC");

	lua_pushcfunction(L, drawGoal);
	lua_setglobal(L, "drawGoalC");

	lua_pushcfunction(L, drawText);
	lua_setglobal(L, "drawTextC");

	lua_pushcfunction(L, moveEntity);
	lua_setglobal(L, "moveEntityC");

	lua_pushcfunction(L, sleep);
	lua_setglobal(L, "sleepC");

	/******************************************************* "Init" *******************************************************/
	player.setFillColor(getOC(Player));
	goal.setFillColor(getOC(Goal));

	Font font;
	if (!font.loadFromFile("Aller_Rg.ttf"))
	{
		cout << "Font not loaded" << endl;
	}

	text.setFont(font);
	text.setCharacterSize(20);
	text.setColor(sf::Color::Red);
	text.setStyle(sf::Text::Bold);

	lua_getglobal(L, "init");
	lua_pushnumber(L, TILESIZE);
	lua_pushnumber(L, PIXELWIDTH);
	lua_pushnumber(L, PIXELHEIGHT);
	error = lua_pcall(L, 3, 0, 0);
	if (error)
		cerr << "unable to run: init: " << lua_tostring(L, -1) << endl;

	while (window.isOpen())
	{
		/************************************************** "Update" ******************************************************/
		/************************************************ "KeyHandler" ****************************************************/
		Event e;
		int code = 0;
		while (window.pollEvent(e))
		{
			switch (e.type)
			{
			case Event::Closed:
				window.close();
				break;
			case sf::Event::KeyPressed:
				lua_getglobal(L, "keyHandler");
				code = e.key.code;
				lua_pushinteger(L, e.key.code);
				error = lua_pcall(L, 1, 0, 0);
				if (error)
					cerr << "unable to run: keyHandler: " << lua_tostring(L, -1) << endl;
				break;
			default:
				break;
			}
		}
		/************************************************** "Render" ******************************************************/
		lua_getglobal(L, "render");
		error = lua_pcall(L, 0, 0, 0);
		if (error)
		{
			cerr << "unable to run: render: " << lua_tostring(L, -1) << endl;
			lua_pop(L, 1);
		}
	}

	return 0;
}