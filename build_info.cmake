# Writes the build timestamp to OUTPUT_FILE. The build_info target in
# CMakeLists.txt runs this on every build, so the run-start log identifies the
# flashed image.
string(TIMESTAMP BUILD_TIMESTAMP "%Y-%m-%d %H:%M:%S")
file(WRITE "${OUTPUT_FILE}" "#define BUILD_TIMESTAMP \"${BUILD_TIMESTAMP}\"\n")
