#include "patternCV.hpp"

START_NAMESPACE_DISTRHO

   /**
      Plugin class constructor.
      You must set all parameter values to their defaults, matching the value in initParameter().
    */
PatternCV::PatternCV()
    : Plugin(PARAM_COUNT, 0, 0),
    pattern(time, getSampleRate())
{
    pattern.generateEuclideanSequence(0,0,0);
    pattern.generateNthSequence(0,0);
}


// -------------------------------------------------------------------
// Init
// -------------------------------------------------------------------

    /**
       Initialize the audio port @a index.@n
       This function will be called once, shortly after the plugin is created.
     */

    void PatternCV::initAudioPort(bool input, uint32_t index, AudioPort& port)
    {
        /**
           Note that index is independent for input and output.
           In other words, input port index starts from 0 and output port index also starts from 0.
         */
        if (input) { return; }
        // Add more conditions here when increasing DISTRHO_PLUGIN_NUM_INPUTS.
        else
        {
            switch (index)
            {
            case 0:
                port.hints   = kAudioPortIsCV;
                port.name    = "CV Output";
                port.symbol  = "cv_out";
                return;
            }
            // Add more conditions here when increasing DISTRHO_PLUGIN_NUM_OUTPUTS.
        }

        // It shouldn't reach here, but just in case if index is greater than 0.
        Plugin::initAudioPort(input, index, port);
    }

/**
  Initialize the parameter @a index.
  This function will be called once, shortly after the plugin is created.
*/
void PatternCV::initParameter(uint32_t index, Parameter& parameter)
{
    if (index >= PARAM_COUNT) { return; }

    struct ParamProps
    {
        bool automatable, boolean, integer, logarithmic, output, trigger;
        float min, max, def;
        const char* name;
        const char* symbol;
    };


    const auto setParamProps = [](auto& param, ParamProps props)
    {
        if(props.automatable){ param.hints = kParameterIsAutomatable;}
        if(props.boolean){param.hints = kParameterIsBoolean;}
        if(props.integer){param.hints = kParameterIsInteger;}
        if(props.logarithmic){param.hints = kParameterIsLogarithmic;}
        if(props.output){param.hints = kParameterIsOutput;}
        if(props.trigger){param.hints = kParameterIsTrigger | kParameterIsBoolean;}
        param.ranges.min = props.min;
        param.ranges.max = props.max;
        param.ranges.def = props.def;
        param.name = props.name;
        param.symbol = props.symbol;
    };

    const auto createPatternList = []
    {
        auto* const patternList = new ParameterEnumerationValue[3];
        patternList[0].label = "Random";
        patternList[0].value = 0;
        patternList[1].label = "Euclidean";
        patternList[1].value = 1;
        patternList[2].label = "Nth";
        patternList[2].value = 2;
        return patternList;
    };

    switch (index)
    {
        // MASTER PARAMS
        case PARAM_TIME:
            setParamProps(parameter, { .automatable=true, .integer=true, .min=100, .max=2000, .def=500, .name="Time", .symbol="time"});
            break;
        case PARAM_PATTERN_TYPE:
            setParamProps(parameter, { .automatable=true, .integer=true, .min=0, .max=2, .def=0, .name="Pattern Type", .symbol="patternType"});
            parameter.enumValues.count = 3;
            parameter.enumValues.restrictedMode = true;
            parameter.enumValues.values = createPatternList();
            break;
        case PARAM_RANDOM_CHANCE:
            setParamProps(parameter, { .automatable=true, .integer=true, .min=0, .max=100, .def=50, .name="Random Chance", .symbol="randomChance"});
            pattern.setRandomChance(50);
            break;
        case PARAM_EUC_STEPS:
            setParamProps(parameter, { .automatable=true, .integer=true, .min=0, .max=16, .def=0, .name="Euclidean Steps", .symbol="eucSteps"});
            pattern.setEuclideanPatternLength(0);
            break;
        case PARAM_EUC_PULSE:
            setParamProps(parameter, { .automatable=true, .integer=true, .min=0, .max=16, .def=0, .name="Euclidean Pulse", .symbol="eucPulse"});
            break;
        case PARAM_EUC_ROTATION:
            setParamProps(parameter, { .automatable=true, .integer=true, .min=0, .max=15, .def=0, .name="Euclidean Rotation", .symbol="eucRotation"});
            break;
        case PARAM_NTH:
            setParamProps(parameter, { .automatable=true, .integer=true, .min=0, .max=16, .def=0, .name="N-th", .symbol="nth"});
            pattern.setNthPatternLength(0);
            break;
        case PARAM_NTH_ROTATION:
            setParamProps(parameter, { .automatable=true, .integer=true, .min=0, .max=15, .def=0, .name="N-th Rotation", .symbol="nthRotation"});
            break;
        default:
            break;
    }
}

// -------------------------------------------------------------------
// Internal data
// -------------------------------------------------------------------

/**
  Get the current value of a parameter.
  The host may call this function from any context, including realtime processing.
*/
float PatternCV::getParameterValue(uint32_t index) const
{
    switch (index)
    {
        // MASTER PARAMS
        case PARAM_TIME:
            return time;
            // Pattern
        case PARAM_PATTERN_TYPE:
            return patternType;
        case PARAM_RANDOM_CHANCE:
            return randomChance;
        case PARAM_EUC_STEPS:
            return eucSteps;
        case PARAM_EUC_PULSE:
            return eucPulse;
        case PARAM_EUC_ROTATION:
            return eucRota;
        case PARAM_NTH:
            return nth;
        case PARAM_NTH_ROTATION:
            return nthRota;
        default:
            return 0;
    }
}

/**
  Change a parameter value.
  The host may call this function from any context, including realtime processing.
  When a parameter is marked as automable, you must ensure no non-realtime
  operations are performed.
*/
void PatternCV::setParameterValue(uint32_t index, float value)
{
    switch (index)
    {
    case PARAM_TIME:
        this->time = value;
        pattern.calcDelayTimeSamples(time);
        break;
    case PARAM_PATTERN_TYPE:
        this->patternType = value;
        pattern.setPattern(value);
        break;
    case PARAM_RANDOM_CHANCE:
        this->randomChance = value;
        pattern.setRandomChance(value);
        break;
    case PARAM_EUC_STEPS:
        this->eucSteps = value;
        if(patternType==1){ pattern.setPatternLength(value); }
        pattern.setEuclideanPatternLength(value);
        pattern.generateEuclideanSequence(value, eucPulse, eucRota);
        break;
    case PARAM_EUC_PULSE:
        this->eucPulse = value;
        pattern.generateEuclideanSequence(eucSteps, value, eucRota);
        break;
    case PARAM_EUC_ROTATION:
        this->eucRota = value;
        pattern.generateEuclideanSequence(eucSteps, eucPulse, value);
        break;
    case PARAM_NTH:
        this->nth = value;
        if(patternType==2){ pattern.setPatternLength(value); }
        pattern.setNthPatternLength(value);
        pattern.generateNthSequence(value, nthRota);
        break;
    case PARAM_NTH_ROTATION:
        this->nthRota = value;
        pattern.generateNthSequence(nth, value);
        break;
    default:
        break;
    }
}

// -------------------------------------------------------------------
// methods
// -------------------------------------------------------------------

// -------------------------------------------------------------------
// Process

void PatternCV::activate()
{
    // plugin is activated
}

void PatternCV::deactivate()
{
    // plugin is deactivated
}

/**
  Run/process function for plugins with MIDI input.
*/
void PatternCV::run(const float** inputs, float** outputs, uint32_t nframes)
{
    float* const cvOut = outputs[0];

    for (uint32_t i = 0; i < nframes; ++i)
    {
        cvOut[i] = pattern.getCurrentValue();

        pattern.tick();
    }
}

// -------------------------------------------------------------------
// callbacks

/**
    Optional callback to inform the plugin about a sample rate change.
    This function will only be called when the plugin is deactivated.
*/
void PatternCV::sampleRateChanged(double newSampleRate)
{
    sampleRate = newSampleRate;
}

// -----------------------------------------------------------------------

Plugin* createPlugin()
{
    return new PatternCV();
}

// -----------------------------------------------------------------------

END_NAMESPACE_DISTRHO
