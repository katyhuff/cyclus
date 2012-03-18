// ToasterFacility.h
#if !defined(_TOASTERFACILITY_H)
#define _TOASTERFACILITY_H

#include <deque>
#include <map>
#include <vector>
#include "Logger.h"
#include "FacilityModel.h"
#include "DeckStore.h"

/**
  @class ToasterFacility
  
  @brief This FacilityModel is intended to toast material objects
  
  The ToasterFacility class inherits from the FacilityModel class and is 
  dynamically loaded by the Model class when requested.

  @section intro Introduction
  A toaster is a common household implment which adds some carbon to our 
  slices of bread. It usually takes about a minute to heat a slice of bread 
  until it is golden brown. 

  @section modelparams Model Parameters
  To fully define a Toaster prototype, the following parameters must be 
  defined : 
  - int nSlices :  How many slices it can toast at once [ integer number of slices ]
  - string toastiness : How toasted they become [ light, golden, dark, burnt ]
  - double rate : How long it takes to toast a set of slices [ minutes ]
  - string incommodity : The commodity market in which slices of bread are traded 
  - string outcommodity : The commodity market in which toasted bread is traded

  @section optionalparams Optional Parameters
  This model has no optional parameters.

  @section detailed Detailed Behavior
  The ToasterFacility starts operation immediately. 

  @subsection tick On the tick :
  The ToasterFacility immediately offers any toast that exists in the inventory from 
  previous months and begins to request the incommodity. It requests as much sliced 
  bread as it can toast within a timestep. That is, it requests 86400 slices if 
  the timestep is 30 days long, the rate is 2 minutes per set of slices, and  
  n_slices = 4. 
   
  @subsection receive Receiving a Message :
  If the request is matched with an offer from another facility, 
  the ToasterFacility executes that order by adding that quantity to its stocks. 
 
  @subsection tock On the tock :
  On the tock, the ToasterFacility alters the isotopic vectors of each slice of 
  bread in the stocks (up to the monthly capacity) to include more carbon and less
  oxygen (the magnitude of the change is defined by the toastiness parameter). Each 
  (now toasted) slice is then placed in the inventory. 
  
*/
class ToasterFacility : public FacilityModel  {
/* --------------------
 * all MODEL classes have these members
 * --------------------
 */
 public:
  /**
   * @brief Default constructor for ToasterFacility Class
   */
  ToasterFacility();

  /**
   * @brief every model should be destructable
   */
  virtual ~ToasterFacility();
    
  /**
   * @brief every model needs a method to initialize from XML
   *
   * @param cur is the pointer to the model's xml node 
   */
  virtual void init(xmlNodePtr cur);
  
  /**
   * @brief every model needs a method to copy one object to another
   *
   * @param src is the ToasterToaster to copy
   */
  virtual void copy(ToasterFacility* src) ;

  /**
   * @brief This drills down the dependency tree to initialize all relevant 
   * parameters/containers.
   *
   * Note that this function must be defined only in the specific model in 
   * question and not in any inherited models preceding it.
   *
   * @param src the pointer to the original (initialized ?) model to be copied
   */
  virtual void copyFreshModel(Model* src);

  /**
   * @brief every model should be able to print a verbose description
   */
   virtual void print();

  /**
   * @brief Transacted resources are extracted through this method
   * 
   * @param order the msg/order for which resource(s) are to be prepared
   * @return list of resources to be sent for this order
   *
   */ 
  virtual std::vector<rsrc_ptr> removeResource(msg_ptr order);

  /**
   * @brief Transacted resources are received through this method
   *
   * @param trans the transaction to which these resource objects belong
   * @param manifest is the set of resources being received
   */ 
  virtual void addResource(msg_ptr msg,
                              std::vector<rsrc_ptr> manifest);

/* ------------------- */ 


/* --------------------
 * all COMMUNICATOR classes have these members
 * --------------------
 */
 public:
  /**
   * @brief The ToasterFacility should ignore incoming messages
   */
  virtual void receiveMessage(msg_ptr msg);

/* -------------------- */


/* --------------------
 * all FACILITYMODEL classes have these members
 * --------------------
 */
 public:
  /**
   * @brief The handleTick function specific to the ToasterFacility.
   *
   * @param time the time of the tick
   */
  virtual void handleTick(int time);

  /**
   * @brief The handleTick function specific to the ToasterFacility.
   *
   * @param time the time of the tock
   */
  virtual void handleTock(int time);

/* ------------------- */ 


/* --------------------
 * _THIS_ FACILITYMODEL class has these members
 * --------------------
 */

 private:
  /**
   * The number of slices the toaster can handle at one time
   */
  int n_slices_;

  /**
   * The speed (set of slices per minute) with which the toaster toasts
   */
  double rate_;

  /**
   * The name referring to the toastiness of the toast. This can be any of the 
   * allowed_levels_
   */
  std::string toastiness_;

  /**
   * The name of the commodity market for the incoming commodity.
   */
  std::string incommodity_;

  /**
   * The name of the commodity market for the outgoing commodity.
   */
  std::string outcommodity_;

  /**
   * Not yet toasted bread in storage, waiting to be toasted
   */
  DeckStore stocks_;

  /**
   * Toasted bread, waiting to be offered and sent away
   */
  DeckStore inventory_;

  /**
   * Orders for toasted bread from other agents, waiting to be filled
   */
  std::deque<msg_ptr> orders_waiting_;

  std::map<std::string, double> allowed_levels_;

  std::map<int, double> toast_bread_elt_ratio_;

  std::map<int, double> comp_change_;

  void makeRequests();

  void makeOffers();

  std::vector<mat_rsrc_ptr> toast(DeckStore to_toast);

  mat_rsrc_ptr toast(mat_rsrc_ptr resource);

  void sendToast(std::deque<msg_ptr> orders);

  void cleanUp();

  void initToastChem();

/* ------------------- */ 

};

#endif

