msg = -1

player = {}
player["x"] = 1
player["y"] = 1
player["radius"] = 0

worldPos={}
worldPos["x"] = 1
worldPos["y"] = 1
dimensions = 40
worldWidth = 30
worldHeight = 20

world = {}

function init(playerRadius)
	player["radius"] = playerRadius
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
			if(world[y][x] ~= 0) and (world[y][x] ~= 3) then
				moveX = x
			end
		end
	elseif(var == 3 or var == 72) then
		if(player["x"] ~= worldWidth) then
			x = x + 1
			if(world[y][x] ~= 0) and (world[y][x] ~= 3) then
				moveX = x
			end
		end
	elseif(var == 22 or var == 73) then
		if(player["y"] ~= 1) then
			y = y - 1
			if(world[y][x] ~= 0) and (world[y][x] ~= 3) then
				moveY = y
			end
		end
	elseif(var == 18 or var == 74) then
		if(player["y"] ~= worldHeight) then
			y = y + 1
			if(world[y][x] ~= 0) and (world[y][x] ~= 3) then
				moveY = y
			end
		end
	end

	player["x"] = moveX or player["x"]
	player["y"] = moveY or player["y"]

	movePlayerC((player["x"] - 1) * dimensions, (player["y"] - 1) * dimensions)
end

function render()
	clearWindowC();
	
	--[[--]]
	for i=1, worldHeight, 1
	do
		for j=1, worldWidth, 1
		do
			drawSquareC((j-1) * dimensions, (i-1) * dimensions, dimensions, world[i][j]);
		end
	end
	
		
	drawPlayerC()
	
	displayWindowC();
	return 1;
end