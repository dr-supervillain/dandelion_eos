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


struct Vector3 { //emulation of normal cpp vector3
    float x;
    float y;
    float z;
};

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