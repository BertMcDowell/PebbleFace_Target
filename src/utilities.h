#pragma once

#include <pebble.h>

void layer_position_from_angle_distance_set(Layer * pLayer, const GPoint * pCenter, int32_t angle, int32_t distance);
void bitmap_layer_position_from_angle_distance_set(BitmapLayer * bimpLayer, const GPoint * pCenter, int32_t angle, int32_t distance);
void rot_bitmap_layer_position_from_angle_distance_set(RotBitmapLayer * bimpLayer, const GPoint * pCenter, int32_t angle, int32_t distance);