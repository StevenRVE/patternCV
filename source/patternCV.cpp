#include "patternCV.hpp"

START_NAMESPACE_DISTRHO

   /**
      Plugin class constructor.
      You must set all parameter values to their defaults, matching the value in initParameter().
    */
PatternCV::PatternCV()
    : Plugin(PARAM_COUNT, 0, 0) // parameters, programs, states
{
    /**
      Initialize all our parameters to their defaults.
      In this example all parameters have 0 as default, so we can simply zero them.
    */
}


// -------------------------------------------------------------------
// Init
// -------------------------------------------------------------------

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

    const auto createList = []
    {
        ParameterEnumerationValue* const list = new ParameterEnumerationValue[2];
        list[0].label = "Item 0";
        list[0].value = 0;
        list[1].label = "Item 1";
        list[1].value = 1;
        return list;
    };

    switch (index)
    {
        // MASTER PARAMS
        case PARAM_GAIN:
            setParamProps(parameter, { .min=-0.0f, .max=2.0f, .def=1.0f, .name="Gain", .symbol="gain" });
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
        case PARAM_GAIN:
            return gain;
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
    // MASTER PARAMS
    case PARAM_GAIN:
        gain = value;
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
void PatternCV::run(const float**, float**, uint32_t nframes)
{
    // run
}

// -------------------------------------------------------------------
// callbacks

/**
    Optional callback to inform the plugin about a sample rate change.
    This function will only be called when the plugin is deactivated.
*/
void PatternCV::sampleRateChanged(double newSampleRate)
{
    (void)newSampleRate;
}

// -----------------------------------------------------------------------

Plugin* createPlugin()
{
    return new PatternCV();
}

// -----------------------------------------------------------------------

END_NAMESPACE_DISTRHO