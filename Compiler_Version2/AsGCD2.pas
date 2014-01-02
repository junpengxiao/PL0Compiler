var
a,b,c:integer;

function f(a:integer; b:integer):integer;
begin
  repeat begin
    if a<b then begin
      c:=a;
      a:=b;
      b:=c
    end;
    a:=a-b;
  end until b=0;
  f:=a
end;

begin
  read(a,b);
  write(f(a,b))
end.
