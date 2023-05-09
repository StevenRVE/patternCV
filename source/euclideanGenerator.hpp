//
// Created by steve on 13-4-2023.
//

#ifndef SVE_PATTERN_DELAY_EUCLIDEANGENERATOR_HPP
#define SVE_PATTERN_DELAY_EUCLIDEANGENERATOR_HPP

// libs
#include <cstdint>
#include <iostream>
#include <vector>
#include <sstream>
#include <cmath>

// constants
#define MAX_STEPS   16

class EuclideanGenerator {
public:
    EuclideanGenerator();

    ~EuclideanGenerator();

    // getters
    uint32_t getSequenceValue(uint32_t index) const;

    uint32_t getSteps() const;
    uint32_t getPulses() const;
    uint32_t getRotation() const;

    // setters
    void setSteps(uint32_t value);
    void setPulses(uint32_t value);
    void setRotation(uint32_t value);

    // --- methods --- //

    // sequence
    void calcEuclideanSequence(uint32_t steps, uint32_t pulses, uint32_t rotation);

protected:
    // parameters
    uint32_t steps     = 0;
    uint32_t pulses    = 0;
    uint32_t rotation  = 0;

    // sequence
    std::vector<uint32_t> sequence;
};


#endif //SVE_PATTERN_DELAY_EUCLIDEANGENERATOR_HPP
