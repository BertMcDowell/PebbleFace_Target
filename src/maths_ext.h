#pragma once
  
#include <pebble.h>

int float_round(float x)
{   
  return ((x)>=0?(int)((x)+0.45f):(int)((x)-0.45f));
}

GRect grect_center_point_set(GRect * rect, const GPoint * point)
{
  rect->origin.x = float_round(point->x - (rect->size.w * 0.5f));
  rect->origin.y = float_round(point->y - (rect->size.h * 0.5f));
  return *rect;
};