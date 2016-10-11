--Skill: blind

function OnStart(player, target)
	StartTimer(player, target, 5, 1000)
	SendTo(player, "You begin channeling blind.")
end

function OnCast(player, target)
	StartDuration(player, target, 5, 10000)
	SendToRoom(GetRoomID(target), GetName(target).." is blinded!")
	SetCanSee(target, false)
	SetCanAttack(target, false)
end

function OnEnd(player, target)
	SendTo(player, GetName(target).."'s blindness fades.")
	SendTo(target, "Your blindness fades.")
	SetCanSee(target, true)
	SetCanAttack(target, true)
end
