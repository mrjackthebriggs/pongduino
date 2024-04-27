#include <U8g2lib.h>
//#include "pongdisplay.ino"
U8G2_SH1106_128X64_NONAME_F_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);  //11 mosi, 13 clock

// Game Values


//Ball Physics
float ballXPos{15};
float ballYPos{32};
bool movingRight = true; // Expresses ball direction
int16_t ballAngle{};
float ballXSpeed{1.0};
float ballYSpeed{0.5};
float ballYSpeedMultip{1.0};
int8_t hitCounter{};
bool ballPaused = true;
unsigned short pauseCounter {};
bool gameOver = false;

//Player Values
int8_t player1Pos {};
int8_t player2Pos {};
int8_t player1Score {};
int8_t player2Score {};
int8_t winScore{};
int8_t winPlayer{};

//Mechanics Flags...Make sure to add notif
enum class BallBounceMechanics
{
  Classic,
  Barely,
  Light,
  Heavy
};
BallBounceMechanics ballMech{BallBounceMechanics::Classic};

enum class BallSpeed
{
  Low,
  Med,
  High,
  Faster
};
BallSpeed ballSpeed{BallSpeed::Low};



void config()
{
  u8g2.setDisplayRotation(U8G2_R2);
}

//Gets player input, takes 10 to make up for height of paddle
int8_t getPlayerInputs()
{
  int16_t player1Inp = analogRead(1);
  int16_t player2Inp = analogRead(2);
  player1Pos = map(player1Inp, 0, 1024, 0, 54);
  player2Pos = map(player2Inp, 0, 1024, 0, 54);

  //switches, updates values to current pos of switch
  ballMech = (BallBounceMechanics)fourpoleselsw(1);
  ballSpeed = (BallSpeed)fourpoleselsw(2);
}


void ballHBounce()
{
  // Checks to see if at same pos as paddle in certain x co-ord.
  //Calculate new call Angle if true.
    //Change Direction
    movingRight = !movingRight;
    
    //Change Angle
    ballAngle = analogRead(6)%110;

    switch(ballMech)
    {
      case(BallBounceMechanics::Classic):
      {
        if(ballYPos > 23 && ballYPos < 41)
          ballYSpeed = (sin(ballAngle)) * ballYSpeedMultip;
        else
          ballYSpeed = (sin(ballAngle)/2) * ballYSpeedMultip;;
      }break;
      case(BallBounceMechanics::Barely):
      {
        ballYSpeed = (sin(ballAngle)/4) * ballYSpeedMultip;; 
      }break;
      case(BallBounceMechanics::Light):
      {
        ballYSpeed = (sin(ballAngle)/2) * ballYSpeedMultip;; 
      }break;
      case(BallBounceMechanics::Heavy):
      {
        ballYSpeed = (sin(ballAngle)*2) * ballYSpeedMultip;; 
      }break;
    }
  Serial.println(ballYSpeed);
  
  checkBallSpeed();
}


void ballPhysics()
{
  if(!ballPaused)
  {
    if (ballYPos > 60 | ballYPos < 4)
      ballYSpeed = -ballYSpeed;

    // Ball movement delta
    if (movingRight)
    {
      ballXPos += ballXSpeed;
    }
    else
    {
      ballXPos -= ballXSpeed;
    }
    ballYPos += ballYSpeed;
  }
  else
  {
    pauseCounter++;

    if(pauseCounter > 200)
    {
      ballPaused = false;
      pauseCounter = 0;
    }
  }
}


void paddleCollision()
{
  //Player 1 Colision
  if(round(ballXPos) == 6 && (ballYPos > player1Pos - 1 & ballYPos < player1Pos + 11))
  {
    ballHBounce();
    tone(5,3000,70);
  }
  //Player 2 Colision
  else if(round(ballXPos) == 119  && (ballYPos > player2Pos - 1 & ballYPos < player2Pos + 11))
  {
    ballHBounce();
    tone(5,2100,70);
  }
}


void checkBallSpeed()
{
   // Ball Speed
  switch(ballSpeed)
    {
      case(BallSpeed::Low):
      {
        ballXSpeed = 0.6;
        ballYSpeedMultip = 0.8;
      }break;
      case(BallSpeed::Med):
      {
        ballXSpeed = 1.0;
        ballYSpeedMultip = 1;
      }break;
      case(BallSpeed::High):
      {
        ballXSpeed = 1.2;
        ballYSpeedMultip = 1.2;
      }break;
      case(BallSpeed::Faster):
      {
        ballXSpeed += 0.1;
        ballYSpeedMultip += 0.1;
      }break;
    }
}


void scoreManagement()
{
  if(ballXPos < -10)
  {
    player2Score++;
    ballPaused = true;
    ballXPos = 15;
    ballYPos = 32;
    ballHBounce();
  }
  else if(ballXPos > 138)
  {
    player1Score++;  
    ballPaused = true;
    ballXPos = 110;
    ballYPos = 32;
    ballHBounce();
  } 

  if(player1Score > winScore)
  {
    winPlayer = 1;
    gameOver = true;
  }
  else if(player2Score > winScore)
  {
    winPlayer = 2;
    gameOver = true;
  }
}


void draw()
{
  net();
  scoreone(player1Score);
  scoretwo(player2Score);
  swstat((int8_t)ballMech, (int8_t)ballSpeed);
  paddleone(player1Pos);
  paddletwo(player2Pos);
  ball(ballXPos, ballYPos);
}


//MAIN FUNCS
void setup()
{
  pinMode(5,OUTPUT);
  u8g2.begin();
  Serial.begin(9600);
  winScore = 2 * dipsw();
}


void loop()
{
  u8g2.clearBuffer();

  //Game Loop here
  config();

  if(!gameOver)
  {
    getPlayerInputs();
    paddleCollision();
    scoreManagement();
    ballPhysics();
    draw();
  }
  else
    gameOverScreen(winPlayer);
    
  u8g2.sendBuffer();

  delay(4);
}