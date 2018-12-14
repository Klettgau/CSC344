rect([X,Y,W,H]).
%this function checks if the given coord is in the rectangle
checky(rect([X1,Y1,W,H]),X2,Y2):-
    X2 > X1,
    X2 =< W+X1,
    Y2>Y1,    
    Y2 =< H+Y1.
%this function increments the drone, it can only go right,up,down.
move([X,Y],[XX,Y]) :-
	XX is X+1 .
move([X,Y],[X,YY]) :-
	YY is Y+1 .
move([X,Y],[X,YY]) :-
	YY is Y-1 .
%function that adheres to the given parameters.It is optional
flyWrapper(CAVE,[CAVEW,CAVEH],STARTY,PATH):-
	STARTX is 1,
	flyy(CAVE,[CAVEW,CAVEH],STARTY,STARTX,PATH,[]).

%base case is when the x is equal to the width of the cave
flyy(_,[X,_],_,STARTX,NEWPATH,REPEAT) :-
	X == STARTX,
	write(NEWPATH). %writes to the terminal

%the main function that runs the simulation
flyy(CAVE,[CAVEW,CAVEH],STARTY,STARTX,PATH,REPEAT):-
	checkNeg(STARTX,STARTY),
	move([STARTX,STARTY],[XX,YY]),
	checkNeg(XX,YY),
	safey([XX,YY],[CAVEW,CAVEH],CAVE),
	%not(not_mem([[XX,YY]],REPEAT)),
	not_mem([XX,YY],PATH),
	%REPEATY = [[XX,YY]|REPEAT],
	NEWPATH = [[XX,YY]|PATH],
	%write(NEWPATH),
	flyy(CAVE,[CAVEW,CAVEH],YY,XX,NEWPATH,REPEAT).
	%write(NEWPATH).

checkNeg(XX,YY):-
	XX >=0,
	YY >=0.

%check if inside the huge cave then  illegal squares
%safe check if coords are in the illegal rect list
safey([CURRX,CURRY],[CAVEX,CAVEY],CAVELIST):-
	checkInsideCave([CURRX,CURRY],[CAVEX,CAVEY]),	
	checkSafePlace([CURRX,CURRY],CAVELIST).
	
	
checkInsideCave([X,Y],[CAVEX,CAVEY]) :-
	CAVEX >=X,
	CAVEY >=Y,
	X>=0,
	Y >=0.

checkSafePlace([_,_],[]).
checkSafePlace([],[]).
%takes all the elements of each illegal list and then checks to see if the 
%x and y are in the illegal square.
 
checkSafePlace([X,Y],[H|T]):-
	nth0(0,H,XX),
	nth0(1,H,YY),
	nth0(2,H,WW),
	nth0(3,H,HH),
	not(checky(rect([XX,YY,WW,HH]),X,Y)),

	checkSafePlace([X,Y],T).
%checks to see if the current position has been visited before.
not_mem([_,_],[]).
not_mem([X,Y],[H|TAIL]):-
	%write([X,Y]),
	[X,Y] \= H,
	not_mem([X,Y],TAIL).
		
