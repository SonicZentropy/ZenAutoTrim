/*==============================================================================
   JuceHeader.h
   Part of the Zentropia JUCE Collection
   @author Casey Bailey (<a href="SonicZentropy@gmail.com">email</a>)
   @version 0.1
   @date 2016/01/12
   Copyright (C) 2016 by Casey Bailey
   Provided under the [GNU license]
 
   Details: Generic Zen_utils header stuff
 
   Zentropia is hosted on Github at [https: github.com/SonicZentropy]
===============================================================================*/

#ifndef ZEN_HEADER_H_INCLUDED
#define ZEN_HEADER_H_INCLUDED
#include "JuceHeader.h"
#include <atomic>

using namespace juce;

//#include <sstream>
//#include <atomic>
//#include <ctime>
//#include <map>
//#include <algorithm>
//#include <cmath>
//#include "boost/circular_buffer.hpp"
//#include "boost/optional.hpp"


//namespace Zen{
#include "utilities/ZenConstants.h"
#include "utilities/ZenDecibelConversions.hpp"
#include "utilities/ZenBinaryData.h"

#include "GUI/ZenTitleBar.h"
#include "GUI/ZenBypassButton.h"
#include "GUI/ZenMenuButton.h"
#include "GUI/ZenLookAndFeel.h"

#include "utilities/ZenUtils.hpp"
#include "utilities/ZenParamUtils.h"
#include "utilities/ZenTime.h"


#include "tests/ZenUnitTest.hpp"

#include "state/ZenAudioProcessorValueTreeState.h"

#include "parameters/ZenLinearSmoothedValue.h"
#include "parameters/ZenParameter.h"
#include "parameters/ZenBoolParameter.h"
#include "parameters/ZenFloatParameter.h"
#include "parameters/ZenDecibelParameter.h"


#include "components/ZenDecibelTextEditor.h"
#include "components/notepadcomp/NotepadComponent.h"
//#include "components/panslider/PanSlider.h"
#include "components/ZenLabelDisplay.h"
#include "components/ZenImageButton.h"
#include "components/ZenRotaryEditorLabel.h"
#include "components/ZenRotaryFilmStripSlider.h"
#include "components/ZenTextEditor.h"
#include "components/ZenComboBox.h"

#include "debug/GUI/buffer_visualiser.h"
#include "debug/GUI/component_debugger.h"
#include "debug/GUI/font_and_colour_designer.h"
#include "debug/GUI/ZenMidiVisualiserComponent.h"
#include "debug/GUI/value_tree_editor.h"

#include "debug/ZenDebugEditor.h"
#include "debug/ZenDebugPrinter.h"

#include "processing/RMSManager.h"

#include "processing/BufferSampleProcesses.h"


//#include "zen_utils.cpp"
//}

//using namespace Zen;
#endif //ZEN_HEADER_H_INCLUDED

/** GENERAL PROGRAM NOTES

//==============================================================================
Add to AppConfig.h: 
#ifndef ZEN_PLUGIN_CAN_RESIZE
//#define ZEN_PLUGIN_CAN_RESIZE 1
#endif
so presonus VST3 resize works properly.  This requires change in juce_VST3_Wrapper.cpp:

tresult PLUGIN_API canResize() override
{
// #ZEN(Added 2016/06/14): added this to fix studio one issue
#ifdef ZEN_PLUGIN_CAN_RESIZE
return kResultTrue;
#endif
return kResultFalse;
}
//==============================================================================


**/