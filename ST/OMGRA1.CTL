14
1
omgra0
4
12
4
12
Ground Level
omgra2
26
26
26
26
Level 2
@@
;1st pit and pp
PT 03 10
DR 55 10
..
TP 56 10
TE 04 10
..
P1 02 10
CH 03 10 02
..
;2nd pit
PT 02 15
DR 54 15
..
TP 54 14
TE 02 14
..
P1 02 16
CH 02 15 41
..
GP 54 16
CH 54 16 24
..
TP 54 16
TE 02 16
..
;3rd pit
PT 11 16
DR 59 16
..
TP 58 16
TE 10 16
..
L1 59 14
CH 11 16 02
..
; Click on talking picture
GP 14 01
TL 90 01
..
; Picture opens door
PE 90 01
CH 15 02 46
..
; Wrong password - Teleport you
PE 90 02
MV 01 06 12
CH 15 02 19
..	 
; Hidden teleports
GP 14 13
MS Use the righthand rule
..
TP 17 10
TE 12 10
..
TP 17 11
TE 10 10
..
TP 18 11
TE 06 12
..
TP 19 11
TE 12 10
..
TP 19 12
TE 10 10
..
TP 19 13
TE 06 12
..
; Levers in secret passages
L1 22 04
CH 32 05 46
..
L1 30 14
CH 36 05 46
..
;Hidden teles
TP 29 02
TE 53 02
.. 
TP 29 03
TE 53 03
.. 
TP 31 03
TE 55 03
.. 
TP 32 03
TE 56 03
.. 
TP 33 03
TE 57 03
.. 
TP 31 04
TE 55 04
.. 
;Teleport back up from hidden teles
TP 50 02
TE 26 02
..
GP 34 07
MS Find another lever
..
; Lots of teleports
TP 34 13
TE 36 10
..
TP 35 12
TE 36 10
..
TP 36 13
TE 36 10
..
TP 32 13
TE 36 10
..
TP 39 12
TE 36 10
..
TP 40 13
TE 36 10
..
TP 33 14
TE 36 10
..
TP 37 14
TE 36 10
..
TP 39 14
TE 36 10
..
; ... the ones that matter
TP 33 12
TE 39 13
..
TP 38 13
TE 36 14
..
TP 37 12
TE 33 13
..
TP 35 14
TE 32 12
..
; Unlock door
GP 36 15
GT 132
TO 19 46
..
GP 36 26
MS A walk in the park
..
; Hidden teleport down long corridor
TP 22 26
TE 22 21
..
;2 way lever & 2 gates
L1 19 22
TG 19 24 47 21
TG 17 24 47 21
..
L0 19 22
TG 19 24 47 21
TG 17 24 47 21
..
; Hidden door ( v.difficult)
GP 20 31
CH 20 31 02
..
;Clue for hidden door
GP 20 28
MS Knock 3 South
..
; Buttons and Gates
GP 25 33
TG 24 28 21 47
TG 24 29 21 47
..
GP 24 33
TG 24 29 21 47
TG 24 30 21 47
..
GP 26 33
TG 24 30 21 47
TG 24 31 21 47 
..
;Rune message
GP 28 25
MS A Rune to pass
..
; G RUNE
GP 26 25
GT 114
TO 19 46
..
;DEAD END
GP 24 12
MS DEAD END
..
; Glass key door
GP 21 38
GT 133
TO 46 19
..
; Message for above door
GP 21 39
MS Virtualy open
..
; 4 Square teleports onto rugs
TP 18 40
TE 18 32
..
TP 14 40
TE 10 22
..
TP 14 38
TE 10 37
..
TP 18 38
TE 04 38
..
; Teleport Return
TP 04 32
TE 16 39
..
; Hidden Message
GP 06 28
MS You Bloody Cheat!
..
; 4 doors and 4 keys
GP 02 30
SP 63
TO 19 46
DS
..
GP 02 29
SP 63
TO 19 46
DS
..
GP 02 28
SP 63
TO 19 46
DS
..
GP 02 27
SP 63
TO 19 46
DS
..
@@
90
    ,Answer my riddle
name,I have no name
job ,A riddling picture
ridd,What is the map number?
map ,A view overhead
numb,A number in the map
ten ,%01Correct
one ,%02Wrong!
two ,%02Wrong!
thre,%02Wrong!
four,%02Wrong!
five,%02Wrong!
six ,%02Wrong!
seve,%02Wrong!
eigh,%02Wrong!
nine,%02Wrong!
zero,%02Wrong!
10  ,%01Correct
1   ,%02Wrong!
2   ,%02Wrong!
3   ,%02Wrong!
4   ,%02Wrong!
5   ,%02Wrong!
6   ,%02Wrong!
7   ,%02Wrong!
8   ,%02Wrong!
9   ,%02Wrong!
0   ,%02Wrong!
bye ,Good Luck ( you'll need it ! )
@@,@@
0
