#include <unity.h>
#include <point_buffer.h>
// #include <ArduinoFake.h>

void test_add_pop(void)
{
    PointBuffer point_buffer;
    Point pnt(1,2,3);
    point_buffer.addPoint(pnt);
    TEST_ASSERT(point_buffer.num_points == 1);
    Point* retpnt = point_buffer.pop();
    TEST_ASSERT(retpnt->x == pnt.x && retpnt->y == pnt.y && retpnt->z == pnt.z);
    TEST_ASSERT(point_buffer.num_points == 0);
}

void test_points_from_string(void) {
    char* input = "12.2;32;5;\n1.5;6;7.3;\n";
    PointBuffer buffer;
    buffer.addPointsFromList(input);
    Point* pnt;
    pnt = buffer.pop();
    // printf("x %f y %f z %f\n", pnt->x, pnt->y, pnt->z);
    TEST_ASSERT_EQUAL_FLOAT(12.2, pnt->x);
    TEST_ASSERT_EQUAL_FLOAT(32, pnt->y);
    TEST_ASSERT_EQUAL_FLOAT(5, pnt->z);
    pnt = buffer.pop();
    // printf("x %f y %f z %f\n", pnt->x, pnt->y, pnt->z);
    TEST_ASSERT_EQUAL_FLOAT(1.5,pnt->x);
    TEST_ASSERT_EQUAL_FLOAT(6, pnt->y);
    TEST_ASSERT_EQUAL_FLOAT(7.3, pnt->z);
    
}

int main(int argc, char **argv)
{
    UNITY_BEGIN();

    RUN_TEST(test_add_pop);
    RUN_TEST(test_points_from_string);

    UNITY_END();

    return 0;
}