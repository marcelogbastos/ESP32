#include "faces/FaceRenderer.h"
#include <math.h>
void FaceRenderer::heart(int x, int y, int s, uint16_t c) {
  f.fillCircle(x-s/2,y-s/3,s/2,c); f.fillCircle(x+s/2,y-s/3,s/2,c);
  f.fillTriangle(x-s,y-s/4,x+s,y-s/4,x,y+s,c);
}
void FaceRenderer::star(int x,int y,int r,float angle,uint16_t c) {
  int px[10],py[10];
  for(int i=0;i<10;++i) { float a=angle+i*0.62831853f; float rad=i%2?r*0.43f:r; px[i]=x+cosf(a)*rad; py[i]=y+sinf(a)*rad; }
  for(int i=0;i<10;++i) f.fillTriangle(x,y,px[i],py[i],px[(i+1)%10],py[(i+1)%10],c);
}
void FaceRenderer::smile(int y,bool sad,uint16_t c) {
  for(int x=-22;x<=22;++x) {
    int curve=(484-x*x)/55;
    f.fillCircle(120+x,y+(sad?-curve:curve),2,c);
  }
}
void FaceRenderer::draw(const FaceState& s,uint32_t now) {
  const uint16_t colors[]={TFT_CYAN,TFT_GREEN,TFT_WHITE,TFT_ORANGE,TFT_VIOLET};
  uint16_t c=colors[s.palette%5];
  float t=s.age(now)/1000.0f;
  int bob=2*sinf(t*2.5f), gaze=6*sinf(t*0.8f);
  // Brief opening on entry, with natural breathing and blinking afterwards.
  float enter=s.age(now)<250?s.age(now)/250.0f:1.0f;
  float open=s.openness(now)*enter;
  f.fillSprite(TFT_BLACK);
  for(int i=0;i<2;++i) {
    int x=i?170:70, y=57+bob;
    switch(s.expression) {
      case Expression::Love: heart(x,y,19+2*sinf(t*7),c); break;
      case Expression::Stars: star(x,y,27,t*0.5f-1.57f,c); break;
      case Expression::Error:
        for(int k=-2;k<=2;++k) { f.drawLine(x-20,y-20+k,x+20,y+20+k,c); f.drawLine(x-20,y+20+k,x+20,y-20+k,c); } break;
      case Expression::Sleep:
        f.fillRoundRect(x-26,y+6,52,5,2,c); break;
      case Expression::Happy:
      case Expression::Laugh:
        f.fillEllipse(x,y,27,18*open+3,c); f.fillRect(x-28,y+2,57,25,TFT_BLACK); break;
      case Expression::Cool:
        f.fillRoundRect(x-29,y-15,58,32,5,c); f.drawLine(x-20,y-10,x-5,y+6,TFT_BLACK);
        if(i==0) f.fillRect(99,y-9,42,5,c); break;
      default: {
        int h=(s.expression==Expression::Surprise?57:s.expression==Expression::Sad?38:50)*open+3;
        if(s.expression==Expression::Wink && i==1 && uint32_t(s.age(now)%2200)>1600) h=5;
        f.fillRoundRect(x-27,y-h/2,54,h,h>20?18:2,c);
        if(s.expression==Expression::Angry) {
          if(i==0) f.fillTriangle(x-29,y-30,x+29,y-30,x+29,y-3,TFT_BLACK);
          else f.fillTriangle(x-29,y-30,x+29,y-30,x-29,y-3,TFT_BLACK);
        }
        if(s.expression==Expression::Sad) {
          if(i==0) f.fillTriangle(x-29,y-30,x+29,y-30,x-29,y-7,TFT_BLACK);
          else f.fillTriangle(x-29,y-30,x+29,y-30,x+29,y-7,TFT_BLACK);
          int drop=76+int(s.age(now)%1400)*28/1400;
          f.fillCircle(x+22,drop,3,TFT_BLUE);
        }
        if(s.pupils && h>24) { f.fillCircle(x+gaze,y+3,8,TFT_BLACK); f.fillCircle(x+gaze-3,y,2,TFT_WHITE); }
      } break;
    }
  }
  switch(s.expression) {
    case Expression::Surprise: f.fillEllipse(120,101,10,14,c); f.fillEllipse(120,101,5,8,TFT_BLACK); break;
    case Expression::Laugh: f.fillEllipse(120,103,23,14,c); f.fillRect(95,86,50,15,TFT_BLACK); break;
    case Expression::Love: heart(120,103,8,c); break;
    case Expression::Sad: smile(109,true,c); break;
    case Expression::Angry: f.fillRoundRect(103,102,34,4,2,c); break;
    case Expression::Sleep:
      f.fillCircle(120,104,4+2*sinf(t*2),c);
      f.setTextColor(c,TFT_BLACK); f.drawString("z",200,25-int(t*8)%18,2); break;
    case Expression::Error: f.fillRect(104,102,32,4,c); break;
    case Expression::Normal: f.fillRoundRect(108,101,24,4,2,c); break;
    default: smile(99,false,c); break;
  }
  f.pushSprite(0,0);
}
