
% Calculate distance between a cell and a person
distance(X1, Y1, X2, Y2, D) :- cell(X1, Y1), p(X2, Y2), D = ((X2 - X1)**2) + ((Y2 - Y1)**2).

{ neighbor(X1, Y1, X2, Y2) } :- cell(X1, Y1), X2 = X1 - 1, Y2 = Y1, X1 > 0.
{ neighbor(X1, Y1, X2, Y2) } :- cell(X1, Y1), X2 = X1 + 1, Y2 = Y1, X1 < n1 - 1.
{ neighbor(X1, Y1, X2, Y2) }:- cell(X1, Y1), X2 = X1, Y2 = Y1 + 1, Y1 < n1 - 1.

{ good_neighbor(X1, Y1) } :- cell(X1, Y1), p(X2, Y2), distance(X1, Y1, X2, Y2, D), D >= d.

{ edge(X1, Y1, X2, Y2) } :- 
    neighbor(X1, Y1, X2, Y2), 
    good_neighbor(X1, Y1), 
    good_neighbor(X2, Y2).


% Initialize path
{ path(startX, startY, X2, Y2) } :- edge(startX, startY, X2, Y2).

% Extend path to a new cell from an existing cell in the path
{ path(X1, Y1, X2, Y2) } :- path(_, _, X1, Y1), edge(X1, Y1, X2, Y2).

% Constraints
:- not path(startX, 0, _, _).
:- edge(X1, Y1, X2, Y2), edge(X2, Y2, X1, Y1).
:- edge(X1, Y1, X2, Y2), edge(X1, Y1, X3, Y3), X2 != X3, Y2 != Y3.
:- not path(_, _, _, n1-1).


% #show distance/5.
% #show edge/4.
% #show neighbor/4.
% #show good_neighbor/2.
% #show goal_cell/2.
% #show p/2.
#show path/4.

