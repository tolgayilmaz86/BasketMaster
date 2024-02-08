#ifndef MENU_H
#define MENU_H

#include "gamescene.h"

using namespace gameplay;

class Menu : public GameScene, public Control::Listener
{
public:
    Menu();

    void initialize();

    void finalize();

    void update      (gp_float elapsedTime);

    void render      (gp_float elapsedTime);

    void touchEvent  (Touch::TouchEvent evt, gp_int x, gp_int y, gp_uint contactIndex);

    void keyEvent    (Keyboard::KeyEvent keyEvent, gp_int key);

    void controlEvent(Control* control, EventType evt);

    bool mouseEvent  (Mouse::MouseEvent evt, gp_int x, gp_int y, gp_int wheelDelta);

	void setVisibility(bool visibility);

    Form* getForm ();

    static Menu* getInstance();
protected:

    void onPlayClicked();

    void onOptionsClicked();

    void onStatsClicked();

    void onProfileClicked();

    void onExitClicked();

    void setSoundSetting(bool play);

    void setGamepadSetting(bool enable);

    ImageControl *addMenuButton(gp_cString text, gp_cString imagePath);

private:
    Font*           _font;
    gp_uint		    _keyFlags;
    gp_uint		    _headerSize;
    Form*           _mainMenuForm;
    SpriteBatch*    _background;
    ImageControl*   _playButton;
    ImageControl*   _exitButton;
    ImageControl*   _soundOnButton;
    ImageControl*   _soundOffButton;
    ImageControl*   _gamepadOnButton;
    ImageControl*   _gamepadOffButton;

    std::list<ImageControl*> _menuButtons;
};


#endif // MENU_H
