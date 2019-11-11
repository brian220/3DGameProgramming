//
// 3D Game Programming
// NCTU
// Instructor: SAI-KEUNG WONG
//
// Date: 2019/10/28
//
#include "TutorialApplication.h"
#include "BasicTools.h"

#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>

using namespace Ogre;

const float PI = 3.141592654;

#define mMoveDirection_NONE 0
#define mMoveDirection_DOWN (1 << 0)
#define mMoveDirection_UP   (1 << 1)
#define mMoveDirection_LEFT (1 << 2)
#define mMoveDirection_RIGHT (1 << 3)


BasicTutorial_00::BasicTutorial_00(void)
    : 
    mMoveDirection(mMoveDirection_NONE),
    mSmallWeight(1.0),
	mBigWeight(303.0),
	mMaxNumSpheres(500)
{}

void BasicTutorial_00::chooseSceneManager()
{
	mSceneMgrArr[0] = mRoot
		->createSceneManager(ST_GENERIC, "primary");
	mSceneMgrArr[1] = mRoot
		->createSceneManager(ST_GENERIC, "secondary");
}

void BasicTutorial_00::createCamera_00(void)
{
	mSceneMgr = mSceneMgrArr[0];
	mCamera = mCameraArr[0] = mSceneMgr->createCamera("PlayerCam");
	mCamera->setPosition(Ogre::Vector3(120,300,600));
	mCamera->lookAt(Ogre::Vector3(120,0,0));
	mCamera->setNearClipDistance(5);
	mCameraManArr[0] = new OgreBites::SdkCameraMan(mCamera);   // create a default camera controller
}

void BasicTutorial_00::createCamera_01(void)
{
	mSceneMgr = mSceneMgrArr[1];
	mCamera = mCameraArr[1] = mSceneMgr->createCamera("PlayerCam");
	mCamera->setPosition(Ogre::Vector3(0,350,0.0));
	mCamera->lookAt(Ogre::Vector3(0.00001,0,0));
	mCamera->setNearClipDistance(5);
	mCameraManArr[1] = new OgreBites::SdkCameraMan(mCamera);   // create a default camera controller
}



void BasicTutorial_00::createViewport_00(void)
{
	mCamera = mCameraArr[0];
	Ogre::Viewport* vp = mWindow->addViewport(mCamera);
	vp->setBackgroundColour(Ogre::ColourValue(0,0.0,1.0));
	mCamera->setAspectRatio(
		Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));

    mViewportArr[0] = vp;
}

void BasicTutorial_00::createViewport_01(void)
{
}

void BasicTutorial_00::resolveCollision(
    SceneNode *nodeA, SceneNode *nodeB,
    float rA, float rB, float wA, float wB)
{
    Vector3 posA = nodeA->getPosition();
    Vector3 posB = nodeB->getPosition();
    float R = rA + rB;
	Vector3 distance = posA - posB;
	if (distance.length() < R) {
		distance.normalise();
		if (wA == mSmallWeight) {
			// small, small collision
			nodeA->translate(distance);
			checkBoundary(nodeA, 15.0);
		}
			nodeB->translate(-distance);
			checkBoundary(nodeB, 15.0);
	}
}

void BasicTutorial_00::resolveCollisionLargeSphere()
{
    float smallR = 15; // small sphere radius
    float largeR = 1.0/0.15*smallR; // large sphere radius
     for (int i = 1; i < mNumSpheres; ++i)
	{
		resolveCollision(mSceneNode[0], mSceneNode[i], largeR, smallR, mBigWeight, mSmallWeight);
		checkBoundary(mSceneNode[0], largeR);
    }
}

void BasicTutorial_00::checkBoundary(SceneNode *node, float radius) {
	Vector3 pos = node->getPosition();
	if (pos.x + radius > 570 ) {
		node->setPosition(570 - radius, pos.y, pos.z);
	}
	if (pos.x - radius < -570 ) {
		node->setPosition(-570 + radius, pos.y, pos.z);
	}

	if (pos.z + radius > 570 ) {
		node->setPosition(pos.x, pos.y, 570 - radius);
	}
	if (pos.z - radius < -570 ) {
		node->setPosition(pos.x, pos.y, -570 + radius);
	}
}

// perform collision handling for all pairs
void BasicTutorial_00::resolveCollisionSmallSpheres()
{
    float ri = 15; // sphere radius
    float rj = 15; // sphere radius
    for (int i = 1; i < mNumSpheres; ++i)
	{
		for (int j = i + 1; j < mNumSpheres; ++j) {
            resolveCollision(mSceneNode[i], mSceneNode[j], 15.0, 15.0, 1.0, 1.0);
        }
    }
}

void BasicTutorial_00::resolveCollision()
{
    int num = 10;
    for (int i = 0; i < num;++i) {
        resolveCollisionSmallSpheres();
        resolveCollisionLargeSphere();
    }
}

// reset positions of all small spheres
void BasicTutorial_00::reset()
{
    for (int i = 1; i < mNumSpheres; ++i ) {
        float randX = getRandNum(-400.0, 400.0);
		float randZ = getRandNum(-400.0, 400.0);
		mSceneNode[i]->setPosition(randX, 0.0, randZ);
    }
}

float BasicTutorial_00::getRandNum(float lower, float upper) {
	float randNum = (rand() % 10000) / (float)10000.0;
	float randNumInRange = lower + (upper - lower) * randNum;

	return randNumInRange;
}

void BasicTutorial_00::setSphereVisible(int numSphere) {
	for (int i = 1; i < mMaxNumSpheres; ++i ) {
       if (i < numSphere) {
		   mSceneNode[i]->setVisible(true);
	   }
	   else {
		   mSceneNode[i]->setVisible(false); 
	   }
    }
}

// create all spheres
// "Examples/red"
// "Examples/green"
// "Examples/blue"
// "Examples/yellow"
void BasicTutorial_00::createSpace()
{   
	
    String name_en;
    String name_sn;
    for (int i = 0; i < mMaxNumSpheres; ++i) {
         genNameUsingIndex("R1", i, name_en);
         genNameUsingIndex("S1", i, name_sn);
         mEntity[i] = mSceneMgr ->createEntity( name_en, "sphere.mesh" );
		 mSceneNode[i] = mSceneMgr->getRootSceneNode()->createChildSceneNode(name_sn);

		 float randX = getRandNum(-400.0, 400.0);
		 float randZ = getRandNum(-400.0, 400.0);

         switch(rand()%3) {
         case 0:
             mEntity[i]->setMaterialName("Examples/red");
             break;
         case 1:
		 	mEntity[i]->setMaterialName("Examples/green");
             break;
         case 2:
		 	mEntity[i]->setMaterialName("Examples/blue");
             break;
         }
         
		 if (i == 0) {
		 	 // Set the big sphere
		 	 mEntity[i]->setMaterialName("Examples/yellow");
		 	 mSceneNode[i]->setPosition(0.0,0.0,0.0);
		 }
		 else {
             //scale the small spheres
             mSceneNode[i]->scale(0.15, 0.15, 0.15);
			 mSceneNode[i]->setPosition(randX, 0.0, randZ);
		 }

		 mSceneNode[i]->attachObject(mEntity[i]);
    }

	mNumSpheres = 300;
	setSphereVisible(mNumSpheres);
}

void BasicTutorial_00::createBarrel()
{   
	String name_en;
    String name_sn;
	// Create barrel
	int offset = mMaxNumSpheres;
    mNumObstacles = 80;
    for (int i = 0; i < mNumObstacles; ++i ) {
        int index = i + offset;
        genNameUsingIndex("R1", index, name_en);
        genNameUsingIndex("S1", index, name_sn);
        mEntity[index] = mSceneMgr->createEntity( name_en, "Barrel.mesh" );
        mSceneNode[index] = mSceneMgr->getRootSceneNode()->createChildSceneNode(name_sn); 

        mSceneNode[index]->scale(10.0, 10.0, 10.0);
        mSceneNode[index]->attachObject(mEntity[index]);
    }

	float r_obs = 28.0; 	// barrel radius
    float d_gap = 30.0;
    for (int i = 0; i < 20; ++i) {
        int index = i+offset;

        float x = -600.0 + 2 * d_gap;
        float y = 10.0;
        float z = -600.0;
        x += 2 * d_gap * i;
        mSceneNode[index]->setPosition(x,y,z);
    }
    
	offset += 20;
    for (int i = 0; i < 20; ++i) {
        int index = i + offset;

        float x = -600.0;
        float y = 10.0;
        float z = 600.0;
        x += 2*d_gap*i;
        mSceneNode[index]->setPosition(x,y,z);
    }

	offset += 20;
    for (int i = 0; i < 20; ++i) {
        int index = i + offset;

        float x = -600.0;
        float y = 10.0;
        float z = -600.0;
        z += 2 * d_gap * i;
        mSceneNode[index]->setPosition(x,y,z);
    }

	offset += 20;
    for (int i = 0; i < 20; ++i) {
        int index = i + offset;

        float x = 600.0;
        float y = 10.0;
        float z = -600.02 + 2 * d_gap;
        z += 2 * d_gap * i;
        mSceneNode[index]->setPosition(x,y,z);
    }
}

void BasicTutorial_00::createScene_00(void) 
{
	mSceneMgr = mSceneMgrArr[0];
	//mSceneMgr->setAmbientLight( ColourValue( 0.25, 0.25, 0.25 ) ); 

	mSceneMgr->setAmbientLight( ColourValue( 0.5, 0.5, 0.5 ) ); 
	//mSceneMgr->setAmbientLight( ColourValue( 1, 1, 1 ) );  
	mSceneMgr->setShadowTechnique(
		SHADOWTYPE_STENCIL_ADDITIVE); 

	Light *light;
	light = mSceneMgr->createLight("Light1"); 
	light->setType(Light::LT_POINT); 
	light->setPosition(Vector3(150, 250, 100)); 
	light->setDiffuseColour(0.3, 0.3, 0.3);		
	light->setSpecularColour(0.5, 0.5, 0.5);	

	light = mSceneMgr->createLight("Light2"); 
	light->setType(Light::LT_POINT); 
	light->setPosition(Vector3(-150, 300, 250)); 
	light->setDiffuseColour(0.25, 0.25, 0.25);		
	light->setSpecularColour(0.35, 0.35, 0.35);	

	//
	Plane plane(Vector3::UNIT_Y, 0); 
	MeshManager::getSingleton().createPlane(
		"ground", 						ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, 
		plane, 
		1500,1500, 	// width, height
		20,20, 		// x- and y-segments
		true, 		// normal
		1, 			// num texture sets
		5,5, 		// x- and y-tiles
		Vector3::UNIT_Z	// upward vector
		); 

	Entity *ent = mSceneMgr->createEntity(
		"GroundEntity", "ground"); 
	//ent->setMaterialName("Examples/BeachStones");
	//ent->setMaterialName("Examples/Rockwall");


	mSceneMgr
		->getRootSceneNode()
		->createChildSceneNode()
		->attachObject(ent); 

    ent = mSceneMgr->createEntity(
		"Background", "ground"); 
	//ent->setMaterialName("Examples/BeachStones");
	ent->setMaterialName("Examples/Rockwall");

    //Radian angle((45/180.0)*3.141592654);
    Radian angle(3.141952654/2.0);

    Vector3 axis(1.0, 0.0, 0.0);
    mSceneMgr
		->getRootSceneNode()
		->createChildSceneNode(
            Vector3(0.0, 0.0, -750.0),
            Quaternion( angle, axis))
		->attachObject(ent);

    createSpace();

	createBarrel();
   
    resolveCollision();
}

void BasicTutorial_00::createScene_01(void) 
{

}

void BasicTutorial_00::createViewports(void)
{
	createViewport_00();
	createViewport_01();
}

void BasicTutorial_00::createCamera(void) {
	createCamera_00();
	createCamera_01();
	mCameraMan = mCameraManArr[0];
	//mCameraMan = mCameraManArr[1];
}

void BasicTutorial_00::createScene( void ) {
	createScene_00();
	createScene_01();
	//mSceneMgr = mSceneMgrArr[0]; // active SceneManager
	mSceneMgr = mSceneMgrArr[1]; // active SceneManager
    //
    mCamera = mCameraArr[0];
    //mCamera = mCameraArr[1];
    //
    mCameraMan->getCamera()
            ->setPosition(Vector3(-22.30,	409.24,	816.74));
        mCameraMan->getCamera()
            ->setDirection(Vector3(0.02,	-0.23,	-0.97));

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
    msg += ":*** keyPressed ***\n";
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
            ->setPosition(Vector3(-22.30,	409.24,	816.74));
        mCameraMan->getCamera()
            ->setDirection(Vector3(0.02,	-0.23,	-0.97));

    }

    if (arg.key == OIS::KC_2 ) {
        mCameraMan->getCamera()
            ->setPosition(Vector3(-824.52,	468.58,	68.45));
        mCameraMan->getCamera()
            ->setDirection(Vector3(0.94,	-0.31,	-0.11));

        //-1463.00	606.45	-513.24
        //0.88	-0.47	0.10
    }


    if (arg.key == OIS::KC_3 ) {
        mCameraMan->getCamera()
            ->setPosition(Vector3(19.94,	822.63,	30.79));
        mCameraMan->getCamera()
            ->setDirection(Vector3(0.00,	-0.99,	-0.11));
        //19.94	822.63	30.79
        //0.00	-0.99	-0.11
    }

	if (arg.key == OIS::KC_7 ) {
		mNumSpheres = 100;
		setSphereVisible(mNumSpheres);
    }

	if (arg.key == OIS::KC_8 ) {
		mNumSpheres = 200;
		setSphereVisible(mNumSpheres);
    }

	if (arg.key == OIS::KC_9 ) {
		mNumSpheres = 300;
		setSphereVisible(mNumSpheres);
    }

	if (arg.key == OIS::KC_0 ) {
		mNumSpheres = 500;
		setSphereVisible(mNumSpheres);
    }

    if (arg.key == OIS::KC_B ) {
		reset();
    }

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

bool BasicTutorial_00::frameStarted(const Ogre::FrameEvent& evt)
{
	bool flg = Ogre::FrameListener::frameStarted(evt);

	Vector3 mdir = Vector3(0.0, 0.0, 0.0);
	if (mKeyboard->isKeyDown(OIS::KC_U) ) {
		mdir += Vector3(0.0, 0.0, -1.0);
	}

	if (mKeyboard->isKeyDown(OIS::KC_J)) {
		mdir += Vector3(0.0, 0.0, 1.0);
	}

	if (mKeyboard->isKeyDown(OIS::KC_H)) {
		mdir += Vector3(-1.0, 0.0, 0.0);
	}

	if (mKeyboard->isKeyDown(OIS::KC_K)) {
		mdir += Vector3(1.0, 0.0, 0.0);
	}

    mSceneNode[0]->translate(mdir * 5.0);

    resolveCollision();

    return flg;
}

int main(int argc, char *argv[]) {
	BasicTutorial_00 app;
	app.go();  
	return 0;
}
