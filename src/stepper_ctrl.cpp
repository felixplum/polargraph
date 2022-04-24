#include <stepper_ctrl.h>


StepperController::StepperController(uint8_t pin_dir, uint8_t pin_pulse) {
  this->pin_dir = pin_dir;
  this->pin_pulse = pin_pulse;
}

void StepperController::resetSequenceState() {
    this->step_cnt = 0;
    this->is_pulse = false;
    this->stepper_active = false;
    this->time_since_pulse_start = 0;
    this->time_since_pulse_end = 0;
}
void StepperController::incrementAngle(float angle_deg, int32_t sequence_time_musec) {
    this->resetSequenceState();
    this->stepper_angle_target = angle_deg;
    this->sequence_time_musec = sequence_time_musec;

    // Set direction
    this->direction_sign = 1.;
    if (this->stepper_angle_target > 0) {
      digitalWrite(this->pin_dir, HIGH);

    } else {
      this->direction_sign = -1;
      digitalWrite(this->pin_dir, LOW);
    };
    // Num. of steps we need to pulse
    this->num_steps = (int)(this->direction_sign*this->stepper_angle_target / 360. * (float)this->PULSE_PER_REVOLUTION);
    // Serial.print("Increment steps by: ");  Serial.print(this->num_steps ); Serial.print("\n");
    // Serial.print("Sequence time ");  Serial.print(sequence_time_musec); Serial.print("\n");

    if (this->num_steps > 0) {
      this->stepper_active = true;
    } else {
      this->stepper_active = false;
    }
}


void StepperController::updateStepper() {

  if (!this->stepper_active) return;
  uint64_t time_now = micros64();

  // Deactivate pulse after 10 microseconds and increment step counter
  if (this->is_pulse && (time_now - this->time_since_pulse_start) > this->STEP_DELAY_MUSEC) {
    digitalWrite(this->pin_pulse, LOW);
    this->time_since_pulse_end = time_now;
    this->is_pulse = false;
    this->step_cnt++;
    this->total_steps_signed += this->direction_sign;
    // Serial.println("Total steps: "); 
    // Serial.print(this->total_steps_signed); 
  } else if (!this->is_pulse) {
    // if no pulse: determine if we should start

    uint32_t time_per_step_musec = this->sequence_time_musec /  this->num_steps;
    if (time_now - this->time_since_pulse_start > time_per_step_musec) {
      this->is_pulse = true;
      this->time_since_pulse_start = time_now;
      digitalWrite(this->pin_pulse, HIGH);
    }
  }
  
  // reset after finished
  if (this->num_steps == this->step_cnt) {
    this->stepper_active = false;
  }
}