node(0..3).

cell (X,Y) :- node(X), node(Y).

p(1,2).

distance(X2,Y2) :- p(X1,Y1), cell(X2,Y2), ((X2 - X1)**2) + ((Y2 - Y1)**2) <=4.

#show distance/2.