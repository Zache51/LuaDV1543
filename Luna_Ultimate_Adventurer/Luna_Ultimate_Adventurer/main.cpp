
#include "lua.hpp"
#include <iostream>
#include <SFML\Graphics.hpp>

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

CircleShape player = CircleShape(10.f);
RenderWindow window(VideoMode(1200, 800), "Luna the Ultimate Adventurer");
Text text;

int print(lua_State* L)
{
	string msg = lua_tostring(L, 1);
	lua_pop(L, 1);
	cout << msg;
	return 0;
}

int endGame(lua_State* L)
{
	return 0;
}

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

int main()
{
	/***************************************************** "Load Lua" *****************************************************/
	lua_State* L = luaL_newstate();
	luaL_openlibs(L);

	int error = luaL_loadfile(L, "gamelogic.lua") || lua_pcall(L, 0, 0, 0);
	if (error)
	{
		cerr << "unable to run:" << lua_tostring(L, -1) << endl;
		lua_pop(L, 1);
	}
	
	/************************************************** "Push functions" **************************************************/
	lua_pushcfunction(L, print);
	lua_setglobal(L, "printC");

	lua_pushcfunction(L, endGame);
	lua_setglobal(L, "endGameC");
	/******************************************************* "Init" *******************************************************/
	player.setFillColor(getOC(Player));
	text.setCharacterSize(60);
	text.setColor(sf::Color::Red);
	text.setStyle(sf::Text::Bold | sf::Text::Underlined);

	/*lua_getglobal(L, "init");
	lua_pushnumber(L, player.getRadius());
	error = lua_pcall(L, 1, 0, 0);
	if (error)
		cerr << "unable to run:" << lua_tostring(L, -1) << endl;*/

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
					cerr << "unable to run:" << lua_tostring(L, -1) << endl;
				break;
			default:
				break;
			}
			lua_getglobal(L, "print");
			error = lua_pcall(L, 0, 0, 0);
			if (error)
				cerr << "unable to run:" << lua_tostring(L, -1) << endl;
		}
		/************************************************** "Render" ******************************************************/
	}


	system("pause");

	return 0;
}