////////////////////////////////////////
//
// 3D Game Programming
// NCTU
// Instructor: SAI-KEUNG WONG
//
// Date: 2019/09/20
////////////////////////////////////////
// Student Name: CHAO-YU HUANG
// Student ID: 0416094
// Student Email: b608390@gmail.com
//
////////////////////////////////////////
// You can delete or add some functions to do the assignment.
////////////////////////////////////////

#include "TutorialApplication.h"
#include "BasicTools.h"

#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>

using namespace Ogre;

SceneNode* bigPNode;
SceneNode* smallPNode;
std::string animationState = "off";
const float PI = 3.141592654;

BasicTutorial_00::BasicTutorial_00(void) {}

void BasicTutorial_00::chooseSceneManager()
{   
	mSceneMgrArr[0] = mRoot
		->createSceneManager(ST_GENERIC, "primary");
	mSceneMgrArr[1] = mRoot
		->createSceneManager(ST_GENERIC, "secondary");
    //
    // add your own stuff
    //
}

void BasicTutorial_00::createCamera_00(void)
{
	mSceneMgr = mSceneMgrArr[0];
	mCamera = mCameraArr[0] = mSceneMgr->createCamera("PlayerCam0");
	mCamera->setPosition(Ogre::Vector3(120,300,600));
	mCamera->lookAt(Ogre::Vector3(120,0,0));
	mCamera->setNearClipDistance(5);
	mCameraManArr[0] = new OgreBites::SdkCameraMan(mCamera);   // create a default camera controller
}

void BasicTutorial_00::createCamera_01(void)
{   
	mSceneMgr = mSceneMgrArr[1];
	mCamera = mCameraArr[1] = mSceneMgr->createCamera("PlayerCam1");
	mCamera->setPosition(Ogre::Vector3(0,350,0.001));
	mCamera->lookAt(Ogre::Vector3(0,0,0));
	mCamera->setNearClipDistance(5);
	mCameraManArr[1] = new OgreBites::SdkCameraMan(mCamera);   // create a default camera controller
}



void BasicTutorial_00::createViewport_00(void)
{   
	
	mCamera = mCameraArr[0];
	// Create one viewport, entire window
    Ogre::Viewport* vp = mWindow->addViewport(mCamera);
    vp->setBackgroundColour(Ogre::ColourValue(0,0,1));
	mViewportArr[0] = vp;
	
    // Alter the camera aspect ratio to match the viewport
    mCamera->setAspectRatio(
        Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));
	
}

void BasicTutorial_00::createViewport_01(void)
{    
    // add your own stuff	
	mCamera = mCameraArr[1];
    Ogre::Viewport* vp = mWindow->addViewport(mCamera, 1, 0.0, 0.0, 0.25, 0.25);
    vp -> setBackgroundColour(Ogre::ColourValue(0,0.5,0));
	mViewportArr[1] = vp;
	vp -> setOverlaysEnabled(false);

    // Alter the camera aspect ratio to match the viewport
    mCamera->setAspectRatio(
        Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));

}

void BasicTutorial_00::createScene_00(void) 
{
	mSceneMgr = mSceneMgrArr[0];
	mSceneMgr->setAmbientLight( ColourValue( 0.7, 0.7, 0.7 ) ); 
	mSceneMgr->setShadowTechnique(SHADOWTYPE_STENCIL_ADDITIVE);

	Plane plane(Vector3::UNIT_Y, 0); 
	MeshManager::getSingleton().createPlane(
	    "ground", 						
	    ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, 
	    plane, 
	    1500,1500, 	// width, height
	    20,20, 		// x- and y-segments
	    true, 		// normal
	    1, 		// num texture sets
	    5,5, 		// x- and y-tiles
	    Vector3::UNIT_Z	// upward vector
	); 

	// Create ground
	Entity* entGround = mSceneMgr -> createEntity("GroundEntity", "ground");
	SceneNode* groundNode = mSceneMgr->getRootSceneNode() -> createChildSceneNode();
	groundNode -> attachObject(entGround); 
	entGround->setCastShadows(false);
	//entGround->setMaterialName("Examples/Rockwall");

	// Create big penguin
	Entity* entBigP = mSceneMgr -> createEntity("BigPenguin", "penguin.mesh");
	entBigP -> setCastShadows(true); 
	bigPNode = mSceneMgr->getRootSceneNode() -> createChildSceneNode("BigP", Vector3(0, 50, 0));
	bigPNode -> attachObject(entBigP);
	bigPNode -> scale(2, 3, 2);

	// Create small penguin
	Entity* entSmallP = mSceneMgr -> createEntity("SmallPenguin", "penguin.mesh");
	entSmallP -> setCastShadows(true);
	smallPNode = mSceneMgr -> getRootSceneNode() -> createChildSceneNode("smallP", Vector3(200, 20, 0));
	smallPNode -> attachObject(entSmallP);
	smallPNode -> yaw(Degree(-90));

	

	// Create Circle
	int numCubes = 72;
	int L = 255;
	for (int i =0; i < numCubes; i ++) {
		std::string name;
		genNameUsingIndex("c", i, name);
		Entity *entCircle = mSceneMgr -> createEntity(name, "cube.mesh");
		entCircle -> setCastShadows(true);
        entCircle -> setMaterialName("Examples/SphereMappedRustySteel");
	    AxisAlignedBox bb =  entCircle -> getBoundingBox();
	    double cubeSize = bb.getMaximum().x - bb.getMinimum().x;
		SceneNode *circleNode = mSceneMgr -> getRootSceneNode() -> createChildSceneNode();
		circleNode -> attachObject(entCircle);
	    double fx = i / (double) (numCubes - 1); // in range [0,1]
	    double h = (1 + sin(fx * PI * 4)) * 50; // height
        double radius = 100;
        double x1 = radius * cos(fx * PI * 2);
        double z1 = radius * sin(fx * PI * 2);
        double unitF = 1.0 / cubeSize / numCubes * L * 0.8;
        circleNode -> scale(unitF, h / cubeSize, unitF);
	    circleNode -> setPosition(x1, 50, z1);
	}

	// Create Row
	for (int i = 0; i < numCubes; i ++) {
		std::string name;
		genNameUsingIndex("r", i, name);
		Entity *entRow = mSceneMgr -> createEntity(name, "cube.mesh");
		entRow -> setCastShadows(true);
        entRow -> setMaterialName("Examples/Chrome");
	    AxisAlignedBox bb =  entRow -> getBoundingBox();
	    double cubeSize = bb.getMaximum().x - bb.getMinimum().x;
		SceneNode *rowNode = mSceneMgr -> getRootSceneNode() -> createChildSceneNode();
		rowNode -> attachObject(entRow);

	    double fx = 2*i/(double) (numCubes-1); //i from 0 to numCubes-1
        double x = fx*L - L/2.0;
        double h = (1+cos(fx*3.1415*2.0))*20; // height
        double unitF = 1.0/cubeSize/numCubes*L*0.8;
        rowNode->scale(unitF, h/cubeSize, unitF);
        rowNode->setPosition(x, 20, 100);
	}

    // Create 2 Lights
	Light* light1 = mSceneMgr -> createLight("Light1");
	light1 -> setType(Light::LT_SPOTLIGHT); 
	light1 -> setPosition(Vector3(1600, 1600, 1000)); 
	light1 -> setDirection(Vector3(-1.6, -1.6, -1));
	light1 -> setDiffuseColour( 0.8, 0.8, 0.8 );		
	light1 -> setSpecularColour( 0.3, 0.3, 0.3 );
	//light1 -> setSpotlightRange(Degree(35), Degree(50));

	
	Light* light2 = mSceneMgr -> createLight("Light2");
	light2 -> setType(Light::LT_SPOTLIGHT); 
	light2 -> setPosition(Vector3(-1600, 1600, 1000)); 
	light2 -> setDirection(Vector3(1.6, -1.6, -1));
	light2 -> setDiffuseColour( 0.8, 0.8, 0.8 );		
	light2 -> setSpecularColour( 0.3, 0.3, 0.3 );

	// add your own stuff
    /*
    Radian angle(3.141952654/2.0);

    Vector3 axis(1.0, 0.0, 0.0);
    mSceneMgr
		->getRootSceneNode()
		->createChildSceneNode(
            Vector3(0.0, 0.0, -750.0),
            Quaternion( angle, axis))
		->attachObject(ent);
    */
}

void BasicTutorial_00::createScene_01(void) 
{
    mSceneMgr = mSceneMgrArr[1];
	mSceneMgr->setAmbientLight( ColourValue( 0.0, 0.0, 0.5 ) ); 
	mSceneMgr->setShadowTechnique(SHADOWTYPE_STENCIL_ADDITIVE);
	
	Plane plane(Vector3::UNIT_Y, 0); 
	MeshManager::getSingleton().createPlane(
	    "plane", 						
	    ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, 
	    plane, 
	    300,300, 	// width, height
	    20,20, 		// x- and y-segments
	    true, 		// normal
	    1, 		// num texture sets
	    5,5, 		// x- and y-tiles
	    Vector3::UNIT_Z	// upward vector
	); 

	// Create ground
	Entity* entPlane = mSceneMgr -> createEntity("PlaneEntity", "plane");
	entPlane -> setCastShadows(false);
	SceneNode* planeNode = mSceneMgr->getRootSceneNode() -> createChildSceneNode("Plane", Vector3(0, 0, 0));
	planeNode -> attachObject(entPlane); 
	planeNode  -> pitch(Degree(-60));
	
	Entity *entEgg = mSceneMgr -> createEntity("egg", "sphere.mesh");
    entEgg -> setCastShadows(true);
	AxisAlignedBox bb =  entEgg -> getBoundingBox();
	double ballSize = bb.getMaximum().x - bb.getMinimum().x;
    entEgg -> setMaterialName("Examples/green");
	entEgg -> setCastShadows(true);
	SceneNode *eggNode = planeNode -> createChildSceneNode("egg", Vector3(50, ballSize * 0.3 , 0));
	eggNode -> attachObject(entEgg);
	eggNode -> scale(0.2, 0.3, 0.25);
	
	 // Create a Light
	Light* light1 = mSceneMgr -> createLight("Light1");
	light1 -> setType(Light::LT_SPOTLIGHT); 
	SceneNode *lightNode = eggNode -> createChildSceneNode("light", Vector3(300, 300 , 300));
	lightNode -> attachObject(light1);
	light1 -> setDirection(Vector3(-1, -1, -1));
	light1 -> setDiffuseColour( 0.0, 0.0, 1.0 );		
	light1 -> setSpecularColour( 0.0, 0.0, 1.0 );


}

void BasicTutorial_00::createViewports(void)
{
    //Do not modify
	createViewport_00();
	createViewport_01();
}

void BasicTutorial_00::createCamera(void) {
    //Do not modify
	createCamera_00();
	createCamera_01();
	mCameraMan = mCameraManArr[0];
	//mCameraMan = mCameraManArr[1];
}

void BasicTutorial_00::createScene( void ) {
    //Do not modify
	createScene_00();
	createScene_01();
	//mSceneMgr = mSceneMgrArr[0]; // active SceneManager
	mSceneMgr = mSceneMgrArr[1]; // active SceneManager
    //
    mCamera = mCameraArr[0];
    //mCamera = mCameraArr[1];
}

//
// What is stored in the file for arg.key?
// ASCII code? If no, what is it?
//
// To find out the answer:
// Go to see the definition of KeyEvent
//
bool BasicTutorial_00::keyPressed( const OIS::KeyEvent &arg )
{
    bool flg = true;
    stringstream ss;
    ss << arg.key;
    String msg;
    ss >> msg;
    //msg += ":*** keyPressed ***\n";
	msg += animationState;
    Ogre::LogManager::getSingletonPtr()->logMessage( msg );

    
    if (arg.key == OIS::KC_C ) {
        
        //How to clear ss?
        ss.str("");
        ss.clear();
        
        //stringstream ss; // Second way

        // Third way?
        //=============

        // How to get camerea position?
        //-----------------------------
        //This is incorrect.
        //Vector3 pos = mCamera->getPosition();
        //-----------------------------
        Vector3 pos = mCameraMan->getCamera()->getPosition(); //Correct
        ss << std::fixed << std::setprecision(2) 
            << "CameraPosition:" 
            << pos.x << "\t" 
            << pos.y << "\t" 
            << pos.z << "\n";
        Ogre::LogManager::getSingletonPtr()
            ->logMessage( ss.str() );
        //
        ss.str("");
        ss.clear();
        Vector3 dir = mCameraMan->getCamera()->getDirection();
        ss << std::fixed << std::setprecision(2) 
            << "CameraDirection:" 
            << dir.x << "\t" 
            << dir.y << "\t" 
            << dir.z << "\n";
        Ogre::LogManager::getSingletonPtr()
            ->logMessage( ss.str() );
        //
    }

    if (arg.key == OIS::KC_1 ) {
        mCameraMan->getCamera()
            ->setPosition(Vector3(98.14, 450.69, 964.20));
        mCameraMan->getCamera()
            ->setDirection(Vector3(-0.01, -0.30, -0.95));

        //98.14	450.69	964.20
        //-0.01	-0.30	-0.95
    }

    if (arg.key == OIS::KC_2 ) {
		mCameraMan->getCamera()
            ->setPosition(Vector3(-1463.00,	606.45,	-513.24));
        mCameraMan->getCamera()
            ->setDirection(Vector3(0.88, -0.47,	0.10));

        // add your own stuff
        //-1463.00	606.45	-513.24
        //0.88	-0.47	0.10
    }

    if (arg.key == OIS::KC_3 ) {
		mCameraMan->getCamera()
            ->setPosition(Vector3(-1356.16,	634.32,	-964.51));
        mCameraMan->getCamera()
            ->setDirection(Vector3(0.71, -0.44, 0.55));

        // add your own stuff
        //-1356.16	634.32	-964.51
        //0.71	-0.44	0.55
    }

    if (arg.key == OIS::KC_4 ) {
		mCameraMan->getCamera()
            ->setPosition(Vector3(40.39, 155.23, 251.20));
        mCameraMan->getCamera()
            ->setDirection(Vector3(-0.02, -0.41, -0.91));

         // add your own stuff
        //40.39	155.23	251.20
        //-0.02	-0.41	-0.91
    }

    if (arg.key == OIS::KC_5 ) {
		mCameraMan->getCamera()
            ->setPosition(Vector3(19.94, 822.63, 30.79));
        mCameraMan->getCamera()
            ->setDirection(Vector3(0.00, -0.99, -0.11));
        // add your own stuff
        //19.94	822.63	30.79
        //0.00	-0.99	-0.11
    }

    if (arg.key == OIS::KC_M ) {
        
       // Delete the current view ports
       mWindow -> removeViewport(mViewportArr[0] -> getZOrder());
	   mWindow -> removeViewport(mViewportArr[1] -> getZOrder());

	   // Create new
	   mCamera = mCameraArr[0];
       Ogre::Viewport* vp0 = mWindow -> addViewport(mCamera, 1, 0, 0, 0.25, 0.25);
       vp0 -> setBackgroundColour(Ogre::ColourValue(0,0,1));

       // Alter the camera aspect ratio to match the viewport
       mCamera -> setAspectRatio(
          Ogre::Real(vp0->getActualWidth()) / Ogre::Real(vp0->getActualHeight()));
	   mViewportArr[0] = vp0;


	   mCamera = mCameraArr[1];
       Ogre::Viewport* vp1 = mWindow -> addViewport(mCamera);
       vp1 -> setBackgroundColour(Ogre::ColourValue(0,0.5,0));

       // Alter the camera aspect ratio to match the viewport
       mCamera -> setAspectRatio(
          Ogre::Real(vp1 -> getActualWidth()) / Ogre::Real(vp1 -> getActualHeight()));
	   mViewportArr[1] = vp1;
	   vp1 -> setOverlaysEnabled(false);
	   /*
	   Ogre::Viewport* vp = mWindow -> addViewport(c_ptr,0, 0.15, 0.55, 0.45, 0.3);
	   vp->setBackgroundColour(Ogre::ColourValue(0,0.5,0.0));
	   c_ptr->setAspectRatio(
		    Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));
       //delete mViewportArr[0];    // program crashes
       mViewportArr[0] = vp;       // make sure to save the new pointer
	   */
    }

    if (arg.key == OIS::KC_N ) {
        // Delete the current view ports
       mWindow -> removeViewport(mViewportArr[0] -> getZOrder());
	   mWindow -> removeViewport(mViewportArr[1] -> getZOrder());

	   // Create new view ports
	   mCamera = mCameraArr[0];
       Ogre::Viewport* vp0 = mWindow -> addViewport(mCamera);
       vp0 -> setBackgroundColour(Ogre::ColourValue(1,1,0));

       // Alter the camera aspect ratio to match the viewport
       mCamera -> setAspectRatio(
          Ogre::Real(vp0->getActualWidth()) / Ogre::Real(vp0->getActualHeight()));
	   mViewportArr[0] = vp0;
	   
	   mCamera = mCameraArr[1];
       Ogre::Viewport* vp1 = mWindow -> addViewport(mCamera, 1, 0.5, 0.5, 0.25, 0.25);
       vp1 -> setBackgroundColour(Ogre::ColourValue(0,0.5,0));

       // Alter the camera aspect ratio to match the viewport
       mCamera -> setAspectRatio(
          Ogre::Real(vp1 -> getActualWidth()) / Ogre::Real(vp1 -> getActualHeight()));
	   mViewportArr[1] = vp1;
	   vp1 -> setOverlaysEnabled(false);
    }

	if (arg.key == OIS::KC_P ) {
	
		if(animationState == "off") {
			animationState = "on";
		}
		else if (animationState == "on") {
			animationState = "off";
		}
		else {
			animationState = "off";
		}
	}
    // Do not delete this line
    BaseApplication::keyPressed(arg);

    return flg;
}

//
// What is stored in the file for arg.key?
// ASCII code? If no, what is it?
// 
// To find out the answer:
// Go to see the definition of KeyEvent
//
bool BasicTutorial_00::keyReleased( const OIS::KeyEvent &arg )
{
    bool flg = true;
    stringstream ss;
    ss << arg.key;
    String msg;
    ss >> msg;
    msg += ":*** keyReleased ***\n";
    
    Ogre::LogManager::getSingletonPtr()->logMessage( msg );

    BaseApplication::keyReleased(arg);

    return flg;
}

double mAngle = 0.0;
bool BasicTutorial_00::frameStarted(const Ogre::FrameEvent& evt)
{
	bool flg = Ogre::FrameListener::frameStarted(evt);
    //
    // add your own stuff
    //
		double mRadius = 200.0;
	if (animationState == "on") {
		bigPNode -> lookAt(Vector3(smallPNode -> getPosition().x, 50, smallPNode -> getPosition().z), Node::TransformSpace::TS_WORLD, Ogre::Vector3::UNIT_Z);
	    smallPNode -> lookAt(Vector3(bigPNode -> getPosition().x, 20, bigPNode -> getPosition().z), Node::TransformSpace::TS_WORLD, Ogre::Vector3::UNIT_Z);
		Vector3 pos = smallPNode -> getPosition();
		mAngle += (PI / 100);
		if (mAngle > 2 * PI) {
			mAngle = 0.0;
		}
        pos.x = mRadius*cos(mAngle);
        pos.z = mRadius*sin(mAngle);
        smallPNode -> setPosition(pos); 
	}
    return flg;
}
int main(int argc, char *argv[]) {
	BasicTutorial_00 app;
	app.go();  
	return 0;
}

////////////////////////////////////////
// DO NOT DELETE THIS LINE: 2018/09/20. 3D Game Programming
////////////////////////////////////////
