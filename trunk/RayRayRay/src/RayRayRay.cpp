/*
-----------------------------------------------------------------------------
Filename:    RayRayRay.cpp
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
#include <iostream>

#include "RayRayRay.h"
#include "RayFlashInterface.h"

//-------------------------------------------------------------------------------------
RayRayRay::RayRayRay(void):
	mCount(0),
	mCurrentObject(0),
	bLMouseDown(false),
	bRMouseDown(false),
	mRotateSpeed(0.1f),	
	hideTray(false)
{

}
//-------------------------------------------------------------------------------------
RayRayRay::~RayRayRay(void)
{
	mSceneMgr->destroyQuery(mRayScnQuery);
}

//-------------------------------------------------------------------------------------
void RayRayRay::destroyScene(void)
{
	if(rail) delete rail;
	if(rayTerrain) delete rayTerrain;
	if(menu) delete menu;
}


//-------------------------------------------------------------------------------------
void RayRayRay::createScene(void)
{
	// Water

	// Set Hikari
	hViewPort = mCamera->getViewport();
	menu = new RayFlashInterface(this);
	menu->setupHikari();

	// set Rail
	rail = new Rail();
	rayTerrain = new RayTerrain();

	// set camera
	mCamera->setPosition(Ogre::Vector3(357, 70, 171));
    mCamera->lookAt(Ogre::Vector3(357, 80, 200));
    mCamera->setNearClipDistance(5);
    mCamera->setFarClipDistance(50000);
 
    if (mRoot->getRenderSystem()->getCapabilities()->hasCapability(Ogre::RSC_INFINITE_FAR_PLANE))
    {
        mCamera->setFarClipDistance(0);   // enable infinite far clip distance if we can
    }
	 
	// Play with startup Texture Filtering options
	// Note: Pressing T on runtime will discarde those settings
	//  Ogre::MaterialManager::getSingleton().setDefaultTextureFiltering(Ogre::TFO_ANISOTROPIC);
	//  Ogre::MaterialManager::getSingleton().setDefaultAnisotropy(7);
 
	
    Ogre::Vector3 lightdir(0.55, -0.3, 0.75);
    lightdir.normalise();
 
    Ogre::Light* light = mSceneMgr->createLight("tstLight");
    light->setType(Ogre::Light::LT_DIRECTIONAL);
    light->setDirection(lightdir);
    light->setDiffuseColour(Ogre::ColourValue::White);
    light->setSpecularColour(Ogre::ColourValue(0.4, 0.4, 0.4));
	mSceneMgr->setAmbientLight(Ogre::ColourValue(0.4, 0.4, 0.4));
    mSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);
	
	// set up Terrain
	rayTerrain->createTerrain(this->mSceneMgr, light);

    Ogre::Plane plane;
    plane.d = 100;
    plane.normal = Ogre::Vector3::NEGATIVE_UNIT_Y;
 
    //mSceneMgr->setSkyDome(true, "Examples/CloudySky", 5, 8, 500);
	mSceneMgr->setSkyPlane(true, plane, "Examples/CloudySky", 50, 10 , true, 0.7, 10, 10); 
}
 
//-------------------------------------------------------------------------------------
void RayRayRay::chooseSceneManager(void)
{
	//create a scene manager that is meant for handling outdoor scenes
	mSceneMgr = mRoot->createSceneManager(Ogre::ST_EXTERIOR_CLOSE);
}
 
//-------------------------------------------------------------------------------------
void RayRayRay::createFrameListener(void)
{
	//we still want to create the frame listener from the base app
	BaseApplication::createFrameListener();
	
	//mInfoLabel = mTrayMgr->createLabel(OgreBites::TL_TOP, "TInfo", "", 350);
 
	// Set up our raySceneQuery after everything has been initialized
	mRayScnQuery = mSceneMgr->createRayQuery(Ogre::Ray());
}
 
//-------------------------------------------------------------------------------------
bool RayRayRay::frameRenderingQueued(const Ogre::FrameEvent& arg)
{
	
	if(mWindow->isClosed())
        return false;
 
    if(mShutDown)
        return false;
 
    // Need to capture/update each device
    mKeyboard->capture();
    mMouse->capture();
	
	//we want to run everything in the previous frameRenderingQueued call
	//but we also want to do something afterwards, so lets  start off with this
	if(!BaseApplication::frameRenderingQueued(arg))
	{
		return false;
	}

	// hide 
	if(!hideTray)
	{
		mTrayMgr->hideLogo();
		mTrayMgr->toggleAdvancedFrameStats();
		hideTray = true;
		
	}

	// This next big chunk basically sends a raycast straight down from the camera's position
	// It then checks to see if it is under world geometry and if it is we move the camera back up
	Ogre::Vector3 camPos = mCamera->getPosition();
	Ogre::Terrain* pTerrain = rayTerrain->getTerrainGroup()->getTerrain(0, 0);
	Ogre::Ray mouseRay(Ogre::Vector3(camPos.x, 5000.0f, camPos.z), Ogre::Vector3::NEGATIVE_UNIT_Y);
    std::pair <bool, Ogre::Vector3> test;
    test = pTerrain->rayIntersects(mouseRay, true, 0);
	
	if (test.first) 
	{
		//gets the results, fixes camera height
		Ogre::Real terrainHeight = test.second.y;
		if((terrainHeight + 10.0f) > camPos.y)
		{
			mCamera->setPosition(camPos.x, terrainHeight + 10.0f, camPos.z);
		}			
	}
	
	if (rayTerrain->getTerrainGroup()->isDerivedDataUpdateInProgress())
    {
		
        mTrayMgr->moveWidgetToTray(mInfoLabel, OgreBites::TL_TOP, 0);
        mInfoLabel->show();
        if (rayTerrain->getTerrainsImported())
        {
            mInfoLabel->setCaption("Building terrain, please wait...");
        }
        else
        {
            mInfoLabel->setCaption("Updating textures, patience...");
        }
		
    }
    else
    {
		
        //mTrayMgr->removeWidgetFromTray(mInfoLabel);
        //mInfoLabel->hide();
        if (rayTerrain->getTerrainsImported())
        {
            rayTerrain->getTerrainGroup()->saveAllTerrains(true);
            //mTerrainsImported = false;
			rayTerrain->setTerrainsImported(false);
        }
		
    }

	// update Hikari
	menu->update(this->mWindow);

	return true;
}
 
//-------------------------------------------------------------------------------------
bool RayRayRay::mouseMoved(const OIS::MouseEvent& arg)
{
	if(!BaseApplication::mouseMoved(arg))
	{
		return false;
	}
	
	Ogre::Real screenWidth = hViewPort->getWidth();
	Ogre::Real screenHeight = hViewPort->getHeight();

	Ogre::Real offsetX = (float)arg.state.X.abs / (float)arg.state.width;
	Ogre::Real offsetY = (float)arg.state.Y.abs / (float)arg.state.height;

	//using namespace Hikari;
	bool val = menu->hikariMgr->injectMouseMove(arg.state.X.abs, arg.state.Y.abs) ||  menu->hikariMgr->injectMouseWheel(arg.state.Z.rel);
	
	
	//if the left mouse button is held down
	if(bLMouseDown)
	{
		Ogre::Ray mouseRay = mCamera->getCameraToViewportRay(offsetX, offsetY);

		Ogre::Vector3 camPos = mCamera->getPosition();
		Ogre::Terrain* pTerrain = rayTerrain->getTerrainGroup()->getTerrain(0, 0); 
		std::pair <bool, Ogre::Vector3> test;
		test = pTerrain->rayIntersects(mouseRay, true, 0);

		if (mCurrentObject && test.first) 
		{
			mCurrentObject->setPosition(test.second);
		}
	}
	else if(bRMouseDown)	//if the right mouse button is held down, be rotate the camera with the mouse
	{
		mCamera->yaw(Ogre::Degree(-arg.state.X.rel * mRotateSpeed));
		mCamera->pitch(Ogre::Degree(-arg.state.Y.rel * mRotateSpeed));
	}
	
	return val;
}
 
//-------------------------------------------------------------------------------------
bool RayRayRay::mousePressed(const OIS::MouseEvent& arg, OIS::MouseButtonID id)
{
	if(id == OIS::MB_Left)
	{
		//show that the current object has been deselected by removing the bounding box visual
		if(mCurrentObject)
		{
			mCurrentObject->showBoundingBox(false);
		}
		
		Ogre::Real screenWidth = hViewPort->getWidth();
		Ogre::Real screenHeight = hViewPort->getHeight();

		Ogre::Real offsetX = (float)arg.state.X.abs / (float)arg.state.width;
		Ogre::Real offsetY = (float)arg.state.Y.abs / (float)arg.state.height;

		Ogre::Ray mouseRay = mCamera->getCameraToViewportRay(offsetX, offsetY);
		mRayScnQuery->setRay(mouseRay);
		mRayScnQuery->setSortByDistance(true);

		// create rayIntersect fro TerrainGroup		
		Ogre::Terrain* pTerrain = rayTerrain->getTerrainGroup()->getTerrain(0, 0);
		std::pair <bool, Ogre::Vector3> test;
		test = pTerrain->rayIntersects(mouseRay, true, 0);
		
		//This next chunk finds the results of the raycast
		//If the mouse is pointing at world geometry we spawn a robot at that position
		Ogre::RaySceneQueryResult& result = mRayScnQuery->execute();
		Ogre::RaySceneQueryResult::iterator iter = result.begin();

		bool getObject = false;

		for(iter; iter != result.end(); iter++)
		{
			//if you clicked on a robot or ninja it becomes selected
			if(iter->movable && iter->movable->getName().substr(0, 5) != "tile[")
			{
				Ogre::SceneNode* tempNode = iter->movable->getParentSceneNode();
				//std::cout << tempNode->getName() << "\n";
				if(tempNode->getName() == "Unnamed_3") continue;
			
				mCurrentObject = tempNode;
				//std::cout << "~~~ name1" << mCurrentObject->getName() << "\n";
				getObject = true;
				break;
			}
		}

		if(test.first && !getObject)
		{
			mCurrentObject = rail->addPoint(mSceneMgr, test.second);
		}
 
		//now we show the bounding box so the user can see that this object is selected
		if(mCurrentObject)
		{
			mCurrentObject->showBoundingBox(true);
		}
		
		bLMouseDown = true;
		
	}
	else if(id == OIS::MB_Right)	// if the right mouse button is held we hide the mouse cursor for view mode
	{
		bRMouseDown = true;
	}

	using namespace Hikari;
	return  menu->hikariMgr->injectMouseDown(id);
 
	return true;
}
 
//-------------------------------------------------------------------------------------
bool RayRayRay::mouseReleased(const OIS::MouseEvent& arg, OIS::MouseButtonID id)
{
	if(id  == OIS::MB_Left)
	{
		bLMouseDown = false;
	}
	else if(id == OIS::MB_Right)	//when the right mouse is released we then unhide the cursor
	{
		//CEGUI::MouseCursor::getSingleton().show();
		bRMouseDown = false;
	}

	using namespace Hikari;
	return  menu->hikariMgr->injectMouseUp(id);

	return true;
}

//------------------------------------------------------------------------------------- 
bool RayRayRay::keyPressed(const OIS::KeyEvent& arg)
{ 
	//then we return the base app keyPressed function so that we get all of the functionality
	//and the return value in one line
	return BaseApplication::keyPressed(arg);

	//return true;
}


#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
    //INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT )
	int main()
#else
    int main(int argc, char *argv[])
#endif
    {
        // Create application object
        RayRayRay app;

        try {
            app.go();
        } catch( Ogre::Exception& e ) {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
            MessageBox( NULL, e.getFullDescription().c_str(), "An exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
            std::cerr << "An exception has occured: " <<
                e.getFullDescription().c_str() << std::endl;
#endif
        }

        return 0;
    }

#ifdef __cplusplus
}
#endif
