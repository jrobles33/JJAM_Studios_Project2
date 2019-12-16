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
	SceneMoving = true;
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
	//SnTransform* t = new SnTransform;
	l->color(GsColor::orange);
	g->add(s);
	g->add(l);
	//g->add(t);
	//t->set(manip->mat());
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

	//Nature and other objects in scene
	SnGroup* nature = new SnGroup;
	nature->separator(true);
	SnModel* tree1 = new SnModel;
	tree1->model()->load_obj("../src/Models_and_Textures/Lowpoly_tree_sample.obj");
	tree1->model()->centralize();
	tree1->model()->get_bounding_box(b1);
	tree1->model()->translate(GsVec(30, (b1.dy() / 2), 10));
	nature->add(tree1);
	SnModel* tree2 = new SnModel;
	tree2->model()->load_obj("../src/Models_and_Textures/Lowpoly_tree_sample.obj");
	tree2->model()->centralize();
	tree2->model()->get_bounding_box(b1);
	tree2->model()->rotate(GsQuat(GsVec::j, gspidiv2));
	tree2->model()->translate(GsVec(-40, (b1.dy() / 2), 50));
	nature->add(tree2);
	SnModel* tree3 = new SnModel;
	tree3->model()->load_obj("../src/Models_and_Textures/Lowpoly_tree_sample.obj");
	tree3->model()->centralize();
	tree3->model()->get_bounding_box(b1);
	tree3->model()->translate(GsVec(60, (b1.dy() / 2), 90));
	nature->add(tree3);
	SnModel* tree4 = new SnModel;
	tree4->model()->load_obj("../src/Models_and_Textures/Lowpoly_tree_sample.obj");
	tree4->model()->centralize();
	tree4->model()->get_bounding_box(b1);
	tree4->model()->translate(GsVec(-60, (b1.dy() / 2), 90));
	nature->add(tree4);
	SnModel* tree5 = new SnModel;
	tree5->model()->load_obj("../src/Models_and_Textures/Lowpoly_tree_sample.obj");
	tree5->model()->centralize();
	tree5->model()->get_bounding_box(b1);
	tree5->model()->translate(GsVec(60, (b1.dy() / 2), 170));
	nature->add(tree5);
	SnModel* rand1 = new SnModel;
	rand1->model()->load_obj("../src/Models_and_Textures/LowPolyNature.obj");
	rand1->model()->centralize();
	rand1->model()->get_bounding_box(b2);
	rand1->model()->scale(2);
	rand1->model()->rotate(GsQuat(GsVec::i, -gspidiv2));
	rand1->model()->rotate(GsQuat(GsVec::j, gspidiv2));
	rand1->model()->translate(GsVec(-50, (b2.dy() / 2) - 6, 10));
	nature->add(rand1);
	SnModel* rand2 = new SnModel;
	rand2->model()->load_obj("../src/Models_and_Textures/LowPolyNature.obj");
	rand2->model()->centralize();
	rand2->model()->get_bounding_box(b2);
	rand2->model()->scale(2);
	rand2->model()->rotate(GsQuat(GsVec::i, -gspidiv2));
	rand2->model()->rotate(GsQuat(GsVec::j, -gspidiv2));
	rand2->model()->translate(GsVec(50, (b2.dy() / 2) - 6, 50));
	nature->add(rand2);
	SnModel * rand3 = new SnModel;
	rand3->model()->load_obj("../src/Models_and_Textures/LowPolyNature.obj");
	rand3->model()->centralize();
	rand3->model()->get_bounding_box(b2);
	rand3->model()->scale(2);
	rand3->model()->rotate(GsQuat(GsVec::i, -gspidiv2));
	rand3->model()->rotate(GsQuat(GsVec::j, gspidiv2));
	rand3->model()->translate(GsVec(50, (b2.dy() / 2) - 6, 130));
	nature->add(rand3);
	SnModel* rand4 = new SnModel;
	rand4->model()->load_obj("../src/Models_and_Textures/LowPolyNature.obj");
	rand4->model()->centralize();
	rand4->model()->get_bounding_box(b2);
	rand4->model()->scale(2);
	rand4->model()->rotate(GsQuat(GsVec::i, -gspidiv2));
	rand4->model()->rotate(GsQuat(GsVec::j, -gspidiv2));
	rand4->model()->translate(GsVec(-50, (b2.dy() / 2) - 6, 170));
	nature->add(rand4);
	SnModel* rand5 = new SnModel;
	rand5->model()->load_obj("../src/Models_and_Textures/LowPolyNature.obj");
	rand5->model()->centralize();
	rand5->model()->get_bounding_box(b2);
	rand5->model()->scale(2);
	rand5->model()->rotate(GsQuat(GsVec::i, -gspidiv2));
	rand5->model()->rotate(GsQuat(GsVec::j, gspidiv2));
	rand5->model()->translate(GsVec(-50, (b2.dy() / 2) - 6, 130));
	nature->add(rand5);

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


	//OLD MODEL
	/*
	SnModel* Bird = new SnModel;
	SnGroup* BirdGroup = new SnGroup;
	BirdGroup->separator(true);
	Bird->model()->load_obj("../src/Models_and_Textures/bird.obj");
	Bird->model()->centralize();
	Bird->model()->get_bounding_box(birdBox);
	BirdT = new SnTransform;
	Bird->model()->scale(5);
	birdX = 0;
	birdZ = 10;
	birdY = (5 * (birdBox.dy() / 2));
	startingbirdY = (5 * (birdBox.dy() / 2));
	BirdM.translation(GsVec(birdX, birdY, birdZ));
	BirdT->set(BirdM);
	*/

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


	flyT = new SnTransform;
	flyMidT = new SnTransform;
	flyLeftT = new SnTransform;
	flyRightT = new SnTransform;

	SnGroup* flyWhole = new SnGroup;
	SnGroup* flyMidG = new SnGroup;
	SnGroup* flyLeftG = new SnGroup;
	SnGroup* flyRightG = new SnGroup;

	//Flying bird for scene
	GsModel* flyMid = new GsModel;
	SnModel* flyMidS = new SnModel;
	flyMid->load("../src/Models_and_Textures/flyerMid.obj");
	flyMid->scale((float)0.5);
	flyMid->translate(GsVec(40.0f, 80.0f, 60.0f));
	flyMidS->model(flyMid);

	GsModel* flyLeft = new GsModel;
	SnModel* flyLeftS = new SnModel;
	flyLeft->load("../src/Models_and_Textures/flyerLeft.obj");
	flyLeft->scale((float)0.5);
	flyLeft->translate(GsVec(40.0f, 80.0f, 60.0f));
	flyLeftS->model(flyLeft);

	GsModel* flyRight = new GsModel;
	SnModel* flyRightS = new SnModel;
	flyRight->load("../src/Models_and_Textures/flyerRight.obj");
	flyRight->scale((float)0.5);
	flyRight->translate(GsVec(40.0f, 80.0f, 60.0f));
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


	/*//GsModel* ground = new GsModel;
	//GsBox floor;
	//SnShape
	update = true;
	GsPnt p0 = GsVec(-100.0f, -13.0f, 100.0f); //top left
	GsPnt p1 = GsVec(-100.0f, -13.0f, -100.0f); //bottom left
	GsPnt p2 = GsVec(100.0f, -13.0f, -100.0f); //bottom right
	GsPnt p3 = GsVec(100.0f, -13.0f, 100.0f); //top right
	//floor.a = p0;
	//floor.b = p2;
	//ground->make_box(floor);
	SnModel* daground = new SnModel;
	daground->model()->V.push() = p0;
	daground->model()->V.push() = p1;
	daground->model()->V.push() = p2;
	daground->model()->V.push() = p3;
	daground->model()->F.push() = GsModel::Face(0, 2, 1);
	daground->model()->F.push() = GsModel::Face(0, 3, 2);
	daground->model()->N.push() = GsVec(0, 1, 0);
	daground->model()->N.push() = GsVec(0, 1, 0);
	daground->model()->N.push() = GsVec(0, 1, 0);
	daground->model()->N.push() = GsVec(0, 1, 0);
	GsModel& sans = *daground->model();
	GsModel::Group& bruh = *sans.G.push();
	bruh.fi = 0;
	bruh.fn = sans.F.size();
	bruh.dmap = new GsModel::Texture;
	bruh.dmap->fname.set("../src/Snake/eh.jpg");
	sans.M.push().init();
	int nv = sans.V.size();
	sans.T.size(nv);
	sans.T[0].set(1, 0);
	sans.T[1].set(0, 0);
	sans.T[2].set(0, 1);
	sans.T[3].set(1, 1);
	sans.set_mode(GsModel::Smooth, GsModel::PerGroupMtl);
	sans.textured = true;
	rootg()->add(daground);
	globalT = new SnTransform;
	GsMat globalRot;
	globalRot.rotx((float)-GS_PIDIV2);
	globalT->set(globalRot);
	SnGroup* g = new SnGroup; //to hold all the models and transformations
	SnModel* model[19]; //to hold all the models
	SnModel* sModel[19];
	GsMat initialRot;
	SnTransform* globalST = new SnTransform;
	//Our Light Ball
	SnPrimitive* s = new SnPrimitive;
	s->color(GsColor::yellow);
	s->prim().sphere(0.2f);
	_gLight = new SnGroup(new SnTransform, s, true);
	_gLight->get<SnTransform>(0)->get().setrans(lightPos);
	//_gLight->get<SnTransform>(0)->get().rotx((float)-GS_PIDIV2);
	//_gLight->get<SnTransform>(0)->get().mult(_gLight->get<SnTransform>(0)->get(), globalRot);
	//our shadow material
	SnMaterial* snm = new SnMaterial;
	GsMaterial mtl;
	mtl.diffuse = GsColor::black;
	rootg()->add(_gLight);
	g->add(globalT);
	globalST->set(globalRot);
	model[0] = new SnModel;
	if (!model[0]->model()->load_obj("../src/Snake/Snake_Head.obj"))
	{
		gsout << "head was not loaded" << gsnl;
	}
	model[0]->model()->centralize();
	model[0]->model()->get_bounding_box(b0);
	t[0] = new SnTransform;
	initialRot.rotx((float)-GS_PIDIV2);
	//t[0]->set(m[0]);
	m[0].translation(GsVec(0.0f, 0.0f, 0.0f));
	t[0]->set(m[0]);
	//t[0]->get().mult(t[0]->get(), initialRot);
	g->add(t[0]);
	g->add(model[0]);
	model[1] = new SnModel;
	if (!model[1]->model()->load_obj("../src/Snake/sphere.obj")) {
		gsout << "body1 was not loaded" << gsnl;
	}
	model[1]->model()->centralize();
	model[1]->model()->get_bounding_box(b1);
	t[1] = new SnTransform;
	m[1].translation(GsVec(0.0f, 2.5f, 0.0f));
	t[1]->set(m[1]);
	g->add(t[1]);
	g->add(model[1]);
	model[2] = new SnModel;
	if (!model[2]->model()->load_obj("../src/Snake/Snake_Body1.obj")) {
		gsout << "body1 was not loaded" << gsnl;
	}
	model[2]->model()->centralize();
	model[2]->model()->get_bounding_box(b1);
	t[2] = new SnTransform;
	m[2].translation(GsVec(0.0f, 2.5f, 0.0f));
	t[2]->set(m[2]);
	g->add(t[2]);
	g->add(model[2]);
	model[3] = new SnModel;
	if (!model[3]->model()->load_obj("../src/Snake/sphere.obj")) {
		gsout << "sphere2 was not loaded" << gsnl;
	}
	model[3]->model()->centralize();
	model[3]->model()->get_bounding_box(b1);
	t[3] = new SnTransform;
	m[3].translation(GsVec(0.0f, 2.5f, 0.0f));
	t[3]->set(m[3]);
	g->add(t[3]);
	g->add(model[3]);
	model[4] = new SnModel;
	if (!model[4]->model()->load_obj("../src/Snake/Snake_Body2.obj")) {
		gsout << "body2 was not loaded" << gsnl;
	}
	model[4]->model()->centralize();
	model[4]->model()->get_bounding_box(b2);
	t[4] = new SnTransform;
	m[4].translation(GsVec(0.0f, 2.5f, 0.0f));
	t[4]->set(m[4]);
	g->add(t[4]);
	g->add(model[4]);
	model[5] = new SnModel;
	if (!model[5]->model()->load_obj("../src/Snake/sphere.obj")) {
		gsout << "body1 was not loaded" << gsnl;
	}
	model[5]->model()->centralize();
	model[5]->model()->get_bounding_box(b1);
	t[5] = new SnTransform;
	m[5].translation(GsVec(0.0f, 2.5f, 0.0f));
	t[5]->set(m[5]);
	g->add(t[5]);
	g->add(model[5]);
	model[6] = new SnModel;
	if (!model[6]->model()->load_obj("../src/Snake/Snake_Body3.obj")) {
		gsout << "body3 was not loaded" << gsnl;
	}
	model[6]->model()->centralize();
	model[6]->model()->get_bounding_box(b2);
	t[6] = new SnTransform;
	m[6].translation(GsVec(0.0f, 2.5f, 0.0f));
	t[6]->set(m[6]);
	g->add(t[6]);
	g->add(model[6]);
	model[7] = new SnModel;
	if (!model[7]->model()->load_obj("../src/Snake/sphere.obj")) {
		gsout << "body1 was not loaded" << gsnl;
	}
	model[7]->model()->centralize();
	model[7]->model()->get_bounding_box(b1);
	t[7] = new SnTransform;
	m[7].translation(GsVec(0.0f, 2.5f, 0.0f));
	t[7]->set(m[7]);
	g->add(t[7]);
	g->add(model[7]);
	model[8] = new SnModel;
	if (!model[8]->model()->load_obj("../src/Snake/Snake_Body4.obj")) {
		gsout << "body4 was not loaded" << gsnl;
	}
	model[8]->model()->centralize();
	model[8]->model()->get_bounding_box(b2);
	t[8] = new SnTransform;
	m[8].translation(GsVec(0.0f, 2.5f, 0.0f));
	t[8]->set(m[8]);
	g->add(t[8]);
	g->add(model[8]);
	model[9] = new SnModel;
	if (!model[9]->model()->load_obj("../src/Snake/sphere.obj")) {
		gsout << "body1 was not loaded" << gsnl;
	}
	model[9]->model()->centralize();
	model[9]->model()->get_bounding_box(b1);
	t[9] = new SnTransform;
	m[9].translation(GsVec(0.0f, 2.5f, 0.0f));
	t[9]->set(m[9]);
	g->add(t[9]);
	g->add(model[9]);
	model[10] = new SnModel;
	if (!model[10]->model()->load_obj("../src/Snake/Snake_Body5.obj")) {
		gsout << "body5 was not loaded" << gsnl;
	}
	model[10]->model()->centralize();
	model[10]->model()->get_bounding_box(b2);
	t[10] = new SnTransform;
	m[10].translation(GsVec(0.0f, 2.5f, 0.0f));
	t[10]->set(m[10]);
	g->add(t[10]);
	g->add(model[10]);
	model[11] = new SnModel;
	if (!model[11]->model()->load_obj("../src/Snake/sphere.obj")) {
		gsout << "body1 was not loaded" << gsnl;
	}
	model[11]->model()->centralize();
	model[11]->model()->get_bounding_box(b1);
	t[11] = new SnTransform;
	m[11].translation(GsVec(0.0f, 2.5f, 0.0f));
	t[11]->set(m[11]);
	g->add(t[11]);
	g->add(model[11]);
	model[12] = new SnModel;
	if (!model[12]->model()->load_obj("../src/Snake/Snake_Body6.obj")) {
		gsout << "body6 was not loaded" << gsnl;
	}
	model[12]->model()->centralize();
	model[12]->model()->get_bounding_box(b2);
	t[12] = new SnTransform;
	m[12].translation(GsVec(0.0f, 2.5f, 0.0f));
	t[12]->set(m[12]);
	g->add(t[12]);
	g->add(model[12]);
	model[13] = new SnModel;
	if (!model[13]->model()->load_obj("../src/Snake/sphere.obj")) {
		gsout << "body1 was not loaded" << gsnl;
	}
	model[13]->model()->centralize();
	model[13]->model()->get_bounding_box(b1);
	t[13] = new SnTransform;
	m[13].translation(GsVec(0.0f, 2.5f, 0.0f));
	t[13]->set(m[13]);
	g->add(t[13]);
	g->add(model[13]);
	model[14] = new SnModel;
	if (!model[14]->model()->load_obj("../src/Snake/Snake_Body7.obj")) {
		gsout << "body7 was not loaded" << gsnl;
	}
	model[14]->model()->centralize();
	model[14]->model()->get_bounding_box(b2);
	t[14] = new SnTransform;
	m[14].translation(GsVec(0.0f, 2.5f, 0.0f));
	t[14]->set(m[14]);
	g->add(t[14]);
	g->add(model[14]);
	model[15] = new SnModel;
	if (!model[15]->model()->load_obj("../src/Snake/sphere.obj")) {
		gsout << "body1 was not loaded" << gsnl;
	}
	model[15]->model()->centralize();
	model[15]->model()->get_bounding_box(b1);
	t[15] = new SnTransform;
	m[15].translation(GsVec(0.0f, 2.5f, 0.0f));
	t[15]->set(m[15]);
	g->add(t[15]);
	g->add(model[15]);
	model[16] = new SnModel;
	if (!model[16]->model()->load_obj("../src/Snake/Snake_Tail.obj")) {
		gsout << "tail was not loaded" << gsnl;
	}
	model[16]->model()->centralize();
	model[16]->model()->get_bounding_box(b2);
	t[16] = new SnTransform;
	m[16].translation(GsVec(0.0f, 2.5f, 0.0f));
	t[16]->set(m[16]);
	g->add(t[16]);
	g->add(model[16]);
	SnGroup* dshadowg = new SnGroup;
	dshadowg->separator(true);
	dshadowg->add(_tShadow1 = new SnTransform);
	//globalST->get().rotx((float)GS_PIDIV2);
	//dshadowg->add(globalST);
	sModel[0] = new SnModel;
	if (!sModel[0]->model()->load_obj("../src/Snake/Snake_Head.obj"))
	{
		gsout << "head was not loaded" << gsnl;
	}
	sModel[0]->model()->centralize();
	sModel[0]->model()->get_bounding_box(b0);
	sT[0] = new SnTransform;
	//sT[0]->set(sM[0]);
	sM[0].translation(GsVec(0.0f, 0.0f, 0.0f));
	sT[0]->set(sM[0]);
	//sT[0]->get().mult(sT[0]->get(), initialRot);
	dshadowg->add(sT[0]);
	dshadowg->add(sModel[0]);
	sModel[1] = new SnModel;
	if (!sModel[1]->model()->load_obj("../src/Snake/sphere.obj")) {
		gsout << "body1 was not loaded" << gsnl;
	}
	sModel[1]->model()->centralize();
	sModel[1]->model()->get_bounding_box(b1);
	sT[1] = new SnTransform;
	sM[1].translation(GsVec(0.0f, 2.5f, 0.0f));
	sT[1]->set(sM[1]);
	dshadowg->add(sT[1]);
	dshadowg->add(sModel[1]);
	sModel[2] = new SnModel;
	if (!sModel[2]->model()->load_obj("../src/Snake/Snake_Body1.obj")) {
		gsout << "body1 was not loaded" << gsnl;
	}
	sModel[2]->model()->centralize();
	sModel[2]->model()->get_bounding_box(b1);
	sT[2] = new SnTransform;
	sM[2].translation(GsVec(0.0f, 2.5f, 0.0f));
	sT[2]->set(sM[2]);
	dshadowg->add(sT[2]);
	dshadowg->add(sModel[2]);
	sModel[3] = new SnModel;
	if (!sModel[3]->model()->load_obj("../src/Snake/sphere.obj")) {
		gsout << "sphere2 was not loaded" << gsnl;
	}
	sModel[3]->model()->centralize();
	sModel[3]->model()->get_bounding_box(b1);
	sT[3] = new SnTransform;
	sM[3].translation(GsVec(0.0f, 2.5f, 0.0f));
	sT[3]->set(sM[3]);
	dshadowg->add(sT[3]);
	dshadowg->add(sModel[3]);
	sModel[4] = new SnModel;
	if (!sModel[4]->model()->load_obj("../src/Snake/Snake_Body2.obj")) {
		gsout << "body2 was not loaded" << gsnl;
	}
	sModel[4]->model()->centralize();
	sModel[4]->model()->get_bounding_box(b2);
	sT[4] = new SnTransform;
	sM[4].translation(GsVec(0.0f, 2.5f, 0.0f));
	sT[4]->set(sM[4]);
	dshadowg->add(sT[4]);
	dshadowg->add(sModel[4]);
	sModel[5] = new SnModel;
	if (!sModel[5]->model()->load_obj("../src/Snake/sphere.obj")) {
		gsout << "body1 was not loaded" << gsnl;
	}
	sModel[5]->model()->centralize();
	sModel[5]->model()->get_bounding_box(b1);
	sT[5] = new SnTransform;
	sM[5].translation(GsVec(0.0f, 2.5f, 0.0f));
	sT[5]->set(sM[5]);
	dshadowg->add(sT[5]);
	dshadowg->add(sModel[5]);
	sModel[6] = new SnModel;
	if (!sModel[6]->model()->load_obj("../src/Snake/Snake_Body3.obj")) {
		gsout << "body3 was not loaded" << gsnl;
	}
	sModel[6]->model()->centralize();
	sModel[6]->model()->get_bounding_box(b2);
	sT[6] = new SnTransform;
	sM[6].translation(GsVec(0.0f, 2.5f, 0.0f));
	sT[6]->set(sM[6]);
	dshadowg->add(sT[6]);
	dshadowg->add(sModel[6]);
	sModel[7] = new SnModel;
	if (!sModel[7]->model()->load_obj("../src/Snake/sphere.obj")) {
		gsout << "body1 was not loaded" << gsnl;
	}
	sModel[7]->model()->centralize();
	sModel[7]->model()->get_bounding_box(b1);
	sT[7] = new SnTransform;
	sM[7].translation(GsVec(0.0f, 2.5f, 0.0f));
	sT[7]->set(sM[7]);
	dshadowg->add(sT[7]);
	dshadowg->add(sModel[7]);
	sModel[8] = new SnModel;
	if (!sModel[8]->model()->load_obj("../src/Snake/Snake_Body4.obj")) {
		gsout << "body4 was not loaded" << gsnl;
	}
	sModel[8]->model()->centralize();
	sModel[8]->model()->get_bounding_box(b2);
	sT[8] = new SnTransform;
	sM[8].translation(GsVec(0.0f, 2.5f, 0.0f));
	sT[8]->set(sM[8]);
	dshadowg->add(sT[8]);
	dshadowg->add(sModel[8]);
	sModel[9] = new SnModel;
	if (!sModel[9]->model()->load_obj("../src/Snake/sphere.obj")) {
		gsout << "body1 was not loaded" << gsnl;
	}
	sModel[9]->model()->centralize();
	sModel[9]->model()->get_bounding_box(b1);
	sT[9] = new SnTransform;
	sM[9].translation(GsVec(0.0f, 2.5f, 0.0f));
	sT[9]->set(sM[9]);
	dshadowg->add(sT[9]);
	dshadowg->add(sModel[9]);
	sModel[10] = new SnModel;
	if (!sModel[10]->model()->load_obj("../src/Snake/Snake_Body5.obj")) {
		gsout << "body5 was not loaded" << gsnl;
	}
	sModel[10]->model()->centralize();
	sModel[10]->model()->get_bounding_box(b2);
	sT[10] = new SnTransform;
	sM[10].translation(GsVec(0.0f, 2.5f, 0.0f));
	sT[10]->set(sM[10]);
	dshadowg->add(sT[10]);
	dshadowg->add(sModel[10]);
	sModel[11] = new SnModel;
	if (!sModel[11]->model()->load_obj("../src/Snake/sphere.obj")) {
		gsout << "body1 was not loaded" << gsnl;
	}
	sModel[11]->model()->centralize();
	sModel[11]->model()->get_bounding_box(b1);
	sT[11] = new SnTransform;
	sM[11].translation(GsVec(0.0f, 2.5f, 0.0f));
	sT[11]->set(sM[11]);
	dshadowg->add(sT[11]);
	dshadowg->add(sModel[11]);
	sModel[12] = new SnModel;
	if (!sModel[12]->model()->load_obj("../src/Snake/Snake_Body6.obj")) {
		gsout << "body6 was not loaded" << gsnl;
	}
	sModel[12]->model()->centralize();
	sModel[12]->model()->get_bounding_box(b2);
	sT[12] = new SnTransform;
	sM[12].translation(GsVec(0.0f, 2.5f, 0.0f));
	sT[12]->set(sM[12]);
	dshadowg->add(sT[12]);
	dshadowg->add(sModel[12]);
	sModel[13] = new SnModel;
	if (!sModel[13]->model()->load_obj("../src/Snake/sphere.obj")) {
		gsout << "body1 was not loaded" << gsnl;
	}
	sModel[13]->model()->centralize();
	sModel[13]->model()->get_bounding_box(b1);
	sT[13] = new SnTransform;
	sM[13].translation(GsVec(0.0f, 2.5f, 0.0f));
	sT[13]->set(sM[13]);
	dshadowg->add(sT[13]);
	dshadowg->add(sModel[13]);
	sModel[14] = new SnModel;
	if (!sModel[14]->model()->load_obj("../src/Snake/Snake_Body7.obj")) {
		gsout << "body7 was not loaded" << gsnl;
	}
	sModel[14]->model()->centralize();
	sModel[14]->model()->get_bounding_box(b2);
	sT[14] = new SnTransform;
	sM[14].translation(GsVec(0.0f, 2.5f, 0.0f));
	sT[14]->set(sM[14]);
	dshadowg->add(sT[14]);
	dshadowg->add(sModel[14]);
	sModel[15] = new SnModel;
	if (!sModel[15]->model()->load_obj("../src/Snake/sphere.obj")) {
		gsout << "body1 was not loaded" << gsnl;
	}
	sModel[15]->model()->centralize();
	sModel[15]->model()->get_bounding_box(b1);
	sT[15] = new SnTransform;
	sM[15].translation(GsVec(0.0f, 2.5f, 0.0f));
	sT[15]->set(sM[15]);
	dshadowg->add(sT[15]);
	dshadowg->add(sModel[15]);
	sModel[16] = new SnModel;
	if (!sModel[16]->model()->load_obj("../src/Snake/Snake_Tail.obj")) {
		gsout << "tail was not loaded" << gsnl;
	}
	sModel[16]->model()->centralize();
	sModel[16]->model()->get_bounding_box(b2);
	sT[16] = new SnTransform;
	sM[16].translation(GsVec(0.0f, 2.5f, 0.0f));
	sT[16]->set(sM[16]);
	dshadowg->add(sT[16]);
	dshadowg->add(sModel[16]);
	/*dshadowg->add(model[0]);
	dshadowg->add(model[1]);
	dshadowg->add(model[2]);
	dshadowg->add(model[3]);
	dshadowg->add(model[4]);
	dshadowg->add(model[5]);
	dshadowg->add(model[6]);
	dshadowg->add(model[7]);
	dshadowg->add(model[8]);
	dshadowg->add(model[9]);
	dshadowg->add(model[10]);
	dshadowg->add(model[11]);
	dshadowg->add(model[12]);
	dshadowg->add(model[13]);
	dshadowg->add(model[14]);
	dshadowg->add(model[15]);
	dshadowg->add(model[16]);
	rootg()->add(g);
	rootg()->add(dshadowg);
	rootg()->add(snm);
	*/



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

void MyViewer::update_camera() {


	//lt, t0 = gs_time();
	//if (spaceCount == 2) {
	//	do
	//	{
	//		lt = gs_time() - t0;
	//		camera().eye.x += 0.1f;
	//		//camera().eye.y -= 0.01f;
	//		//camera().center.x+=	0.01f;
	//		//camera().center.y -= 0.01f;
	//		//camera().eye.z += 2.0f;
	//		//camera().up.y -= 0.01f;
	//		render();
	//		ws_check();
	//		message().setf("localtime=% f", lt);
	//	} while
	//		(lt < 3);
	//}
	//if (spaceCount == 1) {
	//	camera().eye.x = 0;
	//	camera().eye.y = 50;
	//	camera().eye.z = 10;
	//	render();
	//	ws_check();
	//	message().setf("spaceCount=% i");

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
		flyTo.translation(GsVec(40.0f, 80.0f, 60.0f));
		flyLeftM = flyLeftT->get() * flyTo;
		flyLeftM = flyLeftM * rot;
		flyBack.translation(GsVec(-40.0f, -80.0f, -60.0f));
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

	if (_animating) return; // avoid recursive calls
	_animating = true;

	int ind = 1; // pick one child moves first model
	int ind2 = 2; //move second model


	//GsMat TestProj = BigMat.projxy(GsVec::i, GsVec::j, GsVec::k);

	double frdt = 1.0 / 60.0; // delta time to reach given number of frames per second
	double v = 4; // target velocity is 1 unit per second
	double ti = 0, lt = 0, t0 = gs_time();
	float increment = gs2pi / 60;
	/*
	SnManipulator* BigManip = rootg()->get<SnManipulator>(1); // access big hand manipulators
	GsMat BigMat = BigManip->mat();
	SnManipulator* SmallManip = rootg()->get<SnManipulator>(2); // access little hand manipulator
	GsMat SmallMat = SmallManip->mat();
	*/


	do // run for a while:
	{
		while (ti - lt < frdt) { ws_check(); ti = gs_time() - t0; } // wait until it is time for next frame
		lt = ti;
		int cnt = 0;

		tsd = tsd + increment;
		if (tsd - (gs2pi) >= 0) {


			tmd = tmd + increment;
			if (tmd - gs2pi >= 0) {
				tmd = 0;
			}
		}
		/*SnManipulator* manip = rootg()->get<SnManipulator>(12);
		GsMat m = manip->mat();*/
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
				//leftWT->get().mult(leftWT->get(), wingM);
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
			if (birdY >= startingbirdY)
				birdY = birdY + (float)yinc;
			BirdM.translation(GsVec(birdX, birdY, birdZ));
			BirdT->set(BirdM);
			count += 0.5f;
			floorz = floorz - 0.5f;
			floorM.translation(GsVec(0, 0, floorz));
			floorT->set(floorM);


			rot.roty(0.1f * (-1 * float(GS_2PI) / 30.0f));
			flyM = flyT->get() * rot;
			flyT->set(flyM);

			rot.rotz(0.1f * (float(GS_2PI) / 30.0f));
			flyTo.translation(GsVec(40.0f, 80.0f, 60.0f));
			flyLeftM = flyLeftT->get() * flyTo;
			flyLeftM = flyLeftM * rot;
			flyBack.translation(GsVec(-40.0f, -80.0f, -60.0f));
			flyLeftM = flyLeftM * flyBack;
			flyLeftT->set(flyLeftM);

			rot.rotz(0.1f * (-1 * float(GS_2PI) / 30.0f));
			flyRightM = flyRightT->get() * flyTo;
			flyRightM = flyRightM * rot;
			flyRightM = flyRightM * flyBack;
			flyRightT->set(flyRightM);
			flyRightT->set(flyRightM);

			//if ((int)t % 2 == 0) {
			//	rot.rotz(0.25f * (-1 * float(GS_2PI) / 30.0f));
			//	flyTo.translation(GsVec(40.0f, 80.0f, 60.0f));
			//	flyLeftM = flyLeftT->get() * flyTo;
			//	flyLeftM = flyLeftM * rot;
			//	flyBack.translation(GsVec(-40.0f, -80.0f, -60.0f));
			//	flyLeftM = flyLeftM * flyBack;
			//	flyLeftT->set(flyLeftM);

			//	rot.rotz(0.25f * (-1 * float(GS_2PI) / 30.0f));
			//	flyRightM = flyRightT->get() * flyTo;
			//	flyRightM = flyRightM * rot;
			//	flyRightM = flyRightM * flyBack;
			//	flyRightT->set(flyRightM);
			//	flyRightT->set(flyRightM);
			//}

			//if ((int)t % 2 != 0) {
			//	rot.rotz(0.25f * (float(GS_2PI) / 30.0f));
			//	flyTo.translation(GsVec(40.0f, 80.0f, 60.0f));
			//	flyLeftM = flyLeftT->get() * flyTo;
			//	flyLeftM = flyLeftM * rot;
			//	flyBack.translation(GsVec(-40.0f, -80.0f, -60.0f));
			//	flyLeftM = flyLeftM * flyBack;
			//	flyLeftT->set(flyLeftM);

			//	rot.rotz(0.25f * (float(GS_2PI) / 30.0f));
			//	flyRightM = flyRightT->get() * flyTo;
			//	flyRightM = flyRightM * rot;
			//	flyRightM = flyRightM * flyBack;
			//	flyRightT->set(flyRightM);
			//	flyRightT->set(flyRightM);
			//}

			render(); // notify it needs redraw
			ws_check(); // redraw now
		} while (count < 20);
		_animating = false;
		forward = false;

		//double frdt1 = 1.0 / 60.0; // delta time to reach given number of frames per second
		//double v1 = 4; // target velocity is 1 unit per second
		//double t1 = 0, lt1 = 0, t01 = gs_time();
		//float count = 0.0f;
		//do // run for a while:
		//{
		//	while (t1 - lt1 < frdt1) { ws_check(); t1 = gs_time() - t01; } // wait until it is time for next frame
		//	//if (t > 0.5) count = -count; // after 2 secs: go down
		//	lt1 = t1;
		//	count += 0.5f;
		//	floorz = floorz - 0.5f;
		//	floorM.translation(GsVec(0, 0, floorz));
		//	floorT->set(floorM);
		//	render(); // notify it needs redraw
		//	ws_check(); // redraw now
		//} while (count < 20);



		//sM[0].rotx((float)GS_PIDIV2);
		////ANIMATION IMPLEMENTATION
		//if (animatestart == true) {
		//	if (tsd <= 3) {
		//		GsMat TMH; //translation matrix
		//		GsMat RMH; //rotation matrix
		//		GsMat TBH; //translate back matrix
		//		GsMat ChangedMatH;
		//		//	TMH.translation(GsVec(0.0f, 0.0f, -b2.dz() / 2.0f + b1.dz()));
		//		RMH.rotx((float)tsd * -gspi / 60);
		//		//TBH.translation(GsVec(0.0f, 0.0f, b2.dz() / 2.0f + b1.dz()));
		//		//ChangedMatH = TMH * RMH * TBH;
		//		t[0]->get().mult(t[0]->get(), RMH);
		//		t[1]->get().mult(t[1]->get(), RMH);
		//		//t[2]->set(ChangedMatH);
		//	if (tsd >= 5 && tsd <= 10) {
		//		GsMat TMH; //translation matrix
		//		GsMat RMH; //rotation matrix
		//		GsMat TBH; //translate back matrix
		//		GsMat ChangedMatH;
		//		//	TMH.translation(GsVec(0.0f, 0.0f, -b2.dz() / 2.0f + b1.dz()));
		//		RMH.rotx(gspi / 90);
		//		TMH.rotx(gspi / 70);
		//		//TBH.translation(GsVec(0.0f, 0.0f, b2.dz() / 2.0f + b1.dz()));
		//		//ChangedMatH = TMH * RMH * TBH;
		//		t[0]->get().mult(t[0]->get(), RMH);
		//		t[1]->get().mult(t[1]->get(), TMH);
		//		sT[0]->get().mult(sT[0]->get(), RMH);
		//		sT[1]->get().mult(sT[1]->get(), TMH);
		//		//t[2]->set(ChangedMatH);
		//		render();
		//		ws_check();
		//	}
		//	if (tsd > 11 && tsd < 15) {
		//		GsMat TMH; //translation matrix
		//		GsMat RMH; //rotation matrix
		//		GsMat TBH; //translate back matrix
		//		GsMat ChangedMatH;
		//		//	TMH.translation(GsVec(0.0f, 0.0f, -b2.dz() / 2.0f + b1.dz()));
		//		RMH.rotx(gspi / 100);
		//		TMH.rotx(-gspi / 100);
		//		TBH.rotx(-gspi / 200);
		//		//TBH.translation(GsVec(0.0f, 0.0f, b2.dz() / 2.0f + b1.dz()));
		//		//ChangedMatH = TMH * RMH * TBH;
		//		t[0]->get().mult(t[0]->get(), RMH);
		//		t[1]->get().mult(t[1]->get(), TMH);
		//		t[2]->get().mult(t[2]->get(), TBH);
		//		//t[2]->set(ChangedMatH);
		//		render();
		//		ws_check();
		//	}
		//}
		////ARTICULATION IMPLEMENTATION
		////shoulder up
		//if (body2up == true) {
		//	GsMat TMH; //translation matrix
		//	GsMat RMH; //rotation matrix
		//	GsMat TBH; //translate back matrix
		//	GsMat ChangedMatH;
		//	//	TMH.translation(GsVec(0.0f, 0.0f, -b2.dz() / 2.0f + b1.dz()));
		//	RMH.rotz(-gspi / 120);
		//	//TBH.translation(GsVec(0.0f, 0.0f, b2.dz() / 2.0f + b1.dz()));
		//	//ChangedMatH = TMH * RMH * TBH;
		//	t[3]->get().mult(t[3]->get(), RMH);
		//	sT[3]->get().mult(sT[3]->get(), RMH);
		//	//t[2]->set(ChangedMatH);
		//	render();
		//	ws_check();
		//	body2up = false;
		//	body2down = false;
		//}
		//if (body2down == true) {
		//	GsMat TMH; //translation matrix
		//	GsMat RMH; //rotation matrix
		//	GsMat TBH; //translate back matrix
		//	GsMat ChangedMatH;
		//	//	TMH.translation(GsVec(0.0f, 0.0f, -b2.dz() / 2.0f + b1.dz()));
		//	RMH.rotz(gspi / 120);
		//	//TBH.translation(GsVec(0.0f, 0.0f, b2.dz() / 2.0f + b1.dz()));
		//	//ChangedMatH = TMH * RMH * TBH;
		//	t[3]->get().mult(t[3]->get(), RMH);
		//	sT[3]->get().mult(sT[3]->get(), RMH);
		//	//t[2]->set(ChangedMatH);
		//	render();
		//	ws_check();
		//	body2up = false;
		//	body2down = false;
		//}
		////shoulderup = false;
		////shoulderdown = false;
		////forearm up
		//if (body1up == true) {
		//	GsMat TMH; //translation matrix
		//	GsMat RMH; //rotation matrix
		//	GsMat TBH; //translate back matrix
		//	GsMat ChangedMatH;
		//	//	TMH.translation(GsVec(0.0f, 0.0f, -b2.dz() / 2.0f + b1.dz()));
		//	RMH.rotz(-gspi / 120);
		//	//TBH.translation(GsVec(0.0f, 0.0f, b2.dz() / 2.0f + b1.dz()));
		//	ChangedMatH = TMH * RMH * TBH;
		//	t[1]->get().mult(t[1]->get(), RMH);
		//	sT[1]->get().mult(sT[1]->get(), RMH);
		//	//t[2]->set(ChangedMatH)
		//	render();
		//	ws_check();
		//	body1up = false;
		//	body1down = false;
		//}
		//if (forward == true) {
		//	//gsout << "move forward" << gsnl;
		//	GsMat RM;
		//	RM.rotx((float)-GS_PIDIV2);
		//	startingY = startingY - 0.3f;
		//	//globalT->get().translation(GsVec(startingX, 0.0f, startingZ));
		//	//t[0]->get().mult(t[0]->get(), R);
		//	//t[0]->get().translation(GsVec(startingX, 0.0f, startingZ));
		//	//globalT->get().translation(GsVec(startingX, 0.0f, startingZ));
		//	m[0].translation(GsVec(startingX, startingY, 0.0f));
		//	t[0]->set(m[0]);
		//	sM[0].translation(GsVec(startingX, startingY, 0.0f));
		//	sT[0]->set(sM[0]);
		//	//t[0]->get().mult(t[0]->get(), RM);
		//	forward = false;
		//	render();
	} while (_animating);
	_animating = false; forward = false;


	if (moveCount >= 0) {
		if (moveCount == 0) {
			zmove += 200;
		}
		floormoveM[moveCount].translation(GsVec(0, 0, zmove));
		floormoveT[moveCount]->set(floormoveM[moveCount]);
	}

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
		flyTo.translation(GsVec(40.0f, 80.0f, 60.0f));
		flyLeftM = flyLeftT->get() * flyTo;
		flyLeftM = flyLeftM * rot;
		flyBack.translation(GsVec(-40.0f, -80.0f, -60.0f));
		flyLeftM = flyLeftM * flyBack;
		flyLeftT->set(flyLeftM);

		rot.rotz(0.1f* (-1 * float(GS_2PI) / 30.0f));
		flyRightM = flyRightT->get() * flyTo;
		flyRightM = flyRightM * rot;
		flyRightM = flyRightM * flyBack;
		flyRightT->set(flyRightM);
		flyRightT->set(flyRightM);

		/*if ((int)t % 2 == 0) {
			rot.rotz(0.25f * (-1 * float(GS_2PI) / 30.0f));
			flyTo.translation(GsVec(40.0f, 80.0f, 60.0f));
			flyLeftM = flyLeftT->get() * flyTo;
			flyLeftM = flyLeftM * rot;
			flyBack.translation(GsVec(-40.0f, -80.0f, -60.0f));
			flyLeftM = flyLeftM * flyBack;
			flyLeftT->set(flyLeftM);

			rot.rotz(0.25f * (-1 * float(GS_2PI) / 30.0f));
			flyRightM = flyRightT->get() * flyTo;
			flyRightM = flyRightM * rot;
			flyRightM = flyRightM * flyBack;
			flyRightT->set(flyRightM);
			flyRightT->set(flyRightM);
		}

		if ((int)t % 2 != 0) {
			rot.rotz(0.25f * (float(GS_2PI) / 30.0f));
			flyTo.translation(GsVec(40.0f, 80.0f, 60.0f));
			flyLeftM = flyLeftT->get() * flyTo;
			flyLeftM = flyLeftM * rot;
			flyBack.translation(GsVec(-40.0f, -80.0f, -60.0f));
			flyLeftM = flyLeftM * flyBack;
			flyLeftT->set(flyLeftM);

			rot.rotz(0.25f * (float(GS_2PI) / 30.0f));
			flyRightM = flyRightT->get() * flyTo;
			flyRightM = flyRightM * rot;
			flyRightM = flyRightM * flyBack;
			flyRightT->set(flyRightM);
			flyRightT->set(flyRightM);
		}*/

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
	case 65362: {forward = true; if (moveCount == 9) { moveCount = 0; } else { moveCount++; } run_animation(); render(); break; }
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
	case EvAnimate: forward = true; run_animation(); return 1;
	case EvExit: gs_exit();
	}
	return WsViewer::uievent(e);
}