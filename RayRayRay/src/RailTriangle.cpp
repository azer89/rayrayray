

#include "Stdafx.h"

#include "RailTriangle.h"

//-------------------------------------------------------------------------------------
RailTriangle::~RailTriangle(void)
{

}

//-------------------------------------------------------------------------------------
void RailTriangle::addTriangleStrip(std::vector<Ogre::Vector3> rPoints, std::vector<Ogre::Vector3> lPoints)
{
	Ogre::ManualObject* manual = mSceneMgr->createManualObject("manual");
	manual->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_TRIANGLE_STRIP);


	/*
	// define vertex position of index 0..3
	manual->position(-100.0, -100.0, 0.0);
	manual->position( 100.0, -100.0, 0.0);
	manual->position( 100.0,  100.0, 0.0);
	manual->position(-100.0,  100.0, 0.0);

	// define usage of vertices by refering to the indexes
	manual->index(0);
	manual->index(1);
	manual->index(2);
	manual->index(3);
	manual->index(0);
	*/
	manual->end();
	//mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(manual);
}
