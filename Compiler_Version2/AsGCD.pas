var
a,b,c:integer;

function f(a:integer; b:integer):integer;
begin
  if a=0 then f:=b
  else if b=0 then f:=a
  else begin
    if a<b then b:=b-a
    else a:=a-b;
    f:=f(a,b)
  end;
end;

begin
  read(a,b);
  write(f(a,b))
end.
