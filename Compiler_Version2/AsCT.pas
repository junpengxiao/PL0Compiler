var
a:integer;

function f(i:integer;var j:integer):char;
begin
  j:=j+i;
  f:=j+a;
end;

begin
  a:=12;
  write(f(a,a))
end.
