#include <pebble.h>
#include "maths_ext.h"
  
#define round(x) ((x)>=0?(int)((x)+0.5f):(int)((x)-0.5f))
  
bool layer_position_from_angle_distance_set(Layer * pLayer, const GPoint * pCenter, int32_t angle, int32_t distance)
{
  float mx= TRIG_MAX_RATIO;
  float dist= distance;
  int x= float_round((sin_lookup(angle) / mx) * dist);
  int y= float_round((-cos_lookup(angle) / mx) * dist);
  GPoint pos = { pCenter->x+x, pCenter->y+y };
  GRect rect = layer_get_frame(pLayer);
  GPoint current = grect_center_point(&rect);
  if (!gpoint_equal(&pos,&current))
  {
    layer_set_frame(pLayer, grect_center_point_set(&rect, &pos));
    layer_mark_dirty(pLayer);
    return true;
  }
  return false;
}

void bitmap_layer_position_from_angle_distance_set(BitmapLayer * bimpLayer, const GPoint * pCenter, int32_t angle, int32_t distance)
{
  layer_position_from_angle_distance_set(bitmap_layer_get_layer(bimpLayer), pCenter, angle, distance);  
}

void rot_bitmap_layer_position_from_angle_distance_set(RotBitmapLayer * bimpLayer, const GPoint * pCenter, int32_t angle, int32_t distance)
{
  rot_bitmap_layer_set_angle(bimpLayer, angle);
  Layer * pLayer = (Layer*)(bimpLayer);
  layer_position_from_angle_distance_set(pLayer, pCenter, angle, distance);  
}