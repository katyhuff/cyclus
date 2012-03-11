# Build the cyclus executable from the CYCLUS_SRC source files
ADD_LIBRARY( ToasterFacility        ToasterFacility.cpp )
SET(CYCLUS_LIBRARIES ${CYCLUS_LIBRARIES} ToasterFacility )
SET(FacilityTestSource ${FacilityTestSource} 
  ${CMAKE_CURRENT_SOURCE_DIR}/ToasterFacilityTests.cpp 
  ${CMAKE_CURRENT_SOURCE_DIR}/ToasterFacility.cpp 
  PARENT_SCOPE)
CONFIGURE_FILE(
${CYCLUS_SOURCE_DIR}/Models/Facility/ToasterFacility/ToasterFacility.rng
${CMAKE_CURRENT_BINARY_DIR}/ToasterFacility.rng
COPYONLY
)
