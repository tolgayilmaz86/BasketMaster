#include "gamescene.h"
#include "gamebase.h"
#include "controller.h"

GameScene::GameScene()
{
}

GameScene::~GameScene()
{
    enableScriptCamera(false);
}

bool GameScene::isVsync()
{
    return Game::getInstance()->isVsync();
}

void GameScene::setVsync(bool enable)
{
    Game::getInstance()->setVsync(enable);
}

long GameScene::getAbsoluteTime()
{
    return Game::getInstance()->getAbsoluteTime();
}

long GameScene::getGameTime()
{
    return Game::getInstance()->getGameTime();
}

Game::State GameScene::getState() const
{
    return Game::getInstance()->getState();
}

gp_int GameScene::run()
{
    return Game::getInstance()->run();
}

void GameScene::pause()
{
    Game::getInstance()->pause();
}

void GameScene::resume()
{
    Game::getInstance()->resume();
}

void GameScene::exit()
{
    Game::getInstance()->exit();
}

void GameScene::frame()
{
    Game::getInstance()->frame();
}

gp_uint GameScene::getFrameRate() const
{
    return Game::getInstance()->getFrameRate();
}

const Rectangle& GameScene::getViewport() const
{
    return Game::getInstance()->getViewport();
}

void GameScene::setViewport(const Rectangle& viewport)
{
    Game::getInstance()->setViewport(viewport);
}

gp_uint GameScene::getWidth() const
{
    if(Game::getInstance() == NULL)
        return 1024;
    return Game::getInstance()->getWidth();
}

unsigned int GameScene::getHeight() const
{
    if(Game::getInstance() == NULL)
        return 640;
    return Game::getInstance()->getHeight();
}

float GameScene::getAspectRatio() const
{
    return Game::getInstance()->getAspectRatio();
}

void GameScene::clear(Game::ClearFlags flags, const Vector4& clearColor, gp_float clearDepth, gp_int clearStencil)
{
    Game::getInstance()->clear(flags, clearColor, clearDepth, clearStencil);
}

void GameScene::clear(Game::ClearFlags flags, gp_float red, gp_float green, gp_float blue, gp_float alpha, gp_float clearDepth, gp_int clearStencil)
{
    Game::getInstance()->clear(flags, red, green, blue, alpha, clearDepth, clearStencil);
}

AudioController* GameScene::getAudioController() const
{
    return Game::getInstance()->getAudioController();
}

AnimationController* GameScene::getAnimationController() const
{
    return Game::getInstance()->getAnimationController();
}

PhysicsController* GameScene::getPhysicsController() const
{
    return Game::getInstance()->getPhysicsController();
}

ScriptController* GameScene::getScriptController() const
{
    return Game::getInstance()->getScriptController();
}

void GameScene::displayKeyboard(bool display)
{
    Game::getInstance()->displayKeyboard(display);
}

void GameScene::keyEvent(Keyboard::KeyEvent evt, gp_int key)
{
}

void GameScene::touchEvent(Touch::TouchEvent evt, gp_int x, gp_int y, gp_uint contactIndex)
{
}

bool GameScene::mouseEvent(Mouse::MouseEvent evt, gp_int x, gp_int y, gp_int wheelDelta)
{
    return false;
}

bool GameScene::hasMouse()
{
    return Game::getInstance()->hasMouse();
}

void GameScene::disposeScene()
{
    this->finalize();
}

bool GameScene::isMouseCaptured()
{
    return Game::getInstance()->isMouseCaptured();
}

void GameScene::setMouseCaptured(bool captured)
{
    Game::getInstance()->setMouseCaptured(captured);
}

void GameScene::setMultiTouch(bool enabled)
{
    Game::getInstance()->setMultiTouch(enabled);
}

bool GameScene::isMultiTouch() const
{
    return Game::getInstance()->isMultiTouch();
}

bool GameScene::hasAccelerometer() const
{
    return Game::getInstance()->hasAccelerometer();
}

void GameScene::getAccelerometerValues(gp_float* pitch, gp_float* roll)
{
    Game::getInstance()->getAccelerometerValues(pitch, roll);
}

void GameScene::getRawSensorValues(gp_float* accelX, gp_float* accelY, gp_float* accelZ, gp_float* gyroX, gp_float* gyroY, gp_float* gyroZ)
{
    //Game::getInstance()->getRawSensorValues(accelX, accelY, accelZ, gyroX, gyroY, gyroZ);
}

void GameScene::schedule(long timeOffset, TimeListener* timeListener, void* cookie)
{
    Game::getInstance()->schedule(timeOffset, timeListener, cookie);
}

void GameScene::enableScriptCamera(bool enable)
{
    Game::getInstance()->getScriptController()->executeFunction<void>("camera_setActive", "b", enable);
}

void GameScene::setScriptCameraSpeed(gp_float normal, gp_float fast)
{
    Game::getInstance()->getScriptController()->executeFunction<void>("camera_setSpeed", "ff", normal, fast);
}

bool GameScene::isGestureSupported(Gesture::GestureEvent evt)
{
    return Game::getInstance()->isGestureSupported(evt);
}

void GameScene::registerGesture(Gesture::GestureEvent evt)
{
    Game::getInstance()->registerGesture(evt);
}

void GameScene::unregisterGesture(Gesture::GestureEvent evt)
{
    Game::getInstance()->unregisterGesture(evt);
}

bool GameScene::isGestureRegistered(Gesture::GestureEvent evt)
{
    return Game::getInstance()->isGestureRegistered(evt);
}

void GameScene::gestureSwipeEvent(gp_int x, gp_int y, gp_int direction)
{
}

void GameScene::gesturePinchEvent(gp_int x, gp_int y, gp_float scale)
{
}

void GameScene::gestureTapEvent(gp_int x, gp_int y)
{
}

void GameScene::gamepadEvent(Gamepad::GamepadEvent evt, Gamepad* gamepad)
{
}

unsigned int GameScene::getGamepadCount() const
{
    return Game::getInstance()->getGamepadCount();
}

Gamepad* GameScene::getGamepad(gp_uint index, bool preferPhysical) const
{
    return Game::getInstance()->getGamepad(index, preferPhysical);
}

void GameScene::drawFrameRate(Font* font, const Vector4& color, gp_uint x, gp_uint y, gp_uint fps)
{
    char buffer[10];
    sprintf(buffer, "%u", fps);
    font->start();
    font->drawText(buffer, x, y, color, font->getSize());
    font->finish();
}
