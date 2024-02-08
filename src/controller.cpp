#include "controller.h"

#ifndef debug
    #define debug qDebug()<<__FUNCTION__<<__LINE__
#endif

#define KEY_A_MASK      (1 << 0)
#define KEY_B_MASK      (1 << 1)
#define KEY_SELECT_MASK (1 << 2)

Controller* Controller::_instance           = NULL;
std::string Controller::gameHeader          = "BASKET MASTER";
std::string Controller::playBtnNormalPath	= "res/png/btn_play_normal.png";
std::string Controller::playBtnPressedPath	= "res/png/btn_play_pressed.png";
std::string Controller::exitBtnNormalPath	= "res/png/btn_exit_normal.png";
std::string Controller::exitBtnPressedPath	= "res/png/btn_exit_pressed.png";
std::string Controller::backBtnNormalPath	= "res/png/btn_back_normal.png";
std::string Controller::backBtnPressedPath	= "res/png/btn_back_pressed.png";
std::string Controller::soundOnBtnPath      = "res/png/btn_sound_on.png";
std::string Controller::soundOffBtnPath     = "res/png/btn_sound_off.png";
std::string Controller::gamepadOnBtnPath    = "res/png/btn_gamepad_on.png";
std::string Controller::gamepadOffBtnPath   = "res/png/btn_gamepad_off.png";
std::string Controller::menuBackgroundPath	= "res/png/background.png";
std::string Controller::gameHeaderFont      = "res/common/baveuse3.gpb";
std::string Controller::mainGameFont        = "res/common/airstrip28.gpb";
std::string Controller::mainGameScene       = "res/design/basketmaster.scene";
std::string Controller::defaultTheme		= "res/common/default.theme";
std::string Controller::mainGameSound       = "res/common/game_background_track.ogg";
std::string Controller::mainMenuSound       = "res/common/menu_background_track.ogg";
std::string Controller::stadiumScene		= "res/design/basketmaster.gpb";
std::string Controller::powerIndicator		= "res/png/firetile.png";
std::string Controller::particleEmitter		= "res/common/fire.particle";
std::string Controller::upBar				= "res/png/bar.png";
std::string Controller::upBarIndicator		= "res/png/indicator.png";


Controller::Controller() : _isMenuVisible(false), _isGamepadVisible(false),
                           _menu(NULL), _game(NULL), _isSoundEnabled(true)
{
}

Controller *Controller::instance()
{
	if(_instance == NULL){
		_instance = new Controller;
	}
	return _instance;
}

void Controller::initialize()
{
}

void Controller::switchToMenu()
{
	setGamePlaying(false);
    //_game->getSound()->stop();
    _game->pause();
    setMenuVisibility(true);
	_menu->setVisibility(true);
}

void Controller::switchToGame()
{
    _menu->getSound()->stop();
    setMenuVisibility(false);
	setGamePlaying(true);
    //_menu->getForm()->setVisible(false);
    //_menu->getForm()->setEnabled(false);

	_gamepad = _game->getGamepad(0, _isGamepadVisible);
	if (_gamepad && _gamepad->isVirtual())
    {
		_gamepad->getForm()->setEnabled(_isGamepadVisible);
    }

    ((MainGame*)_game)->initialize();
	((MainGame*)_game)->getGameForm()->setEnabled(true);
    _game->resume();
}

void Controller::setGamepadVisibility(bool visible){
    _isGamepadVisible = visible;
}
