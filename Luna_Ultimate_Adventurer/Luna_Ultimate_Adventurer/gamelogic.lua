pixelWidth = 0;
pixelHeight = 0;

msg = -1

tileSize = 0

player = {}
player["x"] = 1
player["y"] = 1

worldPos={}
worldPos["x"] = 1
worldPos["y"] = 1
worldWidth = 40
worldHeight = 32
world = {}

timeR    = 180;
thirstR  = 80;

function init(tSize, pWidth, pHeight)
	tileSize = tSize
	pixelWidth = pWidth;
	pixelHeight = pHeight;
	
	for i=1, worldHeight, 1
	do
		world[i] = {}
	end
	
	lvl = assert(loadfile(0))
	lvl()
end

function keyHandler(code)
	local var = code
	local moveX = nil
	local moveY = nil
	local x = player["x"]
	local y = player["y"]

	if(var == 0 or var == 71) then
		if(player["x"] ~= 1) then
			x = x - 1
		end
	elseif(var == 3 or var == 72) then
		if(player["x"] ~= worldWidth) then
			x = x + 1
		end
	elseif(var == 22 or var == 73) then
		if(player["y"] ~= 1) then
			y = y - 1
		end
	elseif(var == 18 or var == 74) then
		if(player["y"] ~= worldHeight) then
			y = y + 1
		end
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

	movePlayerC((player["x"] - 1) * tileSize, (player["y"] - 1) * tileSize)
end

function render()
	clearWindowC();
	
	--[[--]]
	for i=1, worldHeight, 1
	do
		for j=1, worldWidth, 1
		do
			drawTileC((j-1) * tileSize, (i-1) * tileSize, world[i][j]);
		end
	end
	
		
	drawPlayerC()
	
	drawTextC("Time: " .. timeR, pixelWidth, 5)
	drawTextC("Thirst: " .. thirstR, pixelWidth, 25)
	
	displayWindowC();
	return 1;
end