with Ada.Calendar;use Ada.Calendar;
with Ada.Text_IO;use Ada.Text_IO;--Put(),Put_Line()
with Ada.Integer_Text_IO;use Ada.Integer_Text_IO;
with Ada.Float_Text_IO;use Ada.Float_Text_IO;

with Gtk.Main, Gtk.Window;
with Gtk.Label; use Gtk.Label;
with Gtk.Box; use Gtk.Box;

procedure alarmsys1255 is

--------------------------------------------------------------------------------
procedure ShowSimpleGUI(msg:in String) is
   Window : Gtk.Window.Gtk_Window;
   Etiqueta:Gtk_Label;
   Caja:Gtk_VBox;
   begin
   Ada.Text_IO.New_Line;
   Ada.Text_IO.Put_Line("ShowSimpleGUI:"&msg);
   Gtk.Main.Init;
   Gtk.Window.Gtk_New (Window);
   Window.Set_Default_Size(400,300);
   Gtk_New_VBox(Caja);
   Window.Add(Caja);
   Gtk_New(Etiqueta,msg);
   Caja.Add(Etiqueta);
   --Gtk.Window.Show (Window);
   Window.Set_Title("ALARM_SYS_0");
   Window.Show_All;
   Gtk.Main.Main;
   --null;
   end ShowSimpleGUI;

   type MES is (enero,febrero,marzo,abril,mayo,junio,
                julio,agosto,septiembre,octubre,noviembre,diciembre);
--------------------------------------------------------------------------------
   function Month_Number_to_MES(M: Month_Number) return MES is
      begin
      case M is
        when 1 => return enero;
        when 2 => return febrero;
        when 3 => return marzo;
        when 4 => return abril;
        when 5 => return mayo;
        when 6 => return junio;
        when 7 => return julio;
        when 8 => return agosto;
        when 9 => return septiembre;
        when 10 => return octubre;
        when 11 => return noviembre;
        when 12 => return diciembre;
      end case;
   end Month_Number_to_MES;

--Constants
  String_hhmm:String:="Son m\'as de las 12:55";
  Day_Duration_hhmm:Day_Duration:=(12*60*60.0)+55*60.0;--12:55;

begin --alarmsys1255
   declare
      Start, Finish:Time;--Ada.Calendar.Time
      --segundos_transcurridos,Start_seconds,Finish_seconds,Intervalo:Day_Duration;
      Start_seconds,Finish_seconds:Day_Duration;
      intStart_seconds,hh,mm,ss:Integer;
      Year:Year_Number;
      Month:Month_Number;
      Day:Day_Number;
   begin
      Start:=Clock;
      --Start_seconds:=Seconds(Start);
      Split(Start,Year,Month,Day,Start_seconds);
      intStart_seconds:=Integer(Start_seconds);
      Put_Line("FECHA: "&Day_Number'Image(Day)&" DE "&
      MES'Image(Month_Number_to_MES(Month))&" DE "&Year_Number'Image(Year));
      hh:=intStart_seconds/(60*60);
      mm:=(intStart_seconds mod (60*60))/60;
      ss:=(intStart_seconds mod (60*60)) mod 60;
      Put_Line("Son m\'as de las "&Integer'Image(hh)&
               " :"&Integer'Image(mm)&" :"&Integer'Image(ss));
      Put("Segundos transcurridos en el d\'ia:"&Day_Duration'Image(Start_seconds));

      loop
         Finish:=Clock;
         Finish_seconds:=Seconds(Finish);
         --segundos_transcurridos:=Finish_seconds-Start_seconds;
         if Finish_seconds>Day_Duration_hhmm then
            Put_Line(String_hhmm);
            ShowSimpleGUI(String_hhmm);
         end if;
         --exit when segundos_transcurridos>Intervalo;
         exit when Finish_seconds>Day_Duration_hhmm;
         delay 60.0;
      end loop;

      end;--begin del declare
end alarmsys1255;


