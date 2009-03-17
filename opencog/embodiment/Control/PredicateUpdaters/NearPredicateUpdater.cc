/** 
 * NearPredicateUpdater.cc
 * 
 * Author: Ari Heljakka, Welter Luigi, Samir Araujo
 * Copyright(c), 2008
 */

#include <opencog/atomspace/SimpleTruthValue.h>

#include "NearPredicateUpdater.h"
#include "AtomSpaceUtil.h"

using namespace OperationalPetController;
using namespace opencog;

NearPredicateUpdater::NearPredicateUpdater(SpaceServer &_spaceServer) : 
    BasicPredicateUpdater(_spaceServer.getAtomSpace()), spaceServer(_spaceServer) {}

NearPredicateUpdater::~NearPredicateUpdater(){
}

void NearPredicateUpdater::update(Handle object, Handle pet, unsigned long timestamp ) {
  // TODO: pet argument is not used here because Near/Next predicates are being 
  // calculated between the changed object and all existing objects. Change this 
  // if we want only the predicates between the pet and other objects.
 
  std::string objectName = atomSpace.getName(object);

  // there is no map, no update is possible
  Handle spaceMapHandle = spaceServer.getLatestMapHandle();
  if (spaceMapHandle == Handle::UNDEFINED) {
    return;
  }

  const SpaceServer::SpaceMap& spaceMap = spaceServer.getLatestMap();
  if (!spaceMap.containsObject(objectName)) {
    MAIN_LOGGER.log(LADSUtil::Logger::DEBUG, "NearPredicateUpdater::update: Did  not find object %s in the map", objectName.c_str());

    vector<std::string> entities; 
    spaceMap.findAllEntities(back_inserter(entities));

    foreach(std::string entity, entities) {
            
      // object not in latest map. If there are previous near predicates
      // define as 'true' then we should set them false 
      Handle entityHandle = getHandle(entity);
      if(entityHandle != Handle::UNDEFINED ) {

	// set false in both directions: near A -> B and near B -> A
	// since to evaluate a near predicate true only the objects
	// inside the latest map are considered (this avoid
	// inconsistency in AtomSpace).
	
	if ( AtomSpaceUtil::isPredicateTrue(atomSpace, "near", object, entityHandle) ) {
	  AtomSpaceUtil::addPropertyPredicate(atomSpace, "near", object, entityHandle,
					      SimpleTruthValue(0.0, 1.0), true);
	  AtomSpaceUtil::addPropertyPredicate(atomSpace, "near", entityHandle, object,
					      SimpleTruthValue(0.0, 1.0), true);

	} // if

	if ( AtomSpaceUtil::isPredicateTrue(atomSpace, "next", object, entityHandle) ) {
	  AtomSpaceUtil::addPropertyPredicate(atomSpace, "next", object, entityHandle,
					      SimpleTruthValue(0.0, 1.0));
	  AtomSpaceUtil::addPropertyPredicate(atomSpace, "next", entityHandle, object,
					      SimpleTruthValue(0.0, 1.0));
	} // if

      } // if
    } // foreach
    return;
  }
    
    
//  const Spatial::Object& petObject = spaceMap.getObject( objectName );
  const Spatial::EntityPtr& agentEntity = spaceMap.getEntity( objectName );

  vector<std::string> entities; 
  spaceMap.findAllEntities(back_inserter(entities));
    
  foreach(std::string entity, entities) {
    if (entity != objectName) {
      //const Spatial::Object& entityObject = spaceMap.getObject( entity );
      const Spatial::EntityPtr& objectEntity = spaceMap.getEntity( entity );
      double dist = agentEntity->distanceTo( objectEntity );//Spatial::getDistanceBetweenObjects( petObject, entityObject );
	    
      // distance to near 3,125%
      double nearDistance = ( spaceMap.xMax( ) - spaceMap.xMin( ) ) * 0.003125;
      // distance to next 10,0%
      double nextDistance = ( spaceMap.xMax( ) - spaceMap.xMin( ) ) * 0.1;

      double nearDistStrength = ( dist < nearDistance ) ? 1 : 0;
      double nextDistStrength = 0;
      //if ( dist < nearDistance ) {
	//nearDistStrength = std::max( 0.0, 1.0 - dist/nearDistance );
      //} // if

      if ( dist < nextDistance ) {
        nextDistStrength = std::max( 0.0, 1.0 - dist/nextDistance );
      } // if

      MAIN_LOGGER.log(LADSUtil::Logger::DEBUG, "NearPredicateUpdater::update: (%s X %s) | dist = %f => nearDistance = %f | nearDistStrength = %f | nextDistance = %f | nextDistStregth = %f", objectName.c_str(), entity.c_str(), dist, nearDistance, nearDistStrength, nextDistance, nextDistStrength );

      Handle entityHandle = getHandle(entity);
      if(entityHandle != Handle::UNDEFINED){                
	// no timestamp
	AtomSpaceUtil::addPropertyPredicate(atomSpace, "near", object, entityHandle,
					    SimpleTruthValue(nearDistStrength, 1.0), true);
	AtomSpaceUtil::addPropertyPredicate(atomSpace, "near", entityHandle, object,
					    SimpleTruthValue(nearDistStrength, 1.0), true);

	AtomSpaceUtil::addPropertyPredicate(atomSpace, "next", object, entityHandle,
					    SimpleTruthValue(nextDistStrength, 1.0), true);
	AtomSpaceUtil::addPropertyPredicate(atomSpace, "next", entityHandle, object,
					    SimpleTruthValue(nextDistStrength, 1.0), true);
      } // if
    } // if
  } // foreach

}

