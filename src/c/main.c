// directive to use Pebble SDK
#include <pebble.h>

// Creates Window variable to we can access it at any time
// s_ denotes that it's static meaning it can only be accessed on that page
static Window *s_main_window;

// Two functions below create an additional layer of abstraction to manage
// the window sub-elements
static void main_window_load(Window *window) {
 
}

static void main_window_unload(Window *window) {
  
}

// below is the recommended app structure
static void init() {
  // Create main Window element and assign to pointer
  // Make sure in the deinit to destroy the window
  s_main_window = window_create();
  
  // Set handlers to manage the elements inside the Window
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload,
  });
  
  // Show the Windo on the watch, with animated=true
  window_stack_push(s_main_window, true);
}

static void deinit() {
  // Destroy Window
  window_destroy(s_main_window);
  
}

// main function calls init and deinit 
// app event loop lets the watchapp wait for system events until it exits
int main(void) {
  init();
  app_event_loop();
  deinit();
}