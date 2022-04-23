#ifndef STEPPER_CTRL_H
#define STEPPER_CTRL_H


#include <consts.h>
#include <Arduino.h>

class StepperController {
    public:
        StepperController();
        void updateStepper();
        void incrementAngle(float angle_deg, int32_t sequence_time_musec);
        void setMaxAngularVelocity(float vel_deg_s);
        void resetSequenceState();
        

        const uint16_t PULSE_PER_REVOLUTION = 1600;
        // Public State
        bool stepper_active = false;
        int64_t total_steps_signed = 0;
    private:
        const float STEP_DELAY_MUSEC = 10;
        // Private State
        uint32_t step_cnt = 0;
        float stepper_angle_target = 0.;
        float direction_sign = 0.;
        uint32_t num_steps = 0;
        uint32_t sequence_time_musec;
        bool is_pulse;
        uint64_t time_since_pulse_start;
        uint64_t time_since_pulse_end;
        

};

#endif