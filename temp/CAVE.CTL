@map
version=1
name=Cave
description=Hermits Cave
id=5
light=false
--
@entry
filename=world
desc=Exit to World
from=10,9,20,10,10,20
to=222,9,88
--
@code
Reply 45,1
ObjectIn 104,45
GetObject 104
--
@talk
id=45
-
name,Garom
job,I am a hermit
hermit,I live in this cave
cave,I needed solitude
solitude,Unlike my cousin
cousin,What is his name?
bron,I'll give you the back spell,1
spell,The back spell,1
bye,Leave in Peace
--