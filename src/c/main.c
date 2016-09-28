// Directive to use Pebble SDK
#include <pebble.h>

// Creates Window variable to we can access it at any time
// s_ denotes that it's static meaning it can only be accessed on that page
static Window *s_main_window;

// Creates TextLayer static variable that can be used in the future
static TextLayer *s_time_layer;

// Function that updates the time logic from tick_handler and main_window_load
static void update_time() {
  // Pull in a tm structure to the info
  time_t temp = time(NULL);
  struct tm *tick_time = localtime(&temp);
  
  // Write the current hours and minutes into a buffer
  static char s_buffer[8];
  strftime(s_buffer, sizeof(s_buffer), clock_is_24h_style() ?
                                          "%H:%M" : "%I:%M", tick_time);
  //Displays the time on the text layer
  text_layer_set_text(s_time_layer, s_buffer);
}

// Function that calls the TickTimer Service whenever the time changes
static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();
}

// Two functions below create an additional layer of abstraction to manage
// the window sub-elements
static void main_window_load(Window *window) {
  // Get informationa about the window
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);
  
  // Creates a textlayer with width and height for square and round pebble
  s_time_layer = text_layer_create(
    GRect(0, PBL_IF_ROUND_ELSE(58, 52), bounds.size.w, 50));
  
  // Takes the text layer and layers over design options to make it look like a watch
  text_layer_set_background_color(s_time_layer, GColorClear);
  text_layer_set_text_color(s_time_layer, GColorBlack);
//   text_layer_set_text(s_time_layer, "00:00");
  text_layer_set_font(s_time_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);
  
  // Add the text layer as a child to the window layer
  layer_add_child(window_layer, text_layer_get_layer(s_time_layer));
}

static void main_window_unload(Window *window) {
  // Destroys TextLayer
  text_layer_destroy(s_time_layer); 
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
    
  // Show the Window on the watch, with animated=true
  window_stack_push(s_main_window, true);
  
  // Calls the function to call the current time
  update_time();
  
  // Registers with the Tick Timer Service tying to function above
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);

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