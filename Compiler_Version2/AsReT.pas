var
a,b:integer;
begin
   a:=3;
   b:=0;
   repeat begin
      a:=a-1;
      b:=b+a
   end until a=0;
   write(b);
end.
