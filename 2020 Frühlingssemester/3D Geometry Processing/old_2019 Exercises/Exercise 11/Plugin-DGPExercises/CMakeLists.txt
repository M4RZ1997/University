include (plugin)

openflipper_plugin(DIRS ./eigen3/ PoissonReconstruction
        DEPS OpenVolumeMesh
        TYPES TRIANGLEMESH TETRAHEDRALMESH)