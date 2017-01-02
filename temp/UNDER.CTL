@map
version=1
name=under
description=Underground Cave
id=10
light=false
--
@entry
filename=world
desc=Exit to World
from=21,9,37,21,10,37
to=22,9,90
-
// Exit (waterfall)
filename=world
desc=Exit to World
from=38,9,20,35,10,20
to=61,9,91
--
@talk
id=42
sayfirst=1
-
name,MARGO! 
job,I am cursed
curse|cursed,A witch cast a spell on me
spell,She turned me into a beast
beast,Now I hide away
witch,The hag Quoxonia!
hag,The witch!
quoxonia,But she said I changed myself
changed|myself,But she did it, I know
rune,In one of my caverns
greeting,GRRRRRR!
dontknow,Grrr ahhh!
bye,Beware the Witch
--