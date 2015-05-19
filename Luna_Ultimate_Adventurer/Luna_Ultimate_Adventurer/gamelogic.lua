pixelWidth = 0;
pixelHeight = 0;

msg = -1

tileSize = 0

player = {}
player["x"] = 1
player["y"] = 1

goal = {}
goal["x"] = 1
goal["y"] = 1

worldWidth = 40
worldHeight = 32
world = {}

timeR    = 0;
thirstR  = 0;

FILENAME = "map.lua"

--[[
Win condition:
Reach the goal (blue circle)

Lose condition:
Deplete time or thirst

Controls:
WASD or arrow keys to move
E to replenish thirst
--]]

function waterFound()
	if ((player["x"] - 1) > 0 and world[player["y"]][player["x"] - 1] == 0) then
		return true;
	elseif ((player["y"] - 1) > 0 and world[player["y"] - 1][player["x"]] == 0) then
		return true;
	elseif ((player["x"] + 1) < worldWidth and world[player["y"]][player["x"] + 1] == 0) then
		return true;
	elseif ((player["y"] + 1) < worldHeight and world[player["y"] + 1][player["x"]] == 0) then
		return true;
	end
	return false
end

function init(tSize, pWidth, pHeight)
	tileSize = tSize
	pixelWidth = pWidth;
	pixelHeight = pHeight;
	
	for i=1, worldHeight, 1
	do
		world[i] = {}
	end
	newGame()
end

function restartProcess()
	displayWindowC();
	sleepC(5);
	newGame();
end

function winOrLose()
	if (player["x"] == goal["x"] and player["y"] == goal["y"]) then
		drawTextC("You Won!", pixelWidth / 2, pixelHeight / 2)
		restartProcess()
	elseif (timeR <= 0 or thirstR <= 0) then
		drawTextC("You Lost!", pixelWidth / 2, pixelHeight / 2)
		restartProcess()
	end
end

function newGame()
	lvl = assert(loadfile(FILENAME))
	lvl()
	
	moveEntityC(player["x"], player["y"], 5)
	moveEntityC(goal["x"], goal["y"], 6)
	
	--[[--]]
	timeR    = 180;
	thirstR  = 80;
	
end

function keyHandler(key)
	local x = player["x"]
	local y = player["y"]

	if(key == 0 or key == 71) then
		if(player["x"] ~= 1) then
			x = x - 1
		end
	elseif(key == 3 or key == 72) then
		if(player["x"] ~= worldWidth) then
			x = x + 1
		end
	elseif(key == 22 or key == 73) then
		if(player["y"] ~= 1) then
			y = y - 1
		end
	elseif(key == 18 or key == 74) then
		if(player["y"] ~= worldHeight) then
			y = y + 1
		end
	elseif(key == 4) then
		if (waterFound() == true) then
			thirstR = 80
			timeR = timeR - 2
		end
	elseif(key == 16) then
		saveLevel(FILENAME);
	end
	
	if (x ~= player["x"] or y ~= player["y"]) then
		if(world[y][x] ~= 0 and world[y][x] ~= 3) then --Water or Stone
			player["x"] = x
			player["y"] = y
			if(world[y][x] == 1) then --Bridge
				timeR = timeR - 4;
				thirstR = thirstR - 2;
			elseif(world[y][x] == 2) then --Tree
				timeR = timeR - 1;
				thirstR = thirstR - 3;
			elseif(world[y][x] == 4) then --Field
				timeR = timeR - 3;
				thirstR = thirstR - 5;
			end;
		end;
	end;
	
	moveEntityC(player["x"], player["y"], 5)
	winOrLose()
end

function render()
	clearWindowC();
	
	for i=1, worldHeight, 1
	do
		for j=1, worldWidth, 1
		do
			drawTileC(j, i, world[i][j]);
		end
	end
		
	drawPlayerC()
	drawGoalC();
	
	drawTextC("Time: " .. timeR, pixelWidth, 5)
	drawTextC("Thirst: " .. thirstR, pixelWidth, 25)
	
	displayWindowC();
	return 1;
end

function ascii34(middle)
return string.char(34) .. middle .. string.char(34)
end

function saveLevel(filename)
	file = io.open(filename, "w")
	
	file:write("player = {}\n")
	file:write("player[".. ascii34("x") .."] = " .. player["x"] .. "\n")
	file:write("player[".. ascii34("y") .."] = " .. player["y"] .. "\n")
	
	--[[
	file:write("\n")
	
	file:write("timeR    = " .. timeR .. "\n")
	file:write("thirstR  = " .. thirstR .. "\n")
	--]]
	
	file:write("\n")
	
	file:write("goal = {}\n")
	file:write("goal[".. ascii34("x") .."] = " .. goal["x"] .. "\n")
	file:write("goal[".. ascii34("y") .."] = " .. goal["y"] .. "\n")
	
	file:write("\n")
	
	file:write("world = {}")
	for i = 1, worldHeight, 1 do
		toWrite = "\n" .. "world[" .. i .. "] = {"
		
		toWrite = toWrite .. world[i][1]
		for j = 2, worldWidth, 1 do
			toWrite = toWrite .. ", " .. world[i][j]
		end
		file:write(toWrite .. "}")
	end
	file:close()
end