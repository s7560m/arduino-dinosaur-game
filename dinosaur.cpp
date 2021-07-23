#include <LiquidCrystal.h> 
int Contrast=100;
const int JUMP = 9;
const int NO = 10;
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);  

// Each element / item in the customChar array is a row of pixels
byte customChar[8] = {
  B01110,
  B01011,
  B01100,
  B01111,
  B11110,
  B11111,
  B01010,
  B01010
};

struct Coordinates {
	int x;
  	int y;
  	
  	// set x and y coords respectively
  	void set(int x, int y) {
      this->x = x;
      this->y = y;
  	}
  	
  	// this is going to be for the dino
  	// check if dino is at (1, 1)
  	// if it is return true, otherwise return false
  	bool isInitialPosition() {
      return this->x == 1 && this->y == 1;
  	}
  	
  	// this is going to be for all the cacti
  	// going to check if cactus is at (2, 1)
  	// if it is return true, otherwise return false
  	bool isCollision() {
      return this->x == 1 && this->y == 1;
  	}
};

// main character
class Dinosaur {
	private: 
  		byte customChar[8] = {
          B01110,
          B01011,
          B01100,
          B01111,
          B11110,
          B11111,
          B01010,
          B01010
        };
  		// coordinates of the dino
  		Coordinates coords;
  		int jumpCounter = 0;
  		bool jumped = false;
  	public:
  		// gonna be run in void setup()
  		// we want to initialize the custom character in the beginning
  		// before anything
  		void init() {
          
          	// initialize the dino custom char, set coords to (1, 1)
        	lcd.createChar(0, customChar);
          	this->coords.set(1, 1);
        }
  		void draw() {
        	lcd.setCursor(this->coords.x, this->coords.y);
          	lcd.write((byte)0);
          	jumpCounter++;
          
          	// have the dinosaur fall after the jumpCounter is
          	// greater than a specific amount
            if (this->jumpCounter > 4) {
              this->coords.y = 1;
              
              // jumped to false when dino touches the ground again
            }
        }
        void jump() {
          	/*
            
            Jump counter is always going to be incrementing 
            because we're always running draw(). When we jump,
            we want to reset the jump counter, and set the y
            to 0 (top line of LCD)
            
            In draw(), we can check when the jumpCounter is 
            greater than 4 before the dinosaur falls.
            
            */
          	jumpCounter = 0;
          	this->coords.y = 0;
          	this->jumped = true;
        }
  
  		// getter function for coords
		struct Coordinates getCoords() {
          return this->coords;
        }
  
  		// getter function to check if dino has jumped
  		bool hasJumped() {
          return this->jumped;
  		}
  
  		// check if touching the ground
  		bool isTouchingGround() {
          return this->coords.y == 1;
  		}
  		
  		// jumped to false
  		void setJumpedFalse() {
          this->jumped = false;
  		}
};

// cactus
/*

if the cactus is at (1, 1) {

	reset the position (15, 1)
    moveEnabled: false
    game counter is going to reset
    game counter is going to increment as long as game.run() is running
    game counter meets a specific threshold, then we can randomly spawn 
    a cactus, set moveEnabled to true
    
    the reason why we create this game counter is because we don't
    a cactus to spawn next to each other
    
    think of this game counter as a sort of "wait time" before we 
    randomly spawn a cactus
}

*/

class Cactus {
	private: 
  		byte customChar[8] = {
          B00100,
          B00100,
          B00101,
          B10101,
          B10111,
          B11100,
          B00100,
          B00100
        };
  
  		// default values for x, y of cactus
  		Coordinates coords;
  		//int x = 15;
  		//int y = 1;
  
  		// moveEnabled, when this is false, the cactus won't move or be drawn
  		bool moveEnabled = false;
  	public: 
        void init() {
			lcd.createChar(1, customChar);
          	this->coords.set(15, 1);
        }
  		void move() {
          	// each time this is called, x is going to go down
          	// by -1
          if (moveEnabled) {
        	this->coords.x--;
          }
        }
  		void draw() {
          if (moveEnabled) {
        	lcd.setCursor(this->coords.x, this->coords.y);
          	lcd.write((byte)1);
          }
        }
  		
  		// enable the cactus to move
  		void enable() {
        	this->moveEnabled = true;
        }
  
  		// reset the position of the cactus
  		void reset() {
          if (this->coords.x < 1) {
            
            // reset the cactus to the rightmost position
          	this->coords.x = 15;
            
            // set to false 
            this->moveEnabled = false;
          }
  		}
  		// getter function for coords
        struct Coordinates getCoords() {
          return this->coords;
        }
};

/* game.run() { 
	 gonna need a cactus array
	 iterate over every cactus, change their x positions 
	 constantly compare dinosaur's x position with every cactus's x position
   }
   game.stop() is going to run when the user collides wiht a cactus
   game.buttonIsPressed() { 
   	 dinosaur.jump() 
   }
*/

Dinosaur dino;

// number of cacti that can be drawn at once
Cactus cactus;
Cactus cactus2;
Cactus cactus3;
Cactus cactus4;

class Score {
	private:
  		int score = 0;
  
  		// Stringify score is going to fill up the digits of the score
        // so that way we can always write a consistent amount of digits for the score
        // when we print on the lcd
        String stringifyScore() {
          if (this->score < 10) {

            // if score is between 0 and 9
            return String("0000" + String(this->score));
          } else if (this->score < 100) {

            // btw 10 (inclusive) and 100 (exclusive)
            return String("000" + String(this->score));
          } else if (this->score < 1000) {

            // btw 100 and 1000
            return String("00" + String(this->score));
          } else if (this->score < 10000) {

            // btw 1000 and 10000
            return String("0" + String(this->score));
          } else {

            // anything after 10000 (inclusive)
            return String(this->score);
          }
        }
  	public:
  		// 00001 => 00010 => 00100 => 99999
  	
  	// when this is called it's going to increment score
  	// by one each time
  	void increment() {
      this->score++;
  	}
  
  	// print score out on LCD
  	void print() {
      lcd.setCursor(11, 0);
      
      // print out the stringified score at (10, 0)
      lcd.print(this->stringifyScore());
  	}
  
  	// reset score
  	void reset() {
      this->score = 0;
  	}
};

Score score;

class Game {
  	private:
  		int counter = 0;
	public:
  		bool lose = false;
  
  		void run() {
          
          // reset screen every frame
          lcd.clear();
          
          // print the score and increment the score respectively
          score.print();
          score.increment();
          
          // draw the dino
          dino.draw();
          
          // draw both cacti
          cactus.draw();
          cactus.move();
          
          cactus2.draw();
          cactus2.move();
          
          cactus3.draw();
          cactus3.move();
          
          //cactus4.draw();
          //cactus4.move();
          
          // reset cacti positions when it's out of bounds
          cactus.reset();
          cactus2.reset();
          cactus3.reset();
          //cactus4.reset();
          
          // always increment the counter
          counter++;
          
          // randomly spawn a cactus
          // counter = [0, 16, 32, 48]
          if (counter % 16 == 0) {
            
          	// TODO: implement a rand function so the cactus can randomly spawn
            // create a random number from 0-2
            if (random(2) == 1) { 
            	cactus.enable();
            }
          }
          
          // counter = [1, 17, 33]
          else if (counter % 16 == 2) {
            if (random(2) == 1) { 
            	cactus2.enable();
            }
          }
          
          // counter = [5, 21, 37]
          else if (counter % 16 == 5) {
            if (random(2) == 1) { 
            	cactus3.enable();
            }
          }
          
          // counter = [10, 26, 42]
          /*
          else if (counter % 16 == 10) {
            if (random(2) == 1) {
            	cactus4.enable();
            }
          }*/
          
          // check for when the push button is pressed. 
          // If it's pressed, dino.jump()
          // same thing as saying if (digitalRead(JUMP) == 1)
          if (digitalRead(JUMP) && !dino.hasJumped()) {
          	dino.jump();
          }
          
          // make sure the dino touches the ground first before we 
          // fire the jump code
          if (dino.isTouchingGround()) {
            dino.setJumpedFalse();
          }
          
          // constantly compare dino with all cacti positions    
          // first check if the dino is at its initial position (1, 1)     
          if (dino.getCoords().isInitialPosition()) {
          	
            // second check: if at least ONE of the cacti are 
            // at the collision position (2, 1)
            if (cactus.getCoords().isCollision() || cactus2.getCoords().isCollision() || cactus3.getCoords().isCollision() || cactus4.getCoords().isCollision()) {
            	Serial.println("You lose!");
              	this->lose = true;
            }
          }
          
          delay(200);
  		}
  
  		// run when dino dies
  		void stop() {
          String pressToContd = "Press btn to continue...";
          while(pressToContd.length() > 0){
            // clear the screen each frame
            lcd.clear();
            
            // print game over on the top line
            lcd.setCursor(0, 0);
          	lcd.print("Game Over!");
          	lcd.setCursor(0, 1);
            
            // scroll 
            lcd.print(pressToContd);
            pressToContd.remove(0, 1);
            delay(200);
            
            // if we press the button, continue the game
            if (digitalRead(JUMP)) {
              this->lose = false;
              break;
          	}
          }
  		}
};

Game game;

void setup()
{
	analogWrite(6,Contrast);
	lcd.begin(16, 2);
  	
  	// pushbuttons
  	pinMode(JUMP, INPUT);
  	pinMode(NO, INPUT);
  	
  	dino.init();
  	cactus.init();
  	cactus2.init();
  	cactus3.init();
  	//cactus4.init();	
  
  	Serial.begin(9600);
  
  	// initialize random seed
  	randomSeed(analogRead(0));	
} 
void loop() { 
  // if game.lose is true, then we can run game.stop()
  if (game.lose) {
    score.reset();
  	game.stop();
  } else {
  	game.run();
  }
}

