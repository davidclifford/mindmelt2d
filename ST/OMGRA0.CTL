13
1
world
12
38
20
28
World
omgra1
4
12
4
12
Level 1
@@
GP 04 37
MS A Rune to pass
..
GP 03 38
GT 112
TO 19 46
..
;open door 1
GP 08 31
GT 122
TO 19 46
..
; lever & door 1
L1 04 32
TG 08 19 19 46
..
; Gem messages
GP 20 23
MS Red Gem to Pass
..
GP 20 21
MS Green Gem to Pass
..
; Gems on pads
P1 20 22
IN 20 22 123
IN 20 20 124
CH 22 19 46
..
P1 20 20
IN 20 22 123
IN 20 20 124
CH 22 19 46
..
; Message to find lever bellow
GP 22 17
MS Find a Lever
..
; Button opens/closes gate
GP 25 24
TG 24 25 21 47
..
; Lever opens door
L1 26 39
CH 24 17 46
..
; Messages for following puzzle
GP 30 41
MS Pad Room
..
GP 22 41
MS Mirror Room
..
; 3 Pressure pads (out of 9) active then open gate.
;   works by changing the path squares in
;   the mirror room to normal floor squares and
;   then testing if the 3 squares are all floor squares.
; 1
P1 31 37
CH 21 37 2
EX 01 01
..
P0 31 37
CH 21 37 38
EX 01 01
; 2
..
P1 29 38
CH 23 38 2
EX 01 01
..
P0 29 38
CH 23 38 38
EX 01 01
..
; 3
P1 30 39
CH 22 39 2
EX 01 01
..
P0 30 39
CH 22 39 38
EX 01 01
..
; 4
P1 29 37
CH 23 37 38
EX 01 01
..
P0 29 37
CH 23 37 2
EX 01 01
..
; 5
P1 30 38
CH 22 38 38
EX 01 01
..
P0 30 38
CH 22 38 2 
EX 01 01
..
; 6
P1 31 38
CH 21 38 38
EX 01 01
..
P0 31 38
CH 21 38 2 
EX 01 01
..
; 7
P1 29 39
CH 23 39 38
EX 01 01
..
P0 29 39
CH 23 39 2 
EX 01 01
..
; 8
P1 30 37
CH 22 37 38
EX 01 01
..
P0 30 37
CH 22 37 2 
EX 01 01
..
; 9
P1 31 39
CH 21 39 38
EX 01 01
..
P0 31 39
CH 21 39 2 
EX 01 01
..
; Open the gate
XX 01 01
IF 21 37 2
IF 23 38 2
IF 22 39 2
IF 21 38 2
IF 21 39 2
IF 22 37 2
IF 22 38 2
IF 23 37 2
IF 23 39 2
CH 26 35 47
..
; off PP with key closes gate
P0 36 18
CH 30 17 21
..
;close door 
P1 36 18
CH 30 17 47
..
; rune door
GP 04 11
GT 113
TO 19 46
..
GP 04 09
MS A Rune to Pass
..
;silver key door
GP 26 15
GT 125
TO 19 46
..
; 6 buttons 3 gates
GP 15 08
CH 16 07 47
CH 16 05 21
..
GP 15 09
CH 16 05 47
CH 16 06 21
..
GP 15 10
CH 16 06 47
CH 16 05 21
..
GP 17 08
CH 16 05 47
CH 16 07 21
..
GP 17 09
CH 16 07 47
..
GP 17 10
CH 16 06 47
CH 16 07 21
..
; PP closes door
P1 25 12
CH 24 11 19
..
; gold key doors
GP 24 11
GT 126
TO 19 46
..
GP 34 03
GT 126
TO 19 46
..
;Teleport to square room
TP 04 04
TE 08 10
..
;Square room
P1 07 10
MA 06 10 06 08
MA 08 10 08 08
..
P1 06 09
MA 06 10 08 10
MA 06 08 08 08
..
P1 07 08
MA 06 08 06 10
MA 08 08 08 10
..
P1 08 09
MA 08 08 06 08
MA 08 10 06 10
..
; Door in square room
GP 08 11
GT 131
TO 46 19
..
@@
0
