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


-- VHDL Custom Instruction Template File for Combinatorial Logic

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity ycbcr_to_rgb is
port(
	signal dataa: in std_logic_vector(31 downto 0);		-- Operand A (always required)
																		-- dataa(7 downto 0) contains Y
	signal datab: in std_logic_vector(31 downto 0);		-- Operand B (optional)
																		-- datab(7 downto 0) contains Cb
																		-- datab(15 downto 8) contains Cr
	signal result: out std_logic_vector(31 downto 0)	-- result (always required)
																		-- result(7 downto 0) is blue
																		-- result(15 downto 8) is green
																		-- result(24 downto 16) is red
);
end entity ycbcr_to_rgb;



architecture a_ycbcr_to_rgb of ycbcr_to_rgb is

	-- local custom instruction signals
	signal Cbb	: signed (7 downto 0);
	signal Crr  : signed (7 downto 0);
	signal YYstd: std_logic_vector(23 downto 0);
	signal YY	: signed (23 downto 0);
	
	signal Cr1402 : signed (23 downto 0);
	signal Cr071414 : signed (22 downto 0);
	signal Cb034414 : signed (21 downto 0);
	signal Cb1722 : signed (23 downto 0);
	
	signal red24 : signed (23 downto 0);
	signal blue24 : signed (23 downto 0);
	signal green24 : signed (23 downto 0);
	
	constant c22970 : signed (15 downto 0) := "0101100110111010";
	constant c11700 : signed (14 downto 0) := "010110110110100";
	constant c5638 : signed (13 downto 0) := "01011000000110";
	constant c29032 : signed (15 downto 0) := "0111000101101000";
	
begin

	-- custom instruction logic (note:  no external interfaces are allowed in combinatorial logic)
	YYstd <= "00" & dataa(7 downto 0) & "00000000000000";
	YY <= signed (YYstd);
	Cbb <= signed ( (not datab(7)) & datab(6 downto 0) );	--subtract 128
	Crr <= signed ( (not datab(15)) & datab(14 downto 8) );
	
	Cr1402 <= Crr * c22970;
	Cr071414 <= Crr * c11700;
	Cb034414 <= Cbb * c5638;
	Cb1722 <= Cbb * c29032;
	
	red24 <= YY + Cr1402;
	blue24 <= YY + Cb1722;
	green24 <= YY - Cb034414 - Cr071414;
	
	result(31 downto 24) <= (others=>'0');
	result(7 downto 0) <= (others=>'0') when blue24(23) = '1' else
									(others=>'1') when blue24(22) = '1' else
									std_logic_vector(blue24(21 downto 14));
									
	result(15 downto 8) <= (others=>'0') when green24(23) = '1' else
									(others=>'1') when green24(22) = '1' else
									std_logic_vector(green24(21 downto 14));
									
	result(23 downto 16) <= (others=>'0') when red24(23) = '1' else
									(others=>'1') when red24(22) = '1' else
									std_logic_vector(red24(21 downto 14));

end architecture a_ycbcr_to_rgb;