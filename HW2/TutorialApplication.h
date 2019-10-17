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
	std::vector <Ogre::SceneNode*> mCurrentObjectVector;
	Ogre::SceneNode* mSingleChooseObject;
	Ogre::Light* light1;
	//! The rotation angle of the light
	double mAngle;
	//! The rotation angular speed of the light
	double mAngularSpeed;
	//! The raduis of the rotation orbit of the light
	double mRadius;

	PlaneBoundedVolumeListSceneQuery *mVolQuery;
	Real left, top, right, bottom;
	SelectionRectangle *mSelectionRect;
};


#endif // #ifndef __BasicTutorial_00_h_