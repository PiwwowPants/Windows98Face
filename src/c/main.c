#include <pebble.h>
#include "globals.h"
#include "window.h"
#include "callbacks.h"
#include "settings.h"
#include "time.h"

 //APP_LOG(APP_LOG_LEVEL_DEBUG, "Loop index now %d", i);

static void init(void);
static void deinit(void);


int main(void) {
  init();
  app_event_loop();
  deinit();
}

static void init(void) {
  
  flick_show_window = false;

  // load saved settings
  load_settings();
  
  // Open AppMessage connection
  app_message_register_inbox_received(inbox_received_handler);
  app_message_open(128, 128);
  
  //manage window
  main_window = window_create();
  
  window_set_window_handlers(main_window,(WindowHandlers) {
        .load = window_load,
        .unload = window_unload
  });

  window_stack_push(main_window, true);
  
  //register for battery level updates
  battery_state_service_subscribe(battery_callback);
  
  
  //register for bluetooth connection updates
  connection_service_subscribe((ConnectionHandlers) {
    .pebble_app_connection_handler = bluetooth_callback
  });
  
    //register for timer tick service
  tick_timer_service_subscribe(MINUTE_UNIT | DAY_UNIT, tick_handler);
    
  color_desktop_text = GColorWhite;
  
  // Subscribe to tap events
  if(settings.flick_enabled)
    accel_tap_service_subscribe(accel_tap_handler);
  
   APP_LOG(APP_LOG_LEVEL_DEBUG, "show_datatime %d, flichs show %d",settings.show_datatime_window,flick_show_window );
  //get time/date/battery for the first time
  update_time();    
  update_date();
  battery_callback(battery_state_service_peek());
    
}

void deinit(void) {
  //destroy window
  if (main_window) {
    window_destroy(main_window);
 }
  if(settings.flick_enabled)  
   accel_tap_service_unsubscribe();

}




