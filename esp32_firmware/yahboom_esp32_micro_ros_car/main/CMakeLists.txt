idf_component_register(
    SRCS
        "main.c"
        "encoder.c"
        "kinematics.c"
        "motor.c"
        "pid.c"
    INCLUDE_DIRS "."
    REQUIRES driver freertos
)
