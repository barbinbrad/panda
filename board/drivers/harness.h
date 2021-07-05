uint8_t car_harness_status = 0U;
#define HARNESS_STATUS_NC 0U
#define HARNESS_STATUS_NORMAL 1U
#define HARNESS_STATUS_FLIPPED 2U

// Threshold voltage (mV) for either of the SBUs to be below before deciding harness is connected
#define HARNESS_CONNECTED_THRESHOLD 2500U

struct harness_configuration {
  const bool has_harness;
  GPIO_TypeDef *GPIO_SBU1;
  GPIO_TypeDef *GPIO_SBU2;
  GPIO_TypeDef *GPIO_relay_SBU1;
  GPIO_TypeDef *GPIO_relay_SBU2;
  uint8_t pin_SBU1;
  uint8_t pin_SBU2;
  uint8_t pin_relay_SBU1;
  uint8_t pin_relay_SBU2;
  uint8_t adc_channel_SBU1;
  uint8_t adc_channel_SBU2;
};

// this function will be the API for tici
void set_intercept_relay(bool intercept) {
  if (car_harness_status != HARNESS_STATUS_NC) {
    if (intercept) {
      puts("switching harness to intercept (relay on)\n");
    } else {
      puts("switching harness to passthrough (relay off)\n");
    }

    if(car_harness_status == HARNESS_STATUS_NORMAL){
      set_gpio_output(current_board->harness_config->GPIO_relay_SBU2, current_board->harness_config->pin_relay_SBU2, !intercept);
    } else {
      set_gpio_output(current_board->harness_config->GPIO_relay_SBU1, current_board->harness_config->pin_relay_SBU1, !intercept);
    }
  }
}

bool harness_check_ignition(void) {
  bool ret = false;
  return ret;
}

uint8_t harness_detect_orientation(void) {
  uint8_t ret = HARNESS_STATUS_NC;

  #ifndef BOOTSTUB
  ret = HARNESS_STATUS_NORMAL;
  #endif

  return ret;
}

void harness_init(void) {
  // delay such that the connection is fully made before trying orientation detection
  current_board->set_led(LED_BLUE, true);
  delay(10000000);
  current_board->set_led(LED_BLUE, false);

  try to detect orientation
  uint8_t ret = harness_detect_orientation();
  if (ret != HARNESS_STATUS_NC) {
    puts("detected car harness with orientation "); puth2(ret); puts("\n");
    car_harness_status = ret;

    // // set the SBU lines to be inputs before using the relay. The lines are not 5V tolerant in ADC mode!
    // set_gpio_mode(current_board->harness_config->GPIO_SBU1, current_board->harness_config->pin_SBU1, MODE_INPUT);
    // set_gpio_mode(current_board->harness_config->GPIO_SBU2, current_board->harness_config->pin_SBU2, MODE_INPUT);

    // // keep busses connected by default
    // set_intercept_relay(false);
  } else {
    puts("failed to detect car harness!\n");
  }
}
