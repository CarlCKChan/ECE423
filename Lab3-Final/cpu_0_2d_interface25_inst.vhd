-- cpu_0_2d_interface25_inst.vhd

-- This file was auto-generated as part of a SOPC Builder generate operation.
-- If you edit it your changes will probably be lost.

library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.numeric_std.all;

entity cpu_0_2d_interface25_inst is
	port (
		clk    : in  std_logic                     := '0';             -- nios_custom_instruction_slave_0.clk
		reset  : in  std_logic                     := '0';             --                                .reset
		clk_en : in  std_logic                     := '0';             --                                .clk_en
		n      : in  std_logic_vector(5 downto 0)  := (others => '0'); --                                .n
		dataa  : in  std_logic_vector(31 downto 0) := (others => '0'); --                                .dataa
		datab  : in  std_logic_vector(31 downto 0) := (others => '0'); --                                .datab
		result : out std_logic_vector(31 downto 0)                     --                                .result
	);
end entity cpu_0_2d_interface25_inst;

architecture rtl of cpu_0_2d_interface25_inst is
	component custominstruction_2d is
		port (
			clk    : in  std_logic                     := 'X';             -- clk
			reset  : in  std_logic                     := 'X';             -- reset
			clk_en : in  std_logic                     := 'X';             -- clk_en
			n      : in  std_logic_vector(5 downto 0)  := (others => 'X'); -- n
			dataa  : in  std_logic_vector(31 downto 0) := (others => 'X'); -- dataa
			datab  : in  std_logic_vector(31 downto 0) := (others => 'X'); -- datab
			result : out std_logic_vector(31 downto 0)                     -- result
		);
	end component custominstruction_2d;

begin

	cpu_0_2d_interface25_inst : component custominstruction_2d
		port map (
			clk    => clk,    -- nios_custom_instruction_slave_0.clk
			reset  => reset,  --                                .reset
			clk_en => clk_en, --                                .clk_en
			n      => n,      --                                .n
			dataa  => dataa,  --                                .dataa
			datab  => datab,  --                                .datab
			result => result  --                                .result
		);

end architecture rtl; -- of cpu_0_2d_interface25_inst
