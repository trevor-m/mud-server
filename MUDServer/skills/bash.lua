--Skill: bash
function OnStart(player, target)
	StartTimer(player, target, 0, 500)
	SendTo(player, "You raise your weapon...")
end

function OnCast(player, target)
	StartDuration(player, target, 0, 3000)
	SendToRoom(GetRoomID(target), GetName(target).." stumbles as "..GetName(player).." brings his weapon down on his head!")
	SetCanAttack(target, false)
	SetCanMove(target, false)
	SetHealth(target, GetHealth(target)-50)
end

function OnEnd(player, target)
	SendToRoom(GetRoomID(target), GetName(target).." regains consicousness.")
	SetCanAttack(target, true)
	SetCanMove(target, true)
end
