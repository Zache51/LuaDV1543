#include "lua.hpp"
#include <iostream>
#include <SFML\Graphics.hpp>

#define TILESIZE 40.0f

using namespace std;
using namespace sf;

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
Vector2f baseTile = Vector2f(TILESIZE, TILESIZE);
RenderWindow window = RenderWindow(VideoMode(1200, 800), "Luna the Ultimate Adventurer");

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
		return Color(10, 10, 10, 255);
	}
	return Color(255, 255, 255, 255);
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
	float posX = lua_tonumber(L, 1);
	float posY = lua_tonumber(L, 2);
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

int movePlayer(lua_State* L)
{
	float posX = lua_tonumber(L, 1);
	float posY = lua_tonumber(L, 2);
	lua_pop(L, 2);

	player.setPosition(posX, posY);
	return 0;
}

int print(lua_State* L)
{
	float msg = lua_tonumber(L, 1);
	lua_pop(L, 1);

	cout << msg << " ";
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

	lua_pushcfunction(L, movePlayer);
	lua_setglobal(L, "movePlayerC");

	lua_pushcfunction(L, print);
	lua_setglobal(L, "printC");

	/******************************************************* "Init" *******************************************************/
	player.setFillColor(getOC(Player));

	lua_getglobal(L, "init");
	lua_pushnumber(L, TILESIZE);
	error = lua_pcall(L, 1, 0, 0);
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
			lua_getglobal(L, "print");
			error = lua_pcall(L, 0, 0, 0);
			if (error)
				cerr << "unable to run: print: " << lua_tostring(L, -1) << endl;
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