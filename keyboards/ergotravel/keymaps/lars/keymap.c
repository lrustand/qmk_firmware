#include QMK_KEYBOARD_H


#define _QWERTY 0
#define _LOWER 1
#define _RAISE 2
#define _ADJUST 16


enum custom_keycodes {
  HYPER = SAFE_RANGE
};

#define LOWER LT(_LOWER, KC_SPC)
#define RAISE LT(_RAISE, KC_SPC)
#define ADJUST MO(_ADJUST)
#define AA RALT(KC_LBRC)
#define AE RALT(KC_QUOT)
#define OE RALT(KC_SCLN)

#define CTL MT(MOD_LCTL, KC_ESC)


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

  [_QWERTY] = LAYOUT(
     XXXXXXX, XXXXXXX, KC_W,    KC_E,    KC_R,    KC_T,    XXXXXXX,          KC_BSPC, KC_Y,    KC_U,    KC_I,    KC_O,    XXXXXXX, XXXXXXX,
     HYPER,   KC_Q,    KC_S,    KC_D,    KC_F,    KC_G,    XXXXXXX,          KC_ENT,  KC_H,    KC_J,    KC_K,    KC_L,    KC_P,    XXXXXXX,
     KC_LSFT, KC_A,    KC_X,    KC_C,    KC_V,    KC_B,    XXXXXXX,          XXXXXXX, KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SCLN, KC_QUOT,
     CTL,     KC_Z,    XXXXXXX,          KC_LALT, KC_LWIN, LOWER,            RAISE ,  KC_RWIN, KC_RALT,          XXXXXXX, KC_SLSH, KC_RSFT
  ),

  [_LOWER] = LAYOUT(
     XXXXXXX, XXXXXXX,  KC_AT,   KC_HASH, KC_DLR,  KC_PERC, XXXXXXX,         KC_DEL,   KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, XXXXXXX, XXXXXXX,
     KC_TILD, KC_EXLM,  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,         XXXXXXX,  KC_LEFT, KC_DOWN, KC_UP,   KC_RIGHT,KC_RPRN, KC_EQL,
     _______, XXXXXXX,  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,         XXXXXXX,  XXXXXXX, XXXXXXX, KC_LBRC, KC_RBRC, KC_MINS, KC_PLUS,
     _______, XXXXXXX,  XXXXXXX,          _______, _______, _______,         _______,  _______, _______,          XXXXXXX, XXXXXXX, _______
  ),

  [_RAISE] = LAYOUT(
     XXXXXXX, XXXXXXX,  KC_2,    KC_3,    KC_4,    KC_5,    XXXXXXX,         XXXXXXX,  KC_6,    KC_7,    KC_8,    KC_9,    XXXXXXX, XXXXXXX,
     XXXXXXX, KC_1,     XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,         XXXXXXX,  KC_HOME, KC_PGDN, KC_PGUP, KC_END,  KC_0,    KC_PIPE,
     _______, XXXXXXX,  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,         XXXXXXX,  XXXXXXX, XXXXXXX, KC_LCBR, KC_RCBR, KC_UNDS, KC_BSLASH,
     _______, XXXXXXX,  XXXXXXX,          _______, _______, _______,         _______,  _______, _______,          XXXXXXX, XXXXXXX, _______
  ),

  [_ADJUST] = LAYOUT(
     XXXXXXX, XXXXXXX,  KC_F2,   KC_F3,   KC_F4,   KC_F5,   XXXXXXX,         XXXXXXX,  KC_F6,   KC_F7,   KC_F8,   KC_F9,   XXXXXXX, XXXXXXX,
     XXXXXXX, KC_F1,    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,         XXXXXXX,  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_F10,  AA,
     _______, XXXXXXX,  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,         XXXXXXX,  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, OE,      AE,
     _______, XXXXXXX,  XXXXXXX,          _______, _______, _______,         _______,  _______, _______,          XXXXXXX, XXXXXXX, _______
  )

};


bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  static uint16_t hyper_timer = 0;
  if(record->event.pressed && keycode != HYPER && hyper_timer>0)
    register_code(KC_CAPS);
  switch (keycode) {
    case HYPER:
      if(record->event.pressed) {
        hyper_timer = timer_read();
      } else {
        unregister_code(KC_CAPS);
        if (timer_elapsed(hyper_timer) < TAPPING_TERM) {
          register_code(KC_TAB);
          wait_ms(50);
          unregister_code(KC_TAB);
        }
        hyper_timer = 0;
      }
      return false; // We handled this keypress
  }
  return true; // We didn't handle other keypresses
}

layer_state_t layer_state_set_user(layer_state_t state) {
   return update_tri_layer_state(state, _LOWER, _RAISE, _ADJUST);
}
