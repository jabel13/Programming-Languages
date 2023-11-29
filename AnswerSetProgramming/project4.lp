% % Constants
% #const n1=25.
% #const startX=13.
% #const startY=0.
% #const d=36. % Squared distance for social distancing

% % Grid and cells
% node(0..n1-1).
% cell(X,Y) :- node(X), node(Y).

% People in the park 
person(13, 7).

start(startX, startY).
goal_cell(X, n1-1) :- cell(X, n1-1). 

neighbor(X1, Y1, X2, Y2) :- cell(X1, Y1), X2 = X1 - 1, Y2 = Y1, X1 > 0.
neighbor(X1, Y1, X2, Y2) :- cell(X1, Y1), X2 = X1 + 1, Y2 = Y1, X1 < n1 - 1.
neighbor(X1, Y1, X2, Y2) :- cell(X1, Y1), X2 = X1, Y2 = Y1 + 1, Y1 < n1 - 1.

% Calculate distance between a cell and a person
distance(X1, Y1, X2, Y2, D) :- cell(X1, Y1), person(X2, Y2), D = ((X2 - X1)**2) + ((Y2 - Y1)**2).

good_neighbor(X1, Y1) :- cell(X1, Y1), person(X2, Y2), distance(X1, Y1, X2, Y2, D), D >= d.

edge(X1, Y1, X2, Y2) :- neighbor(X1, Y1, X2, Y2), good_neighbor(X2, Y2).

% Initialize path
{path(startX, startY, X2, Y2)} :- edge(startX, startY, X2, Y2).

% Path extends to a new cell from an existing cell in the path
{path(X1, Y1, X2, Y2) }:- path(_, _, X1, Y1), edge(X1, Y1, X2, Y2), not path(X2, Y2, _, _).


% Goal is to reach the goal cell
goal_reached :- path(_, _, X, Y), goal_cell(X, Y).

% Constraints
% :- not path(startX, startY, _, _).

% Prevent from looping back to a cell it came from
% :- path(X1, Y1, _, _), path(_, _, X1, Y1).

% Prevent any cell from being visited more than once 
% :- path(X1, Y1, _, _), seen(X1, Y1).

% Ensure that for each person, there is no path within a distance less than the safe distance.
:- person(Xp, Yp), path(X1, Y1, X2, Y2), distance(X1, Y1, Xp, Yp, D), D < 36.
:- person(Xp, Yp), path(X1, Y1, X2, Y2), distance(X2, Y2, Xp, Yp, D), D < 36.


:- not goal_reached.


% Show the path and no-go zones in the output
#show path/4.





