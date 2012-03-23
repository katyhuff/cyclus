// ToasterFacility.cpp
// Implements the ToasterFacility class

#include "ToasterFacility.h"

#include <list>
#include "Logger.h"
#include "InputXML.h"
#include "Timer.h"
#include "GenericResource.h"

using namespace std;

// Database table for toast
table_ptr ToasterFacility::toaster_table = new Table("ToastedResources"); 

/* --------------------
 * all MODEL classes have these members
 * --------------------
 */

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
ToasterFacility::ToasterFacility() {
  allowed_levels_.insert(make_pair("light",.50));
  allowed_levels_.insert(make_pair("golden",.75));
  allowed_levels_.insert(make_pair("dark",1));
  allowed_levels_.insert(make_pair("burnt",1.25));
  toast_bread_elt_ratio_.insert(make_pair(20,.91)); // Ca
  toast_bread_elt_ratio_.insert(make_pair(26,.94)); // Fe
  toast_bread_elt_ratio_.insert(make_pair(19,.90)); // K
  toast_bread_elt_ratio_.insert(make_pair(12,.88)); // Mg
  toast_bread_elt_ratio_.insert(make_pair(11,.91)); // Na
  inventory_.makeUnlimited();
  stocks_.makeUnlimited();
};

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
ToasterFacility::~ToasterFacility() {  
};

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -    
void ToasterFacility::init(xmlNodePtr cur) {
  FacilityModel::init(cur);

  /// move XML pointer to current model
  cur = XMLinput->get_xpath_element(cur,"model/ToasterFacility");

  /// initialize any ToasterFacility-specific datamembers here
  n_slices_ = strtol(XMLinput->get_xpath_content(cur, "nSlices"), NULL, 10);
  toastiness_ = XMLinput->get_xpath_content(cur,"toastiness");
  rate_ = strtod(XMLinput->get_xpath_content(cur, "rate"), NULL);
  incommodity_ = XMLinput->get_xpath_content(cur, "incommodity");
  outcommodity_ = XMLinput->get_xpath_content(cur, "outcommodity");

  // check that toastiness_ is oneof the allowed levels :
  // this gives an example of performing input checking in the module 
  // in case the xml parser is not detailed enough
  if(allowed_levels_.find(toastiness_)==allowed_levels_.end()){
    string msg = "The value given for the toastiness parameter, ";
    msg += toastiness_;
    msg += ", is not within the allowed set. Allowed values are: ";
    map<string,double>::iterator it;
    for (it=allowed_levels_.begin(); it != allowed_levels_.end(); it++){
      msg += " '";
      msg += (*it).first;
      msg += "'";
    }
    msg+=".";
    LOG(LEV_ERROR,"Toast")<<msg;
  }

  // initialize the toastiness dependent chemistry
  initToastChem();
  
  // we haven't toasted anything yet
  current_toast_id_ = -1;
}

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -    
void ToasterFacility::copy(ToasterFacility* src) {
  FacilityModel::copy(src);
  n_slices_=src->n_slices_;
  toastiness_=src->toastiness_;
  rate_=src->rate_;
  incommodity_=src->incommodity_;
  outcommodity_=src->outcommodity_;
  allowed_levels_=src->allowed_levels_;
  toast_bread_elt_ratio_=src->toast_bread_elt_ratio_;
  inventory_.makeUnlimited(); 
  stocks_.makeUnlimited();
  current_toast_id_ = -1;
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

  Manifest materials;
  materials = inventory_.popNum(1);

  return materials;

}
    
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void ToasterFacility::addResource(msg_ptr msg, vector<rsrc_ptr> manifest) {
  stocks_.pushAll(manifest);
}

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void ToasterFacility::handleTick(int time) {
  LOG(LEV_INFO3, "TstFac ") << facName() << " is ticking {";
  makeRequests();
  makeOffers();
  inventory_.pushAll(toast(stocks_)); 

  // log the fact that we just made toast
  current_toast_id_ = TI->time();
  addToTable();
 
  LOG(LEV_INFO3, "SrcFac") << "}";
}

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void ToasterFacility::handleTock(int time) {
  sendToast(orders_waiting_);
  cleanUp();
}

/* ------------------- */ 

/* --------------------
 * this ToasterFacility has these members
 * --------------------
 */

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void ToasterFacility::makeRequests() { 
}

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void ToasterFacility::makeOffers() { 

}

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void ToasterFacility::initToastChem() {
  double lev = allowed_levels_[toastiness_];
  map<int, double>::iterator ratio;
  for (ratio=toast_bread_elt_ratio_.begin();ratio!=toast_bread_elt_ratio_.end(); ratio++){
    comp_change_.insert( make_pair((*ratio).first, lev*(*ratio).second ));
  }
}

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
vector<rsrc_ptr> ToasterFacility::toast(ResourceBuff to_toast) { 
  rsrc_ptr slice;
  vector<rsrc_ptr> toRet;
  while (to_toast.count() > 0) {
    slice = to_toast.popOne();
    toRet.push_back(toast(slice));
  }
  return toRet;
}

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
rsrc_ptr ToasterFacility::toast(rsrc_ptr resource){
  
  LOG(LEV_DEBUG4,"Toast")<<"Toaster " << this->ID() << " made some toast!";

  rsrc_ptr toRet;

  if (resource->type()==MATERIAL_RES){
    IsoVector vec = boost::dynamic_pointer_cast<Material>(resource)->isoVector();
    map<int, double> comp = vec.comp();
    map<int, double>::iterator it;
    int iso, elt;

    for (it = comp.begin(); it != comp.end(); it++){
      iso = (*it).first;
      elt = IsoVector::getAtomicNum(iso);
      if (comp_change_.find(elt) != comp_change_.end()){
        comp[iso] = comp[iso]*comp_change_[elt];
      }
    }
    toRet = rsrc_ptr(new Material(IsoVector(comp)));
  } else {
    toRet = rsrc_ptr(resource);
  }
  
  // return our toast
  return  toRet;
}

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void ToasterFacility::sendToast(std::deque<msg_ptr> orders) { 
}

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void ToasterFacility::cleanUp() { 
}

/* --------------------
 * all MODEL classes have these members
 * --------------------
 */

extern "C" Model* constructToasterFacility() {
  return new ToasterFacility();
}

/* ------------------- */ 


//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void ToasterFacility::define_table() {
  // declare the table columns
  column toaster_id("ID","INTEGER");
  column time("Time","INTEGER");
  column toasted_rsrc_id("ToastedRsrcID","INTEGER");  
  // declare the table's primary key
  primary_key pk;
  pk.push_back("ID"), pk.push_back("ToastedRsrcID");
  toaster_table->setPrimaryKey(pk);
  // add columns to the table
  toaster_table->addColumn(toaster_id);
  toaster_table->addColumn(time);
  toaster_table->addColumn(toasted_rsrc_id);
  // we've now defined the table
  toaster_table->tableDefined();
}

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void ToasterFacility::addToTable(){
  // if we haven't logged some toast yet, define the table
  if ( !toaster_table->defined() ) {
    ToasterFacility::define_table();
  }
  // make a row
  // declare data
  data an_id( this->ID() ), a_time( TI->time() ), 
    a_rsrc_id( this->current_toast_id_ );
  // declare entries
  entry id("ID",an_id), time("Time",a_time), rid("ToastedRsrcID",a_rsrc_id);
  // declare row
  row aRow;
  aRow.push_back(id), aRow.push_back(time), aRow.push_back(rid);
  // add the row
  toaster_table->addRow(aRow);
}
