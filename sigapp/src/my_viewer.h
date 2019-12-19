# pragma once

# include <sig/sn_poly_editor.h>
# include <sig/sn_lines2.h>
#include <sig/sn_transform.h>
# include <sigogl/ui_button.h>
# include <sigogl/ws_viewer.h>
#include <sig/sn_shape.h>
#include <sig/sn_model.h>

// Viewer for this example:
class MyViewer : public WsViewer {
protected:
	bool GameOver;
	bool _animating;
	enum MenuEv { EvNormals, EvAnimate, EvExit };
	UiCheckButton* _nbut;
	double lt, t0;
	GsVec lightPos = GsVec(0, 5, 5);
	SnGroup* _gLight;

public:
	//floor stuff
	SnTransform* floorT; //floor global transform
	SnTransform* floormoveT[10]; //individual floor translations
	GsMat floorM; //floor global matrix
	GsMat floormoveM[10]; //floor individual matrices
	GsBox b1, b2;
	float floorz;
	int moveCount = -2, zmove = 0;
	bool level2;
	float floorX = 350.0f, floorY = 0.0f, floorZP = 20.0f, floorZN = 0.0f;
	int score = 0, highscore = 0; //score

	//Car Stuff
	float car1x, car2x, car3x, car4x, car5x, car1y, car1z, car2z, car3z, car4z, car5z, car2y, car3y, car4y, car5y;
	SnTransform* GlobalCarT; //global car transform
	SnTransform* carT[5]; // individual car Transformations
	GsMat GlobalCarM; //global car matrix
	GsMat carM[5]; //individual car Matrices 
	GsBox c1, c2, c3, c4, c5;

	//GameOver Menu
	SnModel* GameOverScreen;

	//player bird stuff
	int BirdPos;
	SnTransform* BirdT, * leftWT, * rightWT;
	GsMat BirdM, wingM;
	GsBox birdBox;
	float birdX, birdY, birdZ;
	float shadowbirdx, shadowbirdy, shadowbirdz; //SHADOW POSITIONS
	bool forward, left, right;

	//SHADOW STUFF
	SnTransform* ShadowBT;
	SnTransform* leftWTShadow;
	SnTransform* rightWTShadow;
	GsMat Shadowmat;
	GsMat Shadowmat2;
	GsMat perspective_trans;
	GsMat perspective_trans2;
	GsVec Ls;
	GsVec Shadow_origin;
	SnTransform* SceneShadowT;
	GsMat SceneShadowMat;
	GsMat floormoveshadowM[10];
	SnTransform* floormoveshadowT[10];
	int zshadowmove = 0;

	//flying bird stuff
	SnTransform* flyT, * flyMidT, * flyLeftT, * flyRightT;
	GsMat flyM, flyMidM, flyLeftM, flyRightM;
	GsMat rot, flyTo, flyBack;

	MyViewer(int x, int y, int w, int h, const char* l);
	void build_ui();
	void add_model(SnShape* s, GsVec p);
	void build_scene();
	//void update_shadow();
	void SceneMovement();
	void update_camera();
	void show_normals(bool view);
	void run_animation();
	virtual int handle_keyboard(const GsEvent& e) override;
	virtual int uievent(int e) override;
};