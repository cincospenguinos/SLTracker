#include "headers/workout_window.h"
#include "headers/workout.h"

static Window *workout_window; // Declared in the header; allocated here.

static GFont s_res_gothic_14;
static GFont s_res_gothic_18;
static GFont s_res_gothic_28_bold;
static Layer *timer_bar; // Where we'll be drawing the timer
static TextLayer *weight_text; // The current weight's text
static TextLayer *current_set_text; // The current set's text
static TextLayer *exercise_text; // The current exercise text
static BitmapLayer *current_rep_total; // The current total of reps

static GBitmap *five_bitmap;

/*
 * Some quick function declarations here specific to this file
 */

static void update_exercise_text();
static void update_set_text();
static void timer_bar_draw_proc(Layer *layer, GContext *ctx);

void workout_window_init(){
  create_new_workout();

  if(workout_window == NULL){
    workout_window = window_create();
  }

  #ifndef PBL_SDK_3
    window_set_fullscreen(workout_window, true);
  #endif

  window_set_window_handlers(workout_window, (WindowHandlers){
      .load = workout_window_load,
      .unload = workout_window_unload
  });

  window_set_click_config_provider(workout_window, (ClickConfigProvider) workout_window_click_config_provider);

  window_stack_push(workout_window, true);
}

void workout_window_deinit(){
  window_destroy(workout_window);
}

void workout_window_load(void) {
  s_res_gothic_14 = fonts_get_system_font(FONT_KEY_GOTHIC_14);
  s_res_gothic_18 = fonts_get_system_font(FONT_KEY_GOTHIC_18);
  s_res_gothic_28_bold = fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD);

  // timer_bar
  timer_bar = layer_create(GRect(12, 130, 120, 10));
  layer_set_update_proc(timer_bar, timer_bar_draw_proc);
  layer_add_child(window_get_root_layer(workout_window), (Layer *)timer_bar);
  
  // weight_text
  weight_text = text_layer_create(GRect(24, 29, 100, 16));
  text_layer_set_text(weight_text, "999 lbs");
  text_layer_set_text_alignment(weight_text, GTextAlignmentCenter);
  text_layer_set_font(weight_text, s_res_gothic_14);
  layer_add_child(window_get_root_layer(workout_window), (Layer *)weight_text);
  
  // current_set_text
  current_set_text = text_layer_create(GRect(0, 144, 144, 24));
  text_layer_set_text(current_set_text, "1 of 5");
  text_layer_set_text_alignment(current_set_text, GTextAlignmentCenter);
  text_layer_set_font(current_set_text, s_res_gothic_18);
  layer_add_child(window_get_root_layer(workout_window), (Layer *)current_set_text);
  
  // exercise_text
  exercise_text = text_layer_create(GRect(0, 0, 144, 32));
  text_layer_set_text(exercise_text, "Squats");
  text_layer_set_text_alignment(exercise_text, GTextAlignmentCenter);
  text_layer_set_font(exercise_text, s_res_gothic_28_bold);
  layer_add_child(window_get_root_layer(workout_window), (Layer *)exercise_text);
  
  // current_rep_total
  five_bitmap = gbitmap_create_with_resource(RESOURCE_ID_FIVE);
  current_rep_total = bitmap_layer_create(GRect(34, 48, 76, 76));
  bitmap_layer_set_bitmap(current_rep_total, five_bitmap);
  layer_add_child(window_get_root_layer(workout_window), (Layer *)current_rep_total);

  update_set_text();
  update_exercise_text();
}

void workout_window_unload(void) {
  layer_destroy(timer_bar);
  text_layer_destroy(weight_text);
  text_layer_destroy(current_set_text);
  text_layer_destroy(exercise_text);
  bitmap_layer_destroy(current_rep_total);
  gbitmap_destroy(five_bitmap);
}

void workout_window_click_config_provider(Window *window){
  window_single_click_subscribe(BUTTON_ID_UP, add_one_current_reps);
  window_single_click_subscribe(BUTTON_ID_DOWN, subtract_one_current_reps);
  window_single_click_subscribe(BUTTON_ID_SELECT, go_to_next_set);
  window_single_click_subscribe(BUTTON_ID_BACK, go_to_previous_set);
}

void add_one_current_reps(ClickRecognizerRef recognizer, void *context){
  // TODO: This
}

void subtract_one_current_reps(ClickRecognizerRef recognizer, void *context){
  // TODO: This
}

void go_to_next_set(ClickRecognizerRef recognizer, void *context){
  int result = next_set();

  switch(result){
  case 6: // We need to update the exercise information
    update_exercise_text();
    update_set_text();
    break;
  case 7: // We need to save and quit
    store_workout();
    window_stack_pop(true);
    break;
  default:
    update_set_text(result);
  }
}

void go_to_previous_set(ClickRecognizerRef recognizer, void *context){
  int result = previous_set();

  switch(result){
  case 0: // We need to update the exercise information
    update_exercise_text();
    update_set_text();
    break;
  case -1:// We need to quit the application
    window_stack_pop(true);
    break;
  default:
    update_set_text();
  }
}

static void update_exercise_text(){
  text_layer_set_text(exercise_text, get_current_exercise_name());

  int weight = get_current_exercise_weight();
  static char weight_buffer[8] = "    lbs";
  snprintf(weight_buffer, sizeof(weight_buffer), "%i lbs", weight);
  text_layer_set_text(weight_text, weight_buffer);
}

static void update_set_text(){
  int set = get_current_set();
  static char set_buffer[7] = "  of 5";
  snprintf(set_buffer, sizeof(set_buffer), "%i of 5", set);
  text_layer_set_text(current_set_text, set_buffer);
}

// Helper method for draw_proc of timer_bar
static void draw_ticks_timer_bar(Layer *layer, GContext *ctx){
  const GPoint left_pt1 = GPoint(0, 0);
  const GPoint left_pt2 = GPoint(0, 10);
  const GPoint right_pt1 = GPoint(119, 0);
  const GPoint right_pt2 = GPoint(119, 10);

  graphics_context_set_stroke_color(ctx, GColorBlack);
  graphics_draw_line(ctx, left_pt1, left_pt2);
  graphics_draw_line(ctx, right_pt1, right_pt2);
}

static void timer_bar_draw_proc(Layer *layer, GContext *ctx){
  // Note that these points have a RELATIVE set of axes. They do NOT use the
  // the global ones. i.e. 0, 0 here equates to 15, 130 on the whole pebble.
  draw_ticks_timer_bar(layer, ctx);
  

  
}
