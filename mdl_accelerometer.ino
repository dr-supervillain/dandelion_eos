#ifndef mdl_accelerometer_H
#define mdl_accelerometer_H

//honestly you need to look into the "calibrated quat" code from the fastimu examples
//fastimu does most of the things we need, so in this module we'll work on developing the following functions
/*
-usersteps
-watch lifted 
-watch shook
-high g stop
-compass heading
-absolute orentation current //not possible without magnetometer-
-a primitive form of gesture sensing including left/right/up/down hand waves, and forward/back arm pull
-more gestures, arm swing and more. oh yeah baby we casting SPELLS with these gesture movements like a fuckin helldivers combo (lamfo)

*/

void updateIMU()
{
IMU.update(); // call imu update to the lib we're using--should return xyz where z is the up and down axis
IMU.getAccel(&accelData); //acceldata
IMU.getTemp();//how to get the stupid temperature

}



//first function: tell if facing up!





//function to return a float output for the total acceleration of devuce based on [sqrt(xx+yy+zz)], the equation to 3d motion.
float totalG(const AccelData& accelData) {
    return std::sqrt(
        accelData.accelX * accelData.accelX +
        accelData.accelY * accelData.accelY +
        accelData.accelY * accelData.accelZ
    );
} //tip use this to get total motion. if it's more than 1g, the user is on another planet or accelerating

//this component of dandelion os uses parts of the below linked repo's, liscensed under MIT liscense. [-free to use in commercial apps if credited]
//step rate algorythm adaptead from http://mau.diva-portal.org/smash/get/diva2:1474455/FULLTEXT01 who provided https://github.com/Oxford-step-counter/C-Step-Counter
//thank you to Anna Brondin & Marcus Nordstrom at Malmö University.

// i'm not gonna use whatever they did with pointers, because at this point i'm not experienced enough to reliably make correct functional software out of that.
//i'll use arrays


//configs
#define PMeter_sampleLEN 8  // Window size (this can be adjusted based on your data)- i think best if power of 2

#define PMeter_sampleInterval_s 0.1f //how often we sample, and default set this to 0.1

extern vector3 ACCLcurrent_sample; //public current sample set to whatever the fuck this is rn

//our vector sample cache of acceleration data
extern Vector3 ACCLsampleCache[PMeter_sampleLEN]; //set len of this vector on start to to pmeter sample len on boot


 //add the current sample to the current position in the array and set that forward a little bit to make a circular buffer taking samples directly from the imu
void Update_sample_IMU() {
    // Shift all existing samples down the array (newest at index 0)
    for (int i = PMeter_sampleLEN - 1; i > 0; --i) {
        sampleCache[i] = sampleCache[i - 1];
    }

    // Add the new sample to index 0
    sampleCache[0] = current_sample;
}



float processMagnitude(std::vector<float>& magnitudes, int i) {
    // Calculate the summation for the scoring formula
    float score = 0.0f;
    for (int k = -N; k <= N; ++k) { //start for loop to emulate the functionality of summation operations
        if (k != 0) { //skip current index when we're working with this
            int idx = i + k; // Calculate the index for the neighbor index
            if (idx >= 0 && idx < magnitudes.size()) {
                score += std::abs(magnitudes[i] - magnitudes[idx]); // Sum the differences of (m_i - m_(i+k) ) 
            }
        }
    }
    return score / (2 * N); // Normalize the score by the window size 
}

//ooohhhh when will we use this in the real world huh...... RIGHT FUCKING NOW. DO SIGMA SUMMATION NOW!!!!


#endif // mdl_accelerometer_H
