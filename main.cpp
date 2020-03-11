/*
---------------LAOSHU---------------
--Game made for the BBC Microbit.
The game features a mouse that has to "escape" from the laser beams that come to the player
Player utilizes A and B to move rispectevly left and right. When the player
simultaneusly presses A and B the movement axis changes, so the player will 
press A and B to move up and down rispectevly.
*/

//Giacomo Pellizzari
//Student Number: 17045579

#include "MicroBit.h"

#define GAME_ON         false
#define GAME_OVER       true

#define LIGHT_STRONG    255
#define LIGHT_MEDIUM    100
#define LIGHT_LOW       1
#define LIGHT_OFF       0

#define MOVEMENT_SPEED  300 //the lower the faster
#define WAVE_TIME       50
#define EXPLOSION_TIME  10

#define UPDOWN          0
#define LEFTRIGHT       1

#define EXTREME         2
#define DIFFICULT       1
#define EASY            0

void gameOver();            //show score to the player
void intro();               //show the title screen    
void laoshu();              //set the initial state and keep the game running
void topStart();            //control the top laser
void leftStart();           //control the left laser
void rightStart();          //control the right laser
void bottomStart();         //control the bottom laser


MicroBit uBit;
MicroBitImage field(5,5); //the field of play (5x5 matrix of pixels)
MicroBitMessageBus bus; 

bool game_over;
int score;
int difficulty;
bool bottomFlag;        //keep track of wether the bottom laser has started
bool rightFlag;         //keep track of wether the right laser has started
int movementState = 1;  //keep track of the state of the movement (left&right or up&down)

//struct containing the coordinates of a point 
struct Position{
    int x;
    int y;
};
Position mouse;

class Laser{
    public:
    int score = 0;
    Position pos;
    bool exploding = false;
    bool active = true; //activate laser when difficulty requires it
    int timer = WAVE_TIME;
    int explosionTimer = EXPLOSION_TIME;
    
    //make the dot blink until it explodes
    //"Main" function of the fiber... has a timer before exploding
    void tick(){
        while(!game_over){
            exploding = false;
            timer = WAVE_TIME;
            while(!exploding && !game_over){
                //blink
                uBit.sleep(WAVE_TIME);
                timer = timer - 1;
                if(timer<0){
                    exploding = true;
                    if(active)
                        explode();
                    nextLaser();
                }
            }
        }
    }

    //make the laser beam appear in the whole row or column
    void explode(){
        explosionTimer = EXPLOSION_TIME;
        while(explosionTimer > 0){
            for(int i = 0; i <5; i++){
                //make the laser explode
                if(pos.y == 0 || pos.y == 4){
                    uBit.display.image.setPixelValue(pos.x,i,LIGHT_MEDIUM);
                    //check if the gameover function has alredy been called by other fibers
                    if(pos.x == mouse.x && game_over != GAME_OVER){
                        game_over = GAME_OVER;
                    }
                }else{
                    uBit.display.image.setPixelValue(i,pos.y,LIGHT_MEDIUM);
                    if(pos.y == mouse.y && game_over != GAME_OVER){
                        game_over = GAME_OVER;
                    }
                }
                
            }
            explosionTimer--;
        }
    }

    //function that moves the laser to the next position (yet to finish)
    void nextLaser(){
        score++;
        if(pos.x == 0 || pos.x == 4){
            pos.y = 0;
            while(pos.y<1)
                pos.y = uBit.random(4);
        }else{
            pos.x = 0;
            while(pos.x<1)
                pos.x = uBit.random(4);
        }
    }
};


Laser top;
Laser left;
Laser right;
Laser bottom;



//function that displays the initial "loading" scene
void intro()
{
    MicroBitImage mouse("1,1,0,1,1\n1,1,0,1,1\n0,100,1,100,0\n0,1,1,1,0\n0,0,32,0,0\n"); //image of a mouse
    uBit.display.setDisplayMode(DISPLAY_MODE_GREYSCALE);
    for(int y=4; y>=0; y--){ //scroll the image upwards
        uBit.display.image.paste(mouse,0,y);
        uBit.sleep(500);
    }
    uBit.display.scroll("LAOSHU");//show title of the game
}

//function to move the mouse left or up
void onButtonA(MicroBitEvent) {
    if(movementState==LEFTRIGHT){
        if(mouse.x > 1)
            mouse.x--;
    }else{
        if(mouse.y > 1)
            mouse.y--;
    }
}

//function to move the mouse right or down
void onButtonB(MicroBitEvent) {
    if(movementState==LEFTRIGHT){
        if(mouse.x < 3)
            mouse.x++;
    }else{
        if(mouse.y<3)
            mouse.y++;
    }
}

//function to change direction of the movement
void onButtonAB(MicroBitEvent) {
    if(movementState==LEFTRIGHT){
        movementState=UPDOWN;
    }else{
        movementState=LEFTRIGHT;
    }
}


//start the laser in the top row
void topStart(){
    top.tick();
}

void leftStart(){
    left.tick();
}

void rightStart(){
    right.tick();
}

void bottomStart(){
    bottom.tick();
}

//display the score and reset view
void gameOver(){
    game_over = GAME_OVER;
    uBit.display.scroll("SCORE:");
    uBit.display.scroll(score);
    uBit.display.clear();
}





//reset all variables and keep the game running
void laoshu()
{
    game_over = GAME_ON;
    movementState = LEFTRIGHT;
    difficulty = EASY;

    bool bottomFlag = false;
    bool rightFlag = false;

    bottom.active = false;
    right.active = false;
    
    //mouse position
    mouse.x = 2;
    mouse.y = 2;

    score = 0;
    top.score = 0;

    //reset different lasers for each side
    top.pos.x = 1;
    top.pos.y = 0;

    left.pos.x = 0;
    left.pos.y = 3;

    right.pos.x = 4;
    right.pos.y = 3;

    bottom.pos.x = 3;
    bottom.pos.y = 4;


    //control movement of the mouse
    uBit.messageBus.listen(MICROBIT_ID_BUTTON_A, MICROBIT_BUTTON_EVT_CLICK, onButtonA);
    uBit.messageBus.listen(MICROBIT_ID_BUTTON_B, MICROBIT_BUTTON_EVT_CLICK, onButtonB);
    uBit.messageBus.listen(MICROBIT_ID_BUTTON_AB, MICROBIT_BUTTON_EVT_CLICK, onButtonAB);

    //generate the lasers
    create_fiber(topStart);
    create_fiber(leftStart);
    create_fiber(bottomStart);
    create_fiber(rightStart);

    
    
    //run match until player loses
    while(!game_over){
        uBit.sleep(150);
        score = top.score;

        //reset the pitch
        field.clear();

        //display the position of the mouse
        field.setPixelValue(mouse.x,mouse.y,LIGHT_STRONG);

        //display the position of the beams
        field.setPixelValue(top.pos.x,top.pos.y,LIGHT_LOW);
        field.setPixelValue(left.pos.x,left.pos.y,LIGHT_LOW);

        if(difficulty > EASY){
            field.setPixelValue(bottom.pos.x,bottom.pos.y,LIGHT_LOW);
        }
        if(difficulty == EXTREME){
            field.setPixelValue(right.pos.x,right.pos.y,LIGHT_LOW);
        }

        //make things harder once right score is reached
        if(score == 5 && bottomFlag == false){
            bottom.active = true;
            difficulty = DIFFICULT;
            bottomFlag = true;
        }
        if(score == 10 && rightFlag == false){
            right.active = true;
            difficulty = EXTREME;
            rightFlag = true;
        }
 
        //show all of the map
        uBit.display.image.paste(field);
    }

    //display score
    gameOver();
    
}

int main()
{
    uBit.init();

    //show the "title screen"
    intro();

    //keep the game running
    while(1){
        laoshu();
    }

    release_fiber();
}
