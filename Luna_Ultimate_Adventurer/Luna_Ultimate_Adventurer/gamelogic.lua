player = {}
player["x"] = 0
player["y"] = 0

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