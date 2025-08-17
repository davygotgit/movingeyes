//
//  This sketch uses an M5Stack Core2 (or CoreS3) connected to a gesture
//  unit, to move a pair of eyes around the screen.
//
//  License:  MIT. See the LICENSE file in the project root for
//            more details.
//

//  M5Stack headers
#include <M5Unified.h>
#include <M5GFX.h>

//  Header for the gesture unit
#include <DFRobot_PAJ7620U2.h>

//  The number of pixels used when the eye is
//  looking around
const int   lookSize        = 5;

//  Number of different eye sizes
const int   numEyeSizes     = 3;

//  Array indexes for eye sizes
const int   smallEyeIndex   = 0;
const int   mediumEyeIndex  = 1;
const int   largeEyeIndex   = 2;

//  Values for the eye orbit
const int   totalDegrees    = 360 - 90;
const int   orbitAngle      = 45;

//  Ratios of various components
const float smallEyeRatio   = 0.20F;
const float mediumEyeRatio  = 0.30F;
const float largeEyeRatio   = 0.48F;
const float irisRatio       = 0.70F;
const float pupilRatio      = 0.40F;
const float orbitRatio      = 0.25F;

//  Amount of time between eye roll positions
const int   eyeRollTime     = 150;

//  Button sizing
const int   buttonSize      = 50;
const int   halfBtnSize     = buttonSize / 2;


//  The eyes are symmetrical, so we only need one sprite to
//  hold the eyeball, iris and pupil. We can output the
//  sprite at two different locations.
//
//  Using sprites prevents a lot of screen flicker
M5Canvas eyeSprite;

//  We allow 3 different eye sizes which are a percentage of
//  the screen width
int eyeSizes  [numEyeSizes];

//  Current eye radius
int eyeRadius;

//  Current eye size (medium)
int eyeSize = 1;

//  Sizes of the current iris and pupil, 
//  which are a percentage of the full
//  eye size
int irisSize;
int irisRadius;
int pupilSize;
int pupilRadius;

//  X and Y positions for the eye sprites
int leftRegionX;
int rightRegionX;
int regionY;

//  The X and Y position of the iris and pupil, 
//  which are relative to the region
int irisPupilX;
int irisPupilY;

//  Displacements from the center of the eye
//  which are used to make the eye move when
//  looking
int eyeXDisp;
int eyeYDisp;
int lastEyeXDisp;
int lastEyeYDisp;

//  Co-ordinates for the left and right
//  eye orbits. This makes the eyes look
//  like they are rolling
typedef struct orbitXY
{
  int x;
  int y;
} orbitXY_t;

//  Orbits for the left and right eyes which
//  are calculated in setup()
orbitXY_t orbits  [totalDegrees / orbitAngle];

//  Define color buttons to change eye color
const int   buttonColors    []  = {BLUE, BROWN, DARKGREEN, YELLOW};
const int   numColors           = sizeof(buttonColors) / sizeof (buttonColors [0]);
LGFX_Button colorButtons    [4];
int         eyeColor            = buttonColors [0];
int         lastButton          = 0;

//  Gesture sensor
DFRobot_PAJ7620U2 gestureSensor;

//  Calculate various sizes for the eyes
void CalculateEyeSizes (void)
{
  //  Get the radius of the eye
  eyeRadius   = eyeSizes [eyeSize] / 2;

  //  The iris and pupil are a percentage of the eye size
  irisSize    = eyeSizes [eyeSize] * irisRatio;
  pupilSize   = eyeSizes [eyeSize] * pupilRatio;

  //  Now set radius
  irisRadius  = irisSize / 2;
  pupilRadius = pupilSize / 2;

  //  Calculate the oribit of the rolling eye
  for (int i = 0; i < totalDegrees; i += orbitAngle)
  {
    //  Size of the orbit
    int orbitSize = eyeRadius * orbitRatio;
    
    //  Eye orbit
    double orbitX = orbitSize * cos(i);
    double orbitY = orbitSize * sin(i);

    //  Current orbit element
    int orbitElement = i / orbitAngle;

    //  Cache the orbit information
    orbits [orbitElement].x = orbitX;
    orbits [orbitElement].y = orbitY;
  }
}


//  Draw the eyeball
void DrawEyeBall (const int color)
{
  //  The eyeball is drawn in the sprite
  eyeSprite.fillCircle(irisPupilX, irisPupilY, eyeRadius, color);
  
  //  The eyeballs are placed on the screen
  eyeSprite.pushSprite(&M5.Lcd, leftRegionX, regionY);
  eyeSprite.pushSprite(&M5.Lcd, rightRegionX, regionY);
}


//  Draw the iris and pupil at their current location
void DrawIrisPupil (void)
{
  //  Clear any old position
  eyeSprite.fillCircle(irisPupilX + lastEyeXDisp, irisPupilY + lastEyeYDisp, irisRadius, WHITE);

  //  Draw new  iris and pupil
  eyeSprite.fillCircle(irisPupilX + eyeXDisp, irisPupilY + eyeYDisp, irisRadius, eyeColor);
  eyeSprite.fillCircle(irisPupilX + eyeXDisp, irisPupilY + eyeYDisp, pupilRadius, BLACK);

  //  Output the eye sprites to the screen
  eyeSprite.pushSprite(&M5.Lcd, leftRegionX, regionY);
  eyeSprite.pushSprite(&M5.Lcd, rightRegionX, regionY);

  //  Track last X and Y position
  lastEyeXDisp = eyeXDisp;
  lastEyeYDisp = eyeYDisp;
}


//  Resize eyes
void ResizeEyes (const int newSize)
{
  //  Clear out the old eyes
  DrawEyeBall(BLACK);

  //  Set new size and calculate 
  eyeSize = newSize;
  CalculateEyeSizes();

  //  Re-draw eyeball
  DrawEyeBall(WHITE);

  //  Display the new eye
  DrawIrisPupil();
}


//  Make it look like the eyes are rolling 
//  in a left direction
void RollEyesLeft (void)
{
  //  Loop through pre-computed orbits
  for (int i = (totalDegrees / orbitAngle) - 1; i >= 0; i --)
  {
    //  Set the X and Y displacements from the orbits
    //  we calculated
    eyeXDisp = orbits [i].x;
    eyeYDisp = orbits [i].y;

    //  Draw the new eye position
    DrawIrisPupil();

    delay(eyeRollTime);
  }

  //  Reset the eye to the center
  eyeXDisp = 0;
  eyeYDisp = 0;
  DrawIrisPupil();
}


//  Make it look like the eyes are rolling 
//  in a right direction
void RollEyesRight (void)
{
  //  Loop through pre-computed orbits
  for (int i = 0; i < totalDegrees / orbitAngle; i ++)
  {
    //  Set the X and Y displacements from the orbits
    //  we calculated
    eyeXDisp = orbits [i].x;
    eyeYDisp = orbits [i].y;

    //  Draw the new eye position
    DrawIrisPupil();

    delay(eyeRollTime);
  }

  //  Reset the eye to the center
  eyeXDisp = 0;
  eyeYDisp = 0;
  DrawIrisPupil();
}


void setup (void) 
{
  M5.begin();

  //  Configure display
  M5.Display.setRotation(0);

  //  Initialize the gesture sensor
  while (gestureSensor.begin() != 0)
  {
      //  Give some indication we are working on something
      M5.Lcd.print(".");
      delay(500);
  }

  //  Clear the screen
  M5.Lcd.fillScreen(BLACK);

  //  Enable gesture detection
  gestureSensor.setGestureHighRate(true);

  //  Get screen dimensions
  int screenWidth   = M5.Lcd.width();
  int screenHeight  = M5.Lcd.height();

  //  We have 3 eye sizes - small, medium and large 
  //  which are percentages are of the screen width
  eyeSizes [0] = screenWidth * smallEyeRatio;
  eyeSizes [1] = screenWidth * mediumEyeRatio;
  eyeSizes [2] = screenWidth * largeEyeRatio;

  //  We use region to hold the two eye balls. This
  //  prevents too much screen flicker. Calculate
  //  the X and Y of the regions
  leftRegionX   = 0;
  rightRegionX  = screenWidth - eyeSizes [2];
  regionY       = (screenHeight / 2) - (eyeSizes [2] / 2);

  //  Create the eye sprite
  eyeSprite.createSprite(eyeSizes [2], eyeSizes [2]);

  //  Calculate the sizes of the eyes
  CalculateEyeSizes();

  //  Set X and Y position of the iris and pupil which
  //  will be center of the regions
  irisPupilX    = eyeSizes [2] / 2;
  irisPupilY    = eyeSizes [2] / 2;

  //  Initial X and Y displacements are 0 (centered)
  eyeXDisp      = 0;
  eyeYDisp      = 0;
  lastEyeXDisp  = 0;
  lastEyeYDisp  = 0;

  //  Set X and Y position for color buttons
  const int buttonsOnX  = screenWidth / numColors;
  int       colorX      = halfBtnSize;
  int       colorY      = screenHeight - halfBtnSize;

  //  Add color buttons
  for (int i = 0; i < numColors; i ++)
  {
    //  The default color is the red button, which we highlight
    int outlineColor = (i == 0) ? WHITE : buttonColors [i];

    //  Get the other button colors
    int color = buttonColors [i];

    //  Create the button
    colorButtons [i].initButton(&M5.Lcd, colorX, colorY, buttonSize, buttonSize, outlineColor, color, color, " ", 4, 4);
    colorButtons [i].drawButton();

    //  Next X position
    colorX += buttonsOnX;
  }

  //  Draw the eyeball (this position does not change) unless
  //  the eye is resized
  DrawEyeBall(WHITE);

  //  Draw a centered iris and pupil
  DrawIrisPupil();
}

void loop (void) 
{
  M5.update();
 
  //  Get information about where the user touched the screen
  auto touchDetail = M5.Touch.getDetail();

  if (touchDetail.isPressed())
  {
    //  Check color buttons
    for (int i = 0; i < numColors; i ++)
    {
      //M5.Lcd.printf("%d ", touchDetail.x);
      if (colorButtons [i].contains(touchDetail.x, touchDetail.y))
      {
        //  Ignore multiple presses on the same button
        if (i == lastButton)
        {
          return;
        }

        //  De-select the last button
        colorButtons [lastButton].setOutlineColor(buttonColors [lastButton]);
        colorButtons [lastButton].drawButton();

        //  Select this button
        colorButtons [i].setOutlineColor(WHITE);
        colorButtons [i].drawButton();

        //  Set the eye color and last button pressed
        lastButton  = i;
        eyeColor    = buttonColors [i];

        //  Re-draw the iris with the new color
        DrawIrisPupil();

        //  We are done
        return;
      }
    }
  }

  //  Check for gestures
  DFRobot_PAJ7620U2::eGesture_t gesture = gestureSensor.getGesture();

  //  See what we need to do with the eyes
  switch (gesture)
  {
    case gestureSensor.eGestureLeft:
      //  Change the X displacement to look left
      eyeXDisp = -lookSize;
      eyeYDisp = 0;
      DrawIrisPupil();
      break;

    case gestureSensor.eGestureRight:
      //  Change the X displacement to look right
      eyeXDisp = lookSize;
      eyeYDisp = 0;
      DrawIrisPupil();
      break;

    case gestureSensor.eGestureUp:
      //  Change the Y displacement to look up
      eyeXDisp = 0;
      eyeYDisp = -lookSize;
      DrawIrisPupil();
      break;

    case gestureSensor.eGestureDown:
      //  Change the Y displacement to look down
      eyeXDisp = 0;
      eyeYDisp = lookSize;
      DrawIrisPupil();
      break;

    case gestureSensor.eGestureClockwise:
      RollEyesRight();
      break;

    case gestureSensor.eGestureAntiClockwise:
      RollEyesLeft();
      break;

    case gestureSensor.eGestureBackward:
      if (eyeSize != 0)
      {
        //  Go one size smaller
        eyeSize --;
        ResizeEyes(eyeSize);
      }
      break;

    case gestureSensor.eGestureForward:
      if (eyeSize != numEyeSizes - 1)
      {
        //  Go one size larger
        eyeSize ++;
        ResizeEyes(eyeSize);
      }
      break;
  }
}
