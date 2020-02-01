# Space Invaders - Emulator
This is an emulation of the 1978 arcade machine used to play Space Invaders written in C. The emulator consists of two parts: the Intel 8080 CPU found inside the machine and the rest of the hardware necessary to run the game.

## Menu screen and attract mode
![main menu](https://media.giphy.com/media/MY7TGkNY0FZ47LLUJX/giphy.gif) ![attract mode](https://media.giphy.com/media/gIO63RKk3RjbLmAVED/giphy.gif)

## How to build

Building the project requires a **C compiler** and is easiest built using **make**.

##### Dependencies:
* SDL2

#### Ubuntu 18.04
1) Get SDL2 library:

        sudo apt install libsdl2-dev

2)  In project folder, run:
    * gcc:
    
            make && ./spaceinvaders
            
    * clang or other compiler:
    
            make CC=clang && ./spaceinvaders
        
## Controls
| ACTION    | KEY   |
|:---------:|:-----:|
| Insert Coin | c |
| Start 1P  | return |
| Start 2P  | 2 |
| Shoot     | Space |
| Move Left | &larr; |
| Move Right| &rarr; |
| Quit      | q |

## References
* [Intel 8080 official manual](https://altairclone.com/downloads/manuals/8080%20Programmers%20Manual.pdf)
* [Space Invaders - Hardware and Code](https://computerarcheology.com/Arcade/SpaceInvaders/)
* [Table of opcodes](https://pastraiser.com/cpu/i8080/i8080_opcodes.html)
* [Reference implementation](https://github.com/superzazu/invaders)
