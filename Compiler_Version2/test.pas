const
a=1,b='c';
var
   c : integer;
   d : array[4]of char;
procedure proc1(var a: integer);
var 
   c : integer;
   d : array[4]of integer;
begin
end;
function func1(b : char;var a : integer):char;
const			      
   ff = 4;
var
   d : array[5]of integer;
begin
   d[1]:=func1(b,d[2]);
end;
begin
end.