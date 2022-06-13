RGB_MATRIX_EFFECT(MD_RGB_MATRIX)

#ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS

void md_rgb_matrix_init(void);
void md_rgb_matrix_effect_start(void);
void md_rgb_matrix_effect_render(uint8_t i);

static bool MD_RGB_MATRIX(effect_params_t* params) {
  if (params->init) md_rgb_matrix_init();
  if (params->iter == 0) md_rgb_matrix_effect_start();

  RGB_MATRIX_USE_LIMITS(led_min, led_max);
  for (uint8_t i = led_min; i < led_max; i++) {
    md_rgb_matrix_effect_render(i);
  }
  return rgb_matrix_check_finished_leds(led_max);
}

#endif // RGB_MATRIX_CUSTOM_EFFECT_IMPLS
