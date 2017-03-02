#include "SolarSystem.pch"

//-----------------------------------------------------------------------------------------------------------------
SolarSystem::SolarSystem (SceneManager *sm): GravityConstant(1)
{
	oSceneManager = sm;
}


//-----------------------------------------------------------------------------------------------------------------
SolarSystem::~SolarSystem ()
{		// Delete the memory we allocated
	for(unsigned int i = 0; i < vPlanets.size(); i++) delete vPlanets[i];
	for(unsigned int i = 0; i < vMoons.size(); i++) delete vMoons[i];
	for(unsigned int i = 0; i < vDebris.size(); i++) delete vDebris[i];
}


//-----------------------------------------------------------------------------------------------------------------
void SolarSystem::addPlanets()
{
	Entity* Sun;
	SceneNode* SunNode;
		// Create the Sun Entity and place it at origin
	Sun = oSceneManager->createEntity ("Sun", "Sun1.mesh");
	SunNode = oSceneManager->getRootSceneNode()->createChildSceneNode ("Sun", Vector3 (0, 0, 0));
	
	SunNode->attachObject(Sun);
	SunNode->scale(2000, 2000, 2000);
		
		// Add Planets
	addPlanet("Mercury", "Mercury.mesh","Sun", 30.0, 4879, 57900.0, -28.0, -6.0, 0.5, 0.0, 1.0, -10.0, 0.0, 8.0);
	addPlanet("Venus", "Venus1.mesh", "Sun", 1.0, 12104, 108200.0, 31.0, 2.5, 0.0, 0.0, -1.0, 0.0, 0.0, -8.0);
	addPlanet("Earth", "Earth.mesh", "Sun", 1.0, 12756, 149600.0, 36.525, 4.0, 1.0, 0.0, -1.0, 0.0, 0.0, 8.0);
	addPlanet("Mars",  "Mars.mesh", "Sun", 1.0, 6792, 227900.0, -40.0, -5.0, -0.5, 0.0, -1.0, -10.0, 0.0, -10.0);
	addPlanet("Jupiter", "Jupiter.mesh", "Sun", 1.0, 142984, 778600.0, 60.0, 7.0, 2.0, 0.0, -1.0, 0.0, 0.0, 9.0);
	addPlanet("Saturn", "Saturn.mesh", "Sun", 1.0, 120536, 1433500.0, 45.0, 8.0, -1.5, 0.0, 0.7, 10.0, 0.0, 20.0);
	addPlanet("Uranus", "Uranus.mesh", "Sun", 50.0, 51118, 2872500.0, -60.0, -10.0, 1.2, 0.0, 0.0, 10.0, 0.0, 5.0);
	addPlanet("Neptune", "Neptune.mesh", "Sun", 1.0, 49528, 4495100.0, 70.0, 9.0, -1.1, 0.0, 1.0, 0.0, 0.0, -9.0);
	addPlanet("Pluto", "Pluto.mesh", "Sun", 1.0, 2390, 5870000.0, -90.0, 9.0, 0.0, 0.0, 0.5, 8.0, 0.0, 8.0);
  	 
		// Add Moons
	addPlanet("Luna (Earth's Moon)", "Moon.mesh", "Earth", 10.0, 3475, 2250, 8.0, 0.0, 5.0, 0.0, -10.0, 0.0, 0.0, 0.0);
}


//-----------------------------------------------------------------------------------------------------------------
void SolarSystem::addPlanet(String pName, String pMesh, String pIsFacing, Real pScale, Real pDiameter, Real pOrbitRadius, 
	Real pYear, Real pSpin, Real pXorb, Real pYorb, Real pZorb, Real pXort, Real pYort, Real pZort)
{
		// Set body info
	OrbitalBody *oPlanet = new OrbitalBody(pName, pMesh, pIsFacing, pDiameter, pOrbitRadius, pYear, pSpin,
		pXorb, pYorb, pZorb, pXort, pYort, pZort);
		
		// Turn into entity and plot
	addBody(oPlanet, pScale);
}


//-----------------------------------------------------------------------------------------------------------------
void SolarSystem::addBody(OrbitalBody *oBody, Real Scale)
{
	SceneNode *oNode;
	Entity *oEntity;
	
		// Add this body to its vector
	//if(oBody->sOrbiting == "Sun");
		vPlanets.push_back(oBody);
	//else vMoons.push_back(oBody);
		
		// Make an entitiy for this body
	oEntity = oSceneManager->createEntity (oBody->sName, oBody->sMesh); 
	oNode = oSceneManager->getRootSceneNode()->createChildSceneNode(oBody->sName);
	  
		// Position body
	oNode->attachObject (oEntity);
	oNode->setPosition(oBody->getPosition());
	oNode->setScale (Scale, Scale, Scale);
}


//-----------------------------------------------------------------------------------------------------------------
void SolarSystem::Move(Real Speed)
{
	SceneNode* node;
		
		// Loop through entire system vector setting coordinats of each body for next frame
	for(unsigned int i = 0; i < vPlanets.size(); i++)
	{
		vPlanets[i]->getNext(Speed, oSceneManager, GravityConstant);
		node = oSceneManager->getSceneNode(vPlanets[i]->getName());
		node->setPosition(vPlanets[i]->getPosition());
		node->setOrientation(vPlanets[i]->getOrientation());
	}

	for(unsigned int i = 0; i < vMoons.size(); i++)
	{
		vMoons[i]->getNext(Speed, oSceneManager, GravityConstant);
		node = oSceneManager->getSceneNode(vMoons[i]->getName());
		node->setPosition(vMoons[i]->getPosition());
		node->setOrientation(vMoons[i]->getOrientation());
	}

	for(unsigned int i = 0; i < vDebris.size(); i++)
	{
		vDebris[i]->getNext(Speed, oSceneManager, GravityConstant);
		node = oSceneManager->getSceneNode(vDebris[i]->getName());
		node->setPosition(vDebris[i]->getPosition());
		node->setOrientation(vDebris[i]->getOrientation());
	}
}


//-----------------------------------------------------------------------------------------------------------------
Vector3 SolarSystem::findPlanet()
{
	static int i = 0;
	
	if(i==vPlanets.size())
		i = 0;

	return vPlanets[i++]->getPosition();
}


//-----------------------------------------------------------------------------------------------------------------
Vector3 SolarSystem::findPlanet(const String sName)
{
	for(unsigned int i = 0; i < vPlanets.size(); i++)
		if(vPlanets[i]->getName() == sName)
			return vPlanets[i]->getPosition();
}


//-----------------------------------------------------------------------------------------------------------------
bool SolarSystem::CheckEclipse()
{
	for(unsigned int i = 0; i < vPlanets.size(); i++)
	{
		for(unsigned int j = i+1; j < vPlanets.size(); j++)
			if(vPlanets[i]->getIsOrbiting() == vPlanets[j]->getName() 
				&& vPlanets[i]->getPosition().squaredDistance(Vector3(0, 0, 0))  > vPlanets[j]->getPosition().squaredDistance(Vector3(0, 0, 0)))
				return true;
	}
	
	return false;
}


//-----------------------------------------------------------------------------------------------------------------
bool SolarSystem::CheckEclipse(const String Body1, const String Body2)
{
	for(unsigned int i = 0; i < vPlanets.size(); i++)
	{
		if(vPlanets[i]->getName()== Body1 || vPlanets[i]->getName()== Body2)
			for(unsigned int j = i+1; j < vPlanets.size(); j++)
				if(vPlanets[i]->getName()== Body1 || vPlanets[i]->getName() == Body2
					&& vPlanets[i]->getPosition() - Vector3(0, 0, 0) > vPlanets[j]->getPosition() - Vector3(0, 0, 0)) 
					return true;
	}

	return false;	
}

