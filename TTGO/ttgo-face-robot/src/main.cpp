#include <Arduino.h>
#include <TFT_eSPI.h>
#include "input/Button.h"
#include "faces/FaceRenderer.h"
namespace {
TFT_eSPI display;
TFT_eSprite frame(&display);
FaceRenderer renderer(frame);
FaceState state;
Button navigation, action;
bool ready=false;
uint32_t lastFrame=0;
const char* names[]={"Neutro","Feliz","Muito feliz","Rindo","Bravo","Furioso","Triste","Chorando","Surpreso","Assustado","Sonolento","Dormindo","Piscando","Curioso","Pensando","Desconfiado","Confuso","Timido","Carinhoso","Animado","Entediado","Concentrado","Preocupado","Travesso"};
static_assert(sizeof(names)/sizeof(names[0]) == static_cast<uint8_t>(Expression::Count), "Every expression needs a name");
void report() { Serial.printf("Face: %s / automatico=%s / paleta=%u / pupilas=%s\n",names[static_cast<uint8_t>(state.expression)],state.automatic?"sim":"nao",state.palette,state.pupils?"sim":"nao"); }
}
void setup() {
  Serial.begin(115200);
  pinMode(35,INPUT); pinMode(0,INPUT_PULLUP);
  navigation.begin(digitalRead(35)==LOW,millis()); action.begin(digitalRead(0)==LOW,millis());
  display.init(); display.setRotation(1); pinMode(4,OUTPUT); digitalWrite(4,HIGH);
  frame.setColorDepth(8); ready=frame.createSprite(240,135)!=nullptr;
  state.since=millis();
  Serial.printf("TTGO Face Robot 1.1.1 / ESP32 classico / sprite=%s\n",ready?"OK":"FAIL");
  if(!ready) { display.fillScreen(TFT_BLACK); display.setTextColor(TFT_RED); display.drawString("Falha de memoria",10,55,2); }
  report();
}
void loop() {
  uint32_t now=millis();
  ButtonEvent left=navigation.update(digitalRead(35)==LOW,now);
  ButtonEvent right=action.update(digitalRead(0)==LOW,now);
  if(left==ButtonEvent::Click) { state.automatic=false; state.next(now); report(); }
  if(left==ButtonEvent::Hold) { state.automatic=!state.automatic; state.since=now; report(); }
  if(right==ButtonEvent::Click) { state.palette=(state.palette+1)%5; report(); }
  if(right==ButtonEvent::Hold) { state.pupils=!state.pupils; report(); }
  Expression before=state.expression; state.tick(now);
  if(before!=state.expression) report();
  if(ready && uint32_t(now-lastFrame)>=33) { lastFrame=now; renderer.draw(state,now); }
  delay(1);
}


