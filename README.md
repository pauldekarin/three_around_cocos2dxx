# Block Removal Game
<img src="https://github.com/pauldekarin/three_around_cocos2dxx/blob/main/preview/game.gif"/>

## Overview

This project is a test task for a C++ developer position at Dankolab, implemented using the Cocos2d game engine. The task was to create a game where the player removes as many colored blocks from a rectangular field as possible. When a block is clicked, all adjacent blocks of the same color are found and, if there are three or more, they are destroyed. The remaining blocks above fall down to fill the empty spaces. The interface includes a button to start the game.

![reference](./images/reference.png)

## Game Mechanics

### Specifications

- The game launches correctly on a desktop with a screen resolution of 1280x1024.
- The game starts immediately with the following settings: width = 16, height = 10, colors = 3.
- The game field is centered and scaled to fit the screen with a small margin.
- Upon game start, the field is completely filled with blocks of randomly chosen colors.
- Clicking on a block counts and removes all adjacent blocks of the same color (adjacent blocks share a side).
- Remaining blocks above fall down to fill the empty spaces after removal.
- The game can be restarted by pressing the "Start" button.

## Additional Features

- Input fields for width, height, and number of colors in the game interface.
- Validation of user input data.
- Score calculation for removed blocks.
- End-game screen when no more moves are available.
- Animation for block removal and falling.
- Sound effects.
- Custom graphics.

## Installation

### Requirements

- Cocos2d game engine: [Download Cocos2d](https://disk.yandex.ru/d/WFSVqvCGur3hyA)
- C++ Compiler (e.g., g++)

### Building the Project

1. Clone the repository:
   ```sh
   git clone https://github.com/your_username/block_removal_game.git
   cd block_removal_game
   ```



