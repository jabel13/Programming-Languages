
goal_cell(X, n1-1) :- cell(X, n1-1). 

% Calculate distance between a cell and a person
distance(X1, Y1, X2, Y2, D) :- cell(X1, Y1), p(X2, Y2), D = ((X2 - X1)**2) + ((Y2 - Y1)**2).

% neighbor(X1, Y1, X2, Y2) :- cell(X1, Y1), X2 = X1 - 1, Y2 = Y1, X1 >= 0.
% neighbor(X1, Y1, X2, Y2) :- cell(X1, Y1), X2 = X1 + 1, Y2 = Y1, X1 <= n1 - 1.
% neighbor(X1, Y1, X2, Y2) :- cell(X1, Y1), X2 = X1, Y2 = Y1 + 1, Y1 <= n1 - 1.

% Horizontal neighbors
neighbor(X, Y, X1, Y) :- node(X), node(Y), X1 = X + 1, X1 < n1.
neighbor(X, Y, X1, Y) :- node(X), node(Y), X1 = X - 1, X1 >= 0.

% Vertical neighbors
neighbor(X, Y, X, Y1) :- node(X), node(Y), Y1 = Y + 1, Y1 < n1.


good_neighbor(X1, Y1) :- cell(X1, Y1), p(X2, Y2), distance(X1, Y1, X2, Y2, D), D >= d.

edge(X1, Y1, X2, Y2) :- 
    neighbor(X1, Y1, X2, Y2), 
    good_neighbor(X1, Y1), 
    good_neighbor(X2, Y2).


% Initialize path
{path(startX, startY, X2, Y2)} :- edge(startX, startY, X2, Y2).

% Path extends to a new cell from an existing cell in the path
{path(X1, Y1, X2, Y2) }:- path(_, _, X1, Y1), edge(X1, Y1, X2, Y2).

% Goal is to reach the goal cell
goal_reached :- path(_, _, X, Y), goal_cell(X, Y).

% Constraints
% :- not path(startX, startY, _, _).
% Prevent from looping back to a cell it came from
% :- path(_, _, X1, Y1), path(X1, Y1, _, _).
% Ensure that for each person, there is no path within a distance less than the safe distance.
% :- p(Xp, Yp), path(X1, Y1, X2, Y2), distance(X1, Y1, Xp, Yp, D), D < 2.
% :- p(Xp, Yp), path(X1, Y1, X2, Y2), distance(X2, Y2, Xp, Yp, D), D < 2.
:- not goal_reached.

% #show distance/5.
% #show edge/4.
% #show neighbor/4.
% #show good_neighbor/2.
% #show goal_cell/2.
% #show p/2.
#show path/4.

