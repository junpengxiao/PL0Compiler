const
a=1,b=2,c='x';
var
   i   : integer;
   j   : char;
   arr : array[10]of integer;
function test():integer;
begin
   if test()>0 then test:=1;
end;
begin
   if 1=2 then i:=1;
   if 1<2 then begin i:=1; j:=2 end;
   if (a+b-c)*i<arr[0] then j:=1;
   if arr[a*c]<>arr[b+i] then arr[0]:=0 else arr[0]:=1;
   if a<test() then begin arr[0]:=1 end else begin arr[9]:=2; arr[8]:=1 end
end.