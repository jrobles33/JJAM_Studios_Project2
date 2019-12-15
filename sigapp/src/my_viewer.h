# pragma once

# include <sig/sn_poly_editor.h>
# include <sig/sn_lines2.h>
#include <sig/sn_transform.h>
# include <sigogl/ui_button.h>
# include <sigogl/ws_viewer.h>
#include <sig/sn_shape.h>

// Viewer for this example:
class MyViewer : public WsViewer
{
protected:
	bool _animating;
	enum MenuEv { EvNormals, EvAnimate, EvExit };
	UiCheckButton* _nbut;
	double lt, t0;
	GsVec lightPos = GsVec(0, 5, 5);
	SnGroup* _gLight;
	GsBox b1, b2, birdBox;
	float birdX, birdY, birdZ, floorz;
	float startingbirdY;
	bool bird2moving;
	int moveCount = -2;
	int zmove = 0;
	//SnTransform* globalT;
	//SnTransform* sT[19];
	SnTransform* t[10]; //to hold all transformations 
	GsMat m[10]; //to hold all the transformation matrices
	//GsMat sM[19];
	//bool update = true;
	//GsMat currRot;
public:

	SnTransform* BirdT; // our bird transformations
	SnTransform* floorT;//floor global translation
	SnTransform* floormoveT[10]; //individual floor translations
	GsMat BirdM, floorM, wingM;
	GsMat floormoveM[10];
	bool SceneMoving;
	bool forward, left, right;
	SnTransform* leftWT;
	SnTransform* rightWT;

	//flying bird stuff
	SnTransform* flyT, * flyMidT, * flyLeftT, * flyRightT;
	GsMat flyM, flyMidM, flyLeftM, flyRightM;
	GsMat rot, flyTo, flyBack;

	MyViewer(int x, int y, int w, int h, const char* l);
	void build_ui();
	void add_model(SnShape* s, GsVec p);
	void build_scene();
	//void update_shadow();
	void update_camera();
	void SceneMovement();
	void show_normals(bool view);
	void run_animation();
	virtual int handle_keyboard(const GsEvent& e) override;
	virtual int uievent(int e) override;
};