# Set the paths
$sourceDir = "."
$buildDir = "build"
$generator = "Ninja"

# Run CMake to configure the project
cmake -S $sourceDir -B $buildDir -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -G $generator
if ($?) {
    # Build and install the project
    cmake --build $buildDir --target install
    if ($?) {
        # Determine the platform
        $platform = [System.Environment]::OSVersion.Platform

        # Run the appropriate executable
        if ($platform -eq 'Win32NT') {
            # If on Windows
            & "./build/bin/engine.exe"
        } else {
            # On Unix-like systems (Linux, macOS, etc.)
            & "./build/bin/engine"
        }
    }
}
