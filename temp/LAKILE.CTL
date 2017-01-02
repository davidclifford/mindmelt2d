@map
version=1
name=lakile
description=Lake Island
id=11
light=true
--
@entry
filename=world
desc=Exit to World
from=19,9,41,19,10,41
to=85,9,56
--
@code
Reply 44,1
ObjectIn 100,44
GetObject 100
--
@talk
id=61
talkfirst=1
-
name,Joms sir
job,I'm the butler sir
butler,Of my master sir
master,An old mage sir
mage,He is now retired sir
retired,Ask the master sir
greeting,Yes sir
bye,Yes sir
dontknow,I see sir
-
id=44
sayfirst=1
-
name,I am the great Ramog
job,I'm a retired mage
retired,I have lossed my magic
lossed,It just went one day
magic,I know a usefull spell
spell,Have the X-ray spell,1
greeting,Welcome wizard
bye,Use magic wisely
dontknow,Indeed?
--