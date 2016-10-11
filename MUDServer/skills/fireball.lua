--Skill: fireball

function OnStart(player, target)
	math.randomseed(os.time())
	SendTo(player, "You begin channeling fireball...")
	StartTimer(player, target, 1, 2000)
end

function OnCast(player, target)
	SendToRoom(GetRoomID(target), GetName(target).." bursts into flame as "..GetName(player).." hurls a fireball at him!")
	SetHealth(target, GetHealth(target)-20)
	StartDuration(player, target, 1, 500)
end

function OnEnd(player, target)
	SendToRoom(GetRoomID(target), GetName(target).." shrieks in pain as flames consume his body!")
	SetHealth(target, GetHealth(target)-50)
	if math.random(0, 100) > 25 then
		StartDuration(player, target, 1, 500)
	end
end
