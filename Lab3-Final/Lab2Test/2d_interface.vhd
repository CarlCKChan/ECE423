--Copyright (C) 1991-2004 Altera Corporation
--Any megafunction design, and related net list (encrypted or decrypted),
--support information, device programming or simulation file, and any other
--associated documentation or information provided by Altera or a partner
--under Altera's Megafunction Partnership Program may be used only to
--program PLD devices (but not masked PLD devices) from Altera.  Any other
--use of such megafunction design, net list, support information, device
--programming or simulation file, or any other related documentation or
--information is prohibited for any other purpose, including, but not
--limited to modification, reverse engineering, de-compiling, or use with
--any other silicon devices, unless such use is explicitly licensed under
--a separate agreement with Altera or a megafunction partner.  Title to
--the intellectual property, including patents, copyrights, trademarks,
--trade secrets, or maskworks, embodied in any such megafunction design,
--net list, support information, device programming or simulation file, or
--any other related documentation or information provided by Altera or a
--megafunction partner, remains with Altera, the megafunction partner, or
--their respective licensors.  No other licenses, including any licenses
--needed under any third party's intellectual property, are provided herein.
--Copying or modifying any file, or portion thereof, to which this notice
--is attached violates this copyright.


-- VHDL Custom Instruction Template File for Extended Logic

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity custominstruction_2d is
port(
	signal clk: in std_logic;				-- CPU system clock (always required)
	signal reset: in std_logic;				-- CPU master asynchronous active high reset (always required)
	signal clk_en: in std_logic;				-- Clock-qualifier (always required)
	--signal start: in std_logic;				-- Active high signal used to specify that inputs are valid (always required)
	--signal done: out std_logic;				-- Active high signal used to notify the CPU that result is valid (required for variable multi-cycle)
	signal n: in std_logic_vector(5 downto 0);		-- N-field selector (always required), modify width to match the number of unique operations within the custom instruction
	signal dataa: in std_logic_vector(31 downto 0);		-- Operand A (always required)
	signal datab: in std_logic_vector(31 downto 0);		-- Operand B (optional)
	signal result: out std_logic_vector(31 downto 0)	-- result (always required)
);
end entity custominstruction_2d;



architecture customarchitecture_2d of custominstruction_2d is

component idct_1D 
generic(
	CONST_BITS: integer := 13;
	PASS1_BITS: integer := 2
);
port(
	signal pass : in std_logic;			-- 0: Pass 1; 1: Pass 2
	
	signal i0: in std_logic_vector(15 downto 0);
	signal i1: in std_logic_vector(15 downto 0);
	signal i2: in std_logic_vector(15 downto 0);
	signal i3: in std_logic_vector(15 downto 0);
	signal i4: in std_logic_vector(15 downto 0);
	signal i5: in std_logic_vector(15 downto 0);
	signal i6: in std_logic_vector(15 downto 0);
	signal i7: in std_logic_vector(15 downto 0);
	
	signal o0: out std_logic_vector(15 downto 0);
	signal o1: out std_logic_vector(15 downto 0);
	signal o2: out std_logic_vector(15 downto 0);
	signal o3: out std_logic_vector(15 downto 0);
	signal o4: out std_logic_vector(15 downto 0);
	signal o5: out std_logic_vector(15 downto 0);
	signal o6: out std_logic_vector(15 downto 0);
	signal o7: out std_logic_vector(15 downto 0)
);
end component;

--signal tempInput1 : std_logic_vector(31 downto 0);
--signal tempInput2 : std_logic_vector(31 downto 0);
--signal tempInput3 : std_logic_vector(31 downto 0);
--signal tempInput4 : std_logic_vector(31 downto 0);
signal temp_pass : std_logic;
type idct_1d_vec is array (0 to 7) of std_logic_vector(15 downto 0);
type idct_2d_array is array (0 to 7, 0 to 7) of std_logic_vector(15 downto 0);
signal temp_input : idct_1d_vec;
signal temp_workspace_input_vec : idct_1d_vec;
signal temp_workspace_output_vec : idct_1d_vec;

signal temp_workspace : idct_2d_array;
signal temp_output : idct_1d_vec;
--signal tempOutput1 : std_logic_vector(31 downto 0);
--signal tempOutput2 : std_logic_vector(31 downto 0);
--signal tempOutput3 : std_logic_vector(31 downto 0);
--signal tempOutput4 : std_logic_vector(31 downto 0);
--signal counter : std_logic_vector(15 downto 0);

signal input_en : std_logic;
signal output_en : std_logic;

signal i_o_state : std_logic;
signal column_row : std_logic_vector(2 downto 0);
signal column_row_int : integer range 0 to 7;
signal i_o_step : std_logic_vector(1 downto 0);
signal i_o_step_int : integer range 0 to 3;

signal a_left : std_logic_vector(15 downto 0);
signal a_right : std_logic_vector(15 downto 0);
signal b_left : std_logic_vector(15 downto 0);
signal b_right : std_logic_vector(15 downto 0);

begin

	a_left <= dataa(31 downto 16);
	a_right <= dataa(15 downto 0);
	b_left <= datab(31 downto 16);
	b_right <= datab(15 downto 0);
  
  i_o_state <= n(0);
  column_row <= n(3 downto 1);
  i_o_step <= n(5 downto 4);
  column_row_int <= to_integer(unsigned(column_row));
  i_o_step_int <= to_integer(unsigned(i_o_step));
  
  PROCESS(clk)
  BEGIN
    if rising_edge(clk) AND clk_en = '1' AND i_o_state = '0' THEN
      if i_o_step = "00" THEN
        temp_input(0) <= a_right;
        temp_input(1) <= a_left;
        temp_input(2) <= b_right;
        temp_input(3) <= b_left;
      END if;
    END if;
  END PROCESS;


  temp_workspace_input_vec(0) <= temp_input(0) when i_o_state = '0' else temp_workspace(column_row_int, 0);
  temp_workspace_input_vec(1) <= temp_input(1) when i_o_state = '0' else temp_workspace(column_row_int, 1);
  temp_workspace_input_vec(2) <= temp_input(2) when i_o_state = '0' else temp_workspace(column_row_int, 2);
  temp_workspace_input_vec(3) <= temp_input(3) when i_o_state = '0' else temp_workspace(column_row_int, 3);
  temp_workspace_input_vec(4) <= a_right when i_o_state = '0' else temp_workspace(column_row_int, 4);
  temp_workspace_input_vec(5) <= a_left when i_o_state = '0' else temp_workspace(column_row_int, 5);
  temp_workspace_input_vec(6) <= b_right when i_o_state = '0' else temp_workspace(column_row_int, 6);
  temp_workspace_input_vec(7) <= b_left when i_o_state = '0' else temp_workspace(column_row_int, 7);	

  
  PROCESS(clk)
  BEGIN
    if rising_edge(clk)  THEN --AND output_en = '1' THEN
      if(i_o_state = '0' AND clk_en = '1') then
		    for i in 0 to 7 loop
		    	temp_workspace(i,column_row_int) <= temp_output(i);
	     	end loop;
	  end if;
    END if;
  END PROCESS;

  result <= temp_output(0) & temp_output(1) when i_o_step = "00" else
		    temp_output(2) & temp_output(3) when i_o_step = "01" else
		    temp_output(4) & temp_output(5) when i_o_step = "10" else
		    temp_output(6) & temp_output(7);

  
  
  IDCT_1: idct_1D port map (i_o_state,
                            temp_workspace_input_vec(0), temp_workspace_input_vec(1), temp_workspace_input_vec(2), temp_workspace_input_vec(3), temp_workspace_input_vec(4), temp_workspace_input_vec(5), temp_workspace_input_vec(6), temp_workspace_input_vec(7), 
                            temp_output(0), temp_output(1), temp_output(2), temp_output(3), temp_output(4), temp_output(5), temp_output(6), temp_output(7));

 
end architecture customarchitecture_2d;

