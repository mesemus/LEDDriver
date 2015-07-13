
#include <Arduino.h>

#include <SoftTablePWM.h>


INSTALL_PWM_INTERRUPT

// The setup() method runs once, when the sketch starts
void setup()   {
  // initialize the digital pin as an output:
  Serial.begin(115200);
  pwm_add(PWM_D, 2);
  pwm_add(PWM_D, 3);
  pwm_add(PWM_D, 4);
  pwm_add(PWM_D, 5);
  pwm_add(PWM_D, 6);
  pwm_add(PWM_D, 7);
  pwm_add(PWM_C, 0);
  pwm_add(PWM_C, 1);
  pwm_add(PWM_C, 2);
  pwm_add(PWM_C, 3);
  pwm_init(true);
  for (int i=0; i<10; i++) pwm_set(i, 0);
  Serial.println("initialized");
}

// the loop() method runs over and over again,
// as long as the Arduino has power

bool ramping_up = true;

typedef enum {
	STATE_INITIAL,
	STATE_ERROR,
	STATE_PIN,
	STATE_VAL1,
	STATE_VAL2
} LineState;

LineState state;
bool will_dim = false;
int pin;
int val;

void _error(int c) {
	Serial.println();
	Serial.print("Error in input. State");
	Serial.print(state);
	Serial.print(", input char: ");
	Serial.println(c, HEX);
	state = STATE_ERROR;
}

void loop()
{
	while (Serial.available()) {
		int c = Serial.read();
		if (c == '\n' || c == '\r') {
			state = STATE_INITIAL;
			continue;
		}
		if (c == ' ' || c == '\t') {
			continue; // always ignore whitespaces
		}
		if (c == 'S' || c == 'F') {
			state = STATE_INITIAL; // shortcut
		}
		switch (state) {
		case STATE_ERROR:
			Serial.println("In error, press enter to continue");
			break;				// continue to the next newline
		case STATE_INITIAL:
			switch (c) {
			case 'S' :          // set without dimming
				will_dim = false;
				state = STATE_PIN;
				break;
			case 'F':
				will_dim = true;
				state = STATE_PIN;
				break;
			default:
				_error(c);
			}
			break;
		case STATE_PIN:
			if (c>='0' && c<='9') {
				pin = c - '0';
				state = STATE_VAL1;
				Serial.println();
				Serial.print("Have pin: ");
				Serial.println(pin);
			} else {
				state = STATE_ERROR;
			}
			break;
		case STATE_VAL1:
			if (c>='0' && c<='9') {
				val = c - '0';
			} else if (c>='a' && c<='g') {
				val = 10 + c - 'a';
			} else {
				_error(c);
				break;
			}
			Serial.println();
			Serial.print("Have first half of val: ");
			Serial.println(val);
			state = STATE_VAL2;
			break;
		case STATE_VAL2:
			val *= 16;
			if (c>='0' && c<='9') {
				val += c - '0';
			} else if (c>='a' && c<='g') {
				val += 10 + c - 'a';
			} else {
				_error(c);
				break;
			}
			if (will_dim) {
				pwm_fade(pin, val);
			} else {
				pwm_set(pin, val);
			}
			Serial.print("Setting ");
			Serial.print(pin);
			Serial.print("to val");
			Serial.println(val);
			state = STATE_PIN; // and next pin
			break;
		}
	}

	pwm_run_fade();
	delay(10);
}



int main(void) {

  init();
  setup();
  while(true) {
    loop();
  }
}
