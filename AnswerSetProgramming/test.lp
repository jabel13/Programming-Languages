#const n1=25.
#const startX=13.
#const startY=0.
#const d=36. 

% Grid and cells
node(0..n1-1).
cell(X,Y) :- node(X), node(Y).

p(13, 7).
p(20, 4).
p(4, 19).