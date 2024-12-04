#ifndef MDL_MATH_HELPER_H
#define MDL_MATH_HELPER_H
//this module is made to provide arduino the half dozen math things it doesn't natively support because god fucking help me why woulnd't we have basics like vector 3? noooooo that would bne too fucking hard wouldn't it - sera5m 12/1/2024


struct Vector3 {
    int16_t x; // Scaled to 2 decimal places
    int16_t y; // Scaled to 2 decimal places
    int16_t z; // Scaled to 2 decimal places

    Vector3(); // Default constructor
    Vector3(float xVal, float yVal, float zVal); // Float constructor
    float magnitude() const; // Magnitude calculation
};



// Assembly function definitions
extern "C++" { //start inline asm
    void Vector3_ctor(Vector3* self, float xVal, float yVal, float zVal) {
        //take the regular vector as the input and assign to stuff?
        int16_t xInt = static_cast<int16_t>(xVal * 100);
        int16_t yInt = static_cast<int16_t>(yVal * 100);
        int16_t zInt = static_cast<int16_t>(zVal * 100);

        asm volatile( //allocate vars into asm
            "s16i %1, %0, 0\n" // Store x
            "s16i %2, %0, 2\n" // Store y
            "s16i %3, %0, 4\n" // Store z
            :
            : "r"(self), "r"(xInt), "r"(yInt), "r"(zInt));
    }
float Vector3_magnitude(const Vector3* self) {
    int32_t result; // Use 32 bits for intermediate calculations

    asm volatile(
        // Load x, y, z values from the struct (16-bit integers)
        "l16ui a2, %1, 0\n"      // Load x
        "l16ui a3, %1, 2\n"      // Load y
        "l16ui a4, %1, 4\n"      // Load z

        // Calculate x^2 + y^2 + z^2
        "mull a5, a2, a2\n"      // x^2
        "mull a6, a3, a3\n"      // y^2
        "add a5, a5, a6\n"       // x^2 + y^2
        "mull a6, a4, a4\n"      // z^2
        "add a5, a5, a6\n"       // x^2 + y^2 + z^2

        // Integer square root approximation (basic version)
        "movi a6, 0\n"           // Initialize result = 0
        "movi a7, 0x4000\n"      // Initialize bit = 1 << 14 (16-bit safe)

        "1:\n"                   // sqrt_loop
        "blt a7, a5, 2f\n"       // If bit < sum, exit loop
        "add a8, a6, a7\n"       // temp = result + bit
        "bge a5, a8, 3f\n"       // If sum >= temp, update result
        "j 4f\n"                 // Otherwise, skip update

        "3:\n"                   // sqrt_update
        "sub a5, a5, a8\n"       // sum -= temp
        "or a6, a6, a7\n"        // result |= bit

        "4:\n"                   // sqrt_skip
        "srli a7, a7, 2\n"       // bit >>= 2
        "j 1b\n"                 // Repeat loop

        "2:\n"                   // sqrt_done
        "mov %0, a6\n"           // Store final result in output

        : "=r"(result)           // Output
        : "r"(self)              // Input
        : "a2", "a3", "a4", "a5", "a6", "a7", "a8" // Clobbered registers
    );

    return static_cast<float>(result) / 100.0f; // Scale back for float output
}


}//ends the inline asm

// Implementations
Vector3::Vector3() : x(0), y(0), z(0) {}

Vector3::Vector3(float xVal, float yVal, float zVal) {
    Vector3_ctor(this, xVal, yVal, zVal); // Assembly function
}

float Vector3::magnitude() const {
    return Vector3_magnitude(this); // Call external assembly function
}

/*
struct Vector3 {
    int16_t x; // Scaled to 2 decimal places
    int16_t y; // Scaled to 2 decimal places
    int16_t z; // Scaled to 2 decimal places

    Vector3(); // Default constructor
    Vector3(float xVal, float yVal, float zVal); // Float constructor
    float magnitude() const; // Magnitude calculation
};
*/


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