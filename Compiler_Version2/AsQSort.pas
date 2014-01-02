var
  n,i,b:integer;
  a:array[10] of integer;
begin
  read(n);
  for i:=0 to n-1 do begin
    read(b);
    a[i]:=b;
  end;
  for i:=0 to n-1 do write(a[i]);
end.
