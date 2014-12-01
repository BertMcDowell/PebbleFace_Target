#include <pebble.h>
#include "watch.h"
  
// Function Forward Declaration
void handle_init(void);
void handle_deinit(void);

// Variable Declaration
static Window * s_main_window;

void handle_init(void) {
  s_main_window = window_create();
  
  // Set handlers to manage the elements inside the Window
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = handle_main_window_load,
    .unload = handle_main_window_unload
  });
  
  window_stack_push(s_main_window, true);
}

void handle_deinit(void) {
  window_destroy(s_main_window);
}

int main(void) {
  handle_init();
  app_event_loop();
  handle_deinit();
}