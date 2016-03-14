--Legal Notice: (C)2015 Altera Corporation. All rights reserved.  Your
--use of Altera Corporation's design tools, logic functions and other
--software and tools, and its AMPP partner logic functions, and any
--output files any of the foregoing (including device programming or
--simulation files), and any associated documentation or information are
--expressly subject to the terms and conditions of the Altera Program
--License Subscription Agreement or other applicable license agreement,
--including, without limitation, that your use is for the sole purpose
--of programming logic devices manufactured by Altera and sold by Altera
--or its authorized distributors.  Please refer to the applicable
--agreement for further details.


-- turn off superfluous VHDL processor warnings 
-- altera message_level Level1 
-- altera message_off 10034 10035 10036 10037 10230 10240 10030 

library altera;
use altera.altera_europa_support_lib.all;

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;

entity mailbox_1 is 
        port (
              -- inputs:
                 signal address : IN STD_LOGIC_VECTOR (1 DOWNTO 0);
                 signal chipselect : IN STD_LOGIC;
                 signal clk : IN STD_LOGIC;
                 signal data_from_cpu : IN STD_LOGIC_VECTOR (31 DOWNTO 0);
                 signal read : IN STD_LOGIC;
                 signal reset_n : IN STD_LOGIC;
                 signal write : IN STD_LOGIC;

              -- outputs:
                 signal data_to_cpu : OUT STD_LOGIC_VECTOR (31 DOWNTO 0)
              );
end entity mailbox_1;


architecture europa of mailbox_1 is
                signal mutex0 :  STD_LOGIC_VECTOR (31 DOWNTO 0);
                signal mutex1 :  STD_LOGIC_VECTOR (31 DOWNTO 0);
                signal mutex_free0 :  STD_LOGIC;
                signal mutex_free1 :  STD_LOGIC;
                signal mutex_own0 :  STD_LOGIC;
                signal mutex_own1 :  STD_LOGIC;
                signal mutex_write_enable0 :  STD_LOGIC;
                signal mutex_write_enable1 :  STD_LOGIC;
                signal reset_reg0 :  STD_LOGIC;
                signal reset_reg1 :  STD_LOGIC;
                signal reset_write_enable0 :  STD_LOGIC;
                signal reset_write_enable1 :  STD_LOGIC;
                signal select_mutex0 :  STD_LOGIC;
                signal select_mutex1 :  STD_LOGIC;
                signal select_reset_reg0 :  STD_LOGIC;
                signal select_reset_reg1 :  STD_LOGIC;

begin

  process (clk, reset_n)
  begin
    if reset_n = '0' then
      mutex0 <= std_logic_vector'("00000000000000000000000000000000");
    elsif clk'event and clk = '1' then
      if std_logic'(mutex_write_enable0) = '1' then 
        mutex0 <= data_from_cpu;
      end if;
    end if;

  end process;

  process (clk, reset_n)
  begin
    if reset_n = '0' then
      mutex1 <= std_logic_vector'("00000000000000000000000000000000");
    elsif clk'event and clk = '1' then
      if std_logic'(mutex_write_enable1) = '1' then 
        mutex1 <= data_from_cpu;
      end if;
    end if;

  end process;

  process (clk, reset_n)
  begin
    if reset_n = '0' then
      reset_reg0 <= std_logic'('1');
    elsif clk'event and clk = '1' then
      if std_logic'(reset_write_enable0) = '1' then 
        reset_reg0 <= std_logic'('0');
      end if;
    end if;

  end process;

  process (clk, reset_n)
  begin
    if reset_n = '0' then
      reset_reg1 <= std_logic'('1');
    elsif clk'event and clk = '1' then
      if std_logic'(reset_write_enable1) = '1' then 
        reset_reg1 <= std_logic'('0');
      end if;
    end if;

  end process;

  mutex_free0 <= to_std_logic(((std_logic_vector'("0000000000000000") & (mutex0(15 DOWNTO 0))) = std_logic_vector'("00000000000000000000000000000000")));
  mutex_free1 <= to_std_logic(((std_logic_vector'("0000000000000000") & (mutex1(15 DOWNTO 0))) = std_logic_vector'("00000000000000000000000000000000")));
  mutex_own0 <= to_std_logic((mutex0(31 DOWNTO 16) = data_from_cpu(31 DOWNTO 16)));
  mutex_own1 <= to_std_logic((mutex1(31 DOWNTO 16) = data_from_cpu(31 DOWNTO 16)));
  select_mutex0 <= chipselect AND to_std_logic((((std_logic_vector'("000000000000000000000000000000") & (address)) = std_logic_vector'("00000000000000000000000000000000"))));
  select_mutex1 <= chipselect AND to_std_logic((((std_logic_vector'("000000000000000000000000000000") & (address)) = std_logic_vector'("00000000000000000000000000000010"))));
  select_reset_reg0 <= chipselect AND to_std_logic((((std_logic_vector'("000000000000000000000000000000") & (address)) = std_logic_vector'("00000000000000000000000000000001"))));
  select_reset_reg1 <= chipselect AND to_std_logic((((std_logic_vector'("000000000000000000000000000000") & (address)) = std_logic_vector'("00000000000000000000000000000011"))));
  mutex_write_enable0 <= (((mutex_free0 OR mutex_own0)) AND select_mutex0) AND write;
  mutex_write_enable1 <= (((mutex_free1 OR mutex_own1)) AND select_mutex1) AND write;
  reset_write_enable0 <= (chipselect AND write) AND select_reset_reg0;
  reset_write_enable1 <= (chipselect AND write) AND select_reset_reg1;
  data_to_cpu <= A_WE_StdLogicVector((std_logic'(select_mutex0) = '1'), mutex0, A_WE_StdLogicVector((std_logic'(select_mutex1) = '1'), mutex1, (std_logic_vector'("0000000000000000000000000000000") & (A_TOSTDLOGICVECTOR(A_WE_StdLogic((std_logic'(select_reset_reg0) = '1'), reset_reg0, reset_reg1))))));
  --s1, which is an e_avalon_slave

end europa;

