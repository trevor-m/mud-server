 --Skill: deathbolt

function OnStart(player, target)
        StartTimer(player, target, 3, 1500)
		SendTo(player, "You begin channeling the True Power...")
		StartTimer(player, target, 3, 1000)
		SendTo(player, "You weave flows of Shadow and Fire...")
		StartTimer(player, target, 3, 2000)
		SendTo(player, "You weave flows of Spirit and Air into your growing lance of shadows...")
		StartTimer(player, target, 3, 1500)
		SendTo(player, "You finish weaving your lance of death.")
		StartTimer(player, target, 3, 500)
		SendTo(player, "You shoot your lance towards "..GetName(target).."!")
end

function OnCast(player, target)
        StartDuration(player, target, 3, 0)
		SetHealth(target, GetHealth(target)-GetHealth(target))
		SendToRoom(GetRoomID(target), GetName(target).. " hits the ground with a hollow thud as "..GetName(player).." obliterates him with a lance of death!")
end

function OnEnd(player, target)
end
