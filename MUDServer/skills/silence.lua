--Skill: silence

function OnStart(player, target)
	SendTo(player, "You begin channeling silence.")
	StartTimer(player, target, 6, 1000)
end

function OnCast(player, target)
	SendToRoom(GetRoomID(target), GetName(target).." is silenced!")
	StartDuration(player, target, 6, 10000)
	SetCanTalk(target, false)
end

function OnEnd(player, target)
	SendTo(player, GetName(target).." can talk again.")
	SendTo(target, "You can now talk once more.")
	SetCanTalk(target, true)
end
