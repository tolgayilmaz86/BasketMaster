#ifndef BASKETMASTER_H
#define BASKETMASTER_H

#include "gp_types.h"

class Menu;
class MainGame;

using namespace gameplay;

class GameBase : public Game
{
public:
    GameBase();

    virtual ~GameBase();

    void keyEvent(Keyboard::KeyEvent evt, gp_int key);


	void touchEvent(Touch::TouchEvent evt, gp_int x, gp_int y, gp_uint contactIndex);

	bool mouseEvent(Mouse::MouseEvent evt, gp_int x, gp_int y, gp_int wheelDelta);

	void gamepadEvent(Gamepad::GamepadEvent evt, Gamepad* gamepad);

    void menuEvent();

    static GameBase* getInstance();

protected:
    void initialize();

    void finalize();

    void update(gp_float elapsedTime);

    void render(gp_float elapsedTime);

private:

    void drawSplash(void* param);

    RenderState::StateBlock* _stateBlock;

    Menu*       _menu;
    MainGame*   _mainGame;
    Node*       _lightNode;
};
#endif // BASKETMASTER_H
