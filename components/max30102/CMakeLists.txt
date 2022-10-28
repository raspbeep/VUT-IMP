set(component_srcs "algorithm.c" "algorithm.c" "i2c_api.c" "i2c_api.h" "max30102_api.c" "max30102_api.h")

idf_component_register(SRCS "${component_srcs}"
                       PRIV_REQUIRES driver
                       INCLUDE_DIRS ".")
