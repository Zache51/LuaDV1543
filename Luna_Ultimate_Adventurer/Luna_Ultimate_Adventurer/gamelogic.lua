msg = -1

player = {}
player["x"] = 0
player["y"] = 0
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
end

function keyHandler(code)
	local var = code

	if(var == 0 or var == 71) then
		player["x"] = player["x"] - player["radius"] * 2
	elseif(var == 3 or var == 72) then
		player["x"] = player["x"] + player["radius"] * 2
	elseif(var == 22 or var == 73) then
		player["y"] = player["y"] - player["radius"] * 2
	elseif(var == 18 or var == 74) then
		player["y"] = player["y"] + player["radius"] * 2
	end

	movePlayerC(player["x"], player["y"])
end

function render()
	clearWindowC();
	
	--[[
	for i=1, worldHeight, 1
	do
		for j=1, worldWidth, 1
		do
			world[i][j] = 3;
			drawSquareC((j-1) * dimensions, (i-1) * dimensions, dimensions, world[i][j]);
		end
	end
	--]]
		
	drawPlayerC()
	
	displayWindowC();
	return 1;
end