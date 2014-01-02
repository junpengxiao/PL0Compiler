var
a:integer;

function f(i:integer;var j:integer):char;
	function g():integer;
	begin
		a:=24;
		g:=a;
	end;
begin
  g();
  j:=j+i;
  f:=j+a;
end;
function k():char;
	function g():integer;
	begin
		a:=12;
	end;
begin
	a:=12;
end;
begin
  a:=12;
  write(f(a,a))
end.
