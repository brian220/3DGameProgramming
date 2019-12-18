#include "monsters.h"
#include "BasicTools.h"

MONSTER::MONSTER(SceneManager *a_SceneMgr) : GAME_OBJ(a_SceneMgr)
{
}

//
// Make the monster look at mTarget
//
void MONSTER::updateViewDirection()
{
	if (mTarget == 0) return;
	Vector3 pos = mSceneNode->getPosition();
	Vector3 target_pos = mTarget->getPosition();
    
	Vector3 mDirection = target_pos - pos;
	Vector3 src = mSceneNode->getOrientation() * Vector3::UNIT_Z;
	src.y = 0;
	mDirection.y = 0;
	src.normalise();
	// Special case for turning degree 180,
	// It may throw devide by zero error if we don't use this special case
	if ((1.0 + src.dotProduct(mDirection)) < 0.0001) {
        mSceneNode->yaw(Ogre::Degree(180));
    }
    else {
        Ogre::Quaternion quat = src.getRotationTo(mDirection);
        mSceneNode->rotate(quat);
    }
}

//
// Update the position of the monster.
//
void MONSTER::update(const Ogre::FrameEvent& evt)
{   
	Vector3 new_pos = mSceneNode->getPosition();
	
	Vector3 init_to_target = mTarget->getPosition() - mInitPosition;
	init_to_target.y = 0.0;
	
	Vector3 mv = mSceneNode->getPosition() - mTarget->getPosition();

	double dt = evt.timeSinceLastFrame;
	mVelocity.y = 0.0;

    // Monsters chase player
	mv.y = 0.0;
	Real d = mv.length();
	mv.normalise();

	float check_pass = -mv.dotProduct(init_to_target);
	if (check_pass < 0 || d < 100) {
        mVelocity = mv * 100;
    } else {
        mVelocity = -mv * 100;
    }
    mInitPosition = new_pos;
	new_pos += mVelocity*dt;

	new_pos.y = 100;

	// Monster move up and down
	mTime += dt + dt*(5*mRandSpeed)*0.1;
    Vector3 offset(0,0,0);
    offset.y = mAmplitude*sin(mTime)*0.01;
    if (mTime > 3.14159*2) {
        mTime -= 3.14159*2;
    }
	
	new_pos += offset;
	if (new_pos.y > 200) {
		new_pos.y = 200;
	}
	else if (new_pos.y < 50){
		new_pos.y = 50;
	}

	mSceneNode->setPosition(new_pos);
    updateViewDirection();
}