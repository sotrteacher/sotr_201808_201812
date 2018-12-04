-----------------------------------------------------------------------------
--  This Example was used with a set of LEDs attached to the parallel port
--  in order to show a first example of an interaction with the hardware
--
--  Build command: mgnatmake leds_parallel_port.adb
--
--  daniel.sangorrin@gmail.com
--
--  mgnatmake leds_parallel_port.adb -Imarte_src_dirs
-----------------------------------------------------------------------------
--  for outb, io_port
with MaRTE.HAL.IO; use MaRTE.HAL.IO;
--  for IRQs
with POSIX_Hardware_Interrupts;
with System;
with MaRTE.Direct_IO;
with MaRTE.Integer_Types;

procedure LEDs_Parallel_Port_With_At is

   package PHI renames POSIX_Hardware_Interrupts;
   package MIT renames MaRTE.Integer_Types;

   ------------------
   -- 1) Constants --
   ------------------
   PP_IRQ         : constant PHI.Hardware_Interrupt := PHI.PARALLEL1_INTERRUPT;
   PP_BASE_REG    : constant IO_Port := 16#378#; --
   PP_DATA_REG    : constant IO_Port := PP_BASE_REG + 0; -- Data port offset
   PP_STATUS_REG  : constant IO_Port := PP_BASE_REG + 1; -- Status port offset
   PP_CONTROL_REG : constant IO_Port := PP_BASE_REG + 2; -- Control port offset
   PP_IRQ_ENABLE  : constant := 2#0001_0000#;

   type Bit_Status is (Off, On);
   for Bit_Status'Size use 1;
   for Bit_Status use (Off => 0, On  => 1);

   type PP_DATA_Register is record
      Data_Bit_7      : Bit_Status := Off;
      Data_Bit_6      : Bit_Status := Off;
      Data_Bit_5      : Bit_Status := Off;
      Data_Bit_4      : Bit_Status := Off;
      Data_Bit_3      : Bit_Status := Off;
      Data_Bit_2      : Bit_Status := Off;
      Data_Bit_1      : Bit_Status := Off;
      Data_Bit_0      : Bit_Status := Off;
   end record;
   for PP_DATA_Register'Size use 8;

   for PP_DATA_Register use
      record
         Data_Bit_7           at 0 range 7..7;
         Data_Bit_6           at 0 range 6..6;
         Data_Bit_5           at 0 range 5..5;
         Data_Bit_4           at 0 range 4..4;
         Data_Bit_3           at 0 range 3..3;
         Data_Bit_2           at 0 range 2..2;
         Data_Bit_1           at 0 range 1..1;
         Data_Bit_0           at 0 range 0..0;
      end record;

   pragma Volatile(PP_DATA_Register);
   pragma Pack (PP_DATA_Register);

   --------------------------
   -- PP_Interrupt_Handler --
   --------------------------
   Notify_IRQ : Boolean := True;
   function PP_IRQ_Handler
     (Area : in System.Address;
      Intr : in PHI.Hardware_Interrupt)
     return PHI.Handler_Return_Code is
   begin
      MaRTE.Direct_IO.Put ("irq");
      if Notify_IRQ = True then
         Notify_IRQ := False;
         return PHI.POSIX_INTR_HANDLED_NOTIFY;
      else
         return PHI.POSIX_INTR_HANDLED_DO_NOT_NOTIFY;
      end if;
   end PP_IRQ_Handler;
   Intr    : PHI.Hardware_Interrupt;
   Handler : PHI.Interrupt_Handler;

   Data : PP_DATA_Register;

begin
   PHI.Associate (PP_IRQ, PP_IRQ_Handler'Unrestricted_Access, System.Null_Address, 0);
   --  Enable interrups in the printer port
   Outb_P (PP_CONTROL_REG, PP_IRQ_ENABLE);
   --  Enable the interrupt
   PHI.Unlock (PP_IRQ);
   loop
      Notify_IRQ := True;
      PHI.Wait (Intr, Handler);
      Data.Data_Bit_0 := On;
      Data.Data_Bit_3 := On;
      Outb_P (PP_DATA_REG, MIT.Unsigned_8(Data)); delay (0.5);
      Outb_P (PP_DATA_REG, 2#00_010_010#); delay (0.5);
      Outb_P (PP_DATA_REG, 2#00_100_100#); delay (0.5);
      Outb_P (PP_DATA_REG, 2#00_010_010#); delay (0.5);
      Outb_P (PP_DATA_REG, 2#00_001_001#); delay (0.5);
      Outb_P (PP_DATA_REG, 2#00_000_000#);
   end loop;
end LEDs_Parallel_Port_With_At;