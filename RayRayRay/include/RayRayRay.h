/*
-----------------------------------------------------------------------------
Filename:    RayRayRay.h
Author:      Reza Adhitya Saputra
-----------------------------------------------------------------------------


This source file is generated by the
   ___                   _              __    __ _                  _ 
  /___\__ _ _ __ ___    /_\  _ __  _ __/ / /\ \ (_)______ _ _ __ __| |
 //  // _` | '__/ _ \  //_\\| '_ \| '_ \ \/  \/ / |_  / _` | '__/ _` |
/ \_// (_| | | |  __/ /  _  \ |_) | |_) \  /\  /| |/ / (_| | | | (_| |
\___/ \__, |_|  \___| \_/ \_/ .__/| .__/ \/  \/ |_/___\__,_|_|  \__,_|
      |___/                 |_|   |_|                                 
      Ogre 1.7.x Application Wizard for VC9 (July 2011)
      http://code.google.com/p/ogreappwizards/
-----------------------------------------------------------------------------
*/
#ifndef __RayRayRay_h_
#define __RayRayRay_h_

#include <Terrain/OgreTerrain.h>
#include <Terrain/OgreTerrainGroup.h>

#include <cstdlib>
#include <ctime>

#include "ClassDefine.h"
#include "Rail.h"
#include "RayTerrain.h"
#include "BaseApplication.h"

#include "CEGUI.h"
#include "CEGUIOgreRenderer.h"

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#include "../res/resource.h"
#endif

class RayRayRay : public BaseApplication
{
public:
    RayRayRay(void);
    virtual ~RayRayRay(void);

	// Hikari
	RayFlashInterface* menu;
	Ogre::Viewport* hViewPort;

protected:
    virtual void createScene(void);
	virtual void destroyScene(void);
	virtual void chooseSceneManager(void);
	virtual void createFrameListener(void);
 
	virtual bool frameRenderingQueued(const Ogre::FrameEvent& arg);
 
	// OIS::MouseListener
	virtual bool mouseMoved(const OIS::MouseEvent& arg);
	virtual bool mousePressed(const OIS::MouseEvent& arg, OIS::MouseButtonID id);
	virtual bool mouseReleased(const OIS::MouseEvent& arg, OIS::MouseButtonID id);

	// OIS::KeyListener
	virtual bool keyPressed(const OIS::KeyEvent& arg);

	//virtual bool keyReleased( const OIS::KeyEvent &arg );
	//bool quit(const CEGUI::EventArgs &e);
 
	Ogre::SceneNode *mCurrentObject;	//pointer to our currently selected object
	Ogre::RaySceneQuery* mRayScnQuery;	//pointer to our ray scene query
	CEGUI::Renderer* mGUIRenderer;		//our CEGUI renderer
 
	bool bLMouseDown, bRMouseDown;		//true if mouse buttons are held down
	int mCount;							//number of objects created
	float mRotateSpeed;					//the rotation speed for the camera

	bool hideTray;

private:
	Rail* rail;
	RayTerrain* rayTerrain;

    OgreBites::Label* mInfoLabel;
};

#endif // #ifndef __RayRayRay_h_
