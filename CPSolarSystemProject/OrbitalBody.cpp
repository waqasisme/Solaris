#include "OrbitalBody.pch"


//-----------------------------------------------------------------------------------------------------------------
OrbitalBody::OrbitalBody(String Name, String MeshName, String IsOrbiting, Real Diameter, Real OrbitRadius, Real Year, Real Spin,
			Real rXorb, Real rYorb, Real rZorb, Real rXort, Real rYort, Real rZort) : 
		sName(Name), sMesh(MeshName), sOrbiting(IsOrbiting), rBodyDiam(Diameter), rOrbitRadius(OrbitRadius), 
			rYear(Year), rSpin(Spin), degInitialAngle(0.0), rX(rXorb), rY(rYorb), rZ(rZorb)  
{
	qOrbitSlant = Quaternion (Degree (rXorb), Vector3::UNIT_X) * Quaternion (Degree (rYorb), Vector3::UNIT_Y) *
				Quaternion (Degree (rZorb), Vector3::UNIT_Z);
	qOrbitSlant.normalise();

	qSlant = Quaternion (Degree (rXort), Vector3::UNIT_X) * Quaternion (Degree (rYort), Vector3::UNIT_Y) *
			Quaternion (Degree (rZort), Vector3::UNIT_Z);
	qSlant.normalise();
}


//-----------------------------------------------------------------------------------------------------------------
String OrbitalBody::getName()
{
   return sName;
}


//-----------------------------------------------------------------------------------------------------------------
String OrbitalBody::getIsOrbiting()
{
   return sOrbiting;
}


//-----------------------------------------------------------------------------------------------------------------
Vector3 OrbitalBody::getPosition()
{
   return Vector3(rX, rY, rZ);
}


//-----------------------------------------------------------------------------------------------------------------
Quaternion OrbitalBody::getOrientation()
{
   return qOrientationNow;
}


//-----------------------------------------------------------------------------------------------------------------
OrbitalBody::~OrbitalBody()
{
   
}


//-----------------------------------------------------------------------------------------------------------------
void OrbitalBody::getNext(Real iTime, SceneManager* oSceneMgr, Real G)
{
	Quaternion qSpin;				// Quaternion for spin orientation
	Radian degOrbit, degSpin;		// Angles for Orbital tilt and Spin
	Vector3 vCoo;					// Centre of orbit vector
	Real rCooX, rCooY, rCooZ;		// Temp Coordinates 
	Vector3 oVector;				// Temp Vector

	if (sOrbiting == "Sun")
		rCooX = rCooY = rCooZ = 0.0;
	else
	{
		vCoo = oSceneMgr->getSceneNode(sOrbiting)->getPosition();
		rCooX = vCoo.x;
		rCooY = vCoo.y;
		rCooZ = vCoo.z;
	}

	degOrbit = CircleFraction(rYear/G, iTime);
	
	oVector = Vector3 (Math::Cos (degOrbit), 0, Math::Sin (degOrbit));
	oVector = rOrbitRadius/G * oVector;
	oVector = qOrbitSlant * oVector;
  
	rX = rCooX + oVector.x;
	rY = rCooY + oVector.y;
	rZ = rCooZ + oVector.z;

	degSpin = CircleFraction (rYear/G, iTime);
  
	qSpin = Quaternion(degSpin, Vector3::UNIT_Y);
	qOrientationNow = qSlant * qSpin * Quaternion(Degree (-90.0), Vector3::UNIT_X);

}


//-----------------------------------------------------------------------------------------------------------------
Radian OrbitalBody::CircleFraction(Real rYear, Real Time)
{
  Real rTemp;
  
  rTemp = Time / (rYear * (1000.0 / Math::TWO_PI));
  rTemp = rTemp - int (rTemp);
  
  if (rTemp < 0.0)
    rTemp += 1.0;
  
  return Radian (rTemp * Math::TWO_PI);
}




