@map
version=1
id=15
name=omgra2
description=Level 2
light=false
--
@entry
filename=omgra1
desc=Level 1
from=26,26,0
to=26,26,1
-
filename=omgra3
desc=Level 3
from=2,2,0
to=2,2,1
-
filename=omgra2
from=40,24,0
to=40,24,-1
desc=Secret Passage
-
filename=omgra2
from=40,24,-2
to=40,24,1
desc=Level 2
-
filename=omgra2
from=40,14,0
to=40,14,-1
desc=Secret Passage
-
filename=omgra2
from=40,14,-2
to=40,14,1
desc=Level 2
-
filename=omgra2
from=28,13,0
to=28,13,-1
desc=Pit Passage
-
filename=omgra2
from=28,13,-2
to=28,13,1
desc=Level 2
--
@code
Activate 21,26,1
Message Activated pad locks door
-
PadOn 22,26,1
Close 22,24,1
-
PadOff 22,26,1
Open 22,24,1
-
//hidden pp's to open 2 gates
PadOn 16,24,1
OpenClose 13,24,1
-
PadOn 16,25,1
OpenClose 14,25,1
-
//Secret buttons
Activate 5,28,1
ChangeSquare 5,28,1,2
-
Activate 13,32,1
ChangeSquare 13,32,1,2
-
//Lever to open gate
LeverOn 14,34,1
Open 4,33,1
-
//Throw object over pit and then pull lever to open gate 
Activate 1,34,1
Message Try your strength
-
PadOn 2,39,1
ChangeSquare 2,35,1,2
-
PadOff 2,39,1
ChangeSquare 2,35,1,10
-
LeverOn 2,40,1
Open 8,35,1
-
Teleport 2,34,2
MoveAll 2,34,1
-
//Secret buttons move barrells
Activate 19,37,1
ChangeSquare 18,38,1,2
ChangeSquare 20,40,1,2
ChangeSquare 18,40,1,52
ChangeSquare 20,38,1,52
-
Activate 19,41,1
ChangeSquare 18,38,1,52
ChangeSquare 20,40,1,52
ChangeSquare 18,40,1,2
ChangeSquare 20,38,1,2
-
//Lever opens gate
LeverOn 24,40,1
Open 16,39,1
-
Activate 25,14,1
Message A Rune to pass
-
Activate 26,13,1
GotObject 115
OpenClose
-
Activate 4,1,1
Message A Rune to pass
-
Activate 3,2,1
GotObject 116
OpenClose
-
// 4 direction sign
Activate 23,37,1
?AtPosition 23,36,1
Skip 2
Message North
End
?AtPosition 22,37,1
Skip 2
Message West
End
?AtPosition 23,38,1
Skip 2
Message South
End
?AtPosition 24,37,1
End
Message East
-
// Talking picture 
Activate 39,40,1
TalkTo 91,1
-		     
// picture opens gate
Reply 91,1
Open 38,39,1
-
// Hidden tele in FF's
Teleport 28,38,1
MoveAll 16,32,1
-
Teleport 36,16,1
MoveAll 26,14,1
-
// Nothing door & message
Activate 29,11,1
Message Nothing opens this door!
-
Activate 30,11,1
OpenClose
Message So I lied!
-
Activate 25,11,1
Message Turn back NOW!
--