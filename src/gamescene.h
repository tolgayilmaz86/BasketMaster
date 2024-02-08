#ifndef BASKETMASTER_H_
#define BASKETMASTER_H_

#include "gp_types.h"

using namespace gameplay;

class GameScene
{
public:
    friend class GameBase;

    static bool isVsync();
    static void setVsync(bool enable);
    static long getAbsoluteTime();
    static long getGameTime();
    Game::State getState() const;
    gp_int run();
    void pause();
    void resume();
    void exit();
    void frame();
    gp_uint getFrameRate() const;
    const Rectangle& getViewport() const;
    void setViewport(const Rectangle& viewport);
    gp_uint getWidth() const;
    gp_uint getHeight() const;
    gp_float getAspectRatio() const;
    void clear(Game::ClearFlags flags, const Vector4& clearColor, gp_float clearDepth, gp_int clearStencil);
    void clear(Game::ClearFlags flags, gp_float red, gp_float green, gp_float blue, gp_float alpha, gp_float clearDepth, gp_int clearStencil);
    AudioController* getAudioController() const;
    AnimationController* getAnimationController() const;
    PhysicsController* getPhysicsController() const;
    ScriptController* getScriptController() const;
    void displayKeyboard(bool display);

    virtual void keyEvent(Keyboard::KeyEvent evt, gp_int key);
    virtual void touchEvent(Touch::TouchEvent evt, gp_int x, gp_int y, gp_uint contactIndex);
    virtual bool mouseEvent(Mouse::MouseEvent evt, gp_int x, gp_int y, gp_int wheelDelta);

    bool hasMouse();
    AudioSource* getSound(){return _sound;}
    void disposeScene();
    bool isMouseCaptured();
    void setMouseCaptured(bool captured);
    void setMultiTouch(bool enabled);
    bool isMultiTouch() const;
    bool hasAccelerometer() const;
    void getAccelerometerValues(gp_float* pitch, gp_float* roll);
    void getRawSensorValues(gp_float* accelX, gp_float* accelY, gp_float* accelZ, gp_float* gyroX, gp_float* gyroY, gp_float* gyroZ);
    void schedule(long timeOffset, TimeListener* timeListener, void* cookie = 0);
    void enableScriptCamera(bool enable);
    void setScriptCameraSpeed(gp_float normal, gp_float fast);
    bool isGestureSupported(Gesture::GestureEvent evt);
    void registerGesture(Gesture::GestureEvent evt);
    void unregisterGesture(Gesture::GestureEvent evt);
    bool isGestureRegistered(Gesture::GestureEvent evt);

    virtual void gestureSwipeEvent(gp_int x, gp_int y, gp_int direction);
    virtual void gesturePinchEvent(gp_int x, gp_int y, gp_float scale);
    virtual void gestureTapEvent(gp_int x, gp_int y);
    virtual void gamepadEvent(Gamepad::GamepadEvent evt, Gamepad* gamepad);

    gp_uint getGamepadCount() const;
    Gamepad* getGamepad(gp_uint index, bool preferPhysical = true) const;

protected:
    GameScene();
    virtual ~GameScene();
    virtual void initialize() = 0;
    virtual void finalize() = 0;
    virtual void update(gp_float elapsedTime) = 0;
    virtual void render(gp_float elapsedTime) = 0;
    static void drawFrameRate(Font* font, const Vector4& color, gp_uint x, gp_uint y, gp_uint fps);

    AudioSource* _sound;

private:
    GameScene(const GameScene&);
    GameScene& operator=(const GameScene&);
};

#endif // BASKETMASTER_H_
