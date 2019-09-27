//
// 3D Game Programming
// NCTU
// Instructor: SAI-KEUNG WONG
//
/*!
\brief 3D Game Programming
\n
My Name: CHAO-YU HUANG
\n
My ID: 0856601
\n
My Email: b608390@gmail.com
\n Date: 2019/09/27

This is an assignment of 3D Game Programming
*/

////////////////////////////////////////
// You can delete or add some functions to do the assignment.
////////////////////////////////////////

#ifndef __BasicTutorial_00_h_
#define __BasicTutorial_00_h_

#include "BaseApplication.h"

class BasicTutorial_00 : public BaseApplication
{
public:
	BasicTutorial_00(void);
    virtual void chooseSceneManager(void);
    virtual void createCamera(void);
	virtual void createViewports(void);
	virtual void createScene(void);

	/**
	* @brief Update the animation in scene primary at the start of frame.
	*
	* Update the animation state in scene primary.\n
	* Implement the rotation of two penguins by updating the penguin's angular speed ,angular acceleration and position.
	*
	* @param evt Use the evt.timeSinceLastFrame to compute mWaitingTime, mAngularSpeed, mAngle.
	*
	* List of animation state of scene primary:
	*
	* - state1: The big penguin look at small penguin and wait for 1 second.
	* - state2: The small penguin rotate around the big penguin.
	*/
    virtual bool frameStarted(const Ogre::FrameEvent& evt);
   
protected:
	/**
	* @brief Create a viewport.
	*
	* Create a viewport for the entire screen.
	*/
	void createViewport_00(void);

	/**
	* @brief Create a viewport.
	*
	* Create a viewport for the left-top 1/4 screen.\n
	* Left top of the viewport locate at (0,0), width = 0.25, height = 0.25.
	*/
	void createViewport_01(void);
	
	/**
	* @brief Create a camera.
	* 
	* Create a camera by scene manager "primary".
    */
	void createCamera_00();

	/**
	* @brief Create a camera.
	* 
	* Create a camera by scene manager "secondary".
	*/
	void createCamera_01();

	/**
	* @brief Create a scene.
	* 
	* Create a scence by scene manager "primary" including two penguins.
	*/
	void createScene_00();

	/**
	* @brief Create a scene.
	* 
	* Create a scene by scene manager "secondary" including a scaled sphere and a plane.
	*/
	void createScene_01();

	/**
	* @brief Solve the keyboard input when key pressed.
	*
	*  When key pressed, solve the keyboard input according to different keys and show the key pressing message.
	*
	* @param arg The keyEvent get from the keyboard corresponding input.
	*
	* List of the key board inputs and the events:
	*
	* - key C: Show the camera position and direction.
	* - key 1 ~ key 5: Move the camera to the specific position.
	* - key M: Change the z-order of two scenes for scene switching.\n
	*  (scene primary's z-order from 0 -> 1, and scene secondary's z-order from 1 -> 0).
	* - key N: Change the z-order of two scenes for scene switching.\n
	*  (scene primary's z-order from 1 -> 0, and scene secondary's z-order from 0 -> 1).
	* - key P: Turn on and turn off the animation in scene primary by changing the variable string animation.
	*/
    bool keyPressed( const OIS::KeyEvent &arg );

	/**
	* @brief Solve the keyboard input when key released.
	*
	* When key released, show the key released message.\n
	* Then call the keyReleased() function in class BaseApplication and pass the parameter arg to that function.
	* 
	* @param arg The keyEvent get from the keyboard corresponding input.
	*/
    bool keyReleased( const OIS::KeyEvent &arg );
  
protected:
    Ogre::Viewport* mViewportArr[8];
	Ogre::Camera* mCameraArr[8];
	Ogre::SceneManager* mSceneMgrArr[8];
	OgreBites::SdkCameraMan* mCameraManArr[8];
    
	//! The rotation angle of the small penguin.
	double mAngle;
	//! The rotation angular acceleration of the small penguin.
	double mAngularAcceleration;
	//! The rotation angular speed of the small penguin.
	double mAngularSpeed;
	//! The raduis of the rotation orbit of the small penguin.
	double mRadius;
	//! The time small penguin wait for big penguin to turn to.
	double mWaitingTime; 
	//! To control the rotate direction of the penguin.\n +1 = clockwise  -1 = counter-clockwise.
	double mRotateDir; 
};


#endif // #ifndef __BasicTutorial_00_h_
