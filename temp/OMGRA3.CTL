@map
version=1
id=16
name=omgra3
description=Dungeon
light=false
--
@entry
filename=omgra2
from=2,2,0
to=2,2,1
desc=Level 2
--
@code
Activate 7,1,1
Message No Runes Beyond!
-
Activate 6,7,1
Message Rune Room
-
// Pad opens pits and returns runes to rune room
PadOn 8,2,1
Execute MoveRunesToRoom
-
Routine MoveRunesToRoom
MoveObject 4,4,1,112
MoveObject 2,4,1,113
MoveObject 6,4,1,114
MoveObject 5,4,1,115
MoveObject 3,4,1,116
ChangeSquare 32,2,1,10
ChangeSquare 22,12,1,10
ChangeSquare 20,28,1,10
ChangeSquare 8,32,1,10
ChangeSquare 22,36,1,10
-
PadOn 32,2,2
ChangeSquare 32,2,1,2
MoveAll 2,2,2
Execute TeleportToEndRoom
-
PadOn 22,12,2
ChangeSquare 22,12,1,2
MoveAll 2,2,2
Execute TeleportToEndRoom
-
PadOn 20,28,2
ChangeSquare 20,28,1,2
MoveAll 2,2,2
Execute TeleportToEndRoom
-
PadOn 8,32,2
ChangeSquare 8,32,1,2
MoveAll 2,2,2
Execute TeleportToEndRoom
-
PadOn 22,36,2
ChangeSquare 22,36,1,2
MoveAll 2,2,2
Execute TeleportToEndRoom
-
Routine TeleportToEndRoom
ObjectAt 2,2,2,112
ObjectAt 2,2,2,113
ObjectAt 2,2,2,114
ObjectAt 2,2,2,115
ObjectAt 2,2,2,116
MovePlayer 23,25,1
-
// Riddle picture
Activate 23,27,1
TalkTo 92,1
-
// Correct reply - open gate
Reply 92,1
Open 22,27,1
-
// Go meet Omgra!
Teleport 22,28,1
MoveAll 20,19,2
-
// Rune pits teleport back (for player)
Teleport 3,2,2
MoveAll 8,2,1
--