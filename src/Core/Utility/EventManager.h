// EventManager.h
#if !defined(_EVENTMANAGER_H)
#define _EVENTMANAGER_H

#include "CycException.h"

#include <list>
#include <string>
#include <map>
#include <boost/intrusive_ptr.hpp>
#include <boost/any.hpp>

// TODO: Move away from singleton pattern (that is why we kept EventManager constructor public)
#define EM EventManager::Instance()

class Event;
class EventManager;
class Model;

typedef boost::intrusive_ptr<Event> event_ptr;
typedef std::map<std::string, boost::any> ValMap;
typedef std::list<event_ptr> EventList;

/// default number of events to collect before flushing to backends.
static unsigned int const kDefaultDumpCount = 10000;

/*!
Indicates that a pre-recorded event of the same name (e.g. the same
table) has one or more inconsistent field-value type pairs.
*/
class CycInvalidSchemaErr: public CycException {
    public: CycInvalidSchemaErr(std::string msg) : CycException(msg) {};
};

/*!
An abstract base class for listeners (e.g. output databases) that want
to receive data generated by the simulation.
*/
class EventBackend {
  public:
    /// Used to pass a list of new/collected events
    virtual void notify(EventList events) = 0;

    /*!
    Used to uniquely identify a backend - particularly if there are more
    than one in a simulation.
    */
    virtual std::string name() = 0;

    /*!
    Used to notify the backend that no more events will be sent (i.e.
    the simulatoin is over).
    */
    virtual void close() = 0;
};

/*!
Collects and manages output data generation for the cyclus core and
agents during a simulation.

Example usage:

@code

EventManager* manager = new EventManager();
CustomBackend* myback = new CustomBackend();
manager->registerBackend(myback);
...
...
manager->newEvent(creator, "CapacityFactor");
       ->addVal("Name", aname)
       ->addVal("Capacity", cap)
       ->record();
...
...
manager->close();

@endcode
*/
class EventManager {
  friend class Event;

  private:
    bool isValidSchema(event_ptr ev);
    void notifyBackends();
    void addEvent(event_ptr ev);

    std::list<event_ptr> events_;
    std::map<std::string, event_ptr> schemas_;
    std::list<EventBackend*> backs_;
    unsigned int dump_count_;

    /// A pointer to singleton EventManager.
    static EventManager* instance_;

  public:

    /// create a new event manager with default dump frequency.
    EventManager();
    // TODO: Move away from singleton pattern (that is why we kept the constructor public)

    /// Return the dump frequency, # events buffered between flushes to backends.
    unsigned int dump_count();

    /*!
    set the EventManager to flush its collected events to registered backends
    every [count] events.
    
    @param count # events to buffer before flushing to backends.
    */
    void set_dump_count(unsigned int count);

    /*!
    Creates a new event for creator with title.  It uses uniquely
    identifiable information from creator (e.g. ID) to create a unique
    namespaces for related events.
    
    @param creator the agent that created this event @param title a
    label to distinguish event sets from other sets from the same
    creator.
    */
    event_ptr newEvent(Model* creator, std::string title);

    /*!
    Registers b to receive event notifications for all events collected
    by the EventManager and to receive a close notification when there
    are no more events.
    
    @param b backend to receive events
    */
    void registerBackend(EventBackend* b);

    /// Flushes all events and closes all registered backends.
    void close();

    /// Returns a pointer to the global singleton EventManager
    static EventManager* Instance();

};

/*!
this allows files to use events without having to explicitly include
both EventManager.h and Event.h, while avoiding a circular include
dependency.
*/
#include "Event.h"

#endif
