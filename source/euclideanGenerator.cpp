//
// Created by steve on 13-4-2023.
//

#include "euclideanGenerator.hpp"

EuclideanGenerator::EuclideanGenerator()
        : sequence(MAX_STEPS)
{

}

EuclideanGenerator::~EuclideanGenerator() = default;

// -------------------------------------------------------------------
// getters
// -------------------------------------------------------------------

uint32_t EuclideanGenerator::getSequenceValue(uint32_t index) const
{
    if (index >= MAX_STEPS) return 0;

    return sequence[index];
}

uint32_t EuclideanGenerator::getSteps() const
{
    return steps;
}

uint32_t EuclideanGenerator::getPulses() const
{
    return pulses;
}

uint32_t EuclideanGenerator::getRotation() const
{
    return rotation;
}

// -------------------------------------------------------------------
// setters
// -------------------------------------------------------------------

void EuclideanGenerator::setSteps(uint32_t value)
{
    steps = value;
    if (value < pulses)
    {
        setPulses(value);
    }
    calcEuclideanSequence(steps, pulses, rotation);
}

void EuclideanGenerator::setPulses (uint32_t value)
{
    if (value <= steps)
    {
        pulses = value;
        calcEuclideanSequence(steps, pulses, rotation);
    }
}

void EuclideanGenerator::setRotation (uint32_t value)
{
    rotation = value;
    calcEuclideanSequence(steps, pulses, rotation);
}

// -------------------------------------------------------------------
// methods
// -------------------------------------------------------------------

/**
   function for calculating the euclidean sequence
*/
void EuclideanGenerator::calcEuclideanSequence(uint32_t newSteps, uint32_t newPulses, uint32_t newRotation)
{
    if (newSteps == 0 || newPulses == 0 || newSteps < newPulses) { return; }

    uint32_t prevItem = 0;

    for (uint32_t i = 1; i <= newSteps; i++)
    {
        uint32_t x = (i * newPulses) / newSteps;
        uint32_t y = x - prevItem;

        prevItem = x;

        uint32_t index = (i == newSteps) ? 0 : i;
        index = (index + newRotation) % newSteps;

        // add element to sequence
        sequence[index] = y;
    }
}

