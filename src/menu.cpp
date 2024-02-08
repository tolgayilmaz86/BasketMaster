#include "menu.h"
#include "controller.h"

Menu menu;

using std::string;
using std::pair;

Menu::Menu() :  _keyFlags(0), _font(NULL), _mainMenuForm(NULL),
				_playButton(NULL),		_exitButton(NULL),
				_soundOnButton(NULL),	_soundOffButton(NULL),
				_gamepadOnButton(NULL), _gamepadOffButton(NULL)
{
}

void Menu::finalize()
{
    SAFE_RELEASE(_font);
    SAFE_RELEASE(_mainMenuForm);
    SAFE_RELEASE(_sound);
    SAFE_RELEASE(_playButton);
    SAFE_RELEASE(_exitButton);
    SAFE_RELEASE(_soundOnButton);
    SAFE_RELEASE(_soundOffButton);
    SAFE_RELEASE(_gamepadOnButton);
    SAFE_RELEASE(_gamepadOffButton);

    _menuButtons.clear();
}

void Menu::initialize()
{
    _headerSize = 96;
    _menuButtons.clear();
    setMultiTouch(false);

    _font = Font::create(Controller::gameHeaderFont.c_str());
    _sound = AudioSource::create(Controller::mainMenuSound.c_str());
    if (_sound)
    {
        _sound->setLooped(true);
		_sound->setGain(0.3f);
		if(Controller::instance()->isSoundEnabled())
            _sound->play();
    }

    setVsync(true);
    setMultiTouch(false);
    Controller::instance()->setGamepadVisibility(false);

    Theme* theme = Theme::create("res/common/default.theme");
    Theme::Style* formStyle = theme->getEmptyStyle();
    _background     = SpriteBatch::create(Controller::menuBackgroundPath.c_str());
    _mainMenuForm = Form::create("MainMenu", formStyle, Layout::LAYOUT_ABSOLUTE);
    theme->release();

    gp_uint h = getHeight();
    gp_uint w = getWidth();

    _mainMenuForm->setHeight(h);
    _mainMenuForm->setWidth(w);
    _mainMenuForm->setFont(_font);
    _mainMenuForm->setPosition(0, 0);
    _mainMenuForm->setScroll(Container::SCROLL_NONE);
    _mainMenuForm->setConsumeInputEvents(true);

    _playButton       = addMenuButton("PLAY", Controller::playBtnNormalPath.c_str());
    _exitButton       = addMenuButton("EXIT", Controller::exitBtnNormalPath.c_str());
    _soundOnButton    = addMenuButton("SOUND_ON",Controller::soundOnBtnPath.c_str());
    _soundOffButton   = addMenuButton("SOUND_OFF",Controller::soundOffBtnPath.c_str());
    _gamepadOnButton  = addMenuButton("GAMEPAD_ON",Controller::gamepadOnBtnPath.c_str());
    _gamepadOffButton = addMenuButton("GAMEPAD_OFF",Controller::gamepadOffBtnPath.c_str());

    _soundOffButton->setVisible(false);
    _gamepadOffButton->setVisible(false);
	Controller::instance()->setGamepadVisibility(true);

    _playButton->setPosition(w/2, h/2);
    _exitButton->setPosition(w-_exitButton->getWidth(),
                             h-_exitButton->getHeight());
    _soundOnButton->setPosition(200, h-_soundOnButton->getHeight());
    _soundOffButton->setPosition(200,h-_soundOffButton->getHeight());

    _gamepadOnButton->setPosition(w/2-_gamepadOnButton->getWidth(),
                                  h-_gamepadOnButton->getHeight());
    _gamepadOffButton->setPosition(w/2-_gamepadOffButton->getWidth(),
                                   h-_gamepadOffButton->getHeight());

    //_mainMenuForm->setState(Control::ACTIVE);//This should be deprecated!
    _mainMenuForm->addListener(this,Control::Listener::PRESS |
                                    Control::Listener::RELEASE);
}

ImageControl* Menu::addMenuButton(gp_cString text, gp_cString imagePath)
{
    Theme* theme = Theme::create("res/common/default.theme");
    Theme::Style* buttonStyle =  theme->getEmptyStyle();//Theme::create("res/common/default.theme")->getStyle("buttonStyle");
    ImageControl* button = ImageControl::create(text, buttonStyle);
    button->setImage(imagePath);
    button->setWidth(140);
    button->setHeight(140);
    button->addListener(this, Control::Listener::PRESS | Control::Listener::RELEASE);
    _mainMenuForm->addControl(button);
    _menuButtons.push_back(button);
	theme->release();
    return button;
}

void Menu::update(gp_float elapsedTime)
{
	//if(!Controller::instance()->isMenuVisible()) return;
    _mainMenuForm->update(elapsedTime);
}

void Menu::render(gp_float elapsedTime)
{
    clear(Game::CLEAR_COLOR_DEPTH, Vector4::zero(), 1.0f, 0);
	//if(!Controller::instance()->isMenuVisible()) return;
    
    Rectangle dst(0, 0, getWidth(), getHeight());
    Rectangle src(0, 0, getWidth(), getHeight());

    _background->start();
    _background->draw(dst, src);
    _background->finish();

    _mainMenuForm->draw();
	
	gp_uint w;
    gp_uint h;
	_font->measureText(Controller::gameHeader.c_str(),_headerSize,&w,&h);

    _font->start();
    _font->drawText(Controller::gameHeader.c_str(),
                    (gp_int)(getWidth()/2.f - w/2.f),//set text center top above the menu item
                    20, Vector4(1.f, 1.f, 0.f, 1.f), _headerSize, false);
    _font->finish();
}

void Menu::touchEvent(Touch::TouchEvent evt, gp_int x, gp_int y, gp_uint contactIndex)
{
    switch (evt)
    {
    case Touch::TOUCH_PRESS:
    {
    }
        break;
    case Touch::TOUCH_RELEASE:
    {
    }
        break;
    case Touch::TOUCH_MOVE:
    {
    }
        break;
    default:
        break;
    };
}

void Menu::keyEvent(Keyboard::KeyEvent keyEvent, gp_int key)
{
    if (keyEvent == Keyboard::KEY_PRESS)
    {
        switch (key)
        {
        case Keyboard::KEY_ESCAPE:
            Controller::instance()->getGame()->exit();
            break;
        }
    }
}

void Menu::controlEvent(Control* control, EventType evt)
{
	if (strcmp(control->getId(), _playButton->getId()) == 0){
        if(evt == PRESS){
			((ImageControl*)_mainMenuForm->getControl(control->getId()))->setImage(Controller::playBtnPressedPath.c_str());
        }
        else if(evt == RELEASE){
            ((ImageControl*)_mainMenuForm->getControl(control->getId()))->setImage(Controller::playBtnNormalPath.c_str());
			Controller::instance()->setMenuVisibility(false);
			setVisibility(false);
            onPlayClicked();
			return;
		}
    }

	else if (strcmp(control->getId(), _exitButton->getId()) == 0){
        if(evt == PRESS){
			((ImageControl*)_mainMenuForm->getControl(control->getId()))->setImage(Controller::exitBtnPressedPath.c_str());
        }
        else if(evt == RELEASE){
			((ImageControl*)_mainMenuForm->getControl(control->getId()))->setImage(Controller::exitBtnNormalPath.c_str());
            onExitClicked();
        }
    }

	else if (strcmp(control->getId(), _soundOnButton->getId()) == 0){
        if(evt == PRESS){
            setSoundSetting(false);
        }
    }

	else if (strcmp(control->getId(), _soundOffButton->getId()) == 0){
        if(evt == PRESS){
            setSoundSetting(true);
        }
    }

	else if (strcmp(control->getId(), _gamepadOnButton->getId()) == 0){
        if(evt == PRESS){
            setGamepadSetting(false);
        }
    }

	else if (strcmp(control->getId(), _gamepadOffButton->getId()) == 0){
        if(evt == PRESS){
            setGamepadSetting(true);
        }
    }
}

bool Menu::mouseEvent(Mouse::MouseEvent evt, gp_int x, gp_int y, gp_int wheelDelta){

    return true;
}

void Menu::setSoundSetting(bool play)
{
    if(!play)
        _sound->stop();
    else
        _sound->play();
    _soundOnButton->setVisible(play);
    _soundOffButton->setVisible(!play);
    Controller::instance()->setSoundEnabled(play);
}

void Menu::setGamepadSetting(bool enable)
{
    Controller::instance()->setGamepadVisibility(enable);
    _gamepadOffButton->setVisible(!enable);
    _gamepadOnButton->setVisible(enable);
}

Form *Menu::getForm()
{
    return _mainMenuForm;
}

void Menu::setVisibility(bool visibility)
{
	_mainMenuForm->setVisible(visibility);
	_exitButton->setVisible(visibility);
	_playButton->setVisible(visibility);
	if (Controller::instance()->isSoundEnabled())
		_soundOnButton->setVisible(true);
	else
		_soundOffButton->setVisible(true);
	if (Controller::instance()->isGamepadVisible())
		_gamepadOnButton->setVisible(true);
	else
		_gamepadOffButton->setVisible(true);
}

void Menu::onPlayClicked()
{
    //clear(Game::CLEAR_COLOR_DEPTH_STENCIL, Vector4::zero(), 1.0f, 0);
    Controller::instance()->switchToGame();
}

void Menu::onOptionsClicked()
{
    ///TODO showing options will be added
}

void Menu::onStatsClicked()
{
    ///TODO showing statistics will be added
}

void Menu::onProfileClicked()
{
    ///TODO showing or creating profile will be added
}

void Menu::onExitClicked()
{
    ///TODO may be a confirmation will be added
    Controller::instance()->getGame()->exit();
}

Menu* Menu::getInstance()
{
    return &menu;
}
