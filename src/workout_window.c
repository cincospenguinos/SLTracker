#include "headers/workout_window.h"
#include "headers/workout.h"

static Window *workout_window; // Declared in the header; allocated here.

static GFont s_res_gothic_14;
static GFont s_res_gothic_18;
static GFont s_res_gothic_28_bold;
static Layer *timer_bar; // Where we'll be drawing the timer
static TextLayer *weight_text; // The current weight's text
static TextLayer *current_set_text; // The current set's text
static TextLayer *exercise_text;
static BitmapLayer *current_rep_total;

void workout_window_init(){
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
  current_rep_total = bitmap_layer_create(GRect(34, 48, 76, 76));
  layer_add_child(window_get_root_layer(workout_window), (Layer *)current_rep_total);
}

void workout_window_unload(void) {
  layer_destroy(timer_bar);
  text_layer_destroy(weight_text);
  text_layer_destroy(current_set_text);
  text_layer_destroy(exercise_text);
  bitmap_layer_destroy(current_rep_total);
}
