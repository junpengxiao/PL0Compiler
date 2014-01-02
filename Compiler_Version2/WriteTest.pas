var
i:array[3] of integer;
begin
    i[0]:=1;
    i[i[0]]:=2;
    i[i[i[0]]]:=3;
    write(i[2])
end.
