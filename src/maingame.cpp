#include "maingame.h"
#include "controller.h"

#define BUTTON_1 0
#define BUTTON_2 1

bool _prompt = false;
bool _posApply = true;
bool _pressed = false;
Vector2 _pressStart(0.f,0.f);

MainGame mainGame;
float incrementPrecision = 0.3f;
float upIncrementPrecision = 4.f;

gp_uint MainGame::_score = 0;
MainGame::MainGame() :  _keyFlags(0), _scene(NULL), _font(NULL), _ballTrace(NULL), _lightNode(NULL), _camera(NULL), _moveFlags(0), _zoom(false),
						_backButton(NULL), _floor(NULL), _scorePlane(NULL), _wall(NULL), _hoop(NULL), _trackBall(false), _powerIndicator(NULL), 
						_upIndicator(NULL), _upBar(NULL)
{
	_buttonPressed = new bool[2];
	_initialized = false;
	_score = 0;
	_width = getWidth();
	_height = getHeight();
	_burn = false;
	_shooted = false;
	_scored = false;
	_careerPoint = 0;
	_upPower = 0;
}

void MainGame::initialize()
{
	_initialized = true;
	_burn = false;
	_width = getWidth();
	_height = getHeight();
	_score = 0;
	resetPower();
	_font = Font::create(Controller::mainGameFont.c_str());

	setMultiTouch(true);

	Theme* theme = Theme::create(Controller::defaultTheme.c_str());
	Theme::Style* formStyle = theme->getEmptyStyle();

	_promptMenu = Form::create("res/common/promptmenu.form");

	static_cast<Button*>(_promptMenu->getControl("newGameButton"))->addListener(this, Listener::CLICK);
	static_cast<Button*>(_promptMenu->getControl("quitGameButton"))->addListener(this, Listener::CLICK);
	static_cast<RadioButton*>(_promptMenu->getControl("useGamepad"))->addListener(this, Listener::VALUE_CHANGED);
	static_cast<RadioButton*>(_promptMenu->getControl("useTilt"))->addListener(this, Listener::VALUE_CHANGED);
	if (!Game::getInstance()->canExit())
	{
		// Prevent a programmatic exit on platforms that don't allow it.
		_promptMenu->removeControl("quitGameButton");
	}
	_promptMenu->setFocus();
	_promptMenu->setEnabled(true);
	_promptMenu->addListener(this, Control::Listener::PRESS | Control::Listener::RELEASE);

	_promptMenu->setHeight(200);
	_promptMenu->setWidth(150);
	_promptMenu->setFont(_font);
	_promptMenu->setPosition(_width / 2, _height / 2);
	_promptMenu->setVisible(true);
	_promptMenu->setScroll(Container::SCROLL_NONE);

	_gameSound = AudioSource::create(Controller::mainGameSound.c_str());
	if (_gameSound)
	{
	    _gameSound->setLooped(true);
	    if(Controller::instance()->isSoundEnabled())
	        _gameSound->play();
	    _gameSound->setGain(0.5f);
	}

	Controller::instance()->setGamepadVisibility(Controller::instance()->isGamepadVisible());

	_scene = Scene::load(Controller::mainGameScene.c_str());

	_lightNode = _scene->findNode("directionalLight");
	_scene->visit(this, &MainGame::bindLights);

	_floor = _scene->findNode("Court");
	//_courtModel = _floor->getModel();

	//_lighting = Material::create("res/design/light.material");
	//_courtModel->setMaterial(_lighting);

	//_lighting->setTechnique("directionalBumped");
	//_lighting->getTechnique("directionalBumped")->getParameter("u_ambientColor")->setValue(Vector3(0.0f, 0.0f, 0.0f));
	//_lighting->getTechnique("directionalBumped")->getParameter("u_directionalLightDirection[0]")->bindValue(_lightNode, &Node::getForwardVectorView);

	_gameForm = Form::create("GameMenu", formStyle, Layout::LAYOUT_ABSOLUTE);
	_gameForm->setFont(_font);
	_gameForm->setPosition(0, 0);
	_gameForm->setScroll(Container::SCROLL_NONE);

	Theme::Style* buttonStyle = theme->getEmptyStyle();
	_backButton = ImageControl::create("BACK", buttonStyle);
	_backButton->setImage(Controller::backBtnNormalPath.c_str());
	_backButton->setWidth(100);
	_backButton->setHeight(100);
	_backButton->setVisible(true);
	_backButton->setEnabled(true);
	_backButton->setPosition(0, 0);
	_backButton->addListener(this, Control::Listener::PRESS | Control::Listener::RELEASE);
	theme->release();

	_powerIndicator = SpriteBatch::create(Controller::powerIndicator.c_str());
	_indHeight = 1023.f;
	_indHeight = 169.f;

	_upBar			= SpriteBatch::create(Controller::upBar.c_str());
	_upIndicator	= SpriteBatch::create(Controller::upBarIndicator.c_str());

	_gameForm->addListener(this, Control::Listener::PRESS | Control::Listener::RELEASE);
	_gameForm->addControl(_backButton);
	_gameForm->setHeight(_backButton->getHeight() + 5);
	_gameForm->setWidth(_backButton->getWidth() + 5);

	if (_scene->getActiveCamera())
	{
		_camera = _scene->getActiveCamera();
		_camera->getNode()->setIdentity();
		_camera->getNode()->setTranslation(.0f, 4.f, 0.f);
		_camera->getNode()->setRotation(Vector3(0, 1, 0), MATH_DEG_TO_RAD(90));
		//_ballTrace = _basketball->getModel()->getMaterial()->getTechniqueByIndex(0)->getPassByIndex(0)->getParameter("u_modulateAlpha");
	}
	else
	{
		_camera = Camera::createPerspective(45.f, (float)getWidth() / (float)getHeight(), 6.9f, -5.f);
		_scene->setActiveCamera(_camera);
	}
	_camera->setFarPlane(120.f);
	_cameraNode = _camera->getNode();
	_basketball = _scene->findNode("Basketball");
	_wall = _scene->findNode("Background");
	_pot = _scene->findNode("Pot");
	_hoop = _scene->findNode("Hoop");
	_scorePlane = _scene->findNode("Score");
	//_materialParameterAlpha = _scorePlane->getModel()->getMaterial()->getTechniqueByIndex(0)->getPassByIndex(0)->getParameter("u_modulateAlpha");
	_scorePlane->setTag("transparent", "true");
	//_materialParameterAlpha->setValue(0);

	Node *up = Node::create("UpCamera");
	up->setCamera(Camera::createPerspective(45.f, (float)getWidth() / (float)getHeight(), 6.9f, -5.f));
	_upCamera = up->getCamera();
	_upCamera->getNode()->setIdentity();
	_upCamera->getNode()->setTranslation(-27.f, 20.f, -.9f);
	_upCamera->getNode()->setRotation(Vector3(0, 1, 0), MATH_DEG_TO_RAD(90));
	_upCamera->getNode()->setRotation(Vector3(1, 0, 0), MATH_DEG_TO_RAD(-90));
	_upCamera->setFarPlane(120.f);

	Node *back = Node::create("BackCamera");
	back->setCamera(Camera::createPerspective(45.f, (float)getWidth() / (float)getHeight(), 6.9f, -5.f));
	_backCamera = back->getCamera();
	_backCamera->getNode()->setIdentity();
	_backCamera->getNode()->setTranslation(2.f, 10.f, -.2f);
	_backCamera->getNode()->setRotation(Vector3(0, 1, 0), MATH_DEG_TO_RAD(90));
	_backCamera->setFarPlane(120.f);

	_scorePlane->getCollisionObject()->addCollisionListener(this);
	_basketball->getCollisionObject()->setEnabled(false);
	_pot->getCollisionObject()->setEnabled(false);
	_gamepad = Controller::instance()->getGamepad();
	_materialParameterAlpha = _basketball->getModel()->getMaterial()->getTechniqueByIndex(0)->getPassByIndex(0)->getParameter("u_modulateAlpha");
	_promptMenu->setX(100.f);
	_promptMenu->setY(200.f);

	resetBall();
	loadEmitters();
}

void MainGame::update(gp_float elapsedTime)
{
	//_promptMenu->update(elapsedTime);
	Vector3 moveCamera(0,0,0);
	moveCamera.normalize().scale(elapsedTime / 20.f);

	if (_gamepad->isButtonDown(Gamepad::BUTTON_A))
	{
		resetBall();
		_power = fmod(_power + incrementPrecision, 100.f);
		if (_buttonPressed[BUTTON_1])
		{
			_buttonPressed[BUTTON_1] = false;
			_burn = false;
			//_scene->setActiveCamera(_camera);
		}
	}
	else
	{
		if (!_buttonPressed[BUTTON_1] && _distance > 2.f)
		{//sooo player released the button A ha, shot him!
			shot(515, _upPower*-2.5f); //upPower should be between -250:100 optimal is 0 (decreasing gives more overclock)
			_trackBall = true;
			//_scene->setActiveCamera(_backCamera); 
			resetPower();
			_currentTime = elapsedTime;
		}
		_buttonPressed[BUTTON_1] = true;
	}

	if (_gamepad->isButtonDown(Gamepad::BUTTON_B))
	{
		_upPower = fmod(_upPower + 250 + upIncrementPrecision, 350.f) - 250;
		if (_buttonPressed[BUTTON_2])
		{
			_buttonPressed[BUTTON_2] = false;
			_zoom = true;
		}
	}
	else
	{
		if (!_buttonPressed[BUTTON_2])
		{//sooo player released the button B haa!
			_zoom = true;
		}
		_buttonPressed[BUTTON_2] = true;
	}

	_distance = _camera->getNode()->getTranslation().distance(_pot->getTranslation()) * .25f - .3f;
	if (_distance < 0) _distance = 0;

	if (_burn)
	{
		if (elapsedTime - _currentTime < _distance*.7f){
			_particleEmitterNode->getParticleEmitter()->update(elapsedTime);
		}
		else{
			_burn = false;
			resetBall();
		}
	}
	if (_trackBall)
		flyCamera(moveCamera.x);
	if (false)
	{
		lockCameraToNode(_basketball, false);
		return;
	}

	if (_gamepad->getJoystickCount() > 0)
	{
		Vector2 move;
		_gamepad->getJoystickValues(0, &move);

		if (move.x == 0.f && move.y == 0.f)
		{
			adjustCamera();
			return;
		}
		_camera->getNode()->setTranslationZ(-move.x*40.f);
		_camera->getNode()->translateY(move.y*.6f);
		lockCameraToNode(_pot,true);
		resetBall();
	}
	adjustCamera();
}

Mesh *MainGame::createGridMesh(unsigned int lineCount)
{
	// There needs to be an odd number of lines
	lineCount |= 1;
	const unsigned int pointCount = lineCount * 4;
	const unsigned int verticesSize = pointCount * (3 + 3);

	std::vector<float> vertices;
	vertices.resize(verticesSize);

	const float gridLength = (float)(lineCount / 2);
	float value = -gridLength;
	for (unsigned int i = 0; i < verticesSize; ++i)
	{
		// Default line color is dark grey
		Vector4 color(0.3f, 0.3f, 0.3f, 1.0f);

		// Very 10th line is brighter grey
		if (((int)value) % 10 == 0)
		{
			color.set(0.45f, 0.45f, 0.45f, 1.0f);
		}

		// The Z axis is blue
		if (value == 0.0f)
		{
			color.set(0.15f, 0.15f, 0.7f, 1.0f);
		}

		// Build the lines
		vertices[i] = value;
		vertices[++i] = 0.0f;
		vertices[++i] = -gridLength;
		vertices[++i] = color.x;
		vertices[++i] = color.y;
		vertices[++i] = color.z;

		vertices[++i] = value;
		vertices[++i] = 0.0f;
		vertices[++i] = gridLength;
		vertices[++i] = color.x;
		vertices[++i] = color.y;
		vertices[++i] = color.z;

		// The X axis is red
		if (value == 0.0f)
		{
			color.set(0.7f, 0.15f, 0.15f, 1.0f);
		}
		vertices[++i] = -gridLength;
		vertices[++i] = 0.0f;
		vertices[++i] = value;
		vertices[++i] = color.x;
		vertices[++i] = color.y;
		vertices[++i] = color.z;

		vertices[++i] = gridLength;
		vertices[++i] = 0.0f;
		vertices[++i] = value;
		vertices[++i] = color.x;
		vertices[++i] = color.y;
		vertices[++i] = color.z;

		value += 1.0f;
	}
	VertexFormat::Element elements[] =
	{
		VertexFormat::Element(VertexFormat::POSITION, 3),
		VertexFormat::Element(VertexFormat::COLOR, 3)
	};
	Mesh* mesh = Mesh::createMesh(VertexFormat(elements, 2), pointCount, false);
	if (mesh == NULL)
	{
		return NULL;
	}
	mesh->setPrimitiveType(Mesh::LINES);
	mesh->setVertexData(&vertices[0], 0, pointCount);

	return mesh;
}

Model *MainGame::createGridModel(unsigned int lineCount)
{
	Mesh* mesh = createGridMesh(lineCount);
	if (!mesh)
		return NULL;

	Model* model = Model::create(mesh);
	mesh->release();
	assert(model);
	return model;
}

void MainGame::drawDebugText(int x, int y)
{
	_font->start();

	static const int V_SPACE = 15;
	drawVector3("Ball Position", _basketball->getTranslation(), x, y);
	drawVector3("Camera Position", _camera->getNode()->getTranslation(), x, y += V_SPACE);
	drawVector3("UP POWER", Vector3(_upPower, 0, 0), x, y += V_SPACE);
	_font->finish();
}

void MainGame::drawVector3(const char *str, const Vector3 vector, int x, int y)
{
	char buffer[255];
	sprintf(buffer, "%s: (%.2f, %.2f, %.2f)", str, vector.x, vector.y, vector.z);
	_font->drawText(buffer, x, y, Vector4(1.f, 1.f, 0.f, 1.f), 20);
}

void MainGame::debugAnim()
{
	for (gp_uint i = 0; i < _scene->getNodeCount(); i++)
	{
		Animation* anim;
		for (Node* node = _scene->getFirstNode(); node != NULL; node = node->getNextSibling())
		{
			anim = node->getAnimation();
			if (anim != NULL)
			{
				GP_WARN(" ##########################  anim ID = %s", anim->getId());
				GP_WARN(" ########################## anim getClipCount = %d", anim->getClipCount());
				GP_WARN(" ########################## anim getDuration = %d", anim->getDuration());
			}
			GP_WARN(" getChildCount = %d", node->getChildCount());
			GP_WARN(" getType = %d", node->getType());
			GP_WARN(" isStatic = %d", node->isStatic());

			Model *model = node->getModel();
			if (model != NULL)
			{
				GP_WARN(" model->getMeshPartCount() = %d", model->getMeshPartCount());
				model = NULL;
			}

		}

	}
}

void MainGame::finalize()
{
	SAFE_RELEASE(_scene);
	SAFE_RELEASE(_font);
	SAFE_RELEASE(_promptMenu);
	SAFE_RELEASE(_gameForm);
	SAFE_RELEASE(_sound);
	SAFE_DELETE(_powerIndicator);
	SAFE_DELETE(_upBar);
	SAFE_DELETE(_upIndicator);
}

void MainGame::lockCameraToNode(Node* lock, bool rotate)
{
	Matrix m;
	Matrix::createLookAt(_camera->getNode()->getTranslation(), lock->getTranslation(), Vector3::unitY(), &m);
	if (rotate)
	{
		Quaternion q;
		m.transpose();
		m.getRotation(&q);
		_camera->getNode()->setRotation(q);
	}
	else
	{
		Vector3 t;
		m.getTranslation(&t);
		t.z -= 2;
		_camera->getNode()->setTranslation(t);
	}
}

void MainGame::adjustCamera(float elapsedTime)
{
	Node* cameraNode = _scene->getActiveCamera()->getNode();

	Vector3 cameraPosition = cameraNode->getTranslationWorld();

	/// X:-40:40 court length /// Y: 0:40 court height /// Z:-40:40 court width
	if (cameraPosition.x < -40.f){ cameraNode->setTranslationX(-40.f); }
	else if (cameraPosition.x > 40.f){ cameraNode->setTranslationX(40.f); }
	if (cameraPosition.y < 0.f){ cameraNode->setTranslationY(0.f); }
	else if (cameraPosition.y > 40.f){ cameraNode->setTranslationY(40.f); }
	if (cameraPosition.z < -40.f){ cameraNode->setTranslationZ(-40.f); }
	else if (cameraPosition.z > 40.f){ cameraNode->setTranslationZ(40.f); }
}

void MainGame::render(gp_float elapsedTime)
{
	//_promptMenu->draw();
	clear(Game::CLEAR_COLOR_DEPTH, Vector4::zero(), 1.0f, 0.f);
	// Visit all the nodes in the scene for drawing
	_scene->visit(this, &MainGame::drawScene);
	drawScore(_score);

	//Game::getInstance()->getPhysicsController()->drawDebug(_scene->getActiveCamera()->getViewProjectionMatrix());
	if (Controller::instance()->isGamepadVisible()) _gamepad->draw();
	_gameForm->draw();
	drawDebugText(250, 2);
	_promptMenu->draw();
	drawPowerIndicator(_power);
	drawUpIndicator(_upPower);

}

bool MainGame::drawScene(Node* node)
{
	Model* model = node->getModel();
	if (model){
		model->draw();
	}
	if (node->getParticleEmitter() && _burn)
		node->getParticleEmitter()->draw();
	return true;
}

void MainGame::drawScore(int score)
{
	std::stringstream scoreText;
	std::stringstream distanceText;
	std::stringstream careerPointText;

	scoreText << "Score: " << score;
	distanceText.precision(3);
	distanceText << _distance;
	distanceText << " mt.";
	careerPointText << "CP: " << _careerPoint;

	_font->start();
	_font->drawText(scoreText.str().c_str(), getWidth() - 180, 10, Vector4(1.f, 1.f, 0.f, 1.f), _font->getSize());
	_font->drawText(distanceText.str().c_str(), getWidth() - 180, 50, Vector4(1.f, .7f, 0.f, 1.f), _font->getSize());
	_font->drawText(careerPointText.str().c_str(), getWidth() - 180, 90, Vector4(0.f, .2f, .8f, 1.f), _font->getSize());
	_font->finish();
}

void MainGame::resetBall()
{
	Vector3 cameraForwardVector(_camera->getNode()->getForwardVectorWorld());
	Vector3 translation(_camera->getNode()->getTranslationWorld() + cameraForwardVector.normalize() * 2.f);
	_basketball->setTranslation(translation.x, translation.y-0.7f, translation.z);

	if (_shooted && !_scored)
		_careerPoint -= 1;
	if (_careerPoint < 0)
		_careerPoint = 0;
	_shooted = false;
}

void MainGame::touchEvent(Touch::TouchEvent evt, gp_int x, gp_int y, gp_uint contactIndex)
{
    gp_int startx, starty, endx, endy;
	switch (evt)
	{
	case Touch::TOUCH_RELEASE:
            _pressed = false;
		break;

	case Touch::TOUCH_PRESS:
			_pressStart.x = x;
			_pressStart.y = y;
			_pressed = true;
        break;

	case Touch::TOUCH_MOVE:
			if (_pressed)
			{
				_camera->getNode()->translateX((_pressStart.y - y)*.05f);
				lockCameraToNode(_pot, true);
				resetBall();
			}
		break;
	};
}

void MainGame::keyEvent(Keyboard::KeyEvent keyEvent, gp_int key)
{
	if (keyEvent == Keyboard::KEY_PRESS)
	{
		switch (key)
		{
		case Keyboard::KEY_ESCAPE:
			promptMenu();
			//exitGame();
			break;

		case Keyboard::KEY_SPACE:

			break;

			// ROT: UP:Y+ Down:Y- Left:X+ Right:X-
		case Keyboard::KEY_LEFT_ARROW:
			_scene->getActiveCamera()->getNode()->rotateY(.2f);
			GP_WARN("rotation x y z = %.2f, %.2f ,%.2f", _scene->getActiveCamera()->getNode()->getRotation().x, 
				_scene->getActiveCamera()->getNode()->getRotation().y,
				_scene->getActiveCamera()->getNode()->getRotation().z);
			break;

		case Keyboard::KEY_RIGHT_ARROW:
			_scene->getActiveCamera()->getNode()->rotateY(-.2f);
			GP_WARN("rotation x y z = %.2f, %.2f, %.2f", _scene->getActiveCamera()->getNode()->getRotation().x,
				_scene->getActiveCamera()->getNode()->getRotation().y,
				_scene->getActiveCamera()->getNode()->getRotation().z);
			break;

		case Keyboard::KEY_UP_ARROW:
			_scene->getActiveCamera()->getNode()->rotateZ(.2f);
				GP_WARN("rotation x y z = %.2f, %.2f, %.2f", _scene->getActiveCamera()->getNode()->getRotation().x,
				_scene->getActiveCamera()->getNode()->getRotation().y,
				_scene->getActiveCamera()->getNode()->getRotation().z);
			break;

		case Keyboard::KEY_DOWN_ARROW:
			_scene->getActiveCamera()->getNode()->rotateZ(-.2f);
			GP_WARN("rotation x y z = %.2f, %.2f, %.2f", _scene->getActiveCamera()->getNode()->getRotation().x,
				_scene->getActiveCamera()->getNode()->getRotation().y,
				_scene->getActiveCamera()->getNode()->getRotation().z);
			break;

		case Keyboard::KEY_D:
			_scene->getActiveCamera()->getNode()->translateX(.2f);
			break;

		case Keyboard::KEY_A:
			_scene->getActiveCamera()->getNode()->translateX(-.2f);
			break;

		case Keyboard::KEY_W:
			_scene->getActiveCamera()->getNode()->translateY(.2f);
			break;

		case Keyboard::KEY_S:
			_scene->getActiveCamera()->getNode()->translateY(-.2f);
			break;

		case Keyboard::KEY_Z:
			_scene->getActiveCamera()->getNode()->translateZ(.2f);
			break;

		case Keyboard::KEY_X:
			_scene->getActiveCamera()->getNode()->translateZ(-.2f);
			promptMenu();
			break;
		}
		lockCameraToNode(_pot, true);
	}
}

bool MainGame::mouseEvent(Mouse::MouseEvent evt, gp_int x, gp_int y, gp_int wheelDelta)
{
	switch (evt)
	{
		case Mouse::MOUSE_PRESS_LEFT_BUTTON:
			_pressStart.x = x;
			_pressStart.y = y;
			_pressed = true;
			break;
		case Mouse::MOUSE_RELEASE_LEFT_BUTTON:
			_pressed = false;
			break;
		case Mouse::MOUSE_MOVE:
			if (_pressed)
			{
				_camera->getNode()->translateX((_pressStart.y - y)*.05f);
				lockCameraToNode(_pot, true);
				resetBall();
			}
			break;
	}
	return true;
}

void MainGame::gamepadEvent(Gamepad::GamepadEvent evt, Gamepad* gamepad)
{
	switch (evt)
	{
	case Gamepad::CONNECTED_EVENT:
	case Gamepad::DISCONNECTED_EVENT:
		Controller::instance()->setGamepad(getGamepad(0));
		break;
	}
}

void MainGame::controlEvent(Control* control, Control::Listener::EventType evt)
{
	if (strcmp(control->getId(), _backButton->getId()) == 0){
		if (evt == PRESS){
			((ImageControl*)_gameForm->getControl(control->getId()))->setImage(Controller::backBtnPressedPath.c_str());
		}
		else if (evt == RELEASE){
			((ImageControl*)_gameForm->getControl(control->getId()))->setImage(Controller::backBtnNormalPath.c_str());
			exitGame();
		}
	}
}

bool MainGame::bindLights(Node* node)
{
	Model* model = node->getModel();
	if (model)
	{
		Material* material = model->getMaterial();
		if (material)
		{
			MaterialParameter* ambientColorParam = material->getParameter("u_ambientColor");
			if (ambientColorParam)
			{
				ambientColorParam->setValue(_scene->getAmbientColor());
			}

			if (_lightNode && _lightNode->getLight())
			{
				MaterialParameter* lightDirectionParam = material->getParameter("u_directionalLightDirection[0]");
				MaterialParameter* lightColorParam = material->getParameter("u_directionalLightColor[0]");
				if (lightDirectionParam)
				{
					lightDirectionParam->bindValue(_lightNode, &Node::getForwardVectorView);
				}
				if (lightColorParam)
				{
					lightColorParam->setValue(_lightNode->getLight()->getColor());
				}
			}
		}
	}
	return true;
}

void MainGame::shot(gp_int x, gp_int y)
{
	Ray ray;
	_scene->getActiveCamera()->pickRay(getViewport(), x, y, &ray);
	_basketball->setTranslation(ray.getOrigin());
	_ballRigidBody = static_cast<PhysicsRigidBody*>(_basketball->setCollisionObject("res/design/basketmaster.physics#Basketball"));

	_basketball->getCollisionObject()->setEnabled(true);

	//_ballTrace->setValue(0.2f);
	//_basketball->getModel()->getMaterial()->getTechniqueByIndex(0)->getPassByIndex(0)->getParameter("u_modulateAlpha")->setValue(0.1f);
	Vector3 impulse(ray.getDirection());
	impulse.normalize();
	impulse.scale(_power * _ballRigidBody->getMass());
	_ballRigidBody->applyImpulse(impulse);
	_shooted = true;
}

void MainGame::collisionEvent(PhysicsCollisionObject::CollisionListener::EventType type,
	const PhysicsCollisionObject::CollisionPair& collisionPair, const Vector3& contactPointA, const Vector3& contactPointB)
{
	// objectB -> scoreArea, only care about collisions between the scorePlane and the basketball.
	if (type == PhysicsCollisionObject::CollisionListener::COLLIDING
		&& collisionPair.objectA == _basketball->getCollisionObject())
	{
		_trackBall = false;
		if (fabs(contactPointA.distance(_basketball->getTranslation())) < fabs(_scorePlane->getTranslation().distance(_basketball->getScale() / 2)))
		{
			Vector3 velocity = -_basketball->getForwardVectorWorld();
			velocity.normalize();
			//GP_WARN("BALL VELOCITY VECTOR %f, %f, %f", velocity.x, velocity.y, velocity.z);
			if (velocity.y < 0) //make sure that ball is coming form up :)
			{
				++_score;
				if (_distance > 1)
					_careerPoint += ceil(_distance);
				_burn = true;
				_scored = true;
				_shooted = false;
			}
		}
	}
}

void MainGame::drawPowerIndicator(float power)
{
	float powerRatio = power / 100.f;

	Rectangle dstActive(_width / 4.f, _height / 1.4f, 480 * powerRatio, 150);
	Rectangle srcActive(0, 0, 1023 * powerRatio, 150);
	_powerIndicator->start();
	_powerIndicator->draw(dstActive, srcActive);
	_powerIndicator->finish();

	std::stringstream powerPercentage;
	powerPercentage.precision(2);
	powerPercentage << power;
	powerPercentage << "%";
	gp_uint w, h;
	_font->measureText(powerPercentage.str().c_str(), 1, &w, &h);
	_font->start();
	_font->drawText(powerPercentage.str().c_str(),
		_width / 4.f - w * 20 + 500 * powerRatio,
		_height - 60 * h,
		Vector4(powerRatio, 1 - powerRatio, 0.f, 1.f),
		_font->getSize());
	_font->finish();
}

void MainGame::drawUpIndicator(float upPower)
{
	Rectangle dstActive(25, 120, 16, 250);
	Rectangle srcActive(0, 0, 16, 445);

	_upBar->start();
	_upBar->draw(dstActive, srcActive);
	_upBar->finish();

	srcActive.width = 32;
	srcActive.height = 10;

	Rectangle dstActiveInd(18, 190 - 0.7f*upPower,32,10);
	_upIndicator->start();
	_upIndicator->draw(dstActiveInd, srcActive);
	_upIndicator->finish();
}

void MainGame::flyCamera(float by)
{
	Vector3 cameraForwardVector(_camera->getNode()->getForwardVectorWorld());
	cameraForwardVector.normalize().scale(by);
	_camera->getNode()->translate(cameraForwardVector);
}

void MainGame::promptMenu()
{
	_prompt = !_prompt;
	if (_prompt)
	{
		pause();
		_promptMenu->setEnabled(true);
		_promptMenu->setFocus();
	}
	else
	{
		resume();
		_promptMenu->setEnabled(false);
	}
}

void MainGame::stopGame()
{

}

void MainGame::loadEmitters()
{
	// Load the default particle emitter
	_particleEmitter = ParticleEmitter::create(Controller::particleEmitter.c_str());

	//_particleEmitter->setEnergy(10, 250);
	//_particleEmitter->setSize(10, 50, 30, 100);
	_particleEmitterNode = _scene->addNode("Particle Emitter");
	_particleEmitterNode->setTranslation(_pot->getTranslation());
	_particleEmitterNode->setParticleEmitter(_particleEmitter);
	_particleEmitter->start();
	//emitterChanged();
}

void MainGame::emitterChanged()
{
	ParticleEmitter* emitter = _particleEmitter;

	// Set the new emitter on the node.
	_particleEmitterNode->setParticleEmitter(_particleEmitter);
	_particleEmitter->release();

	// Parse editor section of particle properties
	Properties* p = Properties::create(_url.c_str());
	Properties* ns = p->getNamespace("editor", true);
	if (ns)
	{
	}
	emitter->start();
}

void MainGame::exitGame()
{
	_gameForm->setEnabled(false);
	Controller::instance()->switchToMenu();
}

MainGame* MainGame::getInstance(){
	return &mainGame;
}