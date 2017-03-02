/*
-------------------------------------------------------------------------------------------------
Filename:    Main.cpp
-------------------------------------------------------------------------------------------------

This source file is part of the iMagiNext 2014 Solar Exploratorium Project.
   
   Main.cpp is the implementation for Main.pch. It contains definitions for all the functions and
   the WinAPI and Main() entry point for our program.

-------------------------------------------------------------------------------------------------
*/

#include "Main.pch"


//-----------------------------------------------------------------------------------------------
MainProgram::MainProgram(): mOgreRoot(0), mOgreOverlaySystem(0), mOgreSceneMgr(0)
{
	Files = new FilePaths();
	mResourceMgr = new ResourceLoader();
	mListener = new InputListener();
}


//-----------------------------------------------------------------------------------------------
MainProgram::~MainProgram()
{
	delete mSolarSystem;
	delete mListener;
	delete mResourceMgr;
	delete Files;
	delete mOgreOverlaySystem;
	delete mOgreRoot;
}


//-----------------------------------------------------------------------------------------------
void MainProgram::CreateScene()
{
	mOgreSceneMgr->setAmbientLight(Ogre::ColourValue(0.35, 0.35, 0.35));
	mOgreSceneMgr->setShadowTechnique (SHADOWTYPE_STENCIL_ADDITIVE);
	
	Light* lp = mOgreSceneMgr->createLight("SunLight5");
	lp->setPosition(0, 0, 0);
	lp->setType(Light::LT_POINT);
    lp->setDiffuseColour (ColourValue::White);

	mSolarSystem = new SolarSystem(mOgreSceneMgr);
	
	mSolarSystem->addPlanets();

	// Add this Solar System as animation target
	mListener->AddSystem(*&mSolarSystem);
}


//-----------------------------------------------------------------------------------------------
bool MainProgram::RunProgram()
{
		// files registered for default values. (In case of folder change, just add folder path)
	Files->setFilePaths();

		// Construct an Ogre Root object, for access to memory.
	mOgreRoot = new Root(Files->getFilePaths("p"), Files->getFilePaths("c"), Files->getFilePaths("l"));

		// Show the configuration dialog and initialise the system Or if previous config exists, restore config
	if(!(mOgreRoot->restoreConfig() || mOgreRoot->showConfigDialog()))
	{
		return false;
	}

		// Setup Resources.
	mResourceMgr->setupResources(Files->getFilePaths("r"));

		// Create A Window for THIS Root, with this name.
	mListener->setWindow(mOgreRoot, "iMagiNext 2014 Solar Exploratorium 0.9 (Build: Luna)");
	
		// Create A Scene Manager for THIS Root.
	mOgreSceneMgr = mOgreRoot->createSceneManager(ST_GENERIC, "OurScene Wenger");
	mOgreOverlaySystem = new Ogre::OverlaySystem();
    mOgreSceneMgr->addRenderQueueListener(mOgreOverlaySystem);

		// Create Camera and Viewports for THIS window.
	mListener->setupCamera(mOgreSceneMgr, mListener->getWindow(), "SolarCamera");
	
		// Initialize Resources
	mResourceMgr->initResources();

		// Set A Skybox
	mOgreSceneMgr->setSkyBox(true, "MySkyBox", 10000000);

		// Register Devices, Window and Camera for Input Control.
	mListener->setupInput();
	mListener->setupGUI("SolarGUI", mListener->getWindow());

		// Call Create Scene (sets up planets, animations etc)
	CreateScene();

		// Register everything as a Framelistener (capture events at every frame)
	mListener->setupFrameListener(mOgreRoot);

		// Start Rendering Scene
	mOgreRoot->startRendering();

	return true;
}




//-----------------------------------------------------------------------------------------------
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#endif
 
#ifdef __cplusplus
extern "C" {
#endif
 
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
    INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT )
#else
    int main(int argc, char *argv[])
#endif
    {
        // Create application object of the MainProgram class.
        MainProgram app;
 
        try {
            app.RunProgram();
        } catch( Ogre::Exception& e ) {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
            MessageBox( NULL, e.getFullDescription().c_str(), "An exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
            std::cerr << "An exception has occured: " <<
                e.getFullDescription().c_str() << std::endl;
#endif
        }
		catch(...) {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
            MessageBox( NULL, "Planet Files Not Found. Please Locate and Run Program Again", "File Not Found Error!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
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