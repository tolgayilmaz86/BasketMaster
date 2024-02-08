#ifndef MAINGAME_H
#define MAINGAME_H

#include "gamescene.h"

using namespace gameplay;

class MainGame : public GameScene, public Control::Listener, public PhysicsCollisionObject::CollisionListener
{
public:
    MainGame();

    void initialize();

    void finalize();

    void menuEvent();

    void update			(gp_float elapsedTime);

    void render			(gp_float elapsedTime);

    void touchEvent		(Touch::TouchEvent evt, gp_int x, gp_int y, gp_uint contactIndex);

    void keyEvent		(Keyboard::KeyEvent keyEvent, gp_int key);

    bool mouseEvent		(Mouse::MouseEvent evt, gp_int x, gp_int y, gp_int wheelDelta);

	void controlEvent(Control* control, Control::Listener::EventType evt);

	void gamepadEvent	(Gamepad::GamepadEvent evt, Gamepad* gamepad);

    Scene* getScene(){ return _scene;}

    AudioSource* getSound(){return _gameSound;}

    void exitGame();

	void promptMenu();

    void drawScore		(gp_int score);

    int  getScore(){return _score;}

    void setSCore(gp_int score){_score = score;}

	Form* getGameForm(){ return _gameForm; }

    static MainGame* getInstance();

    Mesh* createGridMesh(unsigned int lineCount);

    Model* createGridModel(unsigned int lineCount);

    void drawDebugText(int x, int y);

    void drawVector3(const char* str, const Vector3 vector, int x, int y);

	void drawPowerIndicator(float power = .0f);

	void drawUpIndicator(float power = .0f);

    void debugAnim();

	void shot(gp_int x, gp_int y);

	void stopGame();

	void collisionEvent(PhysicsCollisionObject::CollisionListener::EventType type,
		const PhysicsCollisionObject::CollisionPair& collisionPair,
		const Vector3& contactPointA = Vector3::zero(),
		const Vector3& contactPointB = Vector3::zero());

	bool isInitialized(){return _initialized;}
	void setInitialized(bool b){_initialized = b;}
protected:
    bool drawScene(Node* node);
	bool bindLights(Node* node);
	bool _initialized;
	void loadEmitters();
	void emitterChanged();
	ParticleEmitter* _particleEmitter;
	std::string _url;

private:
	static gp_uint   _score;
    gp_uint		    _keyFlags;
	gp_uint			_moveFlags;
	gp_int			_careerPoint;
    float           _x, _y, _power, _upPower, _distance, _currentTime;
	float			_width, _height, _indWidth, _indHeight;

    Font*           _font;
	Node*			_lightNode;
	Node*			_basketball;
	Node*			_floor;
	Node*			_hoop;
	Node*			_wall;
	Node*			_pot;
	Node*			_scorePlane;
	Node*			_cameraNode;
	Node*			_particleEmitterNode;
	Model*			_courtModel;
	Scene*          _scene;
    Camera*         _camera;
	Camera*         _backCamera;
	Camera*         _upCamera;
    AudioSource*    _gameSound;
	Gamepad*		_gamepad;
	ImageControl*   _backButton;
	SpriteBatch*	_powerIndicator;
	SpriteBatch*	_upBar;
	SpriteBatch*	_upIndicator;
	Material*		_lighting;
	Form*           _gameForm;
	Form*			_promptMenu;
	PhysicsRigidBody* _ballRigidBody;
	MaterialParameter* _materialParameterAlpha;

	bool*			_buttonPressed;
	bool			_trackBall;
	bool			_burn,_zoom, _shooted, _scored;
	MaterialParameter* _ballTrace;

	void resetPower(){ _power = 10.f; }

	void lockCameraToNode(Node* lock, bool rotate = false);

	void adjustCamera(float elapsedTime = 0);

	void resetBall();

	void flyCamera(float by);
};

#endif // MAINGAME_H
