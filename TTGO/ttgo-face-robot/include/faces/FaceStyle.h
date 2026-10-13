#pragma once
#include "faces/FaceState.h"
enum class Mouth : uint8_t { Flat, Smile, Sad, Open, Tiny, Laugh };
struct FaceStyle { uint8_t height; float leftSlope, rightSlope; Mouth mouth; int8_t gazeX, gazeY; };
static const FaceStyle faceStyles[] = {
 {48,0,0,Mouth::Flat,0,0}, {30,0,0,Mouth::Smile,0,0},
 {18,0,0,Mouth::Smile,0,0}, {14,0,0,Mouth::Laugh,0,0},
 {36,-1,1,Mouth::Sad,0,5}, {32,-1,1,Mouth::Sad,0,5},
 {37,1,-1,Mouth::Flat,0,0}, {24,1,-1,Mouth::Sad,0,0},
 {58,0,0,Mouth::Open,0,0}, {56,-.3f,.3f,Mouth::Open,0,0},
 {19,0,0,Mouth::Flat,0,5}, {5,0,0,Mouth::Tiny,0,0},
 {48,0,0,Mouth::Smile,0,0}, {48,0,.4f,Mouth::Tiny,7,-5},
 {38,0,-.4f,Mouth::Flat,-7,-6}, {22,0,0,Mouth::Flat,8,0},
 {44,-.5f,-.5f,Mouth::Tiny,-4,0}, {38,0,0,Mouth::Smile,0,6},
 {28,0,0,Mouth::Smile,0,0}, {42,0,0,Mouth::Laugh,0,0},
 {17,0,0,Mouth::Flat,0,3}, {28,.4f,-.4f,Mouth::Flat,0,-2},
 {38,-.7f,.7f,Mouth::Sad,0,0}, {28,.7f,-.7f,Mouth::Smile,6,0}
};
static_assert(sizeof(faceStyles)/sizeof(faceStyles[0]) == static_cast<uint8_t>(Expression::Count), "Every expression needs a style");
inline bool hasTears(Expression expression) { return expression == Expression::Crying; }
inline int tearOffset(uint8_t eye) { return eye == 1 ? 10 : 22; }
