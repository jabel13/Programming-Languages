#const n1=3.
#const startX=1.
#const startY=0.
#const d=2. 

% Grid and cells
node(0..n1-1).
cell(X,Y) :- node(X), node(Y).

p(1, 2).