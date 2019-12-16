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
	_animating = false;
	build_ui();
	build_scene();

}


void MyViewer::build_ui()
{
	UiPanel* p, * sp;
	UiManager* uim = WsWindow::uim();
	p = uim->add_panel("", UiPanel::HorizLeft);
	p->add(new UiButton("View", sp = new UiPanel()));
	{	UiPanel* p = sp;
	p->add(_nbut = new UiCheckButton("Normals", EvNormals));
	}
	p->add(new UiButton("Animate", EvAnimate));
	p->add(new UiButton("Exit", EvExit)); p->top()->separate();
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
	//The floor of the scene
	SnModel* floor[10];
	SnGroup* floortrans[10];
	float floorX, floorY, floorZP, floorZN;
	floorX = 100.0f;
	floorY = 0.0f;
	floorZP = 20.0f;
	floorZN = 0.0f;
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
	floorgroupGrass.dmap->fname.set("../src/Models_and_Textures/grass.png");
	floorgroupRoad.dmap->fname.set("../src/Models_and_Textures/road.jpg");

	floorgroupGrass1.fi = 0;
	floorgroupRoad1.fi = 0;
	floorgroupGrass1.fn = m2.F.size();
	floorgroupRoad1.fn = m3.F.size();
	floorgroupGrass1.dmap = new GsModel::Texture;
	floorgroupRoad1.dmap = new GsModel::Texture;
	floorgroupGrass1.dmap->fname.set("../src/Models_and_Textures/grass.png");
	floorgroupRoad1.dmap->fname.set("../src/Models_and_Textures/road.jpg");

	floorgroupGrass2.fi = 0;
	floorgroupRoad2.fi = 0;
	floorgroupGrass2.fn = m4.F.size();
	floorgroupRoad2.fn = m5.F.size();
	floorgroupGrass2.dmap = new GsModel::Texture;
	floorgroupRoad2.dmap = new GsModel::Texture;
	floorgroupGrass2.dmap->fname.set("../src/Models_and_Textures/grass.png");
	floorgroupRoad2.dmap->fname.set("../src/Models_and_Textures/road.jpg");

	floorgroupGrass3.fi = 0;
	floorgroupRoad3.fi = 0;
	floorgroupGrass3.fn = m6.F.size();
	floorgroupRoad3.fn = m7.F.size();
	floorgroupGrass3.dmap = new GsModel::Texture;
	floorgroupRoad3.dmap = new GsModel::Texture;
	floorgroupGrass3.dmap->fname.set("../src/Models_and_Textures/grass.png");
	floorgroupRoad3.dmap->fname.set("../src/Models_and_Textures/road.jpg");

	floorgroupGrass4.fi = 0;
	floorgroupRoad4.fi = 0;
	floorgroupGrass4.fn = m8.F.size();
	floorgroupRoad4.fn = m9.F.size();
	floorgroupGrass4.dmap = new GsModel::Texture;
	floorgroupRoad4.dmap = new GsModel::Texture;
	floorgroupGrass4.dmap->fname.set("../src/Models_and_Textures/grass.png");
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
	fence1->model()->load_obj("../src/Models_and_Textures/fence.obj");
	fence1->model()->centralize();
	fence1->model()->get_bounding_box(b2);
	fence1->model()->scale(1.9);
	fence1->color(GsColor::yellow);
	fence1->model()->rotate(GsQuat(GsVec::i, -gspidiv2));
	fence1->model()->rotate(GsQuat(GsVec::j, gspi));
	fence1->model()->translate(GsVec(-90, (b2.dy() / 2) - 1, 10));
	floortrans[0]->add(fence1);
	SnModel* fence2 = new SnModel;
	fence2->model()->load_obj("../src/Models_and_Textures/fence.obj");
	fence2->model()->centralize();
	fence2->model()->get_bounding_box(b2);
	fence2->model()->scale(1.9);
	fence2->color(GsColor::yellow);
	fence2->model()->rotate(GsQuat(GsVec::i, -gspidiv2));
	fence2->model()->rotate(GsQuat(GsVec::j, gs2pi));
	fence2->model()->translate(GsVec(90, (b2.dy() / 2) - 1, 10));
	floortrans[0]->add(fence2);
	SnModel* fence3 = new SnModel;
	fence3->model()->load_obj("../src/Models_and_Textures/fence.obj");
	fence3->model()->centralize();
	fence3->model()->get_bounding_box(b2);
	fence3->model()->scale(1.9);
	fence3->color(GsColor::yellow);
	fence3->model()->rotate(GsQuat(GsVec::i, -gspidiv2));
	fence3->model()->rotate(GsQuat(GsVec::j, gspi));
	fence3->model()->translate(GsVec(-90, (b2.dy() / 2) - 1, 50));
	floortrans[2]->add(fence3);
	SnModel* fence4 = new SnModel;
	fence4->model()->load_obj("../src/Models_and_Textures/fence.obj");
	fence4->model()->centralize();
	fence4->model()->get_bounding_box(b2);
	fence4->model()->scale(1.9);
	fence4->color(GsColor::yellow);
	fence4->model()->rotate(GsQuat(GsVec::i, -gspidiv2));
	fence4->model()->rotate(GsQuat(GsVec::j, gs2pi));
	fence4->model()->translate(GsVec(90, (b2.dy() / 2) - 1, 50));
	floortrans[2]->add(fence4);
	SnModel* fence5 = new SnModel;
	fence5->model()->load_obj("../src/Models_and_Textures/fence.obj");
	fence5->model()->centralize();
	fence5->model()->get_bounding_box(b2);
	fence5->model()->scale(1.9);
	fence5->color(GsColor::yellow);
	fence5->model()->rotate(GsQuat(GsVec::i, -gspidiv2));
	fence5->model()->rotate(GsQuat(GsVec::j, gspi));
	fence5->model()->translate(GsVec(-90, (b2.dy() / 2) - 1, 90));
	floortrans[4]->add(fence5);
	SnModel* fence6 = new SnModel;
	fence6->model()->load_obj("../src/Models_and_Textures/fence.obj");
	fence6->model()->centralize();
	fence6->model()->get_bounding_box(b2);
	fence6->model()->scale(1.9);
	fence6->color(GsColor::yellow);
	fence6->model()->rotate(GsQuat(GsVec::i, -gspidiv2));
	fence6->model()->rotate(GsQuat(GsVec::j, gs2pi));
	fence6->model()->translate(GsVec(90, (b2.dy() / 2) - 1, 90));
	floortrans[4]->add(fence6);
	SnModel* fence7 = new SnModel;
	fence7->model()->load_obj("../src/Models_and_Textures/fence.obj");
	fence7->model()->centralize();
	fence7->model()->get_bounding_box(b2);
	fence7->model()->scale(1.9);
	fence7->color(GsColor::yellow);
	fence7->model()->rotate(GsQuat(GsVec::i, -gspidiv2));
	fence7->model()->rotate(GsQuat(GsVec::j, gspi));
	fence7->model()->translate(GsVec(-90, (b2.dy() / 2) - 1, 130));
	floortrans[6]->add(fence7);
	SnModel* fence8 = new SnModel;
	fence8->model()->load_obj("../src/Models_and_Textures/fence.obj");
	fence8->model()->centralize();
	fence8->model()->get_bounding_box(b2);
	fence8->model()->scale(1.9);
	fence8->color(GsColor::yellow);
	fence8->model()->rotate(GsQuat(GsVec::i, -gspidiv2));
	fence8->model()->rotate(GsQuat(GsVec::j, gs2pi));
	fence8->model()->translate(GsVec(90, (b2.dy() / 2) - 1, 130));
	floortrans[6]->add(fence8);
	SnModel* fence9 = new SnModel;
	fence9->model()->load_obj("../src/Models_and_Textures/fence.obj");
	fence9->model()->centralize();
	fence9->model()->get_bounding_box(b2);
	fence9->color(GsColor::yellow);
	fence9->model()->scale(1.9);
	fence9->model()->rotate(GsQuat(GsVec::i, -gspidiv2));
	fence9->model()->rotate(GsQuat(GsVec::j, gspi));
	fence9->model()->translate(GsVec(-90, (b2.dy() / 2) - 1, 170));
	floortrans[8]->add(fence9);
	SnModel* fence10 = new SnModel;
	fence10->model()->load_obj("../src/Models_and_Textures/fence.obj");
	fence10->model()->centralize();
	fence10->model()->get_bounding_box(b2);
	fence10->model()->scale(1.9);
	fence10->color(GsColor::yellow);
	fence10->model()->rotate(GsQuat(GsVec::i, -gspidiv2));
	fence10->model()->rotate(GsQuat(GsVec::j, gs2pi));
	fence10->model()->translate(GsVec(90, (b2.dy() / 2) - 1, 170));
	floortrans[8]->add(fence10);

	//Nature and other objects in scene
	SnGroup* nature = new SnGroup;
	nature->separator(true);

	SnModel* tree1 = new SnModel;
	tree1->model()->load_obj("../src/Models_and_Textures/Lowpoly_tree_sample.obj");
	tree1->model()->centralize();
	tree1->model()->get_bounding_box(b1);
	tree1->color(GsColor::green);
	tree1->model()->translate(GsVec(30, (b1.dy() / 2), 10));
	floortrans[0]->add(tree1);
	SnModel* tree2 = new SnModel;
	tree2->model()->load_obj("../src/Models_and_Textures/Lowpoly_tree_sample.obj");
	tree2->model()->centralize();
	tree2->model()->get_bounding_box(b1);
	tree2->color(GsColor::green);
	tree2->model()->translate(GsVec(60, (b1.dy() / 2), 90));
	floortrans[4]->add(tree2);
	SnModel* tree3 = new SnModel;
	tree3->model()->load_obj("../src/Models_and_Textures/Lowpoly_tree_sample.obj");
	tree3->model()->centralize();
	tree3->model()->get_bounding_box(b2);
	tree3->color(GsColor::green);
	tree3->model()->translate(GsVec(-50, (b2.dy() / 2) - 6, 130));
	floortrans[6]->add(tree3);
	SnModel* tree4 = new SnModel;
	tree4->model()->load_obj("../src/Models_and_Textures/Lowpoly_tree_sample.obj");
	tree4->model()->centralize();
	tree4->model()->get_bounding_box(b1);
	tree4->color(GsColor::green);
	tree4->model()->translate(GsVec(60, (b1.dy() / 2), 170));
	floortrans[8]->add(tree4);

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
	pinetree4->model()->translate(GsVec(50, (b1.dy() / 2) + 4, 130));
	floortrans[6]->add(pinetree4);
	SnModel* pinetrees5 = new SnModel;
	pinetrees5->model()->load_obj("../src/Models_and_Textures/pinetrees.obj");
	pinetrees5->model()->centralize();
	pinetrees5->model()->get_bounding_box(b2);
	pinetrees5->model()->scale(2);
	pinetrees5->color(GsColor::darkgreen);
	pinetrees5->model()->rotate(GsQuat(GsVec::i, -gspidiv2));
	pinetrees5->model()->rotate(GsQuat(GsVec::j, -gspidiv2));
	pinetrees5->model()->translate(GsVec(-40, (b1.dy() / 2) + 4, 170));
	floortrans[8]->add(pinetrees5);

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
	rootg()->add(nature);
	rootg()->add(floorG);
	rootg()->add(flyT);
	rootg()->add(flyWhole);

}

//void MyViewer::update_shadow() {
//	float R = 3.0f+lightPos.y;
//	GsMat T1, T2;
//	T1.setrans(lightPos * -1.0f);
//	GsMat ShadowM1(
//		-R, 0, 0, 0,
//		0, -R, 0, 0,
//		0, 0, -R, 0,
//		0, 0, 1, 0);
//	_tShadow1->get() = T2 * ShadowM1 * T1 ;
//	/*GsMat rot;
//	rot.rotx((float)GS_PIDIV2);
//	_tShadow1->get().mult(_tShadow1->get(), rot);*/
//	render();
//}

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
		while (ti - lt < frdt) { ws_check(); ti = gs_time() - t0; } // wait until it is time for next frame
		lt = ti;
		int cnt = 0;

		double frdt = 1.0 / 60.0; // delta time to reach given number of frames per second
		double v = 4; // target velocity is 1 unit per second
		double t = 0, lt = 0, t0 = gs_time();
		float count = 0.0f;
		float winginc = 0.0f;
		GsMat wingtemp, rightwingt;
		do // run for a while:
		{
			while (t - lt < frdt) { ws_check(); t = gs_time() - t0; } // wait until it is time for next frame
			double yinc = 0.5f;

			//animating the bird model of the player
			if (count >= 10.0f) {
				yinc = -yinc; // after 2 secs: go down
				wingtemp.translation(GsVec(4, 3, 1));
				rightwingt.translation(GsVec(-4, 3, 0));
				winginc = float(GS_2PI / 20.0);
				wingM.rotz(winginc);
				leftWT->get().mult(wingtemp, wingM);
				winginc = float(-GS_2PI / 20.0);
				wingM.rotz(winginc);
				rightWT->get().mult(rightwingt, wingM);
			}
			else {
				wingtemp.translation(GsVec(4, 2.5f, 1));
				rightwingt.translation(GsVec(-4, 2.5f, 0));
				winginc = -float(GS_2PI / 15);
				wingM.rotz(winginc);
				leftWT->get().mult(wingtemp, wingM);
				winginc = float(GS_2PI / 15);
				wingM.rotz(winginc);
				rightWT->get().mult(rightwingt, wingM);
			}
			lt = t;
			if (birdY >= 0)
				birdY = birdY + (float)yinc;
			BirdM.translation(GsVec(birdX, birdY, birdZ));
			BirdT->set(BirdM);

			//when player presses forward, move the entire block of all scene objects backward
			count += 0.5f;
			floorz = floorz - 0.5f;
			floorM.translation(GsVec(0, 0, floorz));
			floorT->set(floorM);

			//animating the bird flying around the scene
			rot.roty(0.1f * (-1 * float(GS_2PI) / 30.0f));
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

			render();
			ws_check(); 
		} while (count < 20);//do this loop while only letting the scene move backward by one floor panel's width

		//reset variables
		_animating = false;
		forward = false;

	} while (_animating);

	//every time we move forward, loop floor tiles to give the effect of an endless loop
	if (moveCount >= 0) {
		if (moveCount == 0) {
			zmove += 200;
		}
		floormoveM[moveCount].translation(GsVec(0, 0, zmove));
		floormoveT[moveCount]->set(floormoveM[moveCount]);
	}

	//animate bird above scene while floor is not moving
	do {
		double frdt = 1.0 / 60.0; // delta time to reach given number of frames per second
		double v = 4; // target velocity is 1 unit per second
		double t = 0, lt = 0, t0 = gs_time();

		while (t - lt < frdt) { ws_check(); t = gs_time() - t0; } // wait until it is time for next frame
		lt = t;

		rot.roty(0.1f* (-1 * float(GS_2PI) / 30.0f));
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
	case 65362: {if (_animating == false) { forward = true; if (moveCount == 9) { moveCount = 0; } else { moveCount++; } run_animation(); render(); } break; }
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