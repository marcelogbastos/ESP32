#include "faces/FaceRenderer.h"
#include <math.h>

void FaceRenderer::curve(int x0,int y0,int cx,int cy,int x1,int y1,uint16_t c) {
  for(int i=0;i<=40;++i) {
    float t=i/40.0f,u=1-t;
    f.fillCircle(u*u*x0+2*u*t*cx+t*t*x1,u*u*y0+2*u*t*cy+t*t*y1,2,c);
  }
}
void FaceRenderer::draw(const FaceState& state,uint32_t now) {
  const uint16_t colors[]={TFT_CYAN,TFT_GREEN,TFT_WHITE,TFT_ORANGE,TFT_VIOLET};
  const uint16_t color=colors[state.palette%5];
  const FaceStyle& style=faceStyles[static_cast<uint8_t>(state.expression)];
  const uint32_t age=state.age(now);
  const float time=age/1000.0f;
  const int bob=roundf(sinf(time*2)*1.6f);
  const float blink=state.expression==Expression::Sleep?1:state.openness(now);
  f.fillSprite(TFT_BLACK);
  for(uint8_t eye=0;eye<2;++eye) {
    const int x=eye?168:72,y=57+bob;
    int height=style.height*blink;
    if(state.expression==Expression::Wink && eye==1 && age%2400>1500) height=5;
    if(state.expression==Expression::Curious && eye==1) height=height*.72f;
    if(height<2) height=2;
    const int top=y-height/2, bottom=top+height-1;
    const float slope=eye?style.rightSlope:style.leftSlope;
    const int radius=height/2<16?height/2:16;
    const int pupilX=x+style.gazeX+roundf(sinf(time*.7f)*4),pupilY=y+style.gazeY+roundf(sinf(time*.43f)*1.5f);
    const float pupilRadiusY=height<22?(height-4)/2.0f:9.0f;
    // Rasterize the shared rounded eye, clipping eyelids and optional pupils to its silhouette.
    for(int row=top;row<=bottom;++row) {
      int inset=0;
      int edge=row-top<bottom-row?row-top:bottom-row;
      if(edge<radius) { float dy=radius-edge-.5f; inset=ceilf(radius-sqrtf(radius*radius-dy*dy)); }
      int run=-1;
      for(int col=x-26;col<=x+26;++col) {
        bool lit=col<x+26 && col>=x-26+inset && col<x+26-inset;
        float leftCut=slope>0?slope*18:0,rightCut=slope<0?-slope*18:0;
        float lid=top+leftCut+(rightCut-leftCut)*(col-(x-30))/60.0f;
        if(slope && row<lid) lit=false;
        if(state.pupils && height>14 && lit) {
          float dx=(col-pupilX)/7.0f,dy=(row-pupilY)/pupilRadiusY;
          if(dx*dx+dy*dy<=1) {
            lit=false;
            // Small reflection, clipped to the pupil and eyelid like the dark pupil itself.
            float hx=col-(pupilX-2),hy=row-(pupilY-pupilRadiusY*.35f);
            if(hx*hx+hy*hy<=2.25f) f.drawPixel(col,row,TFT_WHITE);
          }
        }
        if(lit && run<0) run=col;
        if(!lit && run>=0) { f.drawFastHLine(run,row,col-run,color); run=-1; }
      }
    }
    if(hasTears(state.expression)) {
      int drop=(age%1400)*19/1400;
      f.fillEllipse(x+tearOffset(eye),83+drop,3,4,color);
    }
  }
  switch(style.mouth) {
    case Mouth::Flat: f.fillRoundRect(108,99,24,4,2,color); break;
    case Mouth::Tiny: f.fillRoundRect(114,101,12,4,2,color); break;
    case Mouth::Smile: curve(106,99,120,112,134,99,color); break;
    case Mouth::Sad: curve(106,106,120,95,134,106,color); break;
    case Mouth::Open:
      f.fillEllipse(120,103,9,11,color); f.fillEllipse(120,103,5,7,TFT_BLACK); break;
    case Mouth::Laugh:
      f.fillRoundRect(104,97,32,4,2,color);
      curve(134,99,120,120,106,99,color); break;
  }
  f.pushSprite(0,0);
}

