#ifndef mdl_mathHelper_H
#define mdl_mathHelper_H
//this module is made to provide arduino the half dozen math things it doesn't natively support because god fucking help me why woulnd't we have basics like vector 3? noooooo that would bne too fucking hard wouldn't it - sera5m 12/1/2024


struct Vector2 { //emulation of normal cpp vector2
    float x;
    float y;
    };

struct DVector2 { //vector2 if it was doubles
    double x;
    double y;
    
};

//emulate c++ vec3 structure in low accuracy
struct Vector3 {
    int16_t x; // Scaled to 2 decimal places
    int16_t y; // Scaled to 2 decimal places
    int16_t z; // Scaled to 2 decimal places

    Vector3() : x(0), y(0), z(0) {}
    Vector3(float xVal, float yVal, float zVal) {
        x = static_cast<int16_t>(xVal * 100); // Scale and convert to int
        y = static_cast<int16_t>(yVal * 100); // Scale and convert to int
        z = static_cast<int16_t>(zVal * 100); // Scale and convert to int
    }

    float magnitude() const {
        // Calculate the magnitude, returning it as a float with 2 decimals
        return sqrt((x * x + y * y + z * z) / 10000.0f);
    }
};
//i am going to kill myself 

struct DVector3 { //vector3 if it was doubles
    double x;
    double y;
    double z;
};

struct Vector4 { //vec4-for when you need 1 more dimension
    float x;
    float y;
    float z;
    float w;
};

struct DVector4 { //vector4 if it was doubles
    double x;
    double y;
    double z;
    double w;
};

//put function to get len of ANY vector here. eg it goes through, adds em and sqrt and shit, if double then break internally and use double logic

//put do n here
#endif