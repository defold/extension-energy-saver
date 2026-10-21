#define LIB_NAME "EnergySaver"
#define MODULE_NAME "energysaver"

#include <dmsdk/sdk.h>
#include <dmsdk/dlib/profile.h>

#if defined(DM_DEBUG)
DM_PROPERTY_F64(rmtp_CpuEnergyUsage, 0.0, PROFILE_PROPERTY_NONE, "Accumulated CPU energy usage (J)", 0);
#endif

#if defined(DM_PLATFORM_OSX) || defined(DM_PLATFORM_MACOS)
#include <errno.h>
#include <libproc.h>
#include <string.h>
#include <sys/resource.h>
#include <unistd.h>
#endif

static int PushEnergyError(lua_State* L, const char* reason)
{
    DM_LUA_STACK_CHECK(L, 2);
    lua_pushnil(L);
    lua_pushfstring(L, "CPU energy usage is unavailable: %s", reason);
    return 2;
}

static bool ReadCpuEnergyUsage(double* joules, const char** error)
{
#if (defined(DM_PLATFORM_OSX) || defined(DM_PLATFORM_MACOS)) && defined(RUSAGE_INFO_V6)
    struct rusage_info_v6 usage = {};
    if (proc_pid_rusage(getpid(), RUSAGE_INFO_V6, (rusage_info_t*)&usage) != 0)
    {
        *error = strerror(errno);
        return false;
    }

    // The API can succeed even when hardware does not supply an energy counter.
    if (usage.ri_energy_nj == 0)
    {
        *error = "the CPU energy counter is zero or unsupported";
        return false;
    }

    // Cumulative energy for this process since launch, converted from nJ to J.
    *joules = (double)usage.ri_energy_nj * 1.0e-9;
    return true;
#else
    *error = "unsupported platform or macOS SDK";
    return false;
#endif
}

static int GetCpuEnergyUsage(lua_State* L)
{
    double joules;
    const char* error;
    if (!ReadCpuEnergyUsage(&joules, &error))
    {
        return PushEnergyError(L, error);
    }

    DM_LUA_STACK_CHECK(L, 1);
    lua_pushnumber(L, (lua_Number)joules);
    return 1;
}

static const luaL_reg Module_methods[] =
{
    {"get_cpu_energy_usage", GetCpuEnergyUsage},
    {0, 0}
};

static dmExtension::Result InitializeEnergySaver(dmExtension::Params* params)
{
    lua_State* L = params->m_L;
    DM_LUA_STACK_CHECK(L, 0);
    luaL_register(L, MODULE_NAME, Module_methods);
    lua_pop(L, 1);
    return dmExtension::RESULT_OK;
}

static dmExtension::Result FinalizeEnergySaver(dmExtension::Params*)
{
    return dmExtension::RESULT_OK;
}

static dmExtension::Result UpdateEnergySaver(dmExtension::Params*)
{
#if defined(DM_DEBUG)
    double joules;
    const char* error;
    if (ReadCpuEnergyUsage(&joules, &error))
    {
        DM_PROPERTY_SET_F64(rmtp_CpuEnergyUsage, joules);
    }
#endif
    return dmExtension::RESULT_OK;
}

DM_DECLARE_EXTENSION(EnergySaver, LIB_NAME, 0, 0, InitializeEnergySaver, UpdateEnergySaver, 0, FinalizeEnergySaver)
