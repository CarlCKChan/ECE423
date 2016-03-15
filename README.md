# Simple-MPEG-Player
Reference for old embedded systems labs

This plays a simplified version of the MPEG video format.  The goal was to optimize the inefficient ~3 fps player to have a better frame rate, without interfering with quality or skipping frames.

Optimizations include:
  - Implementing custom instructions on the simulated FPGA CPU.
  - Adding a secondary processor and scheduling tasks between the two to increase throughput.
  - Using the difference in width between the processor instruction return and required return values to return multiple results at once.
  - Aligning the writing of results into a array with the return of functions to write multiple values at once.
  - General software optimizations.
