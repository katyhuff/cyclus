// ToasterFacility.cpp
// Implements the ToasterFacility class

#include "ToasterFacility.h"

#include "Logger.h"
#include "CycException.h"
#include "InputXML.h"

using namespace std;

/* --------------------
 * all MODEL classes have these members
 * --------------------
 */

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
ToasterFacility::ToasterFacility() {};

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
ToasterFacility::~ToasterFacility() {};

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -    
void ToasterFacility::init(xmlNodePtr cur) {
  FacilityModel::init(cur);
  /// move XML pointer to current model
  cur = XMLinput->get_xpath_element(cur,"model/ToasterFacility");
  /// initialize any ToasterFacility-specific datamembers here
}

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -    
void ToasterFacility::copy(ToasterFacility* src) {
  FacilityModel::copy(src);
}

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -    
void ToasterFacility::copyFreshModel(Model* src) {
  copy(dynamic_cast<ToasterFacility*>(src));
}

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -    
void ToasterFacility::print() {
  FacilityModel::print();
};

/* ------------------- */ 


/* --------------------
 * all COMMUNICATOR classes have these members
 * --------------------
 */

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void ToasterFacility::receiveMessage(msg_ptr msg) {}

/* ------------------- */ 


/* --------------------
 * all FACILITYMODEL classes have these members
 * --------------------
 */

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
vector<rsrc_ptr> ToasterFacility::removeResource(msg_ptr order) {}
    
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void ToasterFacility::addResource(msg_ptr msg, vector<rsrc_ptr> manifest){}

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void ToasterFacility::handleTick(int time){}

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void ToasterFacility::handleTock(int time){}

/* ------------------- */ 


/* --------------------
 * all MODEL classes have these members
 * --------------------
 */

extern "C" Model* constructToasterFacility() {
  return new ToasterFacility();
}

/* ------------------- */ 

