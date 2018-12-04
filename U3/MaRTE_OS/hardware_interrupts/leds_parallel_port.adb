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

procedure LEDs_Parallel_Port is

   package PHI renames POSIX_Hardware_Interrupts;
   ------------------
   -- 1) Constants --
   ------------------
   PP_IRQ         : constant PHI.Hardware_Interrupt := PHI.PARALLEL1_INTERRUPT;
   PP_BASE_REG    : constant IO_Port := 16#378#; --
   PP_DATA_REG    : constant IO_Port := PP_BASE_REG + 0; -- Data port offset
   PP_STATUS_REG  : constant IO_Port := PP_BASE_REG + 1; -- Status port offset
   PP_CONTROL_REG : constant IO_Port := PP_BASE_REG + 2; -- Control port offset
   PP_IRQ_ENABLE  : constant := 2#0001_0000#;

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

begin
   PHI.Associate (PP_IRQ, PP_IRQ_Handler'Unrestricted_Access, System.Null_Address, 0);
   --  Enable interrups in the printer port
   Outb_P (PP_CONTROL_REG, PP_IRQ_ENABLE);
   --  Enable the interrupt
   PHI.Unlock (PP_IRQ);
   loop
      Notify_IRQ := True;
--      MaRTE.Direct_IO.Put ("ANTES de PHI.Wait (Intr, Handler)");
--      PHI.Wait (Intr, Handler);
--      MaRTE.Direct_IO.Put ("Despues de PHI.Wait (Intr, Handler)");
      MaRTE.Direct_IO.Put ("Se envian datos al puerto paralelo");
      Outb_P (PP_DATA_REG, 2#00_001_001#); delay (0.5);
      Outb_P (PP_DATA_REG, 2#00_010_010#); delay (0.5);
      Outb_P (PP_DATA_REG, 2#00_100_100#); delay (0.5);
      Outb_P (PP_DATA_REG, 2#00_010_010#); delay (0.5);
      Outb_P (PP_DATA_REG, 2#00_001_001#); delay (0.5);
      Outb_P (PP_DATA_REG, 2#00_000_000#);
   end loop;
end LEDs_Parallel_Port;
