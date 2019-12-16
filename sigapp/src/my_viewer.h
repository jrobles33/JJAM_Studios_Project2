# pragma once

# include <sig/sn_poly_editor.h>
# include <sig/sn_lines2.h>
#include <sig/sn_transform.h>
# include <sigogl/ui_button.h>
# include <sigogl/ws_viewer.h>
#include <sig/sn_shape.h>

// Viewer for this example:
class MyViewer : public WsViewer {
protected:
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

	//player bird stuff
	SnTransform* BirdT, *leftWT, *rightWT;
	GsMat BirdM, wingM;
	GsBox birdBox;
	float birdX, birdY, birdZ;
	bool forward, left, right;

	//flying bird stuff
	SnTransform* flyT, *flyMidT, *flyLeftT, *flyRightT;
	GsMat flyM, flyMidM, flyLeftM, flyRightM;
	GsMat rot, flyTo, flyBack;

	MyViewer(int x, int y, int w, int h, const char* l);
	void build_ui();
	void add_model(SnShape* s, GsVec p);
	void build_scene();
	//void update_shadow();
	void SceneMovement();
	void show_normals(bool view);
	void run_animation();
	virtual int handle_keyboard(const GsEvent& e) override;
	virtual int uievent(int e) override;
};