//
// 3D Game Programming
// NCTU
// Instructor: SAI-KEUNG WONG
//
/*!
\brief 3D Game Programming
\n
My Name: CHAO YU HUANG
\n
My ID: 0856601
\n
My Email: b608390@gmail.com
\n Date: 2019/09/20

This is an assignment of 3D Game Programming
*/

////////////////////////////////////////
// You can delete or add some functions to do the assignment.
////////////////////////////////////////

#ifndef __BasicTutorial_00_h_
#define __BasicTutorial_00_h_

#include "BaseApplication.h"
#include "selection_rectangle.h"
#include <vector>
#include <string>

class BasicTutorial_00 : public BaseApplication
{
public:
	BasicTutorial_00(void);
    virtual void chooseSceneManager(void);
    virtual void createCamera(void);
	virtual void createViewports(void);
	virtual void createScene(void);

    //
    virtual bool frameStarted(const Ogre::FrameEvent& evt);
	virtual bool mouseMoved( const OIS::MouseEvent &arg );
	virtual bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
	virtual bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id );

	void mouseReleasedLeftButtonEvent();
	void mouseReleasedRightButtonEvent();

	void mousePressedLeftButtonEvent();
	void mousePressedRightButtonEvent();

	//!For collision detection
	bool isCollidedByRobots(SceneNode* currentNode, double collideRadius);
	//bool isCollisionBetweenRobots(SceneNode* currentNode);
    //
    // Add your own stuff.
    //
protected:
	/*!
	\brief Create a viewport

	Create a viewport for the entire screen.

	\return The sum of two integers.
	*/
	void createViewport_00(void);
	void createViewport_01(void);
	//
	void createCamera_00();
	void createCamera_01();

	void createScene_00();
	void createScene_01();

protected:
    Ogre::Viewport* mViewportArr[8];
	Ogre::Camera* mCameraArr[8];
	Ogre::SceneManager* mSceneMgrArr[8];
	OgreBites::SdkCameraMan* mCameraManArr[8];
	Ogre::Plane mPlane;
	Ogre::SceneNode* mCenterSphere;

	std::vector <Ogre::SceneNode*> mCurrentObjectVector;
	//Ogre::SceneNode* mSingleChooseObject;
	Ogre::Light* light1;
	//! The rotation angle of the light
	double mAngle;
	//! The rotation angular speed of the light
	double mAngularSpeed;
	//! The raduis of the rotation orbit of the light
	double mRadius;

	//! The speed of robots' moving
	double mRobotSpeed;

	PlaneBoundedVolumeListSceneQuery *mVolQuery;
	Real left, top, right, bottom;
	SelectionRectangle *mSelectionRect;

	//! The mask for the query selection
	enum QueryFlags 
	{
	    ROBOT_MASK = 1 << 0,
		SPHERE_MASK = 1 << 1,
		GROUND_MASK = 1 << 2
	};

	double mAnimationSpeedUp;

	//! Store the animation state for each robot
	std::vector <Ogre::AnimationState*> mAnimationStateVector;

	//! Target position for robots moving
	Ogre::Vector3 targetPosition;

	//! Animation State for all the robots
	std::string globalAnimationState;

	Ogre::Vector3 mCurrentBounceBackDirection;

	Ogre::Real mCurrentMove;
};


#endif // #ifndef __BasicTutorial_00_h_