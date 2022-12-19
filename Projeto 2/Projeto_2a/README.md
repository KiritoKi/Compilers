# Syntax Parser

## Exercise1

* In the text file `ArqTeste.txt` must be inserted the entries, the parser returns accepted when the line has only binary number (0 / 1) !! there must be a break line even on the last line


## Projeto 2a Rules

p1 = S -> M
p2 = S -> GM
p3 = S -> FGM
p4 = F -> f(){C;r(E);}
p5 = G -> g(){C;r(E);}
p6 = M -> m(){C;r(E);}
p7 = E -> 0
p8 = E -> 1
p9 = E -> x
p10 = E -> y
p11 = E -> (EXE)
p12 = X -> +
p13 = X -> -
p14 = X -> *
p15 = X -> /
p16 = C -> h=E
p17 = C -> i=E
p18 = C -> j=E
p19 = C -> k=E
p20 = C -> z=E
p21 = C -> (EXE)
p22 = C -> w(E){C;}
p23 = C -> f(E){C;}
p24 = C -> o(E;E;E){C;}

input=

δ0 = _,_,S
δ1 = _,S,M              p1
δ2 = _,S,GM             p2  
δ3 = _,S,FGM            p3
δ4 = _,F,f(){C;r(E);}   p4
δ5 = _,G,g(){C;r(E);}   p5
δ6 = _,M,m(){C;r(E);}   p6
δ7 = _,E,0              p7
δ8 = _,E,1              p8
δ9 = _,E,x              p9
δ10 = _,E,y             p10
δ11 = _,E,(EXE)         p11
δ12 = _,X,+             p12
δ13 = _,X,-             p13
δ14 = _,X,*             p14
δ15 = _,X,/             p15
δ16 = _,C,h=E           p16
δ17 = _,C,i=E           p17
δ18 = _,C,j=E           p18
δ19 = _,C,k=E           p19
δ20 = _,C,z=E           p20
δ21 = _,C,(EXE)         p21
δ22 = _,C,w(E){C;}      p22
δ23 = _,C,f(E){C;}      p23
δ24 = _,C,o(E;E;E){C;}  p24
δ25 = f,f,_
δ26 = g,g,_
δ27 = m,m,_
δ28 = (,(,_
δ29 = ),),_
δ30 = {,{,_
δ31 = },},_
δ32 = r,r,_
δ33 = ;,;,_
δ34 = 0,0,_
δ35 = 1,1,_
δ36 = x,x,_
δ37 = y,y,_
δ38 = +,+,_
δ39 = -,-,_
δ40 = *,*,_
δ41 = /,/,_
δ42 = h,h,_
δ43 = i,i,_
δ44 = j,j,_
δ45 = k,k,_
δ46 = z,z,_
δ47 = =,=,_
δ48 = w,w,_
δ49 = f,f,_
δ50 = o,o,_



_,_,S
_,S,M
_,S,GM
_,S,FGM
_,F,f(){C;r(E);}
_,G,g(){C;r(E);}
_,M,m(){C;r(E);}
_,E,0
_,E,1
_,E,x
_,E,y
_,E,(EXE)
_,X,+
_,X,-
_,X,*
_,X,/
_,C,h=E
_,C,i=E
_,C,j=E
_,C,k=E
_,C,z=E
_,C,(EXE)
_,C,w(E){C;}
_,C,f(E){C;}
_,C,o(E;E;E){C;}
f,f,_
g,g,_
m,m,_
(,(,_
),),_
{,{,_
},},_
r,r,_
;,;,_
0,0,_
1,1,_
x,x,_
y,y,_
+,+,_
-,-,_
*,*,_
/,/,_
h,h,_
i,i,_
j,j,_
k,k,_
z,z,_
=,=,_
w,w,_
f,f,_
o,o,_

teste

_,_,S
_,S,aSb
_,S,c
a,a,_
b,b,_
c,c,_

aaacbbb
aacbb
acb
cab
abc
aacb
acbb
