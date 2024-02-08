#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "gamebase.h"
#include "menu.h"
#include "maingame.h"
#include "gp_types.h"

#ifdef QT_CREATOR
    #include "qdebug.h"
#endif
/**
* Singleton pattern will be used alongside the project.
* there will be a need for communication between gui and game logic
*/
class Controller
{
public:
    static Controller* instance();

	void setMenuVisibility(bool visible){ _isMenuVisible = visible; }

	void setGamePlaying(bool playing){ _isGamePlaying = playing; }

    void setGamepadVisibility(bool visible);

    bool isMenuVisible() { return _isMenuVisible;}

	bool isGamePlaying() { return _isGamePlaying; }

    bool isGamepadVisible() { return _isGamepadVisible;}

    void setMenu(Menu *menu){if(_menu)return; _menu = menu;}

    void setGame(GameScene* game){if(_game)return; _game = game;}
    
	Menu* getMenu(){ return _menu;}

	GameScene* getGame(){ return _game; }

	Gamepad* getGamepad(){ return _gamepad; }

	void setGamepad(Gamepad* gamepad){ _gamepad = gamepad; }

    void initialize();

    void switchToMenu();

    void switchToGame();

    void setSoundEnabled(bool enable){_isSoundEnabled = enable;}

    bool isSoundEnabled(){return _isSoundEnabled;}

private:
    Controller();
    Controller(Controller const&);
    void operator=(Controller const&);
    bool _isMenuVisible;
    bool _isGamepadVisible;
    bool _isSoundEnabled;
	bool _isGamePlaying;
    GameScene* _game;
    Menu* _menu;
	Gamepad* _gamepad;
    static Controller* _instance;

public:
	static std::string gameHeader;
	static std::string playBtnPressedPath;
	static std::string playBtnNormalPath;
	static std::string exitBtnPressedPath;
	static std::string exitBtnNormalPath;
    static std::string soundOnBtnPath;
    static std::string soundOffBtnPath;
    static std::string gamepadOnBtnPath;
	static std::string gamepadOffBtnPath;
	static std::string menuBackgroundPath;
	static std::string backBtnNormalPath;
	static std::string backBtnPressedPath;
    static std::string gameHeaderFont;
    static std::string mainGameFont;
    static std::string mainGameScene;
    static std::string mainGameSound;
    static std::string mainMenuSound;
	static std::string defaultTheme;
	static std::string stadiumScene;
	static std::string powerIndicator;
	static std::string particleEmitter;
	static std::string upBar;
	static std::string upBarIndicator;
};

#endif // CONTROLLER_H
