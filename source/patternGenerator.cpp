//
// Created by StevenRVE on 7-4-2023.
//

#include "patternGenerator.hpp"

PatternGenerator::PatternGenerator(uint32_t delayTime, double sampleRate)
:   patternNumber(0),
    sampleRate(sampleRate),
    generator(randomDevice()),
    distribution(0,99),
    euclideanGenerator(),
    nthGenerator()
{
    calcDelayTimeSamples(delayTime);
}

PatternGenerator::~PatternGenerator() = default;

void PatternGenerator::calcDelayTimeSamples(uint32_t delayTimeMS)
{
    this->delayTimeSamples = delayTimeMS * ((uint32_t)sampleRate / 1000);
}

void PatternGenerator::selectPattern(uint8_t index)
{
    switch (index)
    {
        case PATTERN_TYPE_RANDOM:
            setPatternLength(1);
            break;
        case PATTERN_TYPE_EUCLIDEAN:
            setPatternLength(step);
            break;
        case PATTERN_TYPE_NTH:
            setPatternLength(nth);
            break;
        default:
            break;
    }
}

void PatternGenerator::setPattern(uint8_t newPatternNumber)
{
    if(newPatternNumber >= 0 && newPatternNumber < PATTERN_TYPE_COUNT)
    {
        this->patternNumber=newPatternNumber;
        currentStep = 0;
        selectPattern(patternNumber);
    }
}

uint32_t PatternGenerator::generateRandomNumber()
{
    uint32_t randomNum = distribution(generator);
    return randomNum;
}

void PatternGenerator::setRandomChance(uint32_t randomChance)
{
    this->randomChance = randomChance;
}

void PatternGenerator::generateEuclideanSequence(uint32_t steps, uint32_t pulse, uint32_t rota) {
    euclideanGenerator.calcEuclideanSequence(steps, pulse, rota);
}

void PatternGenerator::generateNthSequence(uint32_t steps, uint32_t rota) {
    nthGenerator.calcEuclideanSequence(steps, 1, rota);
}

void PatternGenerator::tick()
{
    tickCurrentSample();
}

void PatternGenerator::tickCurrentStep()
{
    currentStep++;
    wrapCurrentStep();
    setCurrentValue();
}

void PatternGenerator::wrapCurrentStep()
{
    std::cout << "patternLength: " << patternLength << "\n";
    if (patternLength >= 0 && currentStep >= patternLength)
    {
        currentStep = 0;
    }
}

void PatternGenerator::tickCurrentSample()
{
    currentSample++;
    wrapCurrentSample();
}

void PatternGenerator::wrapCurrentSample()
{
    if (currentSample >= delayTimeSamples)
    {
        currentSample -= delayTimeSamples;
        tickCurrentStep();
    }
}

bool PatternGenerator::getCurrentValue()
{
    return currentValue;
}

void PatternGenerator::setCurrentValue()
{
    switch (patternNumber)
    {
        case PATTERN_TYPE_RANDOM:
            std::cout << "CurrentStep: " << currentStep << " Setting current value to " << currentValue << " using pattern number the random algorithm\n";
            if (generateRandomNumber() < randomChance)
            {
                this->currentValue = 1;
            } else {
                this->currentValue = 0;
            }
            break;
        case PATTERN_TYPE_EUCLIDEAN:
            std::cout << "CurrentStep: " << currentStep << " Setting current value to " << currentValue << " using pattern number using the euclidean algorithm\n";
            this->currentValue = euclideanGenerator.getSequenceValue(currentStep);
            break;
        case PATTERN_TYPE_NTH:
            std::cout << "CurrentStep: " << currentStep << " Setting current value to " << currentValue << " using pattern number using the nth algorithm\n";
            this->currentValue = nthGenerator.getSequenceValue(currentStep);
            break;
        default:
            break;
    }
}

void PatternGenerator::setPatternLength(uint32_t value)
{
    this->patternLength = value;
}

void PatternGenerator::setEuclideanPatternLength(uint32_t value)
{
    this->step = value;
}

void PatternGenerator::setNthPatternLength(uint32_t value)
{
    this->nth = value;
}
