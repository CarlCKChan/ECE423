LIBRARY ieee;
USE ieee.std_logic_1164.ALL;
--USE ieee.std_logic_unsigned.all;
use ieee.numeric_std.all;

-- entity declaration for your testbench.Dont declare any ports here
ENTITY test_tb IS 
END test_tb;

ARCHITECTURE behavior OF test_tb IS
   -- Component Declaration for the Unit Under Test (UUT)
    COMPONENT custominstruction_2d  --'test' is the name of the module needed to be tested.
--just copy and paste the input and output ports of your module as such. 
    PORT( 
        signal clk: in std_logic;				-- CPU system clock (always required)
	      signal reset: in std_logic;				-- CPU master asynchronous active high reset (always required)
	      signal clk_en: in std_logic;				-- Clock-qualifier (always required)
	      signal start: in std_logic;				-- Active high signal used to specify that inputs are valid (always required)
	      signal done: out std_logic;				-- Active high signal used to notify the CPU that result is valid (required for variable multi-cycle)
	      signal n: in std_logic_vector(7 downto 0);		-- N-field selector (always required), modify width to match the number of unique operations within the custom instruction
	      signal dataa: in std_logic_vector(31 downto 0);		-- Operand A (always required)
	      signal datab: in std_logic_vector(31 downto 0);		-- Operand B (optional)
	      signal result: out std_logic_vector(31 downto 0)	-- result (always required)
    );
    END COMPONENT;
   --declare inputs and initialize them
   signal res1 : signed(15 downto 0);
   signal res2 : signed(15 downto 0);
   
   signal CONST_0_bin : std_logic_vector(15 downto 0) := std_logic_vector(to_signed(0, 16)); --"0000000000000000";
   signal CONST_100_bin : std_logic_vector(15 downto 0) := std_logic_vector(to_signed(100, 16)); --"0000000001100100";
   signal CONST_1240_bin : std_logic_vector(15 downto 0) := std_logic_vector(to_signed(1240, 16));
   signal CONST_n10_bin : std_logic_vector(15 downto 0) := std_logic_vector(to_signed(-10, 16));
   signal CONST_n12_bin : std_logic_vector(15 downto 0) := std_logic_vector(to_signed(-12, 16));
   signal CONST_n13_bin : std_logic_vector(15 downto 0) := std_logic_vector(to_signed(-13, 16));
   signal CONST_n14_bin : std_logic_vector(15 downto 0) := std_logic_vector(to_signed(-14, 16));
   signal CONST_n24_bin : std_logic_vector(15 downto 0) := std_logic_vector(to_signed(-24, 16));
   
   type test_vec_type is array (0 to 7) of std_logic_vector(15 downto 0);
   type test_vec_type_2d is array (0 to 7, 0 to 7) of std_logic_vector(15 downto 0);
   
   -- signal test_mat : test_vec_type_2d := (
		-- (CONST_100_bin, CONST_0_bin, CONST_0_bin, CONST_0_bin, CONST_0_bin, CONST_0_bin, CONST_0_bin, CONST_0_bin),
		-- (CONST_0_bin, CONST_100_bin, CONST_0_bin, CONST_0_bin, CONST_0_bin, CONST_0_bin, CONST_0_bin, CONST_0_bin),
		-- (CONST_0_bin, CONST_0_bin, CONST_100_bin, CONST_0_bin, CONST_0_bin, CONST_0_bin, CONST_0_bin, CONST_0_bin),
		-- (CONST_0_bin, CONST_0_bin, CONST_0_bin, CONST_100_bin, CONST_0_bin, CONST_0_bin, CONST_0_bin, CONST_0_bin),
		-- (CONST_0_bin, CONST_0_bin, CONST_0_bin, CONST_0_bin, CONST_100_bin, CONST_0_bin, CONST_0_bin, CONST_0_bin),
		-- (CONST_0_bin, CONST_0_bin, CONST_0_bin, CONST_0_bin, CONST_0_bin, CONST_100_bin, CONST_0_bin, CONST_0_bin),
		-- (CONST_0_bin, CONST_0_bin, CONST_0_bin, CONST_0_bin, CONST_0_bin, CONST_0_bin, CONST_100_bin, CONST_0_bin),
		-- (CONST_0_bin, CONST_0_bin, CONST_0_bin, CONST_0_bin, CONST_0_bin, CONST_0_bin, CONST_0_bin, CONST_100_bin)
   -- );
   
   signal test_mat : test_vec_type_2d := (
		(CONST_1240_bin, CONST_0_bin, CONST_n10_bin, CONST_0_bin, CONST_0_bin, CONST_0_bin, CONST_0_bin, CONST_0_bin),
		(CONST_n24_bin, CONST_n12_bin, CONST_0_bin, CONST_0_bin, CONST_0_bin, CONST_0_bin, CONST_0_bin, CONST_0_bin),
		(CONST_n14_bin, CONST_n13_bin, CONST_0_bin, CONST_0_bin, CONST_0_bin, CONST_0_bin, CONST_0_bin, CONST_0_bin),
		(CONST_0_bin, CONST_0_bin, CONST_0_bin, CONST_0_bin, CONST_0_bin, CONST_0_bin, CONST_0_bin, CONST_0_bin),
		(CONST_0_bin, CONST_0_bin, CONST_0_bin, CONST_0_bin, CONST_0_bin, CONST_0_bin, CONST_0_bin, CONST_0_bin),
		(CONST_0_bin, CONST_0_bin, CONST_0_bin, CONST_0_bin, CONST_0_bin, CONST_0_bin, CONST_0_bin, CONST_0_bin),
		(CONST_0_bin, CONST_0_bin, CONST_0_bin, CONST_0_bin, CONST_0_bin, CONST_0_bin, CONST_0_bin, CONST_0_bin),
		(CONST_0_bin, CONST_0_bin, CONST_0_bin, CONST_0_bin, CONST_0_bin, CONST_0_bin, CONST_0_bin, CONST_0_bin)
   );
   
   --signal test_vector : test_vec_type := (CONST_100_bin, CONST_100_bin, CONST_0_bin, CONST_0_bin, CONST_0_bin, CONST_0_bin, CONST_0_bin, CONST_0_bin); -- 100 100 0 0 0 0 0 0
   --signal test_vector : test_vec_type := (CONST_0_bin, CONST_0_bin, CONST_100_bin, CONST_100_bin, CONST_0_bin, CONST_0_bin, CONST_0_bin, CONST_0_bin); -- 0 0 100 100 0 0 0 0
   --signal test_vector : test_vec_type := (CONST_0_bin, CONST_0_bin, CONST_0_bin, CONST_0_bin, CONST_100_bin, CONST_100_bin, CONST_0_bin, CONST_0_bin); -- 0 0 0 0 100 100 0 0
   --signal test_vector : test_vec_type := (CONST_0_bin, CONST_0_bin, CONST_0_bin, CONST_0_bin, CONST_0_bin, CONST_0_bin, CONST_100_bin, CONST_100_bin); -- 0 0 0 0 0 0 100 100
   --signal test_vector : test_vec_type := (CONST_100_bin, CONST_100_bin, CONST_100_bin, CONST_100_bin, CONST_0_bin, CONST_0_bin, CONST_0_bin, CONST_0_bin); -- 100 100 100 100 0 0 0 0
   --signal test_vector : test_vec_type := (CONST_0_bin, CONST_0_bin, CONST_0_bin, CONST_0_bin, CONST_100_bin, CONST_100_bin, CONST_100_bin, CONST_100_bin); -- 0 0 0 0 100 100 100 100
   --signal test_vector : test_vec_type := (CONST_100_bin, CONST_0_bin, CONST_100_bin, CONST_0_bin, CONST_100_bin, CONST_0_bin, CONST_100_bin, CONST_0_bin); -- 100 0 100 0 100 0 100 0
   --signal test_vector : test_vec_type := (CONST_100_bin, CONST_100_bin, CONST_100_bin, CONST_100_bin, CONST_100_bin, CONST_100_bin, CONST_100_bin, CONST_100_bin); -- 100 100 100 100 100 100 100 100
   
   signal clk : std_logic := '0';
   signal reset : std_logic := '0';
   signal clk_en: std_logic := '1';
   signal start : std_logic := '1';
   signal done : std_logic;
   signal n : std_logic_vector(7 downto 0) := "00000000";
   signal n_i : std_logic_vector(5 downto 0);
   signal dataa: std_logic_vector(31 downto 0);
   signal datab: std_logic_vector(31 downto 0);
   --declare outputs and initialize them
   signal result: std_logic_vector(31 downto 0);
   -- Clock period definitions
   constant clk_period : time := 10 ns;
BEGIN
    -- Instantiate the Unit Under Test (UUT)
   uut: custominstruction_2d PORT MAP (
          clk => clk,
          reset => reset,
          clk_en => clk_en,
          dataa => dataa,
          datab => datab,
          result => result,
          n => n,
          start => start,
          done => done
        );
        
    res1 <= signed(result(31 downto 16));
    res2 <= signed(result(15 downto 0));
	n <= "00" & n_i;

   -- Clock process definitions( clock with 50% duty cycle is generated here.
   clk_process :process
   begin
        clk <= '0';
        wait for clk_period/2;
        clk <= '1';
        wait for clk_period/2;
        
   end process;
   -- Stimulus process
  stim_proc: process
   begin
     
		for i in 0 to 7 loop
			n_i <= "00" & std_logic_vector(to_unsigned(i,3)) & "0";
			dataa <= test_mat(0,i) & test_mat(1,i);
			datab <= test_mat(2,i) & test_mat(3,i);
			wait for clk_period;
			
			n_i <= "01" & std_logic_vector(to_unsigned(i,3)) & "0";
			dataa <= test_mat(4,i) & test_mat(5,i);
			datab <= test_mat(6,i) & test_mat(7,i);
			wait for clk_period;
		end loop;
		
		for j in 0 to 7 loop
			n_i <= "00" & std_logic_vector(to_unsigned(j,3)) & "1";
			wait for clk_period;
			n_i <= "01" & std_logic_vector(to_unsigned(j,3)) & "1";
			wait for clk_period;
			n_i <= "10" & std_logic_vector(to_unsigned(j,3)) & "1";
			wait for clk_period;
			n_i <= "11" & std_logic_vector(to_unsigned(j,3)) & "1";
			wait for clk_period;
		end loop;
        -- reset <= '1';       
        -- wait for 7 ns;
        -- reset <= '0';
        -- wait for 3 ns;
        -- n <= "00000000";
        -- clk_en <= '1';
        -- dataa <= test_vector(0) & test_vector(1);
        -- datab <= test_vector(2) & test_vector(3);
        -- wait for 10 ns;
        -- dataa <= test_vector(4) & test_vector(5);
        -- datab <= test_vector(6) & test_vector(7);
        -- wait for 10 ns;
        -- n <= "00000010";
        -- wait for 10 ns;
        -- wait for 10 ns;
        -- wait for 10 ns;
        -- wait for 10 ns;
        -- n <= "00000011";
        -- wait for 10 ns;
        -- wait for 10 ns;
        -- wait for 10 ns;
        -- wait for 10 ns;
        -- wait;
  end process;

END;
