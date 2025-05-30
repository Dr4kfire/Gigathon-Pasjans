# SOLITARE - GIGATHON 2025 Project

This project was made using C++ in just 9 days (I joined the competition later).
I don't have too much experience with C++ (it was my second ever C++ project!) so I had many problems first compiling the libraries (it took me 3 days).
It's using the ncurses and PDCursesMod libraies for rendering ascii characters inside the terminal.

## Controls:

Use the `arrow keys` to navigate in the menus and to select cards. To move cards use `ENTER` to choose what do you want to move and then press it again while hovering over the place you want to move cards to reposition them. You can undo 3 times using the `u` key and reset using `r`. To exit the game press `ctrl`+`c`.

## Project structure:

The structure looks like this:
1. `Application` class is the base - it draws the characters to the screen and passes down inputs.
2. `Game` class is the middle layer between the `Application` and `Scene`s - Used for changing the scenes and storing important settings.
3. `Scene` class is an **abstract base class** for the `MainMenu` and `GameScene` scenes.
   - `MainMenu` class is used to handle the main menu.
   - `GameScene` class is the core of the game - that's where everything happens when it comes to gameplay.
4. `Card` struct and `Deck` class - they store data about the cards and `Deck` also implements some usefull methods for deck/cards manipulation.

## How to compile the project

First get the project's source files on your computer using this command:
```
git clone https://github.com/Dr4kfire/Gigathon-Pasjans.git
```
In the root of this project there is a `CMakeLists.txt` file used to compile this project. 
The simplest way of doing so would be to install the Microsoft's C/C++ Extension Pack in VSCode and compile ther project using CMake.
