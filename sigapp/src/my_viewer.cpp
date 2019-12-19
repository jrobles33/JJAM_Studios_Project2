# include "my_viewer.h"

# include <sigogl/ui_button.h>
# include <sigogl/ui_radio_button.h>
# include <sig/sn_primitive.h>
# include <sig/sn_transform.h>
# include <sig/sn_manipulator.h>
# include <sig/sn_material.h>
# include <sigogl/ws_run.h>

double tsd = 0;
double tmd = 0;
float i = 0.0001f;
MyViewer::MyViewer(int x, int y, int w, int h, const char* l) : WsViewer(x, y, w, h, l)
{
	_nbut = 0;
	BirdPos = 0;
	_animating = false;
	level2 = false;
	perspective_trans.e12 = -(lightPos.x / lightPos.y);
	perspective_trans.e32 = -(lightPos.z / lightPos.y);
	perspective_trans.e22 = 0; // e24
	perspective_trans.e24 = 0;
	perspective_trans.e33 = 1;
	perspective_trans.e34 = 0;
	perspective_trans.e44 = 1;
	Shadow_origin.x = -0.025f;
	Shadow_origin.y = 0.05f;
	Shadow_origin.z = -0.02f;

	perspective_trans2.e12 = -(lightPos.x / lightPos.y);
	perspective_trans2.e32 = -(lightPos.z / lightPos.y);
	perspective_trans2.e22 = 0; // e24
	perspective_trans2.e24 = 0;
	perspective_trans2.e33 = 1;
	perspective_trans2.e34 = 0;
	perspective_trans2.e44 = 1;




	build_ui();
	build_scene();

}


void MyViewer::build_ui()
{
	//UiPanel* p, * sp;
	//UiManager* uim = WsWindow::uim();
	//p = uim->add_panel("", UiPanel::HorizLeft);
	//p->add(new UiButton("View", sp = new UiPanel()));
	//{	UiPanel* p = sp;
	//p->add(_nbut = new UiCheckButton("Normals", EvNormals));
	//}
	//p->add(new UiButton("Animate", EvAnimate));
	//p->add(new UiButton("Exit", EvExit)); p->top()->separate();
}

void MyViewer::add_model(SnShape * s, GsVec p)
{
	// This method demonstrates how to add some elements to our scene graph: lines,
	// and a shape, and all in a group under a SnManipulator.
	// Therefore we are also demonstrating the use of a manipulator to allow the user to
	// change the position of the object with the mouse. If you do not need mouse interaction,
	// you can just use a SnTransform to apply a transformation instead of a SnManipulator.
	// You would then add the transform as 1st element of the group, and set g->separator(true).
	// Your scene graph should always be carefully designed according to your application needs.

	SnManipulator* manip = new SnManipulator;
	GsMat m;
	m.translation(p);
	manip->initial_mat(m);

	SnGroup* g = new SnGroup;
	SnLines* l = new SnLines;
	l->color(GsColor::orange);
	g->add(s);
	g->add(l);
	manip->child(g);
	manip->visible(false); // call this to turn off mouse interaction

	rootg()->add(manip);
}

void MyViewer::build_scene()
{
	if (!level2) {
		WsViewer::background(GsColor::GsColor(135, 206, 235));
	}
	if (level2) {
		WsViewer::background(GsColor::lightgray);
	}

	//reset scene and all matrices and global variables
	rootg()->remove_all();
	floorM.isid();
	GlobalCarM.isid();
	BirdM.isid();
	wingM.isid();
	Shadowmat.isid();
	Shadowmat2.isid();
	perspective_trans.isid();
	perspective_trans2.isid();
	SceneShadowMat.isid();
	flyM.isid();
	flyMidM.isid();
	flyLeftM.isid();
	flyRightM.isid();
	rot.isid();
	flyTo.isid();
	flyBack.isid();
	for (int i = 0; i < 10; i++) {
		floormoveshadowM[i].isid();
		floormoveM[i].isid();
	}
	for (int i = 0; i < 5; i++) {
		carM[i].isid();
	}
	floorz = 0;
	moveCount = -2;
	zmove = 0;
	zshadowmove = 0;
	floorX = 350.0f;
	floorY = 0.0f;
	floorZP = 20.0f;
	floorZN = 0.0f;

	//Main Menu
	SnModel* menu = new SnModel;
	GsPnt p00 = GsVec(-200, 300, 150);
	GsPnt p01 = GsVec(-200, -300, 150);
	GsPnt p02 = GsVec(200, -300, 150);
	GsPnt p03 = GsVec(200, 300, 150);
	menu->model()->V.push() = p00;
	menu->model()->V.push() = p01;
	menu->model()->V.push() = p02;
	menu->model()->V.push() = p03;
	menu->model()->F.push() = GsModel::Face(0, 1, 2);
	menu->model()->F.push() = GsModel::Face(0, 2, 3);
	menu->model()->N.push() = GsVec(0, 0, 1);
	menu->model()->N.push() = GsVec(0, 0, 1);
	menu->model()->N.push() = GsVec(0, 0, 1);
	menu->model()->N.push() = GsVec(0, 0, 1);
	GsModel& menuM = *menu->model();
	GsModel::Group& menuG = *menuM.G.push();
	menuG.fi = 0;
	menuG.fn = menuM.F.size();
	menuG.dmap = new GsModel::Texture;
	menuG.dmap->fname.set("../src/Models_and_Textures/menu.png");
	menuM.M.push().init();
	int nv1 = menuM.V.size();
	menuM.T.size(nv1);
	menuM.T[0].set(0, 1);
	menuM.T[1].set(0, 0);
	menuM.T[2].set(1, 0);
	menuM.T[3].set(1, 1);
	menuM.set_mode(GsModel::Smooth, GsModel::PerGroupMtl);
	menuM.textured = true;
	rootg()->add(menu);

	//floor stuff
	SnModel* floorback = new SnModel;
	GsPnt a = GsVec(-floorX, floorY - 1, 19);
	GsPnt b = GsVec(-floorX, floorY - 1, -1);
	GsPnt c = GsVec(floorX, floorY - 1, -1);
	GsPnt d = GsVec(floorX, floorY - 1, 19);
	floorback->model()->V.push() = a;
	floorback->model()->V.push() = b;
	floorback->model()->V.push() = c;
	floorback->model()->V.push() = d;
	floorback->model()->F.push() = GsModel::Face(0, 2, 1);
	floorback->model()->F.push() = GsModel::Face(0, 3, 2);
	floorback->model()->N.push() = GsVec(0, 1, 0);
	floorback->model()->N.push() = GsVec(0, 1, 0);
	floorback->model()->N.push() = GsVec(0, 1, 0);
	floorback->model()->N.push() = GsVec(0, 1, 0);
	GsModel& floorbackM = *floorback->model();
	GsModel::Group& floorbackG = *floorbackM.G.push();
	floorbackG.fi = 0;
	floorbackG.fn = floorbackM.F.size();
	floorbackG.dmap = new GsModel::Texture;
	if (!level2) {
		floorbackG.dmap->fname.set("../src/Models_and_Textures/grass.png");
	}
	if (level2) {
		floorbackG.dmap->fname.set("../src/Models_and_Textures/snow.jpg");
	}
	floorbackM.M.push().init();
	int nv2 = floorbackM.V.size();
	floorbackM.T.size(nv2);
	floorbackM.T[0].set(1, 0);
	floorbackM.T[1].set(0, 0);
	floorbackM.T[2].set(0, 1);
	floorbackM.T[3].set(1, 1);
	floorbackM.set_mode(GsModel::Smooth, GsModel::PerGroupMtl);
	floorbackM.textured = true;
	rootg()->add(floorback);

	//The floor of the scene
	SnModel* floor[10];
	SnGroup* floortrans[10];
	SnTransform* global = new SnTransform;
	GsMat m;
	m.roty(gspi);
	global->set(m);

	for (int i = 0; i < 10; i++) {
		GsPnt p0 = GsVec(-floorX, floorY, floorZP);
		GsPnt p1 = GsVec(-floorX, floorY, floorZN);
		GsPnt p2 = GsVec(floorX, floorY, floorZN);
		GsPnt p3 = GsVec(floorX, floorY, floorZP);
		floor[i] = new SnModel;
		floortrans[i] = new SnGroup;
		floortrans[i]->separator(true);
		floormoveT[i] = new SnTransform;
		floor[i]->model()->V.push() = p0;
		floor[i]->model()->V.push() = p1;
		floor[i]->model()->V.push() = p2;
		floor[i]->model()->V.push() = p3;
		floor[i]->model()->F.push() = GsModel::Face(0, 2, 1);
		floor[i]->model()->F.push() = GsModel::Face(0, 3, 2);
		floor[i]->model()->N.push() = GsVec(0, 1, 0);
		floor[i]->model()->N.push() = GsVec(0, 1, 0);
		floor[i]->model()->N.push() = GsVec(0, 1, 0);
		floor[i]->model()->N.push() = GsVec(0, 1, 0);
		floorZP = floorZP + 20.0f;
		floorZN = floorZN + 20.0f;
	}

	GsModel& m0 = *floor[0]->model();
	GsModel& m1 = *floor[1]->model();
	GsModel& m2 = *floor[2]->model();
	GsModel& m3 = *floor[3]->model();
	GsModel& m4 = *floor[4]->model();
	GsModel& m5 = *floor[5]->model();
	GsModel& m6 = *floor[6]->model();
	GsModel& m7 = *floor[7]->model();
	GsModel& m8 = *floor[8]->model();
	GsModel& m9 = *floor[9]->model();

	GsModel::Group& floorgroupGrass = *m0.G.push();
	GsModel::Group& floorgroupRoad = *m1.G.push();
	GsModel::Group& floorgroupGrass1 = *m2.G.push();
	GsModel::Group& floorgroupRoad1 = *m3.G.push();
	GsModel::Group& floorgroupGrass2 = *m4.G.push();
	GsModel::Group& floorgroupRoad2 = *m5.G.push();
	GsModel::Group& floorgroupGrass3 = *m6.G.push();
	GsModel::Group& floorgroupRoad3 = *m7.G.push();
	GsModel::Group& floorgroupGrass4 = *m8.G.push();
	GsModel::Group& floorgroupRoad4 = *m9.G.push();

	floorgroupGrass.fi = 0;
	floorgroupRoad.fi = 0;
	floorgroupGrass.fn = m0.F.size();
	floorgroupRoad.fn = m1.F.size();
	floorgroupGrass.dmap = new GsModel::Texture;
	floorgroupRoad.dmap = new GsModel::Texture;
	if (!level2) {
		floorgroupGrass.dmap->fname.set("../src/Models_and_Textures/grass.png");
	}
	if (level2) {
		floorgroupGrass.dmap->fname.set("../src/Models_and_Textures/snow.jpg");
	}
	floorgroupRoad.dmap->fname.set("../src/Models_and_Textures/road.jpg");

	floorgroupGrass1.fi = 0;
	floorgroupRoad1.fi = 0;
	floorgroupGrass1.fn = m2.F.size();
	floorgroupRoad1.fn = m3.F.size();
	floorgroupGrass1.dmap = new GsModel::Texture;
	floorgroupRoad1.dmap = new GsModel::Texture;
	if (!level2) {
		floorgroupGrass1.dmap->fname.set("../src/Models_and_Textures/grass.png");
	}
	if (level2) {
		floorgroupGrass1.dmap->fname.set("../src/Models_and_Textures/snow.jpg");
	}
	floorgroupRoad1.dmap->fname.set("../src/Models_and_Textures/road.jpg");

	floorgroupGrass2.fi = 0;
	floorgroupRoad2.fi = 0;
	floorgroupGrass2.fn = m4.F.size();
	floorgroupRoad2.fn = m5.F.size();
	floorgroupGrass2.dmap = new GsModel::Texture;
	floorgroupRoad2.dmap = new GsModel::Texture;
	if (!level2) {
		floorgroupGrass2.dmap->fname.set("../src/Models_and_Textures/grass.png");
	}
	if (level2) {
		floorgroupGrass2.dmap->fname.set("../src/Models_and_Textures/snow.jpg");
	}
	floorgroupRoad2.dmap->fname.set("../src/Models_and_Textures/road.jpg");

	floorgroupGrass3.fi = 0;
	floorgroupRoad3.fi = 0;
	floorgroupGrass3.fn = m6.F.size();
	floorgroupRoad3.fn = m7.F.size();
	floorgroupGrass3.dmap = new GsModel::Texture;
	floorgroupRoad3.dmap = new GsModel::Texture;
	if (!level2) {
		floorgroupGrass3.dmap->fname.set("../src/Models_and_Textures/grass.png");
	}
	if (level2) {
		floorgroupGrass3.dmap->fname.set("../src/Models_and_Textures/snow.jpg");
	}
	floorgroupRoad3.dmap->fname.set("../src/Models_and_Textures/road.jpg");

	floorgroupGrass4.fi = 0;
	floorgroupRoad4.fi = 0;
	floorgroupGrass4.fn = m8.F.size();
	floorgroupRoad4.fn = m9.F.size();
	floorgroupGrass4.dmap = new GsModel::Texture;
	floorgroupRoad4.dmap = new GsModel::Texture;
	if (!level2) {
		floorgroupGrass4.dmap->fname.set("../src/Models_and_Textures/grass.png");
	}
	if (level2) {
		floorgroupGrass4.dmap->fname.set("../src/Models_and_Textures/snow.jpg");
	}
	floorgroupRoad4.dmap->fname.set("../src/Models_and_Textures/road.jpg");

	m0.M.push().init();
	m1.M.push().init();
	m2.M.push().init();
	m3.M.push().init();
	m4.M.push().init();
	m5.M.push().init();
	m6.M.push().init();
	m7.M.push().init();
	m8.M.push().init();
	m9.M.push().init();

	int nv = m0.V.size();
	nv = m1.V.size();
	nv = m2.V.size();
	nv = m3.V.size();
	nv = m4.V.size();
	nv = m5.V.size();
	nv = m6.V.size();
	nv = m7.V.size();
	nv = m8.V.size();
	nv = m9.V.size();

	m0.T.size(nv);
	m1.T.size(nv);
	m2.T.size(nv);
	m3.T.size(nv);
	m4.T.size(nv);
	m5.T.size(nv);
	m6.T.size(nv);
	m7.T.size(nv);
	m8.T.size(nv);
	m9.T.size(nv);


	m0.T[0].set(1, 0);
	m0.T[1].set(0, 0);
	m0.T[2].set(0, 1);
	m0.T[3].set(1, 1);
	m0.set_mode(GsModel::Smooth, GsModel::PerGroupMtl);
	m0.textured = true;

	m1.T[0].set(0, 0);
	m1.T[1].set(0, 1);
	m1.T[2].set(1, 1);
	m1.T[3].set(1, 0);
	m1.set_mode(GsModel::Smooth, GsModel::PerGroupMtl);
	m1.textured = true;

	m2.T[0].set(1, 0);
	m2.T[1].set(0, 0);
	m2.T[2].set(0, 1);
	m2.T[3].set(1, 1);
	m2.set_mode(GsModel::Smooth, GsModel::PerGroupMtl);
	m2.textured = true;

	m3.T[0].set(0, 0);
	m3.T[1].set(0, 1);
	m3.T[2].set(1, 1);
	m3.T[3].set(1, 0);
	m3.set_mode(GsModel::Smooth, GsModel::PerGroupMtl);
	m3.textured = true;

	m4.T[0].set(1, 0);
	m4.T[1].set(0, 0);
	m4.T[2].set(0, 1);
	m4.T[3].set(1, 1);
	m4.set_mode(GsModel::Smooth, GsModel::PerGroupMtl);
	m4.textured = true;

	m5.T[0].set(0, 0);
	m5.T[1].set(0, 1);
	m5.T[2].set(1, 1);
	m5.T[3].set(1, 0);
	m5.set_mode(GsModel::Smooth, GsModel::PerGroupMtl);
	m5.textured = true;

	m6.T[0].set(1, 0);
	m6.T[1].set(0, 0);
	m6.T[2].set(0, 1);
	m6.T[3].set(1, 1);
	m6.set_mode(GsModel::Smooth, GsModel::PerGroupMtl);
	m6.textured = true;

	m7.T[0].set(0, 0);
	m7.T[1].set(0, 1);
	m7.T[2].set(1, 1);
	m7.T[3].set(1, 0);
	m7.set_mode(GsModel::Smooth, GsModel::PerGroupMtl);
	m7.textured = true;

	m8.T[0].set(1, 0);
	m8.T[1].set(0, 0);
	m8.T[2].set(0, 1);
	m8.T[3].set(1, 1);
	m8.set_mode(GsModel::Smooth, GsModel::PerGroupMtl);
	m8.textured = true;

	m9.T[0].set(0, 0);
	m9.T[1].set(0, 1);
	m9.T[2].set(1, 1);
	m9.T[3].set(1, 0);
	m9.set_mode(GsModel::Smooth, GsModel::PerGroupMtl);
	m9.textured = true;

	SnGroup* floorG = new SnGroup;
	floorG->separator(true);

	floorz = 0.0f;
	floorT = new SnTransform;
	floorM.translation(GsVec(0.0f, 0.0f, floorz));
	floorT->set(floorM);
	floorG->add(floorT);

	for (int i = 0; i < 10; i++) {
		floormoveM[i].translation(GsVec(0, 0, 0));
		floormoveT[i]->set(floormoveM[i]);
		floortrans[i]->add(floormoveT[i]);
		floortrans[i]->add(floor[i]);
		floorG->add(floortrans[i]);
	}

	//fences
	SnModel* fence1 = new SnModel;
	fence1->model()->load_obj("../src/Models_and_Textures/rocks.obj");
	fence1->model()->centralize();
	fence1->model()->get_bounding_box(b2);
	fence1->model()->scale(1.9f);
	fence1->color(GsColor::gray);
	fence1->model()->rotate(GsQuat(GsVec::i, -gspidiv2));
	fence1->model()->rotate(GsQuat(GsVec::j, gspi));
	fence1->model()->translate(GsVec(-90, (b2.dy() / 2) - 3, 10));
	floortrans[0]->add(fence1);
	SnModel* fence2 = new SnModel;
	fence2->model()->load_obj("../src/Models_and_Textures/rocks.obj");
	fence2->model()->centralize();
	fence2->model()->get_bounding_box(b2);
	fence2->model()->scale(1.9f);
	fence2->color(GsColor::gray);
	fence2->model()->rotate(GsQuat(GsVec::i, -gspidiv2));
	fence2->model()->rotate(GsQuat(GsVec::j, gs2pi));
	fence2->model()->translate(GsVec(90, (b2.dy() / 2) - 3, 10));
	floortrans[0]->add(fence2);
	SnModel* fence3 = new SnModel;
	fence3->model()->load_obj("../src/Models_and_Textures/rocks.obj");
	fence3->model()->centralize();
	fence3->model()->get_bounding_box(b2);
	fence3->model()->scale(1.9f);
	fence3->color(GsColor::gray);
	fence3->model()->rotate(GsQuat(GsVec::i, -gspidiv2));
	fence3->model()->rotate(GsQuat(GsVec::j, gspi));
	fence3->model()->translate(GsVec(-90, (b2.dy() / 2) - 3, 50));
	floortrans[2]->add(fence3);
	SnModel* fence4 = new SnModel;
	fence4->model()->load_obj("../src/Models_and_Textures/rocks.obj");
	fence4->model()->centralize();
	fence4->model()->get_bounding_box(b2);
	fence4->model()->scale(1.9f);
	fence4->color(GsColor::gray);
	fence4->model()->rotate(GsQuat(GsVec::i, -gspidiv2));
	fence4->model()->rotate(GsQuat(GsVec::j, gs2pi));
	fence4->model()->translate(GsVec(90, (b2.dy() / 2) - 3, 50));
	floortrans[2]->add(fence4);
	SnModel* fence5 = new SnModel;
	fence5->model()->load_obj("../src/Models_and_Textures/rocks.obj");
	fence5->model()->centralize();
	fence5->model()->get_bounding_box(b2);
	fence5->model()->scale(1.9f);
	fence5->color(GsColor::gray);
	fence5->model()->rotate(GsQuat(GsVec::i, -gspidiv2));
	fence5->model()->rotate(GsQuat(GsVec::j, gspi));
	fence5->model()->translate(GsVec(-90, (b2.dy() / 2) - 3, 90));
	floortrans[4]->add(fence5);
	SnModel* fence6 = new SnModel;
	fence6->model()->load_obj("../src/Models_and_Textures/rocks.obj");
	fence6->model()->centralize();
	fence6->model()->get_bounding_box(b2);
	fence6->model()->scale(1.9f);
	fence6->color(GsColor::gray);
	fence6->model()->rotate(GsQuat(GsVec::i, -gspidiv2));
	fence6->model()->rotate(GsQuat(GsVec::j, gs2pi));
	fence6->model()->translate(GsVec(90, (b2.dy() / 2) - 3, 90));
	floortrans[4]->add(fence6);
	SnModel* fence7 = new SnModel;
	fence7->model()->load_obj("../src/Models_and_Textures/rocks.obj");
	fence7->model()->centralize();
	fence7->model()->get_bounding_box(b2);
	fence7->model()->scale(1.9f);
	fence7->color(GsColor::gray);
	fence7->model()->rotate(GsQuat(GsVec::i, -gspidiv2));
	fence7->model()->rotate(GsQuat(GsVec::j, gspi));
	fence7->model()->translate(GsVec(-90, (b2.dy() / 2) - 3, 130));
	floortrans[6]->add(fence7);
	SnModel* fence8 = new SnModel;
	fence8->model()->load_obj("../src/Models_and_Textures/rocks.obj");
	fence8->model()->centralize();
	fence8->model()->get_bounding_box(b2);
	fence8->model()->scale(1.9f);
	fence8->color(GsColor::gray);
	fence8->model()->rotate(GsQuat(GsVec::i, -gspidiv2));
	fence8->model()->rotate(GsQuat(GsVec::j, gs2pi));
	fence8->model()->translate(GsVec(90, (b2.dy() / 2) - 3, 130));
	floortrans[6]->add(fence8);
	SnModel* fence9 = new SnModel;
	fence9->model()->load_obj("../src/Models_and_Textures/rocks.obj");
	fence9->model()->centralize();
	fence9->model()->get_bounding_box(b2);
	fence9->color(GsColor::gray);
	fence9->model()->scale(1.9f);
	fence9->model()->rotate(GsQuat(GsVec::i, -gspidiv2));
	fence9->model()->rotate(GsQuat(GsVec::j, gspi));
	fence9->model()->translate(GsVec(-90, (b2.dy() / 2) - 3, 170));
	floortrans[8]->add(fence9);
	SnModel* fence10 = new SnModel;
	fence10->model()->load_obj("../src/Models_and_Textures/rocks.obj");
	fence10->model()->centralize();
	fence10->model()->get_bounding_box(b2);
	fence10->model()->scale(1.9f);
	fence10->color(GsColor::gray);
	fence10->model()->rotate(GsQuat(GsVec::i, -gspidiv2));
	fence10->model()->rotate(GsQuat(GsVec::j, gs2pi));
	fence10->model()->translate(GsVec(90, (b2.dy() / 2) - 3, 170));
	floortrans[8]->add(fence10);

	//Clouds
	SnModel* cloud1 = new SnModel;
	GsModel& cloudM1 = *cloud1->model();
	GsModel::Group& cloudG1 = *cloudM1.G.push();
	SnModel* cloud2 = new SnModel;
	GsModel& cloudM2 = *cloud2->model();
	GsModel::Group& cloudG2 = *cloudM2.G.push();
	SnModel* cloud3 = new SnModel;
	GsModel& cloudM3 = *cloud3->model();
	GsModel::Group& cloudG3 = *cloudM3.G.push();
	SnModel* cloud4 = new SnModel;
	GsModel& cloudM4 = *cloud4->model();
	GsModel::Group& cloudG4 = *cloudM4.G.push();
	SnModel* cloud5 = new SnModel;
	GsModel& cloudM5 = *cloud5->model();
	GsModel::Group& cloudG5 = *cloudM5.G.push();
	SnModel* cloud6 = new SnModel;
	GsModel& cloudM6 = *cloud6->model();
	GsModel::Group& cloudG6 = *cloudM6.G.push();

	SnGroup* allclouds = new SnGroup;

	GsPnt c00 = GsVec(-25, 25, 25);
	GsPnt c01 = GsVec(-25, -25, 25);
	GsPnt c02 = GsVec(25, -25, 25);
	GsPnt c03 = GsVec(25, 25, 25);

	cloud1->model()->V.push() = c00;
	cloud1->model()->V.push() = c01;
	cloud1->model()->V.push() = c02;
	cloud1->model()->V.push() = c03;
	cloud1->model()->F.push() = GsModel::Face(0, 1, 2);
	cloud1->model()->F.push() = GsModel::Face(0, 2, 3);
	cloud1->model()->N.push() = GsVec(0, 0, 1);
	cloud1->model()->N.push() = GsVec(0, 0, 1);
	cloud1->model()->N.push() = GsVec(0, 0, 1);
	cloud1->model()->N.push() = GsVec(0, 0, 1);

	cloud2->model()->V.push() = c00;
	cloud2->model()->V.push() = c01;
	cloud2->model()->V.push() = c02;
	cloud2->model()->V.push() = c03;
	cloud2->model()->F.push() = GsModel::Face(0, 1, 2);
	cloud2->model()->F.push() = GsModel::Face(0, 2, 3);
	cloud2->model()->N.push() = GsVec(0, 0, 1);
	cloud2->model()->N.push() = GsVec(0, 0, 1);
	cloud2->model()->N.push() = GsVec(0, 0, 1);
	cloud2->model()->N.push() = GsVec(0, 0, 1);

	cloud3->model()->V.push() = c00;
	cloud3->model()->V.push() = c01;
	cloud3->model()->V.push() = c02;
	cloud3->model()->V.push() = c03;
	cloud3->model()->F.push() = GsModel::Face(0, 1, 2);
	cloud3->model()->F.push() = GsModel::Face(0, 2, 3);
	cloud3->model()->N.push() = GsVec(0, 0, 1);
	cloud3->model()->N.push() = GsVec(0, 0, 1);
	cloud3->model()->N.push() = GsVec(0, 0, 1);
	cloud3->model()->N.push() = GsVec(0, 0, 1);

	cloud4->model()->V.push() = c00;
	cloud4->model()->V.push() = c01;
	cloud4->model()->V.push() = c02;
	cloud4->model()->V.push() = c03;
	cloud4->model()->F.push() = GsModel::Face(0, 1, 2);
	cloud4->model()->F.push() = GsModel::Face(0, 2, 3);
	cloud4->model()->N.push() = GsVec(0, 0, 1);
	cloud4->model()->N.push() = GsVec(0, 0, 1);
	cloud4->model()->N.push() = GsVec(0, 0, 1);
	cloud4->model()->N.push() = GsVec(0, 0, 1);

	cloud5->model()->V.push() = c00;
	cloud5->model()->V.push() = c01;
	cloud5->model()->V.push() = c02;
	cloud5->model()->V.push() = c03;
	cloud5->model()->F.push() = GsModel::Face(0, 1, 2);
	cloud5->model()->F.push() = GsModel::Face(0, 2, 3);
	cloud5->model()->N.push() = GsVec(0, 0, 1);
	cloud5->model()->N.push() = GsVec(0, 0, 1);
	cloud5->model()->N.push() = GsVec(0, 0, 1);
	cloud5->model()->N.push() = GsVec(0, 0, 1);

	cloud6->model()->V.push() = c00;
	cloud6->model()->V.push() = c01;
	cloud6->model()->V.push() = c02;
	cloud6->model()->V.push() = c03;
	cloud6->model()->F.push() = GsModel::Face(0, 1, 2);
	cloud6->model()->F.push() = GsModel::Face(0, 2, 3);
	cloud6->model()->N.push() = GsVec(0, 0, 1);
	cloud6->model()->N.push() = GsVec(0, 0, 1);
	cloud6->model()->N.push() = GsVec(0, 0, 1);
	cloud6->model()->N.push() = GsVec(0, 0, 1);

	cloudG1.fi = 0;
	cloudG1.fn = cloudM1.F.size();
	cloudG1.dmap = new GsModel::Texture;
	cloudG1.dmap->fname.set("../src/Models_and_Textures/cloud.png");
	cloudG2.fi = 0;
	cloudG2.fn = cloudM2.F.size();
	cloudG2.dmap = new GsModel::Texture;
	cloudG2.dmap->fname.set("../src/Models_and_Textures/cloud.png");
	cloudG3.fi = 0;
	cloudG3.fn = cloudM3.F.size();
	cloudG3.dmap = new GsModel::Texture;
	cloudG3.dmap->fname.set("../src/Models_and_Textures/cloud.png");
	cloudG4.fi = 0;
	cloudG4.fn = cloudM4.F.size();
	cloudG4.dmap = new GsModel::Texture;
	cloudG4.dmap->fname.set("../src/Models_and_Textures/cloud.png");
	cloudG5.fi = 0;
	cloudG5.fn = cloudM5.F.size();
	cloudG5.dmap = new GsModel::Texture;
	cloudG5.dmap->fname.set("../src/Models_and_Textures/cloud.png");
	cloudG6.fi = 0;
	cloudG6.fn = cloudM6.F.size();
	cloudG6.dmap = new GsModel::Texture;
	cloudG6.dmap->fname.set("../src/Models_and_Textures/cloud.png");

	cloudM1.M.push().init();
	cloudM2.M.push().init();
	cloudM3.M.push().init();
	cloudM4.M.push().init();
	cloudM5.M.push().init();
	cloudM6.M.push().init();
	cloudM1.T.size(cloudM1.V.size());
	cloudM1.T[0].set(0, 1);
	cloudM1.T[1].set(0, 0);
	cloudM1.T[2].set(1, 0);
	cloudM1.T[3].set(1, 1);
	cloudM1.set_mode(GsModel::Smooth, GsModel::PerGroupMtl);
	cloudM1.textured = true;
	cloudM2.T.size(cloudM2.V.size());
	cloudM2.T[0].set(0, 1);
	cloudM2.T[1].set(0, 0);
	cloudM2.T[2].set(1, 0);
	cloudM2.T[3].set(1, 1);
	cloudM2.set_mode(GsModel::Smooth, GsModel::PerGroupMtl);
	cloudM2.textured = true;
	cloudM3.T.size(cloudM3.V.size());
	cloudM3.T[0].set(0, 1);
	cloudM3.T[1].set(0, 0);
	cloudM3.T[2].set(1, 0);
	cloudM3.T[3].set(1, 1);
	cloudM3.set_mode(GsModel::Smooth, GsModel::PerGroupMtl);
	cloudM3.textured = true;
	cloudM4.T.size(cloudM4.V.size());
	cloudM4.T[0].set(0, 1);
	cloudM4.T[1].set(0, 0);
	cloudM4.T[2].set(1, 0);
	cloudM4.T[3].set(1, 1);
	cloudM4.set_mode(GsModel::Smooth, GsModel::PerGroupMtl);
	cloudM4.textured = true;
	cloudM5.T.size(cloudM5.V.size());
	cloudM5.T[0].set(0, 1);
	cloudM5.T[1].set(0, 0);
	cloudM5.T[2].set(1, 0);
	cloudM5.T[3].set(1, 1);
	cloudM5.set_mode(GsModel::Smooth, GsModel::PerGroupMtl);
	cloudM5.textured = true;
	cloudM6.T.size(cloudM1.V.size());
	cloudM6.T[0].set(0, 1);
	cloudM6.T[1].set(0, 0);
	cloudM6.T[2].set(1, 0);
	cloudM6.T[3].set(1, 1);
	cloudM6.set_mode(GsModel::Smooth, GsModel::PerGroupMtl);
	cloudM6.textured = true;

	cloud1->model()->translate(GsVec(-200, 45, -230));
	cloud2->model()->translate(GsVec(0, 30, -230));
	cloud3->model()->translate(GsVec(150, 50, -230));

	cloud4->model()->rotate(GsQuat(GsVec::j, gspi));
	cloud4->model()->translate(GsVec(-200, 45, 230));
	cloud5->model()->rotate(GsQuat(GsVec::j, gspi));
	cloud5->model()->translate(GsVec(0, 45, 230));
	cloud6->model()->rotate(GsQuat(GsVec::j, gspi));
	cloud6->model()->translate(GsVec(150, 45, 230));

	SnGroup* cloudAll = new SnGroup;

	cloudAll->add(cloud1);
	cloudAll->add(cloud2);
	cloudAll->add(cloud3);
	cloudAll->add(cloud4);
	cloudAll->add(cloud5);
	cloudAll->add(cloud6);

	//Nature and other objects in scene
	SnGroup* nature = new SnGroup;
	nature->separator(true);

	if (!level2) {
		SnModel* tree1 = new SnModel;
		tree1->model()->load_obj("../src/Models_and_Textures/Lowpoly_tree_sample.obj");
		tree1->model()->centralize();
		tree1->model()->get_bounding_box(b1);
		tree1->color(GsColor::green);
		tree1->model()->translate(GsVec(30, (b1.dy() / 2), 10));
		floortrans[0]->add(tree1);
		SnModel * tree2 = new SnModel;
		tree2->model()->load_obj("../src/Models_and_Textures/Lowpoly_tree_sample.obj");
		tree2->model()->centralize();
		tree2->model()->get_bounding_box(b1);
		tree2->color(GsColor::green);
		tree2->model()->translate(GsVec(60, (b1.dy() / 2), 90));
		floortrans[4]->add(tree2);
		SnModel * tree3 = new SnModel;
		tree3->model()->load_obj("../src/Models_and_Textures/Lowpoly_tree_sample.obj");
		tree3->model()->centralize();
		tree3->model()->get_bounding_box(b2);
		tree3->color(GsColor::green);
		tree3->model()->translate(GsVec(-50, (b2.dy() / 2) - 6, 130));
		floortrans[6]->add(tree3);
		SnModel * tree4 = new SnModel;
		tree4->model()->load_obj("../src/Models_and_Textures/Lowpoly_tree_sample.obj");
		tree4->model()->centralize();
		tree4->model()->get_bounding_box(b1);
		tree4->color(GsColor::green);
		tree4->model()->translate(GsVec(60, (b1.dy() / 2), 170));
		floortrans[8]->add(tree4);
	}
	if (level2) {
		SnModel* tree1 = new SnModel;
		tree1->model()->load_obj("../src/Models_and_Textures/pinetrees.obj");
		tree1->model()->centralize();
		tree1->model()->get_bounding_box(b1);
		tree1->color(GsColor::darkgreen);
		tree1->model()->scale(2);
		tree1->model()->rotate(GsQuat(GsVec::i, -gspidiv2));
		tree1->model()->rotate(GsQuat(GsVec::j, -gspidiv2));
		tree1->model()->translate(GsVec(30, (b1.dy() / 2) + 4, 10));
		floortrans[0]->add(tree1);
		SnModel* tree2 = new SnModel;
		tree2->model()->load_obj("../src/Models_and_Textures/pinetrees.obj");
		tree2->model()->centralize();
		tree2->model()->get_bounding_box(b1);
		tree2->color(GsColor::darkgreen);
		tree2->model()->scale(2);
		tree2->model()->rotate(GsQuat(GsVec::i, -gspidiv2));
		tree2->model()->rotate(GsQuat(GsVec::j, -gspidiv2));
		tree2->model()->translate(GsVec(30, (b1.dy() / 2) + 4, 10));
		floortrans[4]->add(tree2);
		SnModel* tree3 = new SnModel;
		tree3->model()->load_obj("../src/Models_and_Textures/pinetrees.obj");
		tree3->model()->centralize();
		tree3->model()->get_bounding_box(b2);
		tree3->color(GsColor::darkgreen);
		tree3->model()->scale(2);
		tree3->model()->rotate(GsQuat(GsVec::i, -gspidiv2));
		tree3->model()->rotate(GsQuat(GsVec::j, -gspidiv2));
		tree3->model()->translate(GsVec(30, (b1.dy() / 2) + 4, 10));
		floortrans[6]->add(tree3);
		SnModel* tree4 = new SnModel;
		tree4->model()->load_obj("../src/Models_and_Textures/pinetrees.obj");
		tree4->model()->centralize();
		tree4->model()->get_bounding_box(b1);
		tree4->color(GsColor::darkgreen);
		tree4->model()->scale(2);
		tree4->model()->rotate(GsQuat(GsVec::i, -gspidiv2));
		tree4->model()->rotate(GsQuat(GsVec::j, -gspidiv2));
		tree4->model()->translate(GsVec(30, (b1.dy() / 2) + 4, 10));
		floortrans[8]->add(tree4);
	}

	SnModel* pinetree1 = new SnModel;
	pinetree1->model()->load_obj("../src/Models_and_Textures/pinetrees.obj");
	pinetree1->model()->centralize();
	pinetree1->model()->get_bounding_box(b1);
	pinetree1->model()->scale(2);
	pinetree1->color(GsColor::darkgreen);
	pinetree1->model()->rotate(GsQuat(GsVec::i, -gspidiv2));
	pinetree1->model()->rotate(GsQuat(GsVec::j, -gspidiv2));
	pinetree1->model()->translate(GsVec(-40, (b1.dy() / 2) + 4, 50));
	floortrans[2]->add(pinetree1);
	SnModel* pinetree2 = new SnModel;
	pinetree2->model()->load_obj("../src/Models_and_Textures/pinetrees.obj");
	pinetree2->model()->centralize();
	pinetree2->model()->get_bounding_box(b2);
	pinetree2->model()->scale(2);
	pinetree2->color(GsColor::darkgreen);
	pinetree2->model()->rotate(GsQuat(GsVec::i, -gspidiv2));
	pinetree2->model()->rotate(GsQuat(GsVec::j, -gspidiv2));
	pinetree2->model()->translate(GsVec(50, (b1.dy() / 2) + 4, 50));
	floortrans[2]->add(pinetree2);
	SnModel* pinetree3 = new SnModel;
	pinetree3->model()->load_obj("../src/Models_and_Textures/pinetrees.obj");
	pinetree3->model()->centralize();
	pinetree3->model()->get_bounding_box(b1);
	pinetree3->model()->scale(2);
	pinetree3->color(GsColor::darkgreen);
	pinetree3->model()->rotate(GsQuat(GsVec::i, -gspidiv2));
	pinetree3->model()->rotate(GsQuat(GsVec::j, gspidiv2));
	pinetree3->model()->translate(GsVec(-60, (b1.dy() / 2) + 4, 90));
	floortrans[4]->add(pinetree3);
	SnModel* pinetree4 = new SnModel;
	pinetree4->model()->load_obj("../src/Models_and_Textures/pinetrees.obj");
	pinetree4->model()->centralize();
	pinetree4->model()->get_bounding_box(b2);
	pinetree4->model()->scale(2);
	pinetree4->color(GsColor::darkgreen);
	pinetree4->model()->rotate(GsQuat(GsVec::i, -gspidiv2));
	pinetree4->model()->rotate(GsQuat(GsVec::j, gspidiv2));
	pinetree4->model()->translate(GsVec(50, float(b1.dy() / 2) + 4, 130));
	floortrans[6]->add(pinetree4);
	SnModel* pinetrees5 = new SnModel;
	pinetrees5->model()->load_obj("../src/Models_and_Textures/pinetrees.obj");
	pinetrees5->model()->centralize();
	pinetrees5->model()->get_bounding_box(b2);
	pinetrees5->model()->scale(2);
	pinetrees5->color(GsColor::darkgreen);
	pinetrees5->model()->rotate(GsQuat(GsVec::i, -gspidiv2));
	pinetrees5->model()->rotate(GsQuat(GsVec::j, -gspidiv2));
	pinetrees5->model()->translate(GsVec(-40, float(b1.dy() / 2) + 4, 170));
	floortrans[8]->add(pinetrees5);

	float carScale = 0.05f;
	//CARS
	SnGroup* carG1 = new SnGroup;
	carG1->separator(true);
	SnModel* car1 = new SnModel;
	SnModel* car2 = new SnModel;
	SnModel* car3 = new SnModel;
	SnModel* car4 = new SnModel;
	SnModel* car5 = new SnModel;
	carT[0] = new SnTransform;
	carT[1] = new SnTransform;
	carT[2] = new SnTransform;
	carT[3] = new SnTransform;
	carT[4] = new SnTransform;


	car1->model()->load_obj("../src/Models_and_Textures/car1.obj");
	car1->model()->centralize();
	car1->model()->scale(carScale);
	car1->model()->rotate(GsQuat(GsVec::j, -gspidiv2));
	car1->model()->get_bounding_box(c1);
	car1->color(GsColor::red);
	car1x = -5.0f;
	car1y = c1.dy() / 2;
	car1z = 30.0f;
	carM[0].translation(GsVec(car1x, car1y, car1z));
	carT[0]->set(carM[0]);
	carG1->add(carT[0]);
	carG1->add(car1);
	floortrans[1]->add(carG1);

	SnGroup* carG2 = new SnGroup;
	float car2scale = 0.03f;
	carG2->separator(true);
	car2->model()->load_obj("../src/Models_and_Textures/car2.obj");
	car2->model()->centralize();
	car2->model()->scale(car2scale);
	car2->model()->rotate(GsQuat(GsVec::j, -gspidiv2));
	car2->model()->get_bounding_box(c2);
	car2->color(GsColor::blue);
	car2x = -5.0f;
	//keep as car1
	car2y = c2.dy() / 2;
	car2z = 70.0f;
	carM[1].translation(GsVec(car2x, car2y, car2z));
	carT[1]->set(carM[1]);
	carG2->add(carT[1]);
	carG2->add(car2);
	floortrans[3]->add(carG2);

	SnGroup* carG3 = new SnGroup;
	float car3scale = 5;
	carG3->separator(true);
	car3->model()->load_obj("../src/Models_and_Textures/car3.obj");
	car3->model()->centralize();
	car3->model()->scale(car3scale);
	car3->model()->rotate(GsQuat(GsVec::j, gspidiv2));
	car3->model()->get_bounding_box(c3);
	car3->color(GsColor::magenta);
	car3x = -5.0f;
	//keep as car1
	car3y = c3.dy() / 2;
	car3z = 110;
	carM[2].translation(GsVec(car3x, car3y, car3z));
	carT[2]->set(carM[2]);
	carG3->add(carT[2]);
	carG3->add(car3);
	floortrans[5]->add(carG3);

	SnGroup* carG4 = new SnGroup;
	float car4scale = 5;
	carG4->separator(true);
	car4->model()->load_obj("../src/Models_and_Textures/car4.obj");
	car4->model()->centralize();
	car4->model()->scale(carScale);
	car4->model()->rotate(GsQuat(GsVec::j, gspidiv2));
	car4->model()->get_bounding_box(c4);
	car4->color(GsColor::white);
	car4x = -5.0f;
	car4y = c4.dy() / 2;
	car4z = 150;
	carM[3].translation(GsVec(car4x, car4y, car4z));
	carT[3]->set(carM[3]);
	carG4->add(carT[3]);
	carG4->add(car4);
	floortrans[7]->add(carG4);

	SnGroup* carG5 = new SnGroup;
	float car5scale = 5;
	carG5->separator(true);
	car5->model()->load_obj("../src/Models_and_Textures/car5.obj");
	car5->model()->centralize();
	car5->model()->scale(10);
	car5->model()->rotate(GsQuat(GsVec::i, -gspidiv2));
	car5->model()->get_bounding_box(c5);
	car5->color(GsColor::darkgreen);
	car5x = -5.0f;
	car5y = c5.dy() / 2;
	car5z = 190;
	carM[4].translation(GsVec(car5x, car5y, car5z));
	carT[4]->set(carM[4]);
	carG5->add(carT[4]);
	carG5->add(car5);
	floortrans[9]->add(carG5);

	//Nature Stuff
	SnModel* logs1 = new SnModel;
	logs1->model()->load_obj("../src/Models_and_Textures/logs.obj");
	logs1->model()->centralize();
	logs1->model()->get_bounding_box(b2);
	logs1->model()->scale(2);
	logs1->color(GsColor::brown);
	logs1->model()->rotate(GsQuat(GsVec::i, -gspidiv2));
	logs1->model()->rotate(GsQuat(GsVec::j, gspidiv2));
	logs1->model()->translate(GsVec(-70, (b2.dy() / 2) - 1, 10));
	floortrans[0]->add(logs1);
	SnModel* logs2 = new SnModel;
	logs2->model()->load_obj("../src/Models_and_Textures/logs.obj");
	logs2->model()->centralize();
	logs2->model()->get_bounding_box(b2);
	logs2->model()->scale(2);
	logs2->color(GsColor::brown);
	logs2->model()->rotate(GsQuat(GsVec::i, -gspidiv2));
	logs2->model()->rotate(GsQuat(GsVec::j, gs2pi));
	logs2->model()->translate(GsVec(30, (b2.dy() / 2) - 1, 90));
	floortrans[4]->add(logs2);
	SnModel* logs3 = new SnModel;
	logs3->model()->load_obj("../src/Models_and_Textures/logs.obj");
	logs3->model()->centralize();
	logs3->model()->get_bounding_box(b2);
	logs3->model()->scale(2);
	logs3->color(GsColor::brown);
	logs3->model()->rotate(GsQuat(GsVec::i, -gspidiv2));
	logs3->model()->rotate(GsQuat(GsVec::j, gspi));
	logs3->model()->translate(GsVec(-20, (b2.dy() / 2) - 1, 170));
	floortrans[8]->add(logs3);
	SnModel* logs4 = new SnModel;
	logs4->model()->load_obj("../src/Models_and_Textures/logs.obj");
	logs4->model()->centralize();
	logs4->model()->get_bounding_box(b2);
	logs4->model()->scale(2);
	logs4->color(GsColor::brown);
	logs4->model()->rotate(GsQuat(GsVec::i, -gspidiv2));
	logs4->model()->rotate(GsQuat(GsVec::j, gspidiv2));
	logs4->model()->translate(GsVec(30, (b2.dy() / 2) - 1, 170));
	floortrans[8]->add(logs4);
	
	SnModel* sign1 = new SnModel;
	sign1->model()->load_obj("../src/Models_and_Textures/sign.obj");
	sign1->model()->centralize();
	sign1->model()->get_bounding_box(b2);
	sign1->model()->scale(2);
	sign1->color(GsColor::brown);
	sign1->model()->rotate(GsQuat(GsVec::i, -gspidiv2));
	sign1->model()->rotate(GsQuat(GsVec::j, gspidiv2));
	sign1->model()->translate(GsVec(-50, (b2.dy() / 2) + 4, 10));
	floortrans[0]->add(sign1);
	SnModel* sign2 = new SnModel;
	sign2->model()->load_obj("../src/Models_and_Textures/sign.obj");
	sign2->model()->centralize();
	sign2->model()->get_bounding_box(b2);
	sign2->model()->scale(2);
	sign2->color(GsColor::brown);
	sign2->model()->rotate(GsQuat(GsVec::i, -gspidiv2));
	sign2->model()->rotate(GsQuat(GsVec::j, gs2pi));
	sign2->model()->translate(GsVec(20, (b2.dy() / 2) + 4, 130));
	floortrans[6]->add(sign2);
	SnModel* sign3 = new SnModel;
	sign3->model()->load_obj("../src/Models_and_Textures/sign.obj");
	sign3->model()->centralize();
	sign3->model()->get_bounding_box(b2);
	sign3->model()->scale(2);
	sign3->color(GsColor::brown);
	sign3->model()->rotate(GsQuat(GsVec::i, -gspidiv2));
	sign3->model()->rotate(GsQuat(GsVec::j, gspidiv2));
	sign3->model()->translate(GsVec(-80, (b2.dy() / 2) + 4, 179));
	floortrans[8]->add(sign3);


	
	//SHADOWS FOR ENVIRONMENT

	SnGroup* floortransshadow[10];
	SnGroup* floorshadowG = new SnGroup;
	//SHADOW STUFF

	//SHADOWS FOR ENTIRE SCENE
	SnGroup* SceneShadow = new SnGroup;
	SceneShadowT = new SnTransform;
	SceneShadowMat.translation(GsVec(0, 1, 0));
	SceneShadowT->set(SceneShadowMat);
	SceneShadowT->get().mult(SceneShadowT->get(), perspective_trans2);

	SceneShadow->separator(true);
	SceneShadow->add(SceneShadowT);


	for (int i = 0; i < 10; i++) {
		floormoveshadowT[i] = new SnTransform;
		floormoveshadowM[i].translation(GsVec(0, 1.0f, 0));
		floormoveshadowT[i]->set(floormoveshadowM[i]);
		floortransshadow[i] = new SnGroup;
		floortransshadow[i]->separator(true);
		floortransshadow[i]->add(floormoveshadowT[i]);
		//floortransshadow[i]->add(floor[i]);
		SceneShadow->add(floortransshadow[i]);
	}


	SnModel* fencecopy1 = new SnModel;
	fencecopy1->model()->load_obj("../src/Models_and_Textures/fence.obj");
	fencecopy1->model()->centralize();
	fencecopy1->model()->get_bounding_box(b2);
	fencecopy1->model()->scale(1.9f);
	fencecopy1->color(GsColor::black);
	fencecopy1->model()->rotate(GsQuat(GsVec::i, -gspidiv2));
	fencecopy1->model()->rotate(GsQuat(GsVec::j, gspi));
	fencecopy1->model()->translate(GsVec(-90, (b2.dy() / 2) - 1, 10));
	floortransshadow[0]->add(fencecopy1);
	SnModel* fencecopy2 = new SnModel;
	fencecopy2->model()->load_obj("../src/Models_and_Textures/fence.obj");
	fencecopy2->model()->centralize();
	fencecopy2->model()->get_bounding_box(b2);
	fencecopy2->model()->scale(1.9f);
	fencecopy2->color(GsColor::black);
	fencecopy2->model()->rotate(GsQuat(GsVec::i, -gspidiv2));
	fencecopy2->model()->rotate(GsQuat(GsVec::j, gs2pi));
	fencecopy2->model()->translate(GsVec(90, (b2.dy() / 2) - 1, 10));
	floortransshadow[0]->add(fencecopy2);
	SnModel* fencecopy3 = new SnModel;
	fencecopy3->model()->load_obj("../src/Models_and_Textures/fence.obj");
	fencecopy3->model()->centralize();
	fencecopy3->model()->get_bounding_box(b2);
	fencecopy3->model()->scale(1.9f);
	fencecopy3->color(GsColor::black);
	fencecopy3->model()->rotate(GsQuat(GsVec::i, -gspidiv2));
	fencecopy3->model()->rotate(GsQuat(GsVec::j, gspi));
	fencecopy3->model()->translate(GsVec(-90, (b2.dy() / 2) - 1, 50));
	floortransshadow[2]->add(fencecopy3);
	SnModel* fencecopy4 = new SnModel;
	fencecopy4->model()->load_obj("../src/Models_and_Textures/fence.obj");
	fencecopy4->model()->centralize();
	fencecopy4->model()->get_bounding_box(b2);
	fencecopy4->model()->scale(1.9f);
	fencecopy4->color(GsColor::black);
	fencecopy4->model()->rotate(GsQuat(GsVec::i, -gspidiv2));
	fencecopy4->model()->rotate(GsQuat(GsVec::j, gs2pi));
	fencecopy4->model()->translate(GsVec(90, (b2.dy() / 2) - 1, 50));
	floortransshadow[2]->add(fencecopy4);
	SnModel* fencecopy5 = new SnModel;
	fencecopy5->model()->load_obj("../src/Models_and_Textures/fence.obj");
	fencecopy5->model()->centralize();
	fencecopy5->model()->get_bounding_box(b2);
	fencecopy5->model()->scale(1.9f);
	fencecopy5->color(GsColor::black);
	fencecopy5->model()->rotate(GsQuat(GsVec::i, -gspidiv2));
	fencecopy5->model()->rotate(GsQuat(GsVec::j, gspi));
	fencecopy5->model()->translate(GsVec(-90, (b2.dy() / 2) - 1, 90));
	floortransshadow[4]->add(fencecopy5);
	SnModel* fencecopy6 = new SnModel;
	fencecopy6->model()->load_obj("../src/Models_and_Textures/fence.obj");
	fencecopy6->model()->centralize();
	fencecopy6->model()->get_bounding_box(b2);
	fencecopy6->model()->scale(1.9f);
	fencecopy6->color(GsColor::black);
	fencecopy6->model()->rotate(GsQuat(GsVec::i, -gspidiv2));
	fencecopy6->model()->rotate(GsQuat(GsVec::j, gs2pi));
	fencecopy6->model()->translate(GsVec(90, (b2.dy() / 2) - 1, 90));
	floortransshadow[4]->add(fencecopy6);
	SnModel* fencecopy7 = new SnModel;
	fencecopy7->model()->load_obj("../src/Models_and_Textures/fence.obj");
	fencecopy7->model()->centralize();
	fencecopy7->model()->get_bounding_box(b2);
	fencecopy7->model()->scale(1.9f);
	fencecopy7->color(GsColor::black);
	fencecopy7->model()->rotate(GsQuat(GsVec::i, -gspidiv2));
	fencecopy7->model()->rotate(GsQuat(GsVec::j, gspi));
	fencecopy7->model()->translate(GsVec(-90, (b2.dy() / 2) - 1, 130));
	floortransshadow[6]->add(fencecopy7);
	SnModel* fencecopy8 = new SnModel;
	fencecopy8->model()->load_obj("../src/Models_and_Textures/fence.obj");
	fencecopy8->model()->centralize();
	fencecopy8->model()->get_bounding_box(b2);
	fencecopy8->model()->scale(1.9f);
	fencecopy8->color(GsColor::black);
	fencecopy8->model()->rotate(GsQuat(GsVec::i, -gspidiv2));
	fencecopy8->model()->rotate(GsQuat(GsVec::j, gs2pi));
	fencecopy8->model()->translate(GsVec(90, (b2.dy() / 2) - 1, 130));
	floortransshadow[6]->add(fencecopy8);
	SnModel* fencecopy9 = new SnModel;
	fencecopy9->model()->load_obj("../src/Models_and_Textures/fence.obj");
	fencecopy9->model()->centralize();
	fencecopy9->model()->get_bounding_box(b2);
	fencecopy9->color(GsColor::black);
	fencecopy9->model()->scale(1.9f);
	fencecopy9->model()->rotate(GsQuat(GsVec::i, -gspidiv2));
	fencecopy9->model()->rotate(GsQuat(GsVec::j, gspi));
	fencecopy9->model()->translate(GsVec(-90, (b2.dy() / 2) - 1, 170));
	floortransshadow[8]->add(fencecopy9);
	SnModel* fencecopy10 = new SnModel;
	fencecopy10->model()->load_obj("../src/Models_and_Textures/fence.obj");
	fencecopy10->model()->centralize();
	fencecopy10->model()->get_bounding_box(b2);
	fencecopy10->model()->scale(1.9f);
	fencecopy10->color(GsColor::black);
	fencecopy10->model()->rotate(GsQuat(GsVec::i, -gspidiv2));
	fencecopy10->model()->rotate(GsQuat(GsVec::j, gs2pi));
	fencecopy10->model()->translate(GsVec(90, (b2.dy() / 2) - 1, 170));
	floortransshadow[8]->add(fencecopy10);

	//Nature and other objects in scene
	//SnGroup* nature = new SnGroup;
	//nature->separator(true);

	if (!level2) {
		SnModel* treecopy1 = new SnModel;
		treecopy1->model()->load_obj("../src/Models_and_Textures/Lowpoly_tree_sample.obj");
		treecopy1->model()->centralize();
		treecopy1->model()->get_bounding_box(b1);
		treecopy1->color(GsColor::black);
		treecopy1->model()->translate(GsVec(30, (b1.dy() / 2), 10));
		floortransshadow[0]->add(treecopy1);
		SnModel * treecopy2 = new SnModel;
		treecopy2->model()->load_obj("../src/Models_and_Textures/Lowpoly_tree_sample.obj");
		treecopy2->model()->centralize();
		treecopy2->model()->get_bounding_box(b1);
		treecopy2->color(GsColor::black);
		treecopy2->model()->translate(GsVec(60, (b1.dy() / 2), 90));
		floortransshadow[4]->add(treecopy2);
		SnModel * treecopy3 = new SnModel;
		treecopy3->model()->load_obj("../src/Models_and_Textures/Lowpoly_tree_sample.obj");
		treecopy3->model()->centralize();
		treecopy3->model()->get_bounding_box(b2);
		treecopy3->color(GsColor::black);
		treecopy3->model()->translate(GsVec(-50, (b2.dy() / 2) - 6, 130));
		floortransshadow[6]->add(treecopy3);
		SnModel * treecopy4 = new SnModel;
		treecopy4->model()->load_obj("../src/Models_and_Textures/Lowpoly_tree_sample.obj");
		treecopy4->model()->centralize();
		treecopy4->model()->get_bounding_box(b1);
		treecopy4->color(GsColor::black);
		treecopy4->model()->translate(GsVec(60, (b1.dy() / 2), 170));
		floortransshadow[8]->add(treecopy4);
	}
	if (level2) {

		SnModel* treecopy1 = new SnModel;
		treecopy1->model()->load_obj("../src/Models_and_Textures/pinetrees.obj");
		treecopy1->model()->centralize();
		treecopy1->model()->get_bounding_box(b1);
		treecopy1->color(GsColor::black);
		treecopy1->model()->scale(2);
		treecopy1->model()->rotate(GsQuat(GsVec::i, -gspidiv2));
		treecopy1->model()->rotate(GsQuat(GsVec::j, -gspidiv2));
		treecopy1->model()->translate(GsVec(30, (b1.dy() / 2) + 4, 10));
		floortransshadow[0]->add(treecopy1);
		SnModel* treecopy2 = new SnModel;
		treecopy2->model()->load_obj("../src/Models_and_Textures/pinetrees.obj");
		treecopy2->model()->centralize();
		treecopy2->model()->get_bounding_box(b1);
		treecopy2->color(GsColor::black);
		treecopy2->model()->scale(2);
		treecopy2->model()->rotate(GsQuat(GsVec::i, -gspidiv2));
		treecopy2->model()->rotate(GsQuat(GsVec::j, -gspidiv2));
		treecopy2->model()->translate(GsVec(30, (b1.dy() / 2) + 4, 10));
		floortransshadow[4]->add(treecopy2);
		SnModel* treecopy3 = new SnModel;
		treecopy3->model()->load_obj("../src/Models_and_Textures/pinetrees.obj");
		treecopy3->model()->centralize();
		treecopy3->model()->get_bounding_box(b2);
		treecopy3->color(GsColor::black);
		treecopy3->model()->scale(2);
		treecopy3->model()->rotate(GsQuat(GsVec::i, -gspidiv2));
		treecopy3->model()->rotate(GsQuat(GsVec::j, -gspidiv2));
		treecopy3->model()->translate(GsVec(30, (b1.dy() / 2) + 4, 10));
		floortransshadow[6]->add(treecopy3);
		SnModel* treecopy4 = new SnModel;
		treecopy4->model()->load_obj("../src/Models_and_Textures/pinetrees.obj");
		treecopy4->model()->centralize();
		treecopy4->model()->get_bounding_box(b1);
		treecopy4->color(GsColor::black);
		treecopy4->model()->scale(2);
		treecopy4->model()->rotate(GsQuat(GsVec::i, -gspidiv2));
		treecopy4->model()->rotate(GsQuat(GsVec::j, -gspidiv2));
		treecopy4->model()->translate(GsVec(30, (b1.dy() / 2) + 4, 10));
		floortransshadow[8]->add(treecopy4);
	}

	SnModel* pinetreecopy1 = new SnModel;
	pinetreecopy1->model()->load_obj("../src/Models_and_Textures/pinetrees.obj");
	pinetreecopy1->model()->centralize();
	pinetreecopy1->model()->get_bounding_box(b1);
	pinetreecopy1->model()->scale(2);
	pinetreecopy1->color(GsColor::black);
	pinetreecopy1->model()->rotate(GsQuat(GsVec::i, -gspidiv2));
	pinetreecopy1->model()->rotate(GsQuat(GsVec::j, -gspidiv2));
	pinetreecopy1->model()->translate(GsVec(-40, (b1.dy() / 2) + 4, 50));
	floortransshadow[2]->add(pinetreecopy1);
	SnModel* pinetreecopy2 = new SnModel;
	pinetreecopy2->model()->load_obj("../src/Models_and_Textures/pinetrees.obj");
	pinetreecopy2->model()->centralize();
	pinetreecopy2->model()->get_bounding_box(b2);
	pinetreecopy2->model()->scale(2);
	pinetreecopy2->color(GsColor::black);
	pinetreecopy2->model()->rotate(GsQuat(GsVec::i, -gspidiv2));
	pinetreecopy2->model()->rotate(GsQuat(GsVec::j, -gspidiv2));
	pinetreecopy2->model()->translate(GsVec(50, (b1.dy() / 2) + 4, 50));
	floortransshadow[2]->add(pinetreecopy2);
	SnModel* pinetreecopy3 = new SnModel;
	pinetreecopy3->model()->load_obj("../src/Models_and_Textures/pinetrees.obj");
	pinetreecopy3->model()->centralize();
	pinetreecopy3->model()->get_bounding_box(b1);
	pinetreecopy3->model()->scale(2);
	pinetreecopy3->color(GsColor::black);
	pinetreecopy3->model()->rotate(GsQuat(GsVec::i, -gspidiv2));
	pinetreecopy3->model()->rotate(GsQuat(GsVec::j, gspidiv2));
	pinetreecopy3->model()->translate(GsVec(-60, (b1.dy() / 2) + 4, 90));
	floortransshadow[4]->add(pinetreecopy3);
	SnModel* pinetreecopy4 = new SnModel;
	pinetreecopy4->model()->load_obj("../src/Models_and_Textures/pinetrees.obj");
	pinetreecopy4->model()->centralize();
	pinetreecopy4->model()->get_bounding_box(b2);
	pinetreecopy4->model()->scale(2);
	pinetreecopy4->color(GsColor::black);
	pinetreecopy4->model()->rotate(GsQuat(GsVec::i, -gspidiv2));
	pinetreecopy4->model()->rotate(GsQuat(GsVec::j, gspidiv2));
	pinetreecopy4->model()->translate(GsVec(50, (b1.dy() / 2) + 4, 130));
	floortransshadow[6]->add(pinetreecopy4);
	SnModel* pinetreescopy5 = new SnModel;
	pinetreescopy5->model()->load_obj("../src/Models_and_Textures/pinetrees.obj");
	pinetreescopy5->model()->centralize();
	pinetreescopy5->model()->get_bounding_box(b2);
	pinetreescopy5->model()->scale(2);
	pinetreescopy5->color(GsColor::black);
	pinetreescopy5->model()->rotate(GsQuat(GsVec::i, -gspidiv2));
	pinetreescopy5->model()->rotate(GsQuat(GsVec::j, -gspidiv2));
	pinetreescopy5->model()->translate(GsVec(-40, (b1.dy() / 2) + 4, 170));
	floortransshadow[8]->add(pinetreescopy5);

	SnModel* logscopy1 = new SnModel;
	logscopy1->model()->load_obj("../src/Models_and_Textures/logs.obj");
	logscopy1->model()->centralize();
	logscopy1->model()->get_bounding_box(b2);
	logscopy1->model()->scale(2);
	logscopy1->color(GsColor::black);
	logscopy1->model()->rotate(GsQuat(GsVec::i, -gspidiv2));
	logscopy1->model()->rotate(GsQuat(GsVec::j, gspidiv2));
	logscopy1->model()->translate(GsVec(-70, (b2.dy() / 2) - 1, 10));
	floortransshadow[0]->add(logscopy1);
	SnModel* logscopy2 = new SnModel;
	logscopy2->model()->load_obj("../src/Models_and_Textures/logs.obj");
	logscopy2->model()->centralize();
	logscopy2->model()->get_bounding_box(b2);
	logscopy2->model()->scale(2);
	logscopy2->color(GsColor::black);
	logscopy2->model()->rotate(GsQuat(GsVec::i, -gspidiv2));
	logscopy2->model()->rotate(GsQuat(GsVec::j, gs2pi));
	logscopy2->model()->translate(GsVec(30, (b2.dy() / 2) - 1, 90));
	floortransshadow[4]->add(logscopy2);
	SnModel* logscopy3 = new SnModel;
	logscopy3->model()->load_obj("../src/Models_and_Textures/logs.obj");
	logscopy3->model()->centralize();
	logscopy3->model()->get_bounding_box(b2);
	logscopy3->model()->scale(2);
	logscopy3->color(GsColor::black);
	logscopy3->model()->rotate(GsQuat(GsVec::i, -gspidiv2));
	logscopy3->model()->rotate(GsQuat(GsVec::j, gspi));
	logscopy3->model()->translate(GsVec(-20, (b2.dy() / 2) - 1, 170));
	floortransshadow[8]->add(logscopy3);
	SnModel* logscopy4 = new SnModel;
	logscopy4->model()->load_obj("../src/Models_and_Textures/logs.obj");
	logscopy4->model()->centralize();
	logscopy4->model()->get_bounding_box(b2);
	logscopy4->model()->scale(2);
	logscopy4->color(GsColor::black);
	logscopy4->model()->rotate(GsQuat(GsVec::i, -gspidiv2));
	logscopy4->model()->rotate(GsQuat(GsVec::j, gspidiv2));
	logscopy4->model()->translate(GsVec(30, (b2.dy() / 2) - 1, 170));
	floortransshadow[8]->add(logscopy4);

	SnModel* signcopy1 = new SnModel;
	signcopy1->model()->load_obj("../src/Models_and_Textures/sign.obj");
	signcopy1->model()->centralize();
	signcopy1->model()->get_bounding_box(b2);
	signcopy1->model()->scale(2);
	signcopy1->color(GsColor::black);
	signcopy1->model()->rotate(GsQuat(GsVec::i, -gspidiv2));
	signcopy1->model()->rotate(GsQuat(GsVec::j, gspidiv2));
	signcopy1->model()->translate(GsVec(-50, (b2.dy() / 2) + 4, 10));
	floortransshadow[0]->add(signcopy1);
	SnModel* signcopy2 = new SnModel;
	signcopy2->model()->load_obj("../src/Models_and_Textures/sign.obj");
	signcopy2->model()->centralize();
	signcopy2->model()->get_bounding_box(b2);
	signcopy2->model()->scale(2);
	signcopy2->color(GsColor::black);
	signcopy2->model()->rotate(GsQuat(GsVec::i, -gspidiv2));
	signcopy2->model()->rotate(GsQuat(GsVec::j, gs2pi));
	signcopy2->model()->translate(GsVec(20, (b2.dy() / 2) + 4, 130));
	floortransshadow[6]->add(signcopy2);
	SnModel* signcopy3 = new SnModel;
	signcopy3->model()->load_obj("../src/Models_and_Textures/sign.obj");
	signcopy3->model()->centralize();
	signcopy3->model()->get_bounding_box(b2);
	signcopy3->model()->scale(2);
	signcopy3->color(GsColor::black);
	signcopy3->model()->rotate(GsQuat(GsVec::i, -gspidiv2));
	signcopy3->model()->rotate(GsQuat(GsVec::j, gspidiv2));
	signcopy3->model()->translate(GsVec(-80, (b2.dy() / 2) + 4, 179));
	floortransshadow[8]->add(signcopy3);


	//SHADOWS FOR CAR





	SnGroup* carShadowG1 = new SnGroup;
	carShadowG1->separator(true);
	SnModel* carShadow1 = new SnModel;
	SnModel* carShadow2 = new SnModel;
	SnModel* carShadow3 = new SnModel;
	SnModel* carShadow4 = new SnModel;
	SnModel* carShadow5 = new SnModel;
	carShadowT[0] = new SnTransform;
	carShadowT[1] = new SnTransform;
	carShadowT[2] = new SnTransform;
	carShadowT[3] = new SnTransform;
	carShadowT[4] = new SnTransform;


	carShadow1->model()->load_obj("../src/Models_and_Textures/car1.obj");
	carShadow1->model()->centralize();
	carShadow1->model()->scale(carScale);
	carShadow1->model()->rotate(GsQuat(GsVec::j, -gspidiv2));
	carShadow1->model()->get_bounding_box(c1);
	carShadow1->color(GsColor::black);
	carshadow1x = -5.0f;
	carshadow1y = c1.dy() / 2;
	carshadow1z = 30.0f;
	carShadowM[0].translation(GsVec(carshadow1x, carshadow1y, carshadow1z));
	carShadowT[0]->set(carShadowM[0]);
	carShadowG1->add(carShadowT[0]);
	carShadowG1->add(carShadow1);
	floortransshadow[1]->add(carShadowG1);

	SnGroup* carShadowG2 = new SnGroup;
	car2scale = 0.03;
	carShadowG2->separator(true);
	carShadow2->model()->load_obj("../src/Models_and_Textures/car2.obj");
	carShadow2->model()->centralize();
	carShadow2->model()->scale(car2scale);
	carShadow2->model()->rotate(GsQuat(GsVec::j, -gspidiv2));
	carShadow2->model()->get_bounding_box(c2);
	carShadow2->color(GsColor::black);
	carshadow2x = -5.0f;
	//keep as car1
	carshadow2y = c2.dy() / 2;
	carshadow2z = 70.0f;
	carShadowM[1].translation(GsVec(carshadow2x, carshadow2y, carshadow2z));
	carShadowT[1]->set(carShadowM[1]);
	carShadowG2->add(carShadowT[1]);
	carShadowG2->add(carShadow2);
	floortransshadow[3]->add(carShadowG2);

	SnGroup* carShadowG3 = new SnGroup;
	car3scale = 5;
	carShadowG3->separator(true);
	carShadow3->model()->load_obj("../src/Models_and_Textures/car3.obj");
	carShadow3->model()->centralize();
	carShadow3->model()->scale(car3scale);
	carShadow3->model()->rotate(GsQuat(GsVec::j, gspidiv2));
	carShadow3->model()->get_bounding_box(c3);
	carShadow3->color(GsColor::black);
	carshadow3x = -5.0f;
	//keep as car1
	carshadow3y = c3.dy() / 2;
	carshadow3z = 110;
	carShadowM[2].translation(GsVec(carshadow3x, carshadow3y, carshadow3z));
	carShadowT[2]->set(carShadowM[2]);
	carShadowG3->add(carShadowT[2]);
	carShadowG3->add(carShadow3);
	floortransshadow[5]->add(carShadowG3);

	SnGroup* carShadowG4 = new SnGroup;
	car4scale = 5;
	carShadowG4->separator(true);
	carShadow4->model()->load_obj("../src/Models_and_Textures/car4.obj");
	carShadow4->model()->centralize();
	carShadow4->model()->scale(carScale);
	carShadow4->model()->rotate(GsQuat(GsVec::j, gspidiv2));
	carShadow4->model()->get_bounding_box(c4);
	carShadow4->color(GsColor::black);
	carshadow4x = -5.0f;
	carshadow4y = c4.dy() / 2;
	carshadow4z = 150;
	carShadowM[3].translation(GsVec(carshadow4x, carshadow4y, carshadow4z));
	carShadowT[3]->set(carShadowM[3]);
	carShadowG4->add(carShadowT[3]);
	carShadowG4->add(carShadow4);
	floortransshadow[7]->add(carShadowG4);

	SnGroup* carShadowG5 = new SnGroup;
	car5scale = 5;
	carShadowG5->separator(true);
	carShadow5->model()->load_obj("../src/Models_and_Textures/car5.obj");
	carShadow5->model()->centralize();
	carShadow5->model()->scale(10);
	carShadow5->model()->rotate(GsQuat(GsVec::i, -gspidiv2));
	carShadow5->model()->get_bounding_box(c5);
	carShadow5->color(GsColor::black);
	carshadow5x = -5.0f;
	carshadow5y = c5.dy() / 2;
	carshadow5z = 190;
	carShadowM[4].translation(GsVec(carshadow5x, carshadow5y, carshadow5z));
	carShadowT[4]->set(carShadowM[4]);
	carShadowG5->add(carShadowT[4]);
	carShadowG5->add(carShadow5);
	floortransshadow[9]->add(carShadowG5);





	//Bird that acts as the player
	SnModel * Bird = new SnModel;
	SnGroup * BirdGroup = new SnGroup;
	BirdGroup->separator(true);
	Bird->model()->load_obj("../src/Models_and_Textures/birdbody.obj");
	Bird->model()->centralize();
	Bird->color(GsColor::lightblue);
	Bird->model()->get_bounding_box(birdBox);
	BirdT = new SnTransform;
	//Bird->model()->scale(5);
	birdX = 0;
	birdZ = 10;
	//birdY = (5 * (birdBox.dy() / 2));
	birdY = birdBox.dy() / 2;
	//startingbirdY = (5 * (birdBox.dy() / 2));
	BirdM.translation(GsVec(birdX, birdY, birdZ));
	BirdT->set(BirdM);

	SnModel * leftwing = new SnModel();
	SnGroup * leftwingGroup = new SnGroup;
	leftwing->model()->load("../src/Models_and_Textures/leftwing.obj");
	leftwing->model()->centralize();
	leftWT = new SnTransform;
	leftwing->model()->scale(1);
	leftwing->color(GsColor::lightblue);
	wingM.translation(GsVec(4, 3, 0));
	leftWT->set(wingM);
	leftwingGroup->add(leftWT);
	leftwingGroup->add(leftwing);
	leftwingGroup->separator(true);

	SnModel * rightwing = new SnModel();
	SnGroup * rightwingGroup = new SnGroup;
	rightwing->model()->load("../src/Models_and_Textures/rightwing.obj");
	rightwing->model()->centralize();
	rightwing->model()->scale(1);
	rightwing->color(GsColor::lightblue);
	rightWT = new SnTransform;
	wingM.translation(GsVec(-4, 3, 0));
	rightWT->set(wingM);
	rightwingGroup->add(rightWT);
	rightwingGroup->add(rightwing);

	//SHADOW STUFF
	SnGroup* ShadowBirdg = new SnGroup;
	SnModel* BirdShadow = new SnModel;

	//SHADOW FOR THE BIRD'S BODY
	BirdShadow->model()->load_obj("../src/Models_and_Textures/birdbody.obj");
	BirdShadow->model()->centralize();
	BirdShadow->color(GsColor::black);
	BirdShadow->model()->get_bounding_box(birdBox);
	ShadowBT = new SnTransform;
	//Bird->model()->scale(5);
	birdX = 0;
	birdZ = 10;
	//birdY = (5 * (birdBox.dy() / 2));
	birdY = birdBox.dy() / 2;
	//startingbirdY = (5 * (birdBox.dy() / 2));
	shadowbirdx = birdX - 0.0f;
	shadowbirdy = birdY - 4.25f;
	shadowbirdz = birdZ - 5;
	Shadowmat.translation(GsVec(birdX - 0.0f, birdY - 4.25f, birdZ - 5));
	ShadowBT->set(Shadowmat);
	ShadowBT->get().mult(ShadowBT->get(), perspective_trans);

	//SHADOW FOR BIRD'S LEFT WING
	SnModel* leftwingshadow = new SnModel;
	SnGroup* leftwingshadowGroup = new SnGroup;
	leftwingshadow->model()->load("../src/Models_and_Textures/leftwing.obj");
	leftwingshadow->model()->centralize();
	leftWTShadow = new SnTransform;
	leftwingshadow->model()->scale(1);
	leftwingshadow->color(GsColor::black);
	Shadowmat2.translation(GsVec(4, 3, 3));
	leftWTShadow->set(Shadowmat2);
	leftWTShadow->get().mult(leftWTShadow->get(), perspective_trans2);
	leftwingshadowGroup->add(leftWTShadow);
	leftwingshadowGroup->add(leftwingshadow);
	leftwingGroup->separator(true);

	//SHADOW FOR BIRD'S RIGHT WING
	SnModel* rightwingshadow = new SnModel;
	SnGroup* rightwingshadowGroup = new SnGroup;
	rightwingshadow->model()->load("../src/Models_and_Textures/rightwing.obj");
	rightwingshadow->model()->centralize();
	rightWTShadow = new SnTransform;
	rightwingshadow->model()->scale(1);
	rightwingshadow->color(GsColor::black);
	Shadowmat2.translation(GsVec(-7, 4, 5));
	rightWTShadow->set(Shadowmat2);
	rightWTShadow->get().mult(rightWTShadow->get(), perspective_trans2);
	rightwingshadowGroup->add(rightWTShadow);
	rightwingshadowGroup->add(rightwingshadow);
	rightwingshadowGroup->separator(true);

	ShadowBirdg->add(ShadowBT);
	ShadowBirdg->add(BirdShadow);
	ShadowBirdg->add(leftwingshadowGroup);
	ShadowBirdg->add(rightwingshadowGroup);
	ShadowBirdg->separator(true);

	BirdGroup->add(BirdT);
	BirdGroup->add(Bird);
	BirdGroup->add(leftwingGroup);
	BirdGroup->add(rightwingGroup);

	//Flying bird for scene
	flyT = new SnTransform;
	flyMidT = new SnTransform;
	flyLeftT = new SnTransform;
	flyRightT = new SnTransform;

	SnGroup* flyWhole = new SnGroup;
	SnGroup* flyMidG = new SnGroup;
	SnGroup* flyLeftG = new SnGroup;
	SnGroup* flyRightG = new SnGroup;

	GsModel* flyMid = new GsModel;
	SnModel* flyMidS = new SnModel;
	flyMid->load("../src/Models_and_Textures/flyerMid.obj");
	flyMid->scale((float)0.5);
	flyMid->translate(GsVec(40.0f, 30.0f, 60.0f));
	flyMidS->color(GsColor::darkred);
	flyMidS->model(flyMid);

	GsModel* flyLeft = new GsModel;
	SnModel* flyLeftS = new SnModel;
	flyLeft->load("../src/Models_and_Textures/flyerLeft.obj");
	flyLeft->scale((float)0.5);
	flyLeft->translate(GsVec(40.0f, 30.0f, 60.0f));
	flyLeftS->color(GsColor::darkred);
	flyLeftS->model(flyLeft);

	GsModel* flyRight = new GsModel;
	SnModel* flyRightS = new SnModel;
	flyRight->load("../src/Models_and_Textures/flyerRight.obj");
	flyRight->scale((float)0.5);
	flyRight->translate(GsVec(40.0f, 30.0f, 60.0f));
	flyRightS->color(GsColor::darkred);
	flyRightS->model(flyRight);

	flyMidG->separator(true);
	flyMidG->add(flyMidT);
	flyMidG->add(flyMidS);
	flyWhole->add(flyMidG);

	flyLeftG->separator(true);
	flyLeftG->add(flyLeftT);
	flyLeftG->add(flyLeftS);
	flyWhole->add(flyLeftG);

	flyRightG->separator(true);
	flyRightG->add(flyRightT);
	flyRightG->add(flyRightS);
	flyWhole->add(flyRightG);

	//Add everything to rootg in order
	rootg()->add(global);
	rootg()->add(BirdGroup);
	rootg()->add(ShadowBirdg);
	rootg()->add(nature);
	rootg()->add(floorG);
	rootg()->add(SceneShadow);
	rootg()->add(flyT);
	rootg()->add(flyWhole);
	rootg()->add(cloudAll);
}

void MyViewer::SceneMovement() {
	double frdt = 1.0 / 60.0; // delta time to reach given number of frames per second
	double v = 4; // target velocity is 1 unit per second
	double ti = 0, lt = 0, t0 = gs_time();
	float increment = gs2pi / 60;
	while (forward == false) {
		rot.roty(0.25f * (-1 * float(GS_2PI) / 30.0f));
		flyM = flyT->get() * rot;
		flyT->set(flyM);

		rot.rotz(0.25f * (float(GS_2PI) / 30.0f));
		flyTo.translation(GsVec(40.0f, 30.0f, 60.0f));
		flyLeftM = flyLeftT->get() * flyTo;
		flyLeftM = flyLeftM * rot;
		flyBack.translation(GsVec(-40.0f, -30.0f, -60.0f));
		flyLeftM = flyLeftM * flyBack;
		flyLeftT->set(flyLeftM);

		rot.rotz(0.25f * (-1 * float(GS_2PI) / 30.0f));
		flyRightM = flyRightT->get() * flyTo;
		flyRightM = flyRightM * rot;
		flyRightM = flyRightM * flyBack;
		flyRightT->set(flyRightM);
		flyRightT->set(flyRightM);
	}
}

// Below is an example of how to control the main loop of an animation:
void MyViewer::run_animation()
{
	//camera settings
	camera().center.x = 0;
	camera().center.y = 0;
	camera().center.z = -35;
	camera().eye.x = 0;
	camera().eye.y = 35;
	camera().eye.z = 10;
	camera().fovy = gspidiv2;

	//score status
	if (score > highscore) {
		highscore = score;
	}

	message().setf("SCORE: % i HIGH SCORE: % i", score, highscore);
	message_color(GsColor::cyan);

	if (_animating) return; // avoid recursive calls
	_animating = true;

	int ind = 1; // pick one child moves first model
	int ind2 = 2; //move second model

	double frdt = 1.0 / 60.0; // delta time to reach given number of frames per second
	double v = 4; // target velocity is 1 unit per second
	double ti = 0, lt = 0, t0 = gs_time();
	float increment = gs2pi / 60;

	do // run for a while:
	{
		if (BirdPos < 10) { BirdPos++; }
		if (BirdPos >= 10) { BirdPos = 0; }
		//gsout << BirdPos << gsnl;
		while (ti - lt < frdt) { ws_check(); ti = gs_time() - t0; } // wait until it is time for next frame
		lt = ti;
		int cnt = 0;

		double frdt = 1.0 / 60.0; // delta time to reach given number of frames per second
		double v = 4; // target velocity is 1 unit per second
		double t = 0, lt = 0, t0 = gs_time();
		float count = 0.0f;
		float winginc = 0.0f;
		float winginc2 = 0.0f;
		GsMat wingtemp, rightwingt, shadowrightwingt, shadowleftwingt;
		do // run for a while:
		{
			
			while (t - lt < frdt) { ws_check(); t = gs_time() - t0; } // wait until it is time for next frame
			double yinc = 0.5f;
			//animating the bird model of the player
			if (count >= 10.0f) {
				yinc = -yinc; // after 2 secs: go down
				//GOING DOWN
				//LEFT WING
				wingtemp.translation(GsVec(4, 3, 1)); //LEFT WING
				winginc = float(GS_2PI / 20.0);
				wingM.rotz(winginc);
				leftWT->get().mult(wingtemp, wingM);



				//RIGHT WING
				rightwingt.translation(GsVec(-4, 3, 1));
				winginc = float(-GS_2PI / 20);
				wingM.rotz(winginc);
				rightWT->get().mult(rightwingt, wingM);


				//SHADOW ANIMATION FOR LEFT WING
				shadowleftwingt.translation(4, 3, 2);
				winginc2 = float(GS_2PI / 20);
				wingM.rotz(winginc2);
				leftWTShadow->get().mult(shadowleftwingt, wingM);


				//SHADOW ANIMATION FOR RIGHT WING
				shadowrightwingt.translation(GsVec(-7, 4, 1.75f));
				winginc2 = float(-GS_2PI / 10);
				wingM.rotz(winginc2);
				rightWTShadow->get().mult(shadowrightwingt, wingM);
			}
			else {
				//GOING UP
				//BIRD ANIMATION
				wingtemp.translation(GsVec(4, 2.5f, 1));
				winginc = -float(GS_2PI / 15);
				wingM.rotz(winginc);
				leftWT->get().mult(wingtemp, wingM);

				//RIGHT WING
				rightwingt.translation(GsVec(-4, 2.5f, 1));
				winginc = float(GS_2PI / 15);
				wingM.rotz(winginc);
				rightWT->get().mult(rightwingt, wingM);

				//SHADOW ANIMATION FOR LEFT WING
				shadowleftwingt.translation(4, 3, 4);
				winginc2 = float(-GS_2PI / 15);
				wingM.rotz(winginc2);
				leftWTShadow->get().mult(shadowleftwingt, wingM);

				//SHADOW ANIMATION FOR RIGHT WING
				shadowrightwingt.translation(GsVec(-10, 3.5f, 7));
				winginc2 = float(GS_2PI / 10);
				wingM.rotz(winginc2);
				rightWTShadow->get().mult(shadowrightwingt, wingM);
			}
			lt = t;
			if (birdY >= 0) {
				birdY = birdY + (float)yinc;
				shadowbirdz = shadowbirdz - (float)yinc;
			}


			//MOVING THE BIRD'S SHADOW
			Shadowmat.translation(GsVec(shadowbirdx, shadowbirdy, shadowbirdz));
			ShadowBT->get().mult(Shadowmat, perspective_trans);

			//when player presses forward, move the entire block of all scene objects backward
			count += 0.5f;
			floorz = floorz - 0.5f;
			floorM.translation(GsVec(0, 0, floorz));
			floorT->set(floorM);

			//SHADOW FOR THE ENVIRONMENT
			SceneShadowMat.translation(GsVec(0, 0.25f, floorz));
			SceneShadowT->set(SceneShadowMat);
			SceneShadowT->get().mult(SceneShadowT->get(), perspective_trans2);


			//animating the bird flying around the scene
			rot.roty(0.05f * (-1 * float(GS_2PI) / 30.0f));
			flyM = flyT->get() * rot;
			flyT->set(flyM);
			rot.rotz(0.1f * (float(GS_2PI) / 30.0f));
			flyTo.translation(GsVec(40.0f, 30.0f, 60.0f));
			flyLeftM = flyLeftT->get() * flyTo;
			flyLeftM = flyLeftM * rot;
			flyBack.translation(GsVec(-40.0f, -30.0f, -60.0f));
			flyLeftM = flyLeftM * flyBack;
			flyLeftT->set(flyLeftM);
			rot.rotz(0.1f * (-1 * float(GS_2PI) / 30.0f));
			flyRightM = flyRightT->get() * flyTo;
			flyRightM = flyRightM * rot;
			flyRightM = flyRightM * flyBack;
			flyRightT->set(flyRightM);
			flyRightT->set(flyRightM);


			//Car Movement
			float carInc = 4.5;
			car1x = car1x + carInc;
			if (car1x > 400) {
				car1x = -400;
			}
			car1x = car1x + carInc;
			carM[0].translation(car1x, car1y, car1z);
			carT[0]->set(carM[0]);

			float car2Inc = -3;
			if (car2x < -300) {
				car2x = 300;
			}
			car2x = car2x + car2Inc;
			carM[1].translation(car2x, car2y, car2z);
			carT[1]->set(carM[1]);

			float car3Inc = 2.1f;
			if (car3x > 300) {
				car3x = -300;
			}
			car3x = car3x + car3Inc;
			carM[2].translation(car3x, car3y, car3z);
			carT[2]->set(carM[2]);

			float car4Inc = 8.0f;
			if (car4x > 450) {
				car4x = -450;
			}
			car4x = car4x + car4Inc;
			carM[3].translation(car4x, car4y, car4z);
			carT[3]->set(carM[3]);

			float car5Inc = -3.0f;
			if (car5x < -300) {
				car5x = 300;
			}
			car5x = car5x + car5Inc;
			carM[4].translation(car5x, car5y, car5z);
			carT[4]->set(carM[4]);


			if (car1x < 5 && car1x > -5 && BirdPos == 2) {
				gsout << "COLLISION" << gsnl;
			}
			if (car2x < 5 && car2x > -5 && BirdPos == 4) {
				gsout << "COLLISION" << gsnl;
			}
			if (car3x < 5 && car3x > -5 && BirdPos == 6) {
				gsout << "COLLISION" << gsnl;
			}
			if (car4x < 5 && car4x > -5 && BirdPos == 8) {
				gsout << "COLLISION" << gsnl;
			}
			if (car5x < 5 && car5x > -5 && BirdPos == 0) {
				gsout << "COLLISION" << gsnl;
			}
			
			//SHADOW FOR THE CAR
			float carShadowInc = 4.5;
			carshadow1x = carshadow1x + carShadowInc;
			if (carshadow1x > 400) {
				carshadow1x = -400;
			}
			carshadow1x = carshadow1x + carShadowInc;
			carShadowM[0].translation(carshadow1x, carshadow1y, carshadow1z);
			carShadowT[0]->set(carShadowM[0]);

			float carShadowInc2 = -3;
			if (carshadow2x < -300) {
				carshadow2x = 300;
			}
			carshadow2x = carshadow2x + carShadowInc2;
			carShadowM[1].translation(carshadow2x, carshadow2y, carshadow2z);
			carShadowT[1]->set(carShadowM[1]);

			float carShadowInc3 = 2.1;
			if (carshadow3x > 300) {
				carshadow3x = -300;
			}
			carshadow3x = carshadow3x + carShadowInc3;
			carShadowM[2].translation(carshadow3x, carshadow3y, carshadow3z);
			carShadowT[2]->set(carShadowM[2]);

			float carShadowInc4 = 8.0f;
			if (carshadow4x > 450) {
				carshadow4x = -450;
			}
			carshadow4x = carshadow4x + carShadowInc4;
			carShadowM[3].translation(carshadow4x, carshadow4y, carshadow4z);
			carShadowT[3]->set(carShadowM[3]);

			float carShadowInc5 = -3.0f;
			if (carshadow5x < -300) {
				carshadow5x = 300;
			}
			carshadow5x = carshadow5x + carShadowInc5;
			carShadowM[4].translation(carshadow5x, carshadow5y, carshadow5z);
			carShadowT[4]->set(carShadowM[4]);



			BirdM.translation(GsVec(birdX, birdY, birdZ));
			BirdT->set(BirdM);
			

			render();
			ws_check(); 
		} while (count < 20 && forward == true);//do this loop while only letting the scene move backward by one floor panel's width

		//reset variables
		_animating = false;
		forward = false;

	} while (_animating);

	//every time we move forward, loop floor tiles to give the effect of an endless loop
	if (moveCount >= 0) {
		
		if (moveCount%10 == 0) {
			zmove += 200;
			zshadowmove += 200;
		}
		floormoveM[moveCount%10].translation(GsVec(0, 0, zmove));
		floormoveT[moveCount%10]->set(floormoveM[moveCount%10]);
		SceneShadowMat.translation(GsVec(0, 0.25f, zshadowmove));
		floormoveshadowT[moveCount%10]->set(SceneShadowMat);
		floormoveshadowT[moveCount%10]->get().mult(floormoveshadowT[moveCount%10]->get(), perspective_trans2);
	}

	//enter level 2
	if (moveCount == 18 && level2 == false) {
		level2 = true;
		build_scene();
	}

	//animate bird above scene while floor is not moving
	do {
		if (car1x < 5 && car1x > -5 && BirdPos == 2) {
			gsout << "COLLISION" << gsnl;
		}
		if (car1x < 5 && car1x > -5 && BirdPos == 2) {
			gsout << "COLLISION" << gsnl;
		}
		if (car2x < 5 && car2x > -5 && BirdPos == 4) {
			gsout << "COLLISION" << gsnl;
		}
		if (car3x < 5 && car3x > -5 && BirdPos == 6) {
			gsout << "COLLISION" << gsnl;
		}
		if (car4x < 5 && car4x > -5 && BirdPos == 8) {
			gsout << "COLLISION" << gsnl;
		}
		if (car5x < 5 && car5x > -5 && BirdPos == 0) {
			gsout << "COLLISION" << gsnl;
		}
		//gsout << BirdPos << gsnl;
		double frdt = 1.0 / 60.0; // delta time to reach given number of frames per second
		double v = 4; // target velocity is 1 unit per second
		double t = 0, lt = 0, t0 = gs_time();

		while (t - lt < frdt) { ws_check(); t = gs_time() - t0; } // wait until it is time for next frame
		lt = t;

		rot.roty(0.05f* (-1 * float(GS_2PI) / 30.0f));
		flyM = flyT->get() * rot;
		flyT->set(flyM);

		rot.rotz(0.1f* (float(GS_2PI) / 30.0f));
		flyTo.translation(GsVec(40.0f, 30.0f, 60.0f));
		flyLeftM = flyLeftT->get() * flyTo;
		flyLeftM = flyLeftM * rot;
		flyBack.translation(GsVec(-40.0f, -30.0f, -60.0f));
		flyLeftM = flyLeftM * flyBack;
		flyLeftT->set(flyLeftM);

		rot.rotz(0.1f* (-1 * float(GS_2PI) / 30.0f));
		flyRightM = flyRightT->get() * flyTo;
		flyRightM = flyRightM * rot;
		flyRightM = flyRightM * flyBack;
		flyRightT->set(flyRightM);
		flyRightT->set(flyRightM);


		//Car Movement
		float carInc = 4.5;
		car1x = car1x + carInc;
		if (car1x > 400) {
			car1x = -400;
		}
		car1x = car1x + carInc;
		carM[0].translation(car1x, car1y, car1z);
		carT[0]->set(carM[0]);

		float car2Inc = -3;
		if (car2x < -300) {
			car2x = 300;
		}
		car2x = car2x + car2Inc;
		carM[1].translation(car2x, car2y, car2z);
		carT[1]->set(carM[1]);

		float car3Inc = 2.1f;
		if (car3x > 300) {
			car3x = -300;
		}
		car3x = car3x + car3Inc;
		carM[2].translation(car3x, car3y, car3z);
		carT[2]->set(carM[2]);

		float car4Inc = 8.0f;
		if (car4x > 450) {
			car4x = -450;
		}
		car4x = car4x + car4Inc;
		carM[3].translation(car4x, car4y, car4z);
		carT[3]->set(carM[3]);

		float car5Inc = -3.0f;
		if (car5x < -300) {
			car5x = 300;
		}
		car5x = car5x + car5Inc;
		carM[4].translation(car5x, car5y, car5z);
		carT[4]->set(carM[4]);

		BirdM.translation(GsVec(birdX, birdY, birdZ));
		BirdT->set(BirdM);

		//CAR'S SHADOW MOVEMENT WHEN PLAYER IS NOT MOVING
		float carShadowInc = 4.5;
		carshadow1x = carshadow1x + carShadowInc;
		//carshadow1x = carshadow1x + carShadowInc;
		if (carshadow1x > 400) {
			carshadow1x = -400;
		}
		carshadow1x = carshadow1x + carShadowInc;
		carShadowM[0].translation(carshadow1x, carshadow1y, carshadow1z);
		carShadowT[0]->set(carShadowM[0]);

		float carShadow2Inc = -3;
		if (carshadow2x < -300) {
			carshadow2x = 300;
		}
		carshadow2x = carshadow2x + carShadow2Inc;
		carShadowM[1].translation(carshadow2x, carshadow2y, carshadow2z);
		carShadowT[1]->set(carShadowM[1]);

		float carShadow3Inc = 2.1;
		if (carshadow3x > 300) {
			carshadow3x = -300;
		}
		carshadow3x = carshadow3x + carShadow3Inc;
		carShadowM[2].translation(carshadow3x, carshadow3y, carshadow3z);
		carShadowT[2]->set(carShadowM[2]);

		float carShadow4Inc = 8.0f;
		if (carshadow4x > 450) {
			carshadow4x = -450;
		}
		carshadow4x = carshadow4x + carShadow4Inc;
		carShadowM[3].translation(carshadow4x, carshadow4y, carshadow4z);
		carShadowT[3]->set(carShadowM[3]);

		float carShadow5Inc = -3.0f;
		if (carshadow5x < -300) {
			carshadow5x = 300;
		}
		carshadow5x = carshadow5x + carShadow5Inc;
		carShadowM[4].translation(carshadow5x, carshadow5y, carshadow5z);
		carShadowT[4]->set(carShadowM[4]);

		render();
		ws_check();

	} while (forward == false);

}

void MyViewer::show_normals(bool view)
{
	// Note that primitives are only converted to meshes in GsModel
	// at the first draw call.
	GsArray<GsVec> fn;
	SnGroup* r = (SnGroup*)root();
	for (int k = 0; k < r->size(); k++)
	{
		SnManipulator* manip = r->get<SnManipulator>(k);
		SnShape* s = manip->child<SnGroup>()->get<SnShape>(0);
		SnLines* l = manip->child<SnGroup>()->get<SnLines>(1);
		if (!view) { l->visible(false); continue; }
		l->visible(true);
		if (!l->empty()) continue; // build only once
		l->init();
		if (s->instance_name() == SnPrimitive::class_name)
		{
			GsModel& m = *((SnModel*)s)->model();
			m.get_normals_per_face(fn);
			const GsVec* n = fn.pt();
			float f = 0.33f;
			for (int i = 0; i < m.F.size(); i++)
			{
				const GsVec& a = m.V[m.F[i].a]; l->push(a, a + (*n++) * f);
				const GsVec & b = m.V[m.F[i].b]; l->push(b, b + (*n++) * f);
				const GsVec & c = m.V[m.F[i].c]; l->push(c, c + (*n++) * f);
			}
		}
	}
}

int MyViewer::handle_keyboard(const GsEvent & e)
{
	int ret = WsViewer::handle_keyboard(e); // 1st let system check events
	if (ret) return ret;

	switch (e.key)
	{
	case GsEvent::KeyEsc: gs_exit(); return 1;
	case GsEvent::KeyEnter: camera().center.x = 0;
		camera().center.y = 0;
		camera().center.z = -35;
		camera().eye.x = 0;
		camera().eye.y = 35;
		camera().eye.z = 10;
		camera().fovy = gspidiv2; run_animation(); render(); return 1;
	case 65362: {if (_animating == false) { if (moveCount % 2 != 0) { score += 1;} forward = true; moveCount++; run_animation(); render(); } break; }
	default: gsout << "Key pressed: " << e.key << gsnl;
	}
	/*if (update)
	{
		_gLight->get<SnTransform>(0)->get().setrans(lightPos);
		update_shadow();
	}
*/
	return 0;
	return 0;
}

int MyViewer::uievent(int e)
{
	switch (e)
	{
	case EvNormals: show_normals(_nbut->value()); return 1;
	case EvAnimate: run_animation(); return 1;
	case EvExit: gs_exit();
	}
	return WsViewer::uievent(e);
}