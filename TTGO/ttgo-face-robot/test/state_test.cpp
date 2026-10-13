#include "faces/FaceStyle.h"
#include "input/Button.h"
extern "C" int runTests() {
 FaceState s; s.tick(7999); if(s.expression!=Expression::Normal)return 1;
 s.tick(8000); if(s.expression!=Expression::Happy)return 2;
 s.automatic=false; s.tick(16000); if(s.expression!=Expression::Happy)return 3;
 s.expression=Expression::Mischievous; s.next(20000); if(s.expression!=Expression::Normal)return 4;
 s.since=0xfffffff0u; s.automatic=true; s.tick(0x00001f30u); if(s.expression!=Expression::Happy)return 5;
 s.since=0; if(s.openness(0)!=1||s.openness(4000)!=0||s.openness(4100)!=1)return 6;
 Button b; b.begin(false,0); b.update(true,10); b.update(true,40); b.update(false,100);
 if(b.update(false,130)!=ButtonEvent::Click)return 7;
 b.update(true,200); b.update(true,230); if(b.update(true,930)!=ButtonEvent::Hold)return 8;
 b.update(false,940); if(b.update(false,970)!=ButtonEvent::None)return 9;
 b.begin(true,1000); b.update(false,1010); if(b.update(false,1040)!=ButtonEvent::None)return 10;
 if(hasTears(Expression::Furious)||!hasTears(Expression::Crying)||tearOffset(1)!=10)return 11;
 if(static_cast<uint8_t>(Expression::Count)!=24)return 12;
 return 0;
}

