#include "mbed.h"
#include "LSM6DSLSensor.h"

#define PI 3.141592654

static DevI2C devI2c(PB_11,PB_10);
static LSM6DSLSensor acc_gyro(&devI2c,0xD4,D4,D5); // high address

// Create a PWM output for LED control
PwmOut red(PA_15);
PwmOut green(PA_2);
PwmOut blue(PA_7);

float computeAngle(int x, int y, int z){
    float fx = (float) x;                       //Cast int x to float fx
    float fy = (float) y;                       //Cast int y to float fy
    float fz = (float) z;                       //Cast int z to float fz
    float pitch = 0;
    float res = 0;
    
    pitch = atan(-fx/(sqrt((fy*fy)+(fz*fz))));  //All rotations are counter-clockwise with respect to the axis, hence -fx
    res = pitch * (180/PI);                     //Convert from radians to degress
    return res;                                 //Will return a degree value between -90deg to +90deg.
}

int main() {
    uint8_t id;
    int32_t axes[3];
    float res = 0;
    
    acc_gyro.init(NULL);
    acc_gyro.enable_x();
    acc_gyro.enable_g();

    printf("This is an accelerometer example with RGB brightness control running on Mbed OS %d.%d.%d.\n", MBED_MAJOR_VERSION, MBED_MINOR_VERSION, MBED_PATCH_VERSION);
    acc_gyro.read_id(&id);
    printf("LSM6DSL accelerometer & gyroscope = 0x%X\r\n", id);

    while (1) {
        acc_gyro.get_x_axes(axes);
        res = computeAngle(axes[0], axes[1], axes[2]);
        printf("LSM6DSL: X=%6d, Y=%6d, Z=%6d, Pitch=%3.2f deg.\r\n", axes[0], axes[1], axes[2], res);

        // Set values of axis from -1 and 1, so it will be standardized
        float standX = (float)(axes[0] - 1000) / 1000.0; // Standardize X between -1 and 1
        float standY = (float)(axes[1] - 1000) / 1000.0; // Standardize Y between -1 and 1
        float standZ = (float)(axes[2] - 1000) / 1000.0; // Standardize Z between -1 and 1

        // Set LED brightness levels based on standardized values
        red = 0.5 + standX * 0.5; // Adjust brightness range as needed (0.0 to 1.0)
        green = 0.5 + standY * 0.5; // Adjust brightness range as needed (0.0 to 1.0)
        blue = 0.5 + standZ * 0.5; // Adjust brightness range as needed (0.0 to 1.0)

        wait_us(200000);
    }
}