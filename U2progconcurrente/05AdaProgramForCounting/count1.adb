-- Copyright (C) 2006 M. Ben-Ari. See copyright.txt
with Ada.Text_IO;use Ada.Text_IO;
with Ada.Integer_Text_IO; use Ada.Integer_Text_IO;
procedure Count1 is

   N: Integer := 0;
--   pragma Volatile(N);

   task type Count_Task;

   task body Count_Task is
      Temp: Integer;
   begin
      for I in 1..10 loop
         Temp := N;
         delay(0.0);
         Put("I:=");
         Put(I , Width => 2);
         Put("  Temp:=");
         Put(Temp,Width => 2);
         N := Temp + 1;
         Put("  N:=");
         Put(N,Width => 2);
         New_Line;
       end loop;
   end Count_Task;

begin
   declare
      P, Q: Count_Task;
   begin
      null;
   end;
   Ada.Text_Io.Put_Line("The value of N is " & Integer'Image(N));
end Count1;
