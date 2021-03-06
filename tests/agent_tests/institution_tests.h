#ifndef CYCLUS_TESTS_AGENT_TESTS_INSTITUTION_TESTS_H_
#define CYCLUS_TESTS_AGENT_TESTS_INSTITUTION_TESTS_H_

#include <gtest/gtest.h>

#include "agent_tests.h"
#include "institution.h"
#include "suffix.h"
#include "test_context.h"
#include "test_agents/test_facility.h"
#include "test_agents/test_inst.h"
#include "test_agents/test_region.h"

#if GTEST_HAS_PARAM_TEST

using ::testing::TestWithParam;
using ::testing::Values;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// Inside the test body, fixture constructor, SetUp(), and TearDown() we
// can refer to the test parameter by GetParam().  In this case, the test
// parameter is a pointer to a concrete Institution instance

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
class InstitutionTests : public TestWithParam<AgentConstructor*> {
 public:
  virtual void SetUp() {
    //institution_ = new TestInst(tc_.get());
    institution_ = dynamic_cast<cyclus::Institution*>((*GetParam())(tc_.get()));
    test_facility_ = new TestFacility(tc_.get());
    test_region_ = new TestRegion(tc_.get());
    institution_->Build(test_region_);
  }
  virtual void TearDown() {
    delete test_facility_;
    delete institution_;
    delete test_region_;
  }

 protected:
  cyclus::Institution* institution_;
  TestFacility* test_facility_;
  TestRegion* test_region_;
  cyclus::TestContext tc_;
};

#else

// Google Test may not support value-parameterized tests with some
// compilers. If we use conditional compilation to compile out all
// code referring to the gtest_main library, MSVC linker will not link
// that library at all and consequently complain about missing entry
// point defined in that library (fatal error LNK1561: entry point
// must be defined). This dummy test keeps gtest_main linked in.
TEST(DummyTest, ValueParameterizedTestsAreNotSupportedOnThisPlatform) {}

#endif  // GTEST_HAS_PARAM_TEST

#endif  // CYCLUS_TESTS_AGENT_TESTS_INSTITUTION_TESTS_H_
