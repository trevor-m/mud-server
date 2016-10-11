 --Skill: end

function OnStart(player, target)
       StartTimer(player, target, 4, 0)
end

function OnCast(player, target)
        StartDuration(player, target, 4, 500)
end

function OnEnd(player, target)
	if IsImm(player) then
		SetHealth(target, 0)
		SendToRoom(GetRoomID(target), GetName(player).." has ended "..GetName(target).."'s miserable existence.")
		StartDuration(player, target, 4, 500)
	end
end
