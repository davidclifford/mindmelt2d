15
1
omgra1
26
26
26
26
Level 1
omgra3
2
2
2
2
Level 3
omgra2
40
24
47
24
Secret Passage
omgra2
47
24
40
24
Level 2
omgra2
40
14
47
14
Secret Passage
omgra2
47
14
40
14
Level 2
omgra2
28
13
28
53
Pit Passage
omgra2
28
53
28
13
Level 2
@@
GP 21 26
MS Activated pad locks door
..
P1 22 26
CH 22 24 19
..
P0 22 26
CH 22 24 46
..
;hidden pp's to open 2 gates
P1 16 24
TG 14 25 21 47
..
P1 16 25
TG 13 24 21 47
..
;Secret buttons
GP 05 28
CH 05 28 02
..
GP 13 32
CH 13 32 02
..
;Lever to open gate
L1 14 34
CH 04 33 47
..
;Throw object over pit and then pull lever to open gate 
GP 01 34
MS Try your strength
..
P1 02 39
CH 02 35 02
..
P0 02 39
CH 02 35 10
..
L1 02 40
CH 08 35 47
..
PT 02 35
DR 52 35
..
PT 52 34
DR 02 34
..
;Secret buttons move barrells
GP 19 37
CH 18 38 02
CH 20 40 02
CH 18 40 52
CH 20 38 52
..
GP 19 41
CH 18 38 52
CH 20 40 52
CH 18 40 02
CH 20 38 02
..
;Lever opens gate
L1 24 40
CH 16 39 47
..
GP 25 14
MS A Rune to pass
..
GP 26 13
GT 115
TO 19 46
..
GP 04 01
MS A Rune to pass
..
GP 03 02
GT 116
TO 19 46
..
; 4 direction sign
GP 23 37
IN 23 36 01
MS North
..
GP 23 37
IN 24 37 01
MS East 
..
GP 23 37
IN 23 38 01
MS South
..
GP 23 37
IN 22 37 01
MS West 
..
; Talking picture ...
GP 39 40
TL 91 01
..		     
; ... opens gate
PE 91 01
CH 38 39 47
..
; Hidden tele in FF's
TP 28 38
TE 16 32
..
; Nothing door & message
GP 29 11
MS Nothing opens this door!
..
GP 30 11
CH 30 11 46
MS So I lied!
..
; Silly bit
PT 33 14
DR 57 14
..
TP 57 12
TE 33 12
..
TP 57 16
TE 33 16
..
PT 32 13
DR 32 53
..
GP 25 11
MS Turn back NOW!
..
@@
91
    ,Where do all directions meet (xxyy)?
name,Eh?
job ,Eh?
2337,%01Correct
xxyy,eg 1523 x=15 y=23
bye ,A bit tricky eh?
@@,@@
0
