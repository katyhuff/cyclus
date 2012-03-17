// ToasterFacility.cpp
// Implements the ToasterFacility class

#include "ToasterFacility.h"

#include <list>
#include "Logger.h"
#include "InputXML.h"
#include "DeckStore.h"

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
  n_slices_ = strtol(XMLinput->get_xpath_content(cur, "rate"), NULL, 10);
  toastiness_ = XMLinput->get_xpath_content(cur,"toastiness");
  rate_ = strtod(XMLinput->get_xpath_content(cur, "rate"), NULL);
  incommodity_ = XMLinput->get_xpath_content(cur, "incommodity");
  outcommodity_ = XMLinput->get_xpath_content(cur, "outcommodity");

  // check that toastiness_ is oneof the allowed levels :
  // this gives an example of performing input checking in the module 
  // in case the xml parser is not detailed enough
  set<string> allowed_levels;
  allowed_levels.insert("light");
  allowed_levels.insert("golden");
  allowed_levels.insert("dark");
  allowed_levels.insert("burnt");
  if(allowed_levels.find(toastiness_)==allowed_levels.end()){
    string msg = "The value given for the toastiness parameter, ";
    msg += toastiness_;
    msg += ", is not within the allowed set. Allowed values are: ";
    set<string>::iterator it;
    for (it=allowed_levels.begin(); it != allowed_levels.end(); it++){
      msg += " '";
      msg += (*it);
      msg += "'";
    }
    msg+=".";
    LOG(LEV_ERROR,"Toast")<<msg;
  }
}

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -    
void ToasterFacility::copy(ToasterFacility* src) {
  FacilityModel::copy(src);
  n_slices_=src->n_slices_;
  toastiness_=src->toastiness_;
  rate_=src->rate_;
  incommodity_=src->incommodity_;
  outcommodity_=src->outcommodity_;
}

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -    
void ToasterFacility::copyFreshModel(Model* src) {
  copy(dynamic_cast<ToasterFacility*>(src));
}

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -    
void ToasterFacility::print() {
  FacilityModel::print();
  string msg = "ToasterFacility";
  msg += this->ID();
  msg += " makes delicious ";
  msg += toastiness_;
  msg += " toast.";
  LOG(LEV_DEBUG2,"Toast")<<msg;
};

/* ------------------- */ 


/* --------------------
 * all COMMUNICATOR classes have these members
 * --------------------
 */

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void ToasterFacility::receiveMessage(msg_ptr msg) {
  orders_waiting_.push_front(msg);
}

/* ------------------- */ 


/* --------------------
 * all FACILITYMODEL classes have these members
 * --------------------
 */

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
vector<rsrc_ptr> ToasterFacility::removeResource(msg_ptr order) {
  Transaction trans = order->trans();
  if (trans.commod != outcommodity_) {
    string err_msg = "ToasterFacility can only send '" + outcommodity_ ;
    err_msg += + "' materials.";
    throw CycException(err_msg);
  }

  MatManifest materials;
  try {
    materials = inventory_.popQty(trans.resource->quantity());
  } catch(CycNegQtyException err) {
    LOG(LEV_ERROR, "Toast") << "extraction of " << trans.resource->quantity()
                   << " kg failed. Inventory is only "
                   << inventory_.quantity() << " kg.";
  }

  return MatStore::toRes(materials);

}
    
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void ToasterFacility::addResource(msg_ptr msg, vector<rsrc_ptr> manifest) {
  stocks_.pushAll(MatStore::toMat(manifest));
}

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void ToasterFacility::handleTick(int time) {
  request(incommodity_, storage_capacity_);
  offerToast(TI->time_step_in_minutes_/rate_);
  toast(stored_bread_);
}

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void ToasterFacility::handleTock(int time) {
  sendToast(orders_waiting_);
  cleanUp();
}

/* ------------------- */ 


/* --------------------
 * all MODEL classes have these members
 * --------------------
 */

extern "C" Model* constructToasterFacility() {
  return new ToasterFacility();
}

/* ------------------- */ 

