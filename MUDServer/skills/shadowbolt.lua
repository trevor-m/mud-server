--skill: shadowbolt
function OnStart(player, target)
        StartTimer(player, target, 2, 5000)
	SendTo(player, "You begin channeling the true power...")		
end

function OnCast(player, target)
        StartDuration(player, target, 2, 0)
		SetHealth(target, GetHealth(target)-GetHealth(target)*0.9)
		SendToRoom(GetRoomID(target), GetName(target).. " screams in pain as "..GetName(player).." smites him with shadowy lances!")
end
	
function OnEnd(player, target)
end