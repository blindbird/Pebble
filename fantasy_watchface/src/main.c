#include <pebble.h>
  
//create a window for the watchface
static Window *s_main_window;

//create a textlayer for the watchface
static TextLayer *s_time_layer;
//experiment with date
static TextLayer *s_date_layer;
//experiment with the day
static TextLayer *s_day_layer;

//declare custom font
static GFont s_time_font;
static GFont s_date_font;

//bitmap for background
static BitmapLayer *s_background_layer;
static GBitmap *s_background_bitmap;

static void update_time() {
  // Get a tm structure?
  time_t temp = time(NULL); 
  struct tm *tick_time = localtime(&temp);

  // Create a long-lived buffer?
  static char s_time_text[] = "00:00";

  // Write the current hours and minutes into the buffer
  if(clock_is_24h_style() == true) {
    // Use 24 hour format
    strftime(s_time_text, sizeof("00:00"), "%H:%M", tick_time);
  } else {
    // Use 12 hour format
    strftime(s_time_text, sizeof("00:00"), "%I:%M", tick_time);
  }

  // Display this time on the TextLayer
  text_layer_set_text(s_time_layer, s_time_text);
  
  //experiment with date
  static char s_date_text[] = "01.01";
  strftime(s_date_text, sizeof("01.01"), "%d.%m", tick_time);
  text_layer_set_text(s_date_layer, s_date_text);
  
  //experiment with the day
  static char s_day_text[] = "Sun";
  strftime(s_day_text, sizeof("Sun"),"%a", tick_time);
  text_layer_set_text(s_day_layer, s_day_text);
}

//load the window onto the program
static void main_window_load(Window *window) {
  //Create GBitmap, then set to created BitmapLayer
  s_background_bitmap = gbitmap_create_with_resource(RESOURCE_ID_DRAGON);
  s_background_layer = bitmap_layer_create(GRect(0,15,72,146));
  bitmap_layer_set_bitmap(s_background_layer, s_background_bitmap);
  layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(s_background_layer));
  
  //make a text layer in the window
  s_time_layer = text_layer_create(GRect(60,0,84,90));
  text_layer_set_background_color(s_time_layer, GColorClear);
  text_layer_set_text_color(s_time_layer, GColorBlack);
  text_layer_set_text(s_time_layer, "00:00");
  
  //create font
  s_date_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_ENDOR_80));
  //create other font
  s_time_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_ENDOR_40));
  
  //make text aligned, choose font (look like watch)
  text_layer_set_font(s_time_layer, s_time_font);
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentRight);
  
  //text becomes child layer to window
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_time_layer));
  
  //experiment with date
  s_date_layer = text_layer_create(GRect(0,85,144,90));
  text_layer_set_background_color(s_date_layer, GColorClear);
  text_layer_set_text_color(s_date_layer, GColorBlack);
  text_layer_set_text(s_date_layer, "01.01");
  text_layer_set_font(s_date_layer, s_date_font);
  text_layer_set_text_alignment(s_date_layer, GTextAlignmentRight);
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_date_layer));
  
  //experiment with the day
  s_day_layer = text_layer_create(GRect(0,25,144,90));
  text_layer_set_background_color(s_day_layer, GColorClear);
  text_layer_set_text_color(s_day_layer, GColorBlack);
  text_layer_set_text(s_day_layer, "Sun");
  text_layer_set_font(s_day_layer, s_date_font);
  text_layer_set_text_alignment(s_day_layer, GTextAlignmentRight);
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_day_layer));
  
  //time displayed from start
  update_time();
}

static void main_window_unload(Window *window) {
  //destroy text layer
  text_layer_destroy(s_time_layer);
  //experiment with date
  text_layer_destroy(s_date_layer);
  text_layer_destroy(s_day_layer);
  
  //destroy custom font
  fonts_unload_custom_font(s_time_font);
  fonts_unload_custom_font(s_date_font);
  
  //Destroy GBitmap
  gbitmap_destroy(s_background_bitmap);
  //Destroy BitmapLayer
  bitmap_layer_destroy(s_background_layer);
}

//change the changing time...?
static void tick_handler(struct tm *tick_face, TimeUnits units_changed) {
  update_time();
}

//codes for the main program
static void init () {
  //initializes the window
  s_main_window = window_create();
  
  //set window handlers to manage elements in window
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });
  
  //show window on watch, when animated is true
  window_stack_push(s_main_window, true);

  //register with tick timer to tell time
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
}

static void deinit () {
  //destroys window to give back memory to watch
  window_destroy(s_main_window);
}

//main program, loops the initializing
int main(void) {
  init();
  app_event_loop();
  deinit();
}