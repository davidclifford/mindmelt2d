version=1
name=pennin
description=Village of Pennin
num=9
--
// Exit
filename=world
desc=Exit to World
from=21,9,42,21,10,42
to=19,9,23
--
Reply 72,1
ObjectIn 72 64 107
MoveObject 107 01 64
-
Reply 11,1
GotObject 144
MoveObject 19,3,1,144
-
Reply 11,1
GotObject 143
MoveObject 19,3,1,143
-
Reply 12,1
GotObject 141
Message Have you seen my chopper?
-
Reply 13,1
GotObject 142
Message I need that wood!
-
Actvate 5,15,1
GotObject 144
OpenClose
--
id=70
-
name,Drong
job,A ranger
ranger,A wood ranger
wood,Beware the enchanted wood
beware,They say there are caves beneeth
enchanted,To the south of here
cave,But no one knows where
bye,Hi Ho
-
id=71
-
name,Arias
job,A grave digger
grave,I'm very busy
digger,I dig holes
busy,'Cause of the monsters
monsters,They kill people
hole,Like pits
pits,Ask Jen about them
bye,Ho Hum
-
id=72
-
name,Garina
job,I used to work for my brother
brother,He is now in prison
prison,Can you take the money?
money,So you can see how he is
bye,Thank you friend
-
id=73
-
name,Bib
job,I'm just a boy
boy,But I know a girl
girl,Winina in Aporto
winina,She's very smart
bye,Be smart
-
id=14
-
name,Kesla
job,A girning looper
girning,Looper
looper,Girning
bye,Much luck
-
id=15
-
name,Marie
job,A looping girner
girner,Looping
looping,Girner
bye,All the best
- 
id=12
-
name,Login
job,A wood cutter
wood,I give it to Norni
cutter,With an axe
axe,A chopper
bye,Chop Chop
-
id=13
-
name,Norni
job,I carve wood
carv,I use Drongs knife
wood,Login gives me the wood
bye,Be well
-
id=11
-
name,Astori
job,A potter
potter,I make pots
pots,To keep things in
bye,Till next time,1
--
