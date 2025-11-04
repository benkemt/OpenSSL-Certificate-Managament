cmake_minimum_required(VERSION 3.15)
project(CertManager VERSION 1.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find OpenSSL
find_package(OpenSSL REQUIRED)

# Create executable
add_executable(CertManager
    CertManager/main.cpp
    CertManager/CertificateManager.cpp
    CertManager/CertificateManager.h
)

# Include directories
target_include_directories(CertManager PRIVATE
    ${OPENSSL_INCLUDE_DIR}
)

# Link libraries
target_link_libraries(CertManager PRIVATE
    OpenSSL::SSL
    OpenSSL::Crypto
)

# Platform-specific settings
if(WIN32)
    target_link_libraries(CertManager PRIVATE ws2_32 crypt32)
endif()

# Set output directory
set_target_properties(CertManager PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

# Installation rules (optional)
install(TARGETS CertManager
    RUNTIME DESTINATION bin
)

# Print configuration info
message(STATUS "OpenSSL Version: ${OPENSSL_VERSION}")
message(STATUS "OpenSSL Include Dir: ${OPENSSL_INCLUDE_DIR}")
message(STATUS "OpenSSL Libraries: ${OPENSSL_LIBRARIES}")
