//
// Created by StevenRVE on 7-4-2023.
//

#ifndef SVE_PATTERN_DELAY_PATTERNGENERATOR_HPP
#define SVE_PATTERN_DELAY_PATTERNGENERATOR_HPP

// libs
#include <iostream>
#include <cstdint>
#include <array>
#include <random>

// classes
#include "euclideanGenerator.hpp"

class PatternGenerator
{
public:
    enum PatternType{
        PATTERN_TYPE_RANDOM,
        PATTERN_TYPE_EUCLIDEAN,
        PATTERN_TYPE_NTH,
        PATTERN_TYPE_COUNT
    };

    PatternGenerator(uint32_t delayTime, double sampleRate);

    ~PatternGenerator();

    void calcDelayTimeSamples(uint32_t delayTimeMS);

    void selectPattern(uint8_t index);
    void setPattern(uint8_t newPatternNumber);

    uint32_t generateRandomNumber();
    void setRandomChance(uint32_t randomChance);

    void generateEuclideanSequence(uint32_t step, uint32_t pulse, uint32_t rota);
    void generateNthSequence(uint32_t step, uint32_t rota);

    void tick();
    void tickCurrentStep();
    void wrapCurrentStep();
    void tickCurrentSample();
    void wrapCurrentSample();

    bool getCurrentValue();
    void setCurrentValue();

    void setPatternLength(uint32_t value);
    void setEuclideanPatternLength(uint32_t value);
    void setNthPatternLength(uint32_t value);

private:
    // pattern
    uint8_t patternNumber{0};
    uint32_t patternLength{1};
    uint32_t currentStep{0};
    bool currentValue{0};
    uint32_t currentSample{0};
    uint32_t delayTimeSamples{0};
    double sampleRate;

    // random
    std::random_device randomDevice;
    std::mt19937 generator;
    std::uniform_int_distribution<> distribution; // distribution in range [0, 99]
    uint32_t randomChance;

    // euclidean
    EuclideanGenerator euclideanGenerator;
    uint32_t step;

    // nth
    uint32_t nth;
    EuclideanGenerator nthGenerator;
};


#endif //SVE_PATTERN_DELAY_PATTERNGENERATOR_HPP
