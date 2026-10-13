#include "display/Screen.h"
extern "C" void* memcpy(void* target,const void* source,unsigned long size){auto* a=(unsigned char*)target;auto* b=(const unsigned char*)source;for(unsigned long i=0;i<size;++i)a[i]=b[i];return target;}
extern "C" void* memset(void* target,int value,unsigned long size){auto* a=(unsigned char*)target;for(unsigned long i=0;i<size;++i)a[i]=value;return target;}
extern "C" int render(int scene){
  Pet pet; PetState state; state.ageMinutes=135;state.careMinutes=45;state.lifeStage=1;
  View view=View::Home;uint8_t focus=0;
  switch(scene){
    case 1:view=View::Care;break;
    case 2:view=View::Stats;break;
    case 3:view=View::Objectives;state.feedsToday=1;state.playsToday=2;state.restToday=3;break;
    case 4:view=View::Customize;state.name=2;state.color=3;state.owned=3;state.accessory=1;break;
    case 5:view=View::Shop;focus=3;break;
    case 6:state.sleeping=true;break;
    case 11:state.food=10;break;
    case 12:state.hygiene=10;break;
    case 13:state.health=10;break;
    case 14:state.joy=10;break;
    case 15:state.energy=10;break;
    case 16:state.ageMinutes=0;state.careMinutes=0;state.lifeStage=0;break;
    case 17:state.ageMinutes=60;state.careMinutes=20;break;
    case 18:state.ageMinutes=240;state.careMinutes=90;state.lifeStage=2;state.adultForm=1;state.bond=80;break;
  }
  if(!pet.restore(state))return 1;
  Screen screen;screen.begin();
  if(scene==7)screen.animate(Action::Feed,0);
  if(scene==8)screen.animate(Action::Play,0);
  if(scene==9)screen.animate(Action::Clean,0);
  if(scene==10)screen.animate(Action::Heal,0);
  screen.draw(pet,view,0,focus,"",420);return 0;
}
