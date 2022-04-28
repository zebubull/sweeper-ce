# SweeperCE
SweeperCE is a port of minesweeper for the TI-84 Plus CE Graphing Calculator. Simply download the game, transfer it to your calculator, and run to enjoy hours of sweeping fun!

## Controls
Arrows - Move  
Del - Quit  
Enter - Reveal  
Add - Flag  

## Custom Board
Custom board sizes can be set on the main menu. Simply select the height or width option, and use the left and right arrow keys to change the board size. The minimum size is 5x10 and the maximum size is 20x20 due to screen space limitiations. Sizes are saved every time you exit the game so you don't have to reenter them.

# Compiling Manually
## Prerequisites
- [CE C Toolchain](https://github.com/CE-Programming/toolchain)
## Instructions
- Download the source code
- run `make gfx`
- run `make`
- the compiled program will be in the `bin` folder

