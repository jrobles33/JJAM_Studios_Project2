# pragma once

# include <sig/sn_poly_editor.h>
# include <sig/sn_lines2.h>

# include <sigogl/ui_button.h>
# include <sigogl/ws_viewer.h>

// Viewer for this example:
class MyViewer : public WsViewer
{
protected:
	enum MenuEv { EvNormals, EvAnimate, EvExit };
	UiCheckButton* _nbut;
	bool _animating;
	double lt, t0;
	GsVec lightPos = GsVec(0, 5, 5);
	SnGroup* _gLight;
	GsBox b0, b1, b2;
	
	//SnTransform* globalT;
	//SnTransform* sT[19];
	//SnTransform* t[19]; //to hold all transformations 
	//GsMat m[19]; //to hold all the transformation matrices
	//GsMat sM[19];
	//bool update = true;
	//GsMat currRot;
public:
	MyViewer(int x, int y, int w, int h, const char* l);
	void build_ui();
	void add_model(SnShape* s, GsVec p);
	void build_scene();
	//void update_shadow();
	void update_camera();
	void show_normals(bool view);
	void run_animation();
	virtual int handle_keyboard(const GsEvent& e) override;
	virtual int uievent(int e) override;
};
