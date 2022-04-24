#include <trajectory_ctrl.h>

    TrajectoryController::TrajectoryController() {}

    void TrajectoryController::update() {
        // if (!this->is_active) return;
        // update stepper and determine if all are ready (i.e. none is active)
        this->steppers[0].updateStepper();
        this->steppers[1].updateStepper();
        bool isReady = !(this->steppers[0].stepper_active || this->steppers[1].stepper_active);
        if (isReady) {
            // Sequence finished, update current lenghts l0, l1
            // Determine change in lengths via change in angles
            this->updateState();
            // this->l0_curr = this->l0_offset + (float)this->steppers[0].total_steps_signed / (float)this->steppers[0].PULSE_PER_REVOLUTION * wheel_circumference;
            // this->l1_curr = this->l1_offset + (float)this->steppers[1].total_steps_signed / (float)this->steppers[1].PULSE_PER_REVOLUTION * wheel_circumference;
            this->generateNextSequence();
        }
    }
    
    PointBuffer* TrajectoryController::getBuffer() {
        return &(this->point_buffer);
    }

    void TrajectoryController::updateState() {
        this->l0_curr = this->l0_offset + (float)this->steppers[0].total_steps_signed / (float)this->steppers[0].PULSE_PER_REVOLUTION * wheel_circumference;
        this->l1_curr = this->l1_offset + (float)this->steppers[1].total_steps_signed / (float)this->steppers[1].PULSE_PER_REVOLUTION * wheel_circumference;
    }

    void TrajectoryController::init(float x, float y) {
        // Sets the current lengths to x, y and resets steppers
        float yy = y*y;
        this->l0_offset = sqrt(x*x + yy);
        this->l1_offset = sqrt((anchor_distance-x)*(anchor_distance-x) + yy);
        this->steppers[0].total_steps_signed = 0;
        this->steppers[1].total_steps_signed = 0;
        this->updateState();
    }

    Point TrajectoryController::getCurrentLengths() {
        Point pnt(this->l0_curr, this->l1_curr, 0);
        return pnt;
    }

    void TrajectoryController::setMaxAngularVelocity(float vel_deg_s) {
        vel_deg_s = vel_deg_s > LIMIT_ANGULAR_VELOCITY_DEG ? LIMIT_ANGULAR_VELOCITY_DEG : vel_deg_s;
        this->max_angular_velocity_deg = vel_deg_s < 0 ? 0 : vel_deg_s;
    }


    void TrajectoryController::generateNextSequence() {
        Point* pnt_target = this->point_buffer.pop();
        if (pnt_target == nullptr) return;

        // target lengths
        float yy_target = pnt_target->y*pnt_target->y;
        float l0_target = sqrt(pnt_target->x*pnt_target->x + yy_target);
        float l1_target = sqrt((anchor_distance-pnt_target->x)*(anchor_distance-pnt_target->x) + yy_target);
        // angle increments
        float phi0_deg =  (l0_target - this->l0_curr) / wheel_circumference * 360.;      
        float phi1_deg =  (l1_target - this->l1_curr) / wheel_circumference * 360.;

        // Serial.print("phi0_deg: ");
        // Serial.print(phi0_deg);
        // Serial.print(" phi1_deg: ");
        // Serial.print(phi1_deg);
        // Serial.print("\n");

        // minimum time in order to not exceed max. speed
        int32_t dt_musec = 1000000 * max(abs(phi0_deg), abs(phi1_deg)) / this->max_angular_velocity_deg;
    
        // Send control signal to motors to increment angle phi in given time

        // Serial.print("l0: ");
        // Serial.print(l0);
        // Serial.print(" l1: ");
        // Serial.print(l1);
        // Serial.print("\n");

        // Serial.print("l0 target: ");
        // Serial.print(l0_target);
        // Serial.print(" l1 target: ");
        // Serial.print(l1_target);
        // Serial.print("\n");

        // Serial.print("x target: ");
        // Serial.print(pnt_target->x);
        // Serial.print(" y target: ");
        // Serial.print(pnt_target->y);
        // Serial.print("\n");



        // Serial.print("l0: ");
        // Serial.print(l0);
        // Serial.print("l0 target: ");
        // Serial.print(l0_target);
        // Serial.print("\n");
        this->steppers[0].incrementAngle(phi0_deg, dt_musec);
        this->steppers[1].incrementAngle(phi1_deg, dt_musec);

    }
