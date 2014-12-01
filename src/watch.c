#include <pebble.h>
#include "watch.h"
#include "utilities.h"
#include "graphics.h"

#define USER_ROT_BITMAP
//#define USE_TEST_SECONDS
  
typedef enum {
  _hand_positions_INSIDE = 0,
  _hand_positions_OUTSIDE,
  k_number_hand_positions,
} e_hand_positions;

int32_t g_hand_position_distances[k_number_hand_positions] = { 34, 54 };
GPoint g_center= {72, 84};

const int IMAGE_RESOURCE_IDS_BACKGROUNDS[] = {
  RESOURCE_ID_IMAGE_BACKGROUND_GUIDE,
  RESOURCE_ID_IMAGE_BACKGROUND_PLANE,
  RESOURCE_ID_IMAGE_BACKGROUND_PRIMARY,
  RESOURCE_ID_IMAGE_BACKGROUND_PRIMARY_SECONDARY,
  RESOURCE_ID_IMAGE_BACKGROUND_PRIMARY_SECONDARY_MINOR,
};

const int IMAGE_RESOURCE_IDS_HANDS[] = {
  RESOURCE_ID_IMAGE_PT_DOT,
  RESOURCE_ID_IMAGE_PT_LINE,
};

// Function Forward Declaration
static void handle_tick(struct tm *tick_time, TimeUnits units_changed);
static void handle_update(void);
  
// Variable Declaration
static GBitmap *s_background_bitmap= NULL;
static GBitmap *s_point_bitmap= NULL;

static BitmapLayer *s_background_layer= NULL;

#if defined(USER_ROT_BITMAP)
static RotBitmapLayer *s_hour_point_layer= NULL;
static RotBitmapLayer *s_min_point_layer= NULL;
#else 
static BitmapLayer *s_hour_point_layer= NULL;
static BitmapLayer *s_min_point_layer= NULL;
#endif

void handle_main_window_load(Window *window) {
  // Create GBitmap, then set to created BitmapLayer
  s_background_bitmap = gbitmap_create_with_resource(IMAGE_RESOURCE_IDS_BACKGROUNDS[4]);
  s_point_bitmap = gbitmap_create_with_resource(IMAGE_RESOURCE_IDS_HANDS[1]);
  
  s_background_layer = bitmap_layer_create(GRect(0, 12, 144, 144));
  bitmap_layer_set_bitmap(s_background_layer, s_background_bitmap);

#if defined(USER_ROT_BITMAP)
  s_hour_point_layer = rot_bitmap_layer_create(s_point_bitmap);
  rot_bitmap_set_compositing_mode(s_hour_point_layer, GCompOpAnd);
  
  s_min_point_layer = rot_bitmap_layer_create(s_point_bitmap);
  rot_bitmap_set_compositing_mode(s_min_point_layer, GCompOpAnd);
#else
  GRect rec= {{0, 0}, {16, 16}};
  s_hour_point_layer = bitmap_layer_create(rec);
  bitmap_layer_set_bitmap(s_hour_point_layer, s_point_bitmap);
  bitmap_layer_set_compositing_mode(s_hour_point_layer, GCompOpAnd);
  
  s_min_point_layer = bitmap_layer_create(rec);
  bitmap_layer_set_bitmap(s_min_point_layer, s_point_bitmap);
  bitmap_layer_set_compositing_mode(s_min_point_layer, GCompOpAnd);
#endif

  // Update the time now
  handle_update();
  
  Layer * root_layer = window_get_root_layer(window);
  
  // Add it as a child layer to the Window's root layer
  layer_add_child(root_layer, bitmap_layer_get_layer(s_background_layer));
  
#if defined(USER_ROT_BITMAP)
  layer_add_child(root_layer, (Layer*)(s_hour_point_layer));
  layer_add_child(root_layer, (Layer*)(s_min_point_layer));
#else
  layer_add_child(root_layer, bitmap_layer_get_layer(s_hour_point_layer));
  layer_add_child(root_layer, bitmap_layer_get_layer(s_min_point_layer));
#endif
  
  add_invert(root_layer);
  
  // Register with TickTimerService
#if defined(USE_TEST_SECONDS)
  tick_timer_service_subscribe(SECOND_UNIT, handle_tick);
#else
  tick_timer_service_subscribe(MINUTE_UNIT, handle_tick);
#endif
}

void handle_main_window_unload(Window *window) {
  // UnRegister with TickTimerService
  tick_timer_service_unsubscribe();
  
  remove_invert();
  
  // Destroy BitmapLayer
  bitmap_layer_destroy(s_background_layer);
  
#if defined(USER_ROT_BITMAP)
  rot_bitmap_layer_destroy(s_hour_point_layer);
  rot_bitmap_layer_destroy(s_min_point_layer);
#else
  bitmap_layer_destroy(s_hour_point_layer);
  bitmap_layer_destroy(s_min_point_layer);
#endif
  
  // Destroy GBitmap
  gbitmap_destroy(s_background_bitmap);
  gbitmap_destroy(s_point_bitmap);
}

void handle_tick(struct tm *tick_time, TimeUnits units_changed) {
  handle_update();
}

void handle_update(void) {
  // Get a tm structure
  time_t temp = time(NULL); 
  struct tm *tick_time = localtime(&temp);

#if defined(USE_TEST_SECONDS)
  int32_t angle_sec = (tick_time->tm_sec / 60.0f) * TRIG_MAX_ANGLE;
  int32_t angle_hour = angle_sec;
  int32_t angle_min = angle_sec;
#else
  float percentage_hours = ((tick_time->tm_hour%12) / 12.0f);
  float percentage_minutes = (tick_time->tm_min / 59.0f);
  int32_t angle_an_hour = TRIG_MAX_ANGLE / 12;
  int32_t angle_hour = (int32_t)((percentage_hours * TRIG_MAX_ANGLE) + (percentage_minutes * angle_an_hour) ) % TRIG_MAX_ANGLE;
  int32_t angle_min = percentage_minutes * TRIG_MAX_ANGLE;
#endif
  
#if defined(USER_ROT_BITMAP)
  rot_bitmap_layer_position_from_angle_distance_set(s_hour_point_layer, &g_center, angle_hour, g_hand_position_distances[_hand_positions_INSIDE]);
  rot_bitmap_layer_position_from_angle_distance_set(s_min_point_layer, &g_center, angle_min, g_hand_position_distances[_hand_positions_OUTSIDE]);
#else
  bitmap_layer_position_from_angle_distance_set(s_hour_point_layer, &g_center, angle_hour, g_hand_position_distances[_hand_positions_INSIDE]);
  bitmap_layer_position_from_angle_distance_set(s_min_point_layer, &g_center, angle_min, g_hand_position_distances[_hand_positions_OUTSIDE]);
#endif
}