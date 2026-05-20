Import("env")

# Fix for PlatformIO native platform:
# Move C++ standard flags (e.g. -std=gnu++17) from CCFLAGS to CXXFLAGS so
# they are not passed to C file compilations (e.g. Unity framework's unity.c).
# Without this, PlatformIO's SCons builder throws a TypeError when processing
# C source files that have a C++ std flag in CCFLAGS.
ccflags = env.get("CCFLAGS", [])
cxx_std_flags = [str(f) for f in ccflags if "-std=" in str(f) and "++" in str(f)]

if cxx_std_flags:
    env.Replace(CCFLAGS=[f for f in ccflags if str(f) not in cxx_std_flags])
    env.Append(CXXFLAGS=cxx_std_flags)
