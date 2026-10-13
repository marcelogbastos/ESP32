#pragma once
#include <stdint.h>
#include <stddef.h>
namespace std {
 template<class T> class initializer_list {
  const T* first_; size_t count_;
 public:
  constexpr initializer_list(const T* first, size_t count):first_(first),count_(count){}
  constexpr const T* begin() const{return first_;}
  constexpr const T* end() const{return first_+count_;}
 };
}
#define OUTPUT 1
#define HIGH 1
#define TFT_BLACK 0x0000
#define TFT_WHITE 0xffff
#define TFT_RED 0xf800
#define TFT_GREEN 0x07e0
#define TFT_BLUE 0x001f
#define TFT_YELLOW 0xffe0
#define TFT_PINK 0xfe19
#define TFT_SKYBLUE 0x867d
#define TFT_ORANGE 0xfda0
#define TFT_VIOLET 0x915c
#define TFT_BROWN 0x9a60
#define TFT_CYAN 0x07ff
inline void pinMode(int,int){} inline void digitalWrite(int,int){}
inline void ledcSetup(int,int,int){} inline void ledcAttachPin(int,int){} inline void ledcWrite(int,int){}
class String {
 public:
  char data[128] = {};
  String(const char* input="") { unsigned i=0; while(input && input[i] && i<127){data[i]=input[i];++i;} data[i]=0; }
  String(int value){number(value<0?uint64_t(-int64_t(value)):value,value<0);}
  String(unsigned value){number(value,false);} String(long value){number(value<0?uint64_t(-int64_t(value)):value,value<0);}
  String(unsigned long value){number(value,false);}
 private:
  void number(uint64_t value,bool negative){char reversed[32];int n=0;do{reversed[n++]=char('0'+value%10);value/=10;}while(value);int p=0;if(negative)data[p++]='-';while(n)data[p++]=reversed[--n];data[p]=0;}
};
inline String operator+(const String& a,const String& b){String out(a);unsigned n=0;while(out.data[n])++n;unsigned p=0;while(b.data[p]&&n<127)out.data[n++]=b.data[p++];out.data[n]=0;return out;}
extern "C" void doc_emit(int op,int x,int y,int a,int b,int c,int d,int color,const char* text);
class TFT_eSPI {
 public:
  void init(){} void setRotation(int){} void fillScreen(int){} void setTextColor(int){} void println(const char*){}
};
class TFT_eSprite {
  int ox=0,oy=0,color=0xffff;
  void emit(int op,int x=0,int y=0,int a=0,int b=0,int c=0,int d=0,int ink=0,const char* text=nullptr){doc_emit(op,x+ox,y+oy,a,b,c,d,ink,text);}
 public:
  TFT_eSprite(TFT_eSPI*){} void setColorDepth(int){} void* createSprite(int,int){return this;}
  void setTextColor(int ink){color=ink;}
  void fillSprite(int ink){emit(0,0,0,240,135,0,0,ink);}
  void setViewport(int x,int y,int w,int h,bool){ox=x;oy=y;doc_emit(20,x,y,w,h,0,0,0,nullptr);}
  void resetViewport(){ox=oy=0;doc_emit(21,0,0,0,0,0,0,0,nullptr);}
  void pushSprite(int,int){}
  void drawString(const String& text,int x,int y,int font){emit(7,x,y,font,0,0,0,color,text.data);}
  void drawCentreString(const String& text,int x,int y,int font){emit(7,x,y,font,1,0,0,color,text.data);}
  void fillRect(int x,int y,int w,int h,int ink){emit(1,x,y,w,h,0,0,ink);}
  void fillCircle(int x,int y,int r,int ink){emit(2,x,y,r,0,0,0,ink);}
  void fillEllipse(int x,int y,int rx,int ry,int ink){emit(3,x,y,rx,ry,0,0,ink);}
  void fillRoundRect(int x,int y,int w,int h,int r,int ink){emit(4,x,y,w,h,r,0,ink);}
  void fillTriangle(int x,int y,int x2,int y2,int x3,int y3,int ink){emit(5,x,y,x2+ox,y2+oy,x3+ox,y3+oy,ink);}
  void drawLine(int x,int y,int x2,int y2,int ink){emit(6,x,y,x2+ox,y2+oy,0,0,ink);}
  void drawFastHLine(int x,int y,int w,int ink){drawLine(x,y,x+w-1,y,ink);}
  void drawFastVLine(int x,int y,int h,int ink){drawLine(x,y,x,y+h-1,ink);}
  void drawPixel(int x,int y,int ink){emit(8,x,y,0,0,0,0,ink);}
  void drawCircle(int x,int y,int r,int ink){emit(9,x,y,r,0,0,0,ink);}
  void drawRoundRect(int x,int y,int w,int h,int r,int ink){emit(10,x,y,w,h,r,0,ink);}
};
