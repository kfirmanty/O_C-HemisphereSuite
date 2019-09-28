// Copyright (c) 2019, Karol Firmanty
//
// Based on HEM_Calculate by Jason Justian
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include "SegmentDisplay.h"
float punch_SUM(float v1, float v2) {
  return v1 + v2;
}
float punch_SUB(float v1, float v2) {
  return v1 - v2;
}
float punch_MULT(float v1, float v2) {
  return v1 * v2;
}
float punch_DIV(float v1, float v2) {
  return v1 / (v2 == 0 ? 0.001 : v2);
}
int punch_AND(int v1, int v2) {
  return v1 & v2;
}
int punch_OR(int v1, int v2) {
  return v1 | v2;
}
int punch_MOD(int v1, int v2) {
  return v1 % v2 == 0 ? 1 : v2;
}
int punch_XOR(int v1, int v2) {
  return v1 ^ v2;
}

typedef int(*PunchFunction)(int, int);
typedef float(*PunchFunctionF)(float, float);

class Punch : public HemisphereApplet {
  public:

    const char* applet_name() {
      return "PunchEff";
    }

    void Start() {
      segment.Init(SegmentSize::BIG_SEGMENTS);
    }

    float normalize(int v) {
      return (((float)v) / HEMISPHERE_MAX_CV) - 1.0;
    }

    float fold(float v) {
      float rounded = round(0.25 * v + 0.25);
      return 4.0 * (abs(0.25 * v + 0.25 - rounded) - 0.25);
    }

    void Controller() {
      punchIndex = (Gate(0) ? 2 : 0 ) + (Gate(1) ? 1 : 0);
      int i1t = In(0);
      int i2t = In(1);
      in[0] = normalize(i1t);
      in[1] = normalize(i2t);
      ForEachChannel(ch) {
        if (punchIndex < 2) {
          out[ch] = fold(punch_fnf[(punchIndex * 2) + ch](in[0], in[1])) * HEMISPHERE_MAX_CV;
        } else {
          out[ch] = punch_fn[(punchIndex - 2) + ch](i1t, i2t);
        }
        Out(ch, out[ch]);
      }
    }

    void View() {
      gfxHeader(applet_name());
      DrawDisplay();
    }

    void OnButtonPress() {
    }

    void OnEncoderMove(int direction) {
    }

    uint32_t OnDataRequest() {
      uint32_t data = 0;
      return data;
    }

    void OnDataReceive(uint32_t data) {
    }

  protected:
    void SetHelp() {
      //                               "------------------" <-- Size Guide
      help[HEMISPHERE_HELP_DIGITALS] = "1=Bit 1 2=Bit 0";
      help[HEMISPHERE_HELP_CVS]      = "1=CV1 2=CV2";
      help[HEMISPHERE_HELP_OUTS]     = "A=Op1 B=Op2";
      help[HEMISPHERE_HELP_ENCODER]  = "";
      //                               "------------------" <-- Size Guide
    }

  private:
    float in[2];
    int out[2];
    int punchIndex;
    SegmentDisplay segment;
    const char* op_name[8] = {"Sum", "Sub", "Mult", "Div", "And", "Or", "Mod", "Xor"};
    PunchFunction punch_fn[4] = {punch_AND, punch_OR, punch_MOD, punch_XOR};
    PunchFunctionF punch_fnf[4] = {punch_SUM, punch_SUB, punch_MULT, punch_DIV};

    void DrawDisplay() {
      gfxPrint(0, 15, op_name[punchIndex * 2]);
      gfxPrint(0, 25, in[0] * 100);
      gfxPrint(0, 35, in[1] * 100);
      gfxPrint(0, 45, out[0]);
      gfxPrint(0, 55, out[1]);
    }
};


////////////////////////////////////////////////////////////////////////////////
//// Hemisphere Applet Functions
///
///  Once you run the find-and-replace to make these refer to Punch,
///  it's usually not necessary to do anything with these functions. You
///  should prefer to handle things in the HemisphereApplet child class
///  above.
////////////////////////////////////////////////////////////////////////////////
Punch Punch_instance[2];

void Punch_Start(bool hemisphere) {
  Punch_instance[hemisphere].BaseStart(hemisphere);
}

void Punch_Controller(bool hemisphere, bool forwarding) {
  Punch_instance[hemisphere].BaseController(forwarding);
}

void Punch_View(bool hemisphere) {
  Punch_instance[hemisphere].BaseView();
}

void Punch_OnButtonPress(bool hemisphere) {
  Punch_instance[hemisphere].OnButtonPress();
}

void Punch_OnEncoderMove(bool hemisphere, int direction) {
  Punch_instance[hemisphere].OnEncoderMove(direction);
}

void Punch_ToggleHelpScreen(bool hemisphere) {
  Punch_instance[hemisphere].HelpScreen();
}

uint32_t Punch_OnDataRequest(bool hemisphere) {
  return Punch_instance[hemisphere].OnDataRequest();
}

void Punch_OnDataReceive(bool hemisphere, uint32_t data) {
  Punch_instance[hemisphere].OnDataReceive(data);
}
