#ifndef _H__
#define _H__

// framework
#include "DistrhoPlugin.hpp"

// classes
#include "patternGenerator.hpp"

// libs
#include <cstdint>
#include <iostream>

// constants

START_NAMESPACE_DISTRHO

/**
  Your plugin class that subclases the base DPF Plugin one.
*/
class PatternCV : public Plugin
{
public:
    enum Parameters {
        PARAM_TIME,
        PARAM_PATTERN_TYPE,
        PARAM_RANDOM_CHANCE,
        PARAM_EUC_STEPS,
        PARAM_EUC_PULSE,
        PARAM_EUC_ROTATION,
        PARAM_NTH,
        PARAM_NTH_ROTATION,
        PARAM_COUNT
    };

    /**
      Plugin class constructor.
      You must set all parameter values to their defaults, matching the value in initParameter().
    */
    PatternCV();

protected:
    // -------------------------------------------------------------------
    // Information

    /**
      Get the plugin label.
      This label is a short restricted name consisting of only _, a-z, A-Z and 0-9 characters.
    */
    const char* getLabel() const noexcept override { return "Pattern CV Plugin"; }

    /**
      Get an extensive comment/description about the plugin.
      Optional, returns nothing by default.
    */
    const char* getDescription() const override { return "SvE Pattern CV Plugin."; }

    /**
      Get the plugin author/maker.
    */
    const char* getMaker() const noexcept override { return "SvE plugins"; }

    /**
      Get the plugin license (a single line of text or a URL).
      For commercial plugins this should return some short copyright information.
    */
    const char* getLicense() const noexcept override { return "ISC"; }

    /**
      Get the plugin version, in hexadecimal.
      @see d_version()
    */
    uint32_t getVersion() const noexcept override { return d_version(0, 0, 0); }

    /**
      Get the plugin unique Id.
      This value is used by LADSPA, DSSI and VST plugin formats.
      @see d_cconst()
    */
    int64_t getUniqueId() const noexcept override { return d_cconst('P', 'A', 'C', 'V'); }

    // -------------------------------------------------------------------
    // Init

    /**
          Initialize the audio port @a index.@n
          This function will be called once, shortly after the plugin is created.
    */
    void initAudioPort(bool input, uint32_t index, AudioPort& port) override;

    /**
      Initialize the parameter @ index.
      This function will be called once, shortly after the plugin is created.
    */
    void initParameter(uint32_t index, Parameter& parameter) override;

    // -------------------------------------------------------------------
    // Internal data

    /**
      Get the current value of a parameter.
      The host may call this function from any context, including realtime processing.
    */
    float getParameterValue(uint32_t index) const override;

    /**
      Change a parameter value.
      The host may call this function from any context, including realtime processing.
      When a parameter is marked as automable, you must ensure no non-realtime
      operations are performed.
    */
    void setParameterValue(uint32_t index, float value) override;

    // -------------------------------------------------------------------
    // methods
    // -------------------------------------------------------------------

    // -------------------------------------------------------------------
    // Process

    void activate() override;

    void deactivate() override;

    /**
      Run/process function for plugins with MIDI input.
    */
    void run(const float**, float**, uint32_t nframes) override;

    // -------------------------------------------------------------------
    // callbacks

    /**
        Optional callback to inform the plugin about a sample rate change.
        This function will only be called when the plugin is deactivated.
    */
    void sampleRateChanged(double newSampleRate) override;

private:
    // variables
    float time{500};
    float sampleRate{getSampleRate()};
    uint32_t patternType{0};
    uint32_t randomChance{50};
    uint32_t eucSteps{0}, eucPulse{0}, eucRota{0};
    uint32_t nth{0}, nthRota{0};

    // objects
    PatternGenerator pattern;
    /**
        Set our plugin class as non-copyable and add a leak detector just in case.
    */
    DISTRHO_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PatternCV)
};

END_NAMESPACE_DISTRHO

#endif // _H__
