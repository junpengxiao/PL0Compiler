var
a:integer;
function f1(a : integer):integer;
begin
end;
function f2(a :integer;var b : integer):integer;
var
   c : integer;
begin
#  a:=b+c;
#  f2:=f1(a)
end;
procedure p1(a : integer);
begin
#  f1(a);
#  a:=f1(a)+f2(a,a)
end;
procedure p2(b : integer;var a:integer);
begin
#b:=a
end;
begin
   p1(f1(f2(3+3,a)));
end.
