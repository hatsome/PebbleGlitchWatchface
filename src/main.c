#include <pebble.h>

#define ANIM_DURATION 50
#define ANIM_DELAY_MIN 200
#define ANIM_DELAY_MAX 500
#define GLITCH_MIN 1
#define GLITCH_MAX 8
  
static Window *s_main_window;
static TextLayer *s_time_layer_top;
static TextLayer *s_time_layer_middle;
static TextLayer *s_time_layer_bottom;
static GFont s_time_font;
static PropertyAnimation *s_property_animation_top;
static PropertyAnimation *s_property_animation_middle;
static PropertyAnimation *s_property_animation_bottom;
static int s_state_top;
static int s_state_middle;
static int s_state_bottom;
static void next_animation_top();
static void next_animation_middle();
static void next_animation_bottom();


static void anim_top_stopped_handler(Animation *animation, bool finished, void *context) {
  property_animation_destroy(s_property_animation_top);
  
  if (finished) {
    next_animation_top();
  }
}

static void next_animation_top() {
  GRect start = layer_get_frame((Layer*)s_time_layer_top), finish;
  if (s_state_top == 0) {
    finish = GRect(rand() % (GLITCH_MAX -GLITCH_MIN+1)+GLITCH_MIN, 65, 144, rand() % (35 -5 +1)+5);
  } else {
    finish = GRect(0, 65, 144, 20);
  }
  s_property_animation_top = property_animation_create_layer_frame(text_layer_get_layer(s_time_layer_top), &start, &finish);
  animation_set_duration((Animation*)s_property_animation_top, ANIM_DURATION);
  if (s_state_top == 0) {
    animation_set_delay((Animation*)s_property_animation_top, rand() % (ANIM_DELAY_MAX - ANIM_DELAY_MIN +1) + ANIM_DELAY_MIN);
  } else {
    animation_set_delay((Animation*)s_property_animation_top, 0);
  }
  animation_set_curve((Animation*)s_property_animation_top, AnimationCurveLinear);
  animation_set_handlers((Animation*)s_property_animation_top, (AnimationHandlers) {
    .stopped = anim_top_stopped_handler
  }, NULL);
  animation_schedule((Animation*)s_property_animation_top);
  
  s_state_top = (s_state_top +1) %2;
}

static void anim_middle_stopped_handler(Animation *animation, bool finished, void *context) {
  property_animation_destroy(s_property_animation_middle);
  
  if (finished) {
    next_animation_middle();
  }
}

static void next_animation_middle() {
  GRect start = layer_get_frame((Layer*)s_time_layer_middle), finish;
  if (s_state_middle == 0) {
    finish = GRect(rand() % (GLITCH_MAX -GLITCH_MIN+1)+GLITCH_MIN, 65, 144, rand() % (45 -15 +1)+15);
  } else {
    finish = GRect(0, 65, 144, 30);
  }
  s_property_animation_middle = property_animation_create_layer_frame(text_layer_get_layer(s_time_layer_middle), &start, &finish);
  animation_set_duration((Animation*)s_property_animation_middle, ANIM_DURATION);
  if (s_state_middle == 0) {
    animation_set_delay((Animation*)s_property_animation_middle, rand() % (ANIM_DELAY_MAX - ANIM_DELAY_MIN +1) + ANIM_DELAY_MIN);
  } else {
    animation_set_delay((Animation*)s_property_animation_middle, 0);
  }
  animation_set_curve((Animation*)s_property_animation_middle, AnimationCurveLinear);
  animation_set_handlers((Animation*)s_property_animation_middle, (AnimationHandlers) {
    .stopped = anim_middle_stopped_handler
  }, NULL);
  animation_schedule((Animation*)s_property_animation_middle);
  
  s_state_middle = (s_state_middle +1) %2;
}

static void anim_bottom_stopped_handler(Animation *animation, bool finished, void *context) {
  property_animation_destroy(s_property_animation_bottom);
  
  if (finished) {
    next_animation_bottom();
  }
}

static void next_animation_bottom() {
  GRect start = layer_get_frame((Layer*)s_time_layer_bottom), finish;
  if (s_state_bottom == 0) {
    finish = GRect(rand() % (GLITCH_MAX -GLITCH_MIN+1)+GLITCH_MIN, 65, 144, 50);
  } else {
    finish = GRect(0, 65, 144, 50);
  }
  s_property_animation_bottom = property_animation_create_layer_frame(text_layer_get_layer(s_time_layer_bottom), &start, &finish);
  animation_set_duration((Animation*)s_property_animation_bottom, ANIM_DURATION);
  if (s_state_bottom == 0) {
    animation_set_delay((Animation*)s_property_animation_bottom, rand() % (ANIM_DELAY_MAX - ANIM_DELAY_MIN +1) + ANIM_DELAY_MIN);
  } else {
    animation_set_delay((Animation*)s_property_animation_bottom, 0);
  }
  animation_set_curve((Animation*)s_property_animation_bottom, AnimationCurveLinear);
  animation_set_handlers((Animation*)s_property_animation_bottom, (AnimationHandlers) {
    .stopped = anim_bottom_stopped_handler
  }, NULL);
  animation_schedule((Animation*)s_property_animation_bottom);
  
  s_state_bottom = (s_state_bottom +1) %2;
}

static void update_time() {
  time_t temp = time(NULL);
  struct tm * tick_time = localtime(&temp);
  
  static char buffer[] = "00:00";
  
  if (clock_is_24h_style() == true) {
    strftime(buffer, sizeof("00:00"), "%H:%M", tick_time);
  } else {
    strftime(buffer, sizeof("00:00"), "%I:%M", tick_time);
  }
  
  text_layer_set_text(s_time_layer_bottom, buffer);
  text_layer_set_text(s_time_layer_middle, buffer);
  text_layer_set_text(s_time_layer_top, buffer);
}

static void text_layer_load(Window *window, TextLayer *text_layer) {
  text_layer_set_background_color(text_layer, GColorBlack);
  text_layer_set_text_color(text_layer, GColorWhite);
  
  text_layer_set_font(text_layer, s_time_font);
  text_layer_set_text_alignment(text_layer, GTextAlignmentCenter);
  
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(text_layer));
  
}

static void main_window_load(Window *window) {
  s_time_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_COOLVETICA_42));
  
  s_time_layer_bottom = text_layer_create(GRect(0, 65, 144, 50));
  text_layer_load(window, s_time_layer_bottom);
  
  s_time_layer_middle = text_layer_create(GRect(0, 65, 144, 30));
  text_layer_load(window, s_time_layer_middle);
  
  s_time_layer_top = text_layer_create(GRect(0, 65, 144, 20));
  text_layer_load(window, s_time_layer_top);
}

static void main_window_unload(Window *window) {
  text_layer_destroy(s_time_layer_bottom);
  text_layer_destroy(s_time_layer_middle);
  text_layer_destroy(s_time_layer_top);
  fonts_unload_custom_font(s_time_font);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();
}

static void init() {
  srand(time(NULL));
  s_main_window = window_create();
  
  window_set_background_color(s_main_window, GColorBlack);
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });
  
  window_stack_push(s_main_window, true);
  
  update_time();
  
  next_animation_top();
  next_animation_middle();
  next_animation_bottom();
  
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
}

static void deinit() {
  animation_unschedule_all();
  window_destroy(s_main_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}
