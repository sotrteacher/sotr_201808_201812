--
--  Printer Port Interrupt.
--
--  Example of use of the Interrupt Control API provided by MaRTE OS.
--
--  By Mario Aldea Rivas
--
with MaRTE_OS;
with System;
with Basic_Integer_Types; use Basic_Integer_Types;
with POSIX;
with POSIX_Timers;
with POSIX_Hardware_Interrupts;
with IO_Interface; use IO_Interface;
with Basic_Console_IO;

with Text_IO; use Text_IO;


procedure Printer_Port_Interrupts is

   -----------------------------
   -- Parallel Port Interrupt --
   -----------------------------
   PP_Interrupt : POSIX_Hardware_Interrupts.Hardware_Interrupt
     renames POSIX_Hardware_Interrupts.PARALLEL1_INTERRUPT;

   -----------------------------
   -- Parallel Port Registers --
   -----------------------------
   PP_BASE_REG    : constant IO_Port := 16#378#; --
   PP_DATA_REG    : constant IO_Port := 0; -- Data port offset
   PP_STATUS_REG  : constant IO_Port := 1; -- Status port offset
   PP_CONTROL_REG : constant IO_Port := 2; -- Control port offset

   --------------------------------
   -- Parallel Port Control bits --
   --------------------------------
   PP_IENABLE : constant Unsigned_8 := 16#10#;

   ---------------------------------
   -- Data read from printer port --
   ---------------------------------
   Data    : Unsigned_8;

   --------------------------
   -- PP_Interrupt_Handler --
   --------------------------
   function PP_Interrupt_Handler
     (Area : in System.Address;
      Intr : in POSIX_Hardware_Interrupts.Hardware_Interrupt)
     return POSIX_Hardware_Interrupts.Handler_Return_Code is
   begin
      --  Never use Text_IO inside an interrupt handler
      Basic_Console_IO.Put ("In PP_Interrupt_Handler");
      Basic_Console_IO.New_Line;
      Data := IO_Interface.Inb_P (PP_BASE_REG + PP_DATA_REG);
      return POSIX_Hardware_Interrupts.POSIX_INTR_HANDLED_NOTIFY;
   end PP_Interrupt_Handler;

   Intr    : POSIX_Hardware_Interrupts.Hardware_Interrupt;
   Handler : POSIX_Hardware_Interrupts.Interrupt_Handler;

begin
   ------------
   ---- Configure Printer Port
   ----
   Put ("Configuring PP"); New_Line;
   --  Install interrupt handler and atach task with interrupt
   POSIX_Hardware_Interrupts.Associate
     (PP_Interrupt,
      PP_Interrupt_Handler'Unrestricted_Access,
      System.Null_Address, 0);
   --  Enable interrups in the printer port
   IO_Interface.Outb_P (PP_BASE_REG + PP_CONTROL_REG, PP_IENABLE);
   --  Enable the interrupt
   POSIX_Hardware_Interrupts.Unlock (PP_Interrupt);

   ------------
   ---- Wait for interrupts
   ----
   for I in 1 .. 10 loop
      begin
         --  Wait
         POSIX_Hardware_Interrupts.Wait
           (POSIX_Timers.Get_Time (POSIX_Timers.Clock_Realtime) +
            POSIX.To_Timespec (5.0),  -- Five seconds timeout
            Intr, Handler); -- Out parameters

         --  Read data.
         POSIX_Hardware_Interrupts.Lock (PP_Interrupt);
         Put ("Data read:"); Put (Unsigned_8'Image(Data)); New_Line;
         POSIX_Hardware_Interrupts.Unlock (PP_Interrupt);
      exception
         when POSIX.POSIX_Error =>
            Put (" POSIX_Error:" & POSIX.Image (POSIX.Get_Error_Code));
	    New_Line;
      end;
   end loop;

end Printer_Port_Interrupts;
