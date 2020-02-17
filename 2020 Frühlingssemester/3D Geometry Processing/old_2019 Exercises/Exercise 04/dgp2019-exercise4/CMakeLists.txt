include (plugin)

openflipper_plugin(DIRS ./eigen3/
        DEPS OpenVolumeMesh
        TYPES TRIANGLEMESH TETRAHEDRALMESH)