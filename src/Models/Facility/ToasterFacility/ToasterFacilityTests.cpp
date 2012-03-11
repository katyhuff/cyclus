// ToasterFacilityTests.cpp
#include <gtest/gtest.h>

#include "ToasterFacility.h"
#include "CycException.h"
#include "Message.h"
#include "FacilityModelTests.h"
#include "ModelTests.h"

#include <string>
#include <queue>

using namespace std;

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
class FakeToasterFacility : public ToasterFacility {
  public:
    FakeToasterFacility() : ToasterFacility() {
    }

    virtual ~FakeToasterFacility() {
    }
};

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
Model* ToasterFacilityModelConstructor(){
  return dynamic_cast<Model*>(new FakeToasterFacility());
}

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
FacilityModel* ToasterFacilityConstructor(){
  return dynamic_cast<FacilityModel*>(new FakeToasterFacility());
}

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
class ToasterFacilityTest : public ::testing::Test {
  protected:
    FakeToasterFacility* src_facility;
    FakeToasterFacility* new_facility; 

    virtual void SetUp(){
      src_facility = new FakeToasterFacility();
      src_facility->setParent(new TestInst());
      new_facility = new FakeToasterFacility();
      // for facilities that trade commodities, create appropriate markets here
    };

    virtual void TearDown() {
      delete src_facility;
      delete new_facility;
      // for facilities that trade commodities, delete appropriate markets here
    }
};


//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
TEST_F(ToasterFacilityTest, InitialState) {
  // Test things about the initial state of the facility here
}

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
TEST_F(ToasterFacilityTest, CopyFreshModel) {
  new_facility->copyFreshModel(dynamic_cast<Model*>(src_facility)); // deep copy
  EXPECT_NO_THROW(dynamic_cast<ToasterFacility*>(new_facility)); // still a stub facility
  EXPECT_NO_THROW(dynamic_cast<FakeToasterFacility*>(new_facility)); // still a fake stub facility
  // Test that ToasterFacility specific parameters are initialized in the deep copy method here
}

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
TEST_F(ToasterFacilityTest, Print) {
  EXPECT_NO_THROW(src_facility->print());
  // Test ToasterFacility specific aspects of the print method here
}


//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
TEST_F(ToasterFacilityTest, ReceiveMessage) {
  msg_ptr msg;
  // Test ToasterFacility specific behaviors of the receiveMessage function here
}

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
TEST_F(ToasterFacilityTest, Tick) {
  int time = 1;
  EXPECT_NO_THROW(src_facility->handleTick(time));
  // Test ToasterFacility specific behaviors of the handleTick function here
}

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
TEST_F(ToasterFacilityTest, Tock) {
  int time = 1;
  EXPECT_NO_THROW(src_facility->handleTick(time));
  // Test ToasterFacility specific behaviors of the handleTock function here
}

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
INSTANTIATE_TEST_CASE_P(ToasterFac, FacilityModelTests, Values(&ToasterFacilityConstructor));
INSTANTIATE_TEST_CASE_P(ToasterFac, ModelTests, Values(&ToasterFacilityModelConstructor));

