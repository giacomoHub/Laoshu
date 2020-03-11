# LAOSHU 
#### (mouse in chinese)

### PROJECT OVERVIEW
This project is a mini game for the BBC Micro:Bit, created for a university assignment.  
This game celebrates the Chinese year of the rat.

---

### HOW TO PLAY
The “display” is made of a 5*5 matrix of LEDs, the player will be able to move the mouse (one led) in the 3*3 matrix at the centre of the display. 

The Aim of the game is to survive as long as possible before being hit from one of the laser coming from the edges of the display. Some LEDs at the edges will turn on, signalling the player where the laser is going to appear. The player will have to move from the path of the laser before it turns on. More lasers will appear after the player has reached a certain score, making the game more difficult. If the player gets hit by a laser the game ends showing the player his score.

### CONTROLS
The controls are simple, the player utilises the buttons “A” and “B” to move left and right respectively. When the player presses simultaneously “A” and “B” he will change his movement axis, so now the player will have to press “A” and “B” to move up and down respectively. 
 * --> “A”: move left/up.
 * --> “B”: move right/down.
 * --> “A” and “B”: change movement axis.

---

### ADDITIONAL FEATURES
Once the game turns on, it will show a “Title Screen” and the name of the game will scroll through the display. After that the game will start. Once the player loses, a score will be shown to the player, soon after the game will restart (without showing the title screen) to give a fast and clean user experience and replayability.

On the technical aspect this game makes use of the Micro:Bit ability to use fibers. Each "laser" is in fact controlled separately and is not linked to each other. This allows for greated concurrency and management. The game also makes use of the GREYSCALE capability of the LEDs that show 256 levels of brightness.

---
### HOW TO BUILD

1_Download `main.cpp`

2_Clone from: `git clone https://github.com/lancaster-university/microbitsamples`

3_Then copy `main.cpp` to the newly cloned `microbit-samples/source`

4_In terminal move to `microbit-samples` and run the command: `yt build` or `yotta build`.

5_Navigate to `microbit-samples/bbc-microbit-classic-gcc/source/` and copy `microbit-samples-combined.hex` into the BBC Micro:Bit.

---
CREDITS

**Giacomo Pellizzari**
