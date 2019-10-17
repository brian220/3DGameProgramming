////////////////////////////////////////
//
// 3D Game Programming
// NCTU
// Instructor: SAI-KEUNG WONG
//
// Date: 2019/09/20
////////////////////////////////////////
// Student Name: CHAO YU HUANG
// Student ID: 0856601
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
#include <vector>

using namespace std;
using namespace Ogre;


const float PI = 3.141592654;

BasicTutorial_00::BasicTutorial_00(void) {
    mAngle = 0.0;
	mAngularSpeed = 0.5;
	mRadius = 1000.0;
}

void BasicTutorial_00::chooseSceneManager()
{   
	ResourceGroupManager
		::getSingleton()
		.initialiseAllResourceGroups();

	mSceneMgrArr[0] = mRoot
		->createSceneManager(ST_GENERIC, "primary");
	//mSceneMgrArr[1] = mRoot
	//	->createSceneManager(ST_GENERIC, "secondary");
    //
    // add your own stuff
    //
}

void BasicTutorial_00::createCamera_00(void)
{
	mSceneMgr = mSceneMgrArr[0];
	mCamera = mCameraArr[0] = mSceneMgr->createCamera("PlayerCam");
	mCamera->setPosition(Ogre::Vector3(0,600,600));
	mCamera->lookAt(Ogre::Vector3(0,0,0));
	mCamera->setNearClipDistance(5);
	mCameraManArr[0] = new OgreBites::SdkCameraMan(mCamera);   // create a default camera controller
}

void BasicTutorial_00::createCamera_01(void)
{
	// add your own stuff
}



void BasicTutorial_00::createViewport_00(void)
{
	mCamera = mCameraArr[0];
	// Create one viewport, entire window
    Ogre::Viewport* vp = mWindow -> addViewport(mCamera);
    vp->setBackgroundColour(Ogre::ColourValue(0,0,0));
	mViewportArr[0] = vp;
	
    // Alter the camera aspect ratio to match the viewport
    mCamera->setAspectRatio(
        Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));
}

void BasicTutorial_00::createViewport_01(void)
{
    // add your own stuff
}

void BasicTutorial_00::createScene_00(void) 
{
	mSceneMgr = mSceneMgrArr[0];
	mSceneMgr -> setAmbientLight(ColourValue(0.7, 0.7, 0.7));

	// Enable fog
	Ogre::ColourValue fadeColour(0.9, 0.9, 0.9);
	mSceneMgr->setFog(Ogre::FOG_LINEAR, fadeColour, 0.0, 1400, 1600);

	// Enable sky
	mSceneMgr->setSkyBox(
		true, 
		"Examples/SpaceSkyBox"); 

	// Enable shadow
	mSceneMgr->setShadowTechnique(SHADOWTYPE_STENCIL_ADDITIVE); 

	// Create plane
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
	Entity* entGround = mSceneMgr->createEntity("GroundEntity", "ground");
	SceneNode* groundNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	groundNode->attachObject(entGround); 
	entGround->setCastShadows(false);
	entGround->setMaterialName("Examples/Rocky");

	// Create a circle of robots
    int numRobots = 36;
	for (int i = 0; i < numRobots; i ++) {
		std::string name;
		genNameUsingIndex("robotC1", i, name);
		Entity *entRobot = mSceneMgr->createEntity(name, "robot.mesh");
		entRobot->setCastShadows(true);
		SceneNode *robotNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
		robotNode -> attachObject(entRobot);
	    double fx = i / (double) (numRobots - 1); // in range [0,1]
        double radius = 300;
        double x1 = radius * cos(fx * PI * 2);
        double z1 = radius * sin(fx * PI * 2);
		if (i == 0) {
			robotNode->scale(2, 2, 2);
		}
		robotNode -> setPosition(x1, 0, z1);

	}

	// Create a circle of robots
	for (int i = 0; i < numRobots; i ++) {
		std::string name;
		genNameUsingIndex("robotC2", i, name);
		Entity *entRobot = mSceneMgr->createEntity(name, "robot.mesh");
		entRobot->setCastShadows(true);
		SceneNode *robotNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
		robotNode -> attachObject(entRobot);
	    double fx = i / (double) (numRobots - 1); // in range [0,1]
        double radius = 200;
        double x1 = radius * cos(fx * PI * 2);
        double z1 = radius * sin(fx * PI * 2);
	    robotNode -> setPosition(x1, 0, z1);
	}

	// Create Center sphere
	Entity *entSphere = mSceneMgr->createEntity("egg", "sphere.mesh");
    entSphere->setCastShadows(true);
	SceneNode *sphereNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("Sphere", Vector3(0, 0, 0));
	sphereNode->attachObject(entSphere);
	AxisAlignedBox bb =  entSphere->getBoundingBox();
	double sphereSize = bb.getMaximum().x-bb.getMinimum().x;
	double sphereScaling = 70.0/sphereSize;
	sphereNode->scale(sphereScaling, sphereScaling, sphereScaling);
	mSingleChooseObject = sphereNode;

	// Create light
    light1 = mSceneMgr -> createLight("Light1");
	light1 -> setType(Light::LT_SPOTLIGHT); 
	light1 -> setDiffuseColour( 0.8, 0.8, 0.8 );		
	light1 -> setSpecularColour( 0.3, 0.3, 0.3 );

	// Create cursor rectangle
	mSelectionRect = new SelectionRectangle("selectionRect");
	mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(mSelectionRect);
	mSelectionRect->setLightMask(0);
	mSelectionRect->setCastShadows(false);
	mSelectionRect->setVisible(false);

	// Create volume
	PlaneBoundedVolumeList volList;
	mVolQuery = mSceneMgr->createPlaneBoundedVolumeQuery(volList);
}

void BasicTutorial_00::createScene_01(void) 
{
    // add your own stuff
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

bool BasicTutorial_00::frameStarted(const Ogre::FrameEvent& evt)
{
	bool flg = Ogre::FrameListener::frameStarted(evt);
    mAngle += mAngularSpeed * evt.timeSinceLastFrame;
    if (mAngle > 2 * PI) mAngle = 0.0;
    double x = mRadius * cos(mAngle);
    double z = mRadius * sin(mAngle);
    light1 -> setPosition(x, 500, z);
	light1 -> setDirection(Vector3(-x, -500, -z));
    return flg;
}

bool BasicTutorial_00::mouseMoved( const OIS::MouseEvent &arg )
{   
	Ray mRay =mTrayMgr->getCursorRay(mCamera);
	Vector2 scn = mTrayMgr->sceneToScreen(mCamera, mRay.getOrigin());
	left = scn.x;
	top = scn.y;
	mSelectionRect->setCorners(left, top, right, bottom);

    return BaseApplication::mouseMoved( arg);
}

bool BasicTutorial_00::mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{   
	// IF mouse left button is released
    if (id == OIS::MB_Left) {
	    mSelectionRect->setVisible(false);
	    if (left==right && top==bottom) {
	    	return BaseApplication::mouseReleased( arg, id );
	    }
	    
	    //USING mTrayMgr=============
	    Real nleft = left;
	    Real nright = right;
	    Real ntop = 1 + top;
	    Real nbottom = 1 + bottom;
	    
	    // top, bottom is correct but left, right reverse 
	    if (nleft < nright && nbottom < ntop) {
	    	swap(nleft, nright);
	    }
	    
	    // left, right is correct but top, bottom reverse 
	    if (nleft > nright && nbottom > ntop) {
	    	swap(nbottom, ntop);
	    }
	    
	    Ray topLeft = mTrayMgr->screenToScene(mCamera, Vector2(nleft, ntop));
	    Ray topRight = mTrayMgr->screenToScene(mCamera, Vector2(nright, ntop));
	    Ray bottomLeft = mTrayMgr->screenToScene(mCamera, Vector2(nleft, nbottom));
	    Ray bottomRight = mTrayMgr->screenToScene(mCamera, Vector2(nright, nbottom));
	    //End Using mTrayMgr=============
	    
	    // The plane faces the counter clockwise position.
	    PlaneBoundedVolume vol;
	    int np = 100;
	    vol.planes.push_back(Plane(topLeft.getPoint(3), topRight.getPoint(3), bottomRight.getPoint(3)));         // front plane
	    vol.planes.push_back(Plane(topLeft.getOrigin(), topLeft.getPoint(np), topRight.getPoint(np)));         // top plane
	    vol.planes.push_back(Plane(topLeft.getOrigin(), bottomLeft.getPoint(np), topLeft.getPoint(np)));       // left plane
	    vol.planes.push_back(Plane(bottomLeft.getOrigin(), bottomRight.getPoint(np), bottomLeft.getPoint(np)));   // bottom plane
	    vol.planes.push_back(Plane(bottomRight.getOrigin(), topRight.getPoint(np), 	bottomRight.getPoint(np)));     // right plane 
	    
	    PlaneBoundedVolumeList volList;
	    volList.push_back(vol);
	    mVolQuery->setVolumes(volList);
	    
	    SceneQueryResult result = mVolQuery->execute();
	    
	    SceneQueryResultMovableList::iterator itr = result.movables.begin();
	    
	    // Get the results, set the camera height
	    // We are interested in the first intersection. It is ok to traverse all the results.
	    for (itr = result.movables.begin(); itr != result.movables.end(); ++itr)
	    {
	    	if (*itr)
	    	{
	    		SceneNode* currentObject = (*itr)->getParentSceneNode();
	    		// Don't choose the center sphere
	    		if (currentObject->getName() != "Sphere") {
	    		    bool flgShow = currentObject->getShowBoundingBox();
	    		    currentObject->showBoundingBox(!flgShow);
	    		    mCurrentObjectVector.push_back(currentObject);
	    		}
	    	}
	    }
	 }
	 return BaseApplication::mouseReleased( arg, id );
}

bool BasicTutorial_00::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{   
	if (id == OIS::MB_Left) {
	    // Close all the bounding boxes of previous choose
	    for (int i = 0; i < mCurrentObjectVector.size(); i ++) {
	    	mCurrentObjectVector[i]->showBoundingBox(false);
	    }
	    mCurrentObjectVector.clear();
	    
	    if(mSingleChooseObject) {
	    	 mSingleChooseObject->showBoundingBox(false);
	    }
	    Ray mRay = mTrayMgr->getCursorRay(mCamera);
	    
	    Vector2 scn = mTrayMgr->sceneToScreen(mCamera, mRay.getOrigin());
	    left = scn.x;
	    top = scn.y;
	    right = scn.x;
	    bottom = scn.y;
	    mSelectionRect->setCorners(left, top, right, bottom);
	    mSelectionRect->setVisible(true);
	    
	    mSceneMgr = mSceneMgrArr[0];
	    RaySceneQuery *mRaySceneQuery = 
	    	mSceneMgr->createRayQuery ( Ray() ); 
	    
	    mRaySceneQuery->setSortByDistance(true); 
	    										  
	    mRaySceneQuery->setRay(mRay);
	    // Perform the scene query
	    RaySceneQueryResult &result = 
	    	mRaySceneQuery->execute();
	    RaySceneQueryResult::iterator itr = result.begin();
	    
	    // Get the results, set the camera height
	    // We are interested in the first intersection. It is ok to traverse all the results.
	    for (itr = result.begin(); itr != result.end(); itr++)
        {
	        if (itr->movable && itr->movable->getName().substr(0, 5) != "tile[")
	        {
	    	    mSingleChooseObject = itr->movable->getParentSceneNode();
	    		// Don't choose the center sphere
	    		if (mSingleChooseObject->getName() != "Sphere") {
	    	        bool flgShow = mSingleChooseObject->getShowBoundingBox();
	    	        mSingleChooseObject->showBoundingBox(!flgShow);
	    	        break;
	    		}
	        } // if
	        else if (itr->worldFragment) {
	    	  //
	    		
	        }
	    }
	}
	
	return BaseApplication::mousePressed( arg, id );
}

int main(int argc, char *argv[]) {
	BasicTutorial_00 app;
	app.go();  
	return 0;
}

////////////////////////////////////////
// DO NOT DELETE THIS LINE: 2018/09/20. 3D Game Programming
////////////////////////////////////////