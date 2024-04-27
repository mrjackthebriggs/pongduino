#include <U8g2lib.h>

//U8G2_SH1106_128X64_NONAME_F_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);


void paddleone(int8_t& y_pos)
{
  u8g2.drawBox(5,y_pos, 2, 10);
}


void paddletwo(int8_t& y_pos)
{
  u8g2.drawBox(120, y_pos , 2, 10);
}


void ball(float& xpos, float& ypos)
{
  u8g2.drawDisc(round(xpos), round(ypos),1);
}


void scoreone(int8_t score)
{
  if(score < 9)
  {
    u8g2.setFont(u8g2_font_5x7_tf);
    u8g2.drawStr(54,8,String(score).c_str());
  }
  else
  {
    u8g2.setFont(u8g2_font_5x7_tf);
    u8g2.drawStr(48,8,String(score).c_str());
  }
}


void scoretwo(int8_t score)
{
  u8g2.setFont(u8g2_font_5x7_tf);
  u8g2.drawStr(74,8,String(score).c_str());
}


void net()
{
  u8g2.drawLine(64,6,64,58);
}


void gameOverScreen(int8_t winner)
{
  String str{};
  str.concat("Player ");
  str.concat(String(winner));
  str.concat(" wins");
  u8g2.setFont(u8g2_font_6x10_tf);
  u8g2.drawStr(24,52, str.c_str());
  u8g2.setDrawColor(0);
  u8g2.setFont(u8g2_font_10x20_mr);
  u8g2.drawStr(20,32, "Game Over");
  u8g2.setDrawColor(1);
}

void swdebug(int8_t swin1, int8_t swin2)
{
  u8g2.setFont(u8g2_font_4x6_tf);
  u8g2.drawStr(100,11,String(swin2).c_str());

  u8g2.drawStr(11,11,String(swin1).c_str());
}

void swstat(int8_t swin1, int8_t swin2)
{
  if(swin1 == 1) 
    u8g2.drawStr(62,4,">");
  else if(swin1 == 2)
    u8g2.drawStr(60,4,">>");
  else if(swin1 == 3)
    u8g2.drawStr(58,4,">>>");
  else if(swin1 == 4)
    u8g2.drawStr(62,6,"^");

  
  if(swin2 == 1) 
    u8g2.drawStr(62,64,"C");
  else if(swin2 == 2)
    u8g2.drawStr(62,64,"L");
  else if(swin2 == 3)
    u8g2.drawStr(62,64,"M");
  else if(swin2 == 4)
    u8g2.drawStr(62,64,"H");
}