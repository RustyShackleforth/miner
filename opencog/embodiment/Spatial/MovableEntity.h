#ifndef MOVABLEENTITY_H
#define MOVABLEENTITY_H

#include "Entity.h"

namespace Spatial {

  class MovableEntity;
  typedef boost::shared_ptr<MovableEntity> MovableEntityPtr;

  class MovableEntity : public Entity {
  public:    	

    /**
     * Simple Copy constructor
     * @param object
     */
    inline MovableEntity( const MovableEntity& object ) : Entity( object.id, object.name, object.position, object.dimension, object.orientation, object.expansionRadius ), velocity( object.getVelocity() ) { };
    
    /**
     * Custom constructor
     * @param id
     * @param name
     * @param position
     * @param dimension
     * @param orientation
     */
    inline MovableEntity( long id, const std::string& name, const Math::Vector3& position, const Math::Dimension3& dimension, const Math::Quaternion& orientation, double radius = 0.0 ) : Entity( id, name, position, dimension, orientation, radius ), velocity( 0 ) { }

    /**
     * Update it's internal structures after a move or rotation action
     */
    void update( void ) {
      if ( needUpdate ) {
	boundingBox.update( );
      } // if
    }

    /**
     * Rotate this entity by a given quaternion
     * @param rotation
     */
    void rotate( const Math::Quaternion& rotation );

    /**
     * Defines a new position for this entity
     * @param position
     */
    void setPosition(const Math::Vector3& position);

    /**
     * Defines a new orientation for this entity
     * @param orientation
     */
    void setOrientation(const Math::Quaternion& orientation);
	
    /**
     * Getter to the entity velocity
     * @return
     */
    inline double getVelocity( void ) const {
      return this->velocity;
    }

    /**
     * Set the velocity of the entity
     * @param velocity
     */
    inline void setVelocity(double velocity) {
      this->velocity = velocity;
    }

    virtual inline ENTITY_TYPE getType( void ) const {
      return Entity::MOVABLE;
    };

    virtual inline EntityPtr clone( void ) const {
      MovableEntityPtr clone( new MovableEntity( *this ) );
      clone->setVelocity( getVelocity( ) );
      clone->properties = this->properties;
      return clone;
    }

  private:
    double velocity;
    bool needUpdate;

  }; // MovableEntity

}; // Spatial

#endif // MOVABLEENTITY_H
