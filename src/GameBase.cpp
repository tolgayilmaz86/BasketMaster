#include "gamebase.h"
#include "controller.h"
#include "maingame.h"

GameBase game;

GameBase::GameBase() : _stateBlock(NULL), _lightNode(NULL), _menu(NULL)
{
	Controller::instance()->initialize();
}

GameBase::~GameBase()
{
}

void GameBase::initialize()
{
	displayScreen(this, &GameBase::drawSplash, NULL, 1000L);
    _menu = new Menu();
    Controller::instance()->setMenu(_menu);
    _mainGame = new MainGame();
    Controller::instance()->setGame(_mainGame);
    //getScriptController()->loadScript("res/common/camera.lua");

    //_stateBlock = RenderState::StateBlock::create();
    //_stateBlock->setCullFace(true);
    //_stateBlock->setBlend(true);
    //_stateBlock->setBlendSrc(RenderState::BLEND_SRC_ALPHA);
    //_stateBlock->setBlendDst(RenderState::BLEND_ONE_MINUS_SRC_ALPHA);

    Controller::instance()->setMenuVisibility(true);
    _menu->initialize();
}

void GameBase::drawSplash(void *param)
{
	clear(Game::CLEAR_COLOR_DEPTH, Vector4(0, 0, 0, 1), 1.0f, 0);
    SpriteBatch* batch = SpriteBatch::create("res/png/splash.png");
	batch->start();
	batch->draw(this->getWidth() * 0.5f, // X Coord
				this->getHeight() * 0.5f,// Y Coord
				0.0f,                    // Z Coord
				1024.0f,                 // Image width
				640.0f,                  // Image height
				0.0f, 1.0f, 1.0f, 0.0f,
				Vector4::one(), true);
	batch->finish();
	SAFE_DELETE(batch);
}

void GameBase::finalize()
{
	SAFE_RELEASE(_stateBlock);
	SAFE_RELEASE(_lightNode);
}

void GameBase::update(gp_float elapsedTime)
{
	if (Controller::instance()->isMenuVisible())
		Controller::instance()->getMenu()->update(elapsedTime);
	else if (Controller::instance()->isGamePlaying())
		Controller::instance()->getGame()->update(elapsedTime);
}

void GameBase::render(gp_float elapsedTime)
{
	if (Controller::instance()->isMenuVisible())
		Controller::instance()->getMenu()->render(elapsedTime);
	else if (Controller::instance()->isGamePlaying())
		Controller::instance()->getGame()->render(elapsedTime);
}

void GameBase::keyEvent(Keyboard::KeyEvent evt, gp_int key)
{
    if(Controller::instance()->isMenuVisible())
        Controller::instance()->getMenu()->keyEvent(evt,key);
	else if (Controller::instance()->isGamePlaying())
		Controller::instance()->getGame()->keyEvent(evt, key);
}

void GameBase::touchEvent(Touch::TouchEvent evt, gp_int x, gp_int y, gp_uint contactIndex)
{
	if (Controller::instance()->isMenuVisible())
		Controller::instance()->getMenu()->touchEvent(evt,x,y,contactIndex);
	else if (Controller::instance()->isGamePlaying())
		Controller::instance()->getGame()->touchEvent(evt, x, y, contactIndex);
}

bool GameBase::mouseEvent(Mouse::MouseEvent evt, gp_int x, gp_int y, gp_int wheelDelta)
{
	bool mouse = true;
	if (Controller::instance()->isMenuVisible())
		mouse =  Controller::instance()->getMenu()->mouseEvent(evt, x, y, wheelDelta);
	else if (Controller::instance()->isGamePlaying())
		mouse = Controller::instance()->getGame()->mouseEvent(evt, x, y, wheelDelta);
	return mouse;
}

void GameBase::gamepadEvent(Gamepad::GamepadEvent evt, Gamepad* gamepad)
{
	if (!Controller::instance()->isGamePlaying()) return;
	switch (evt)
	{
	case Gamepad::CONNECTED_EVENT:
	case Gamepad::DISCONNECTED_EVENT:
		Controller::instance()->setGamepad(getGamepad(0));
		break;
	}
}

void GameBase::menuEvent()
{
}

GameBase* GameBase::getInstance()
{
    return &game;
}
