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
      toastiness_="golden";
      n_slices_=4;
      rate_ = 2;
      incommodity_="bread";
      outcommodity_="toast";
      initToastChem();
    }

    string getInCommod(){ return incommodity_;}
    string getOutCommod(){ return outcommodity_;}

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
      
    TestMarket* in_market_; 
    TestMarket* out_market_;
    
    mat_rsrc_ptr  bread_;

    virtual void SetUp(){
      src_facility = new FakeToasterFacility();
      src_facility->setParent(new TestInst());
      new_facility = new FakeToasterFacility();
      // for facilities that trade commodities, create appropriate markets here
      out_market_ = new TestMarket(src_facility->getOutCommod());
      in_market_ = new TestMarket(src_facility->getInCommod()); 
      CompMap comp;
      comp.insert(make_pair(20500,1000));
      bread_ = mat_rsrc_ptr(new Material(comp)); 
    };

    virtual void TearDown() {
      delete src_facility;
      delete new_facility;
      // for facilities that trade commodities, delete appropriate markets here
      delete in_market_;
      delete out_market_;
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
TEST_F(ToasterFacilityTest, Toast) {

  msg_ptr bread_msg_ = msg_ptr(new Message(new_facility, src_facility));
  bread_msg_->setResource(bread_);
  bread_msg_->setCommod("bread");

  vector<rsrc_ptr> manifest, returned; 
  manifest.push_back(rsrc_ptr(bread_));
  src_facility->addResource(bread_msg_, manifest);

  double original_mass = (bread_->isoVector()).eltMass(20);
  src_facility->handleTick(1);
  bread_msg_->setCommod("toast");
  returned = src_facility->removeResource(bread_msg_);
  mat_rsrc_ptr toasted_bread = boost::dynamic_pointer_cast<Material>(returned.front());

  ASSERT_LT((toasted_bread->isoVector()).eltMass(20),original_mass);
}

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
INSTANTIATE_TEST_CASE_P(ToasterFac, FacilityModelTests, Values(&ToasterFacilityConstructor));
INSTANTIATE_TEST_CASE_P(ToasterFac, ModelTests, Values(&ToasterFacilityModelConstructor));

