#include <point_buffer.h>


Point::Point(float x, float y, float z) {
    this->x = x;
    this->y = y;
    this->z = z;
}

Point::Point() {}

PointBuffer::PointBuffer() {}


bool PointBuffer::addPoint(Point pnt) {
    if (this->num_points >= this->NUM_POINTS_MAX) return false;
    this->points[this->idx_end] = pnt;
    this->idx_end = (this->idx_end + 1) % this->NUM_POINTS_MAX;
    this->num_points++;
    return true;
};

void PointBuffer::addPointsFromList(const char* input) {
    int i = 0;
    int token_start_idx = 0;
    char* tmp_char;
    uint8_t token_idx = 0;
    Point pnt;

    while (input[i] != '\0')  {
        if(input[i] == '\n' || input[i+1] == '\0') {
            // end of line: add point
            if(!this->addPoint(pnt)) return;
            token_start_idx = i;
            token_idx = 0;
        } else if (input[i] == ';') {
            // end of token: parse token between token_start_idx..idx-1
            int num_chars = i-token_start_idx+1; // +1 for terminaning \0 character
            tmp_char = (char*)malloc(num_chars);
            memcpy(tmp_char, &input[token_start_idx], num_chars);
            float number = atof(tmp_char);
            
            if (token_idx == 0) {
                pnt.x = number;
            } else  if (token_idx == 1) {
                pnt.y = number;
            } else  if (token_idx == 2) {
                pnt.z = number;
            }
            // printf("start %c end %c", input[token_start_idx], input[i-1]);
            // printf("token: %s\n", tmp_char);

            // printf("x %f y %f z %f\n", pnt.x, pnt.y, pnt.z);
            // printf("Num chars: %i\n", num_chars);
            // printf("Number: %f\n", number);
            token_start_idx = i+1;
            free(tmp_char);
            token_idx++;
        }
        i++;
    }
    return;
}

Point* PointBuffer::pop() {
    if (this->num_points == 0) return nullptr;
    uint32_t ret_idx = this->idx_start;
    this->idx_start = (this->idx_start + 1) % this->NUM_POINTS_MAX;
    this->num_points--;
    return &(this->points[ret_idx]); 
};