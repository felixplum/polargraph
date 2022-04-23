#include <point_buffer.h>
#include <stepper_ctrl.h>
#include <consts.h>
#include <Arduino.h>

class TrajectoryController {
    private:
        PointBuffer point_buffer;
        StepperController steppers[2];
        float max_angular_velocity_deg = 180; // deg / s
        // State
        float l0_curr = 0, l1_curr = 0;
        float l0_offset = 0, l1_offset = 0;

    public:  
        TrajectoryController();
        void update();
        void init(float x, float y);
        void generateNextSequence();
        void setMaxAngularVelocity(float vel_deg_s);
        bool is_active = false;
        PointBuffer* getBuffer();
};