// ///////////////////////////////////////////////////////////// //
// Hardware abstraction layer for all different supported boards //
// ///////////////////////////////////////////////////////////// //
#include "stm32fx/peripherals.h"
#include "boards/board_declarations.h"

// ///// Board definition and detection ///// //
#include "drivers/harness.h"
#ifdef PANDA
  #include "drivers/fan.h"
  #include "drivers/rtc.h"
  #include "drivers/clock_source.h"
  #include "boards/white.h"
  #include "boards/grey.h"
  #include "boards/black.h"
  #include "boards/uno.h"
  #include "boards/dos.h"
#else
  #include "boards/pedal.h"
#endif

void detect_board_type(void) {
  #ifdef PANDA
    // SPI lines floating: white (TODO: is this reliable? Not really, we have to enable ESP/GPS to be able to detect this on the UART)
    set_gpio_output(GPIOC, 14, 1);
    set_gpio_output(GPIOC, 5, 1);
    set_gpio_mode(GPIOB, 15, MODE_INPUT);
    delay(100);
    int has_gps  = !get_gpio_input(GPIOB, 15);
    if(has_gps == 0){
      hw_type = HW_TYPE_WHITE_PANDA;
      current_board = &board_white;
    }
    else{
      hw_type = HW_TYPE_BLACK_PANDA;
      current_board = &board_black;
    }  
  #else
    #ifdef PEDAL
      hw_type = HW_TYPE_PEDAL;
      current_board = &board_pedal;
    #else
      hw_type = HW_TYPE_UNKNOWN;
      puts("Hardware type is UNKNOWN!\n");
    #endif
  #endif
}

bool has_external_debug_serial = 0;

void detect_external_debug_serial(void) {
  // detect if external serial debugging is present
  has_external_debug_serial = detect_with_pull(GPIOA, 3, PULL_DOWN);
}
