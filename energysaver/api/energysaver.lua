---@meta
---@diagnostic disable: lowercase-global
---@diagnostic disable: missing-return
--[[
Functions for measuring CPU energy usage with the Energy Saver extension.
The energysaver module is registered automatically; no require() is needed.
--]]

---@class defold_api.energysaver
energysaver = {}

---@class defold_api.energysaver.cpu_energy_usage
---@field joules number Cumulative CPU energy since launch, in joules.
---@field watts number|nil Average CPU power in watts (joules per second), or nil until an interval is available.

---Returns a table with cumulative CPU energy (joules) and average CPU power (watts).
---The reading covers CPU energy only; GPU, display and other device energy are excluded.
---Supported on macOS using proc_pid_rusage() with RUSAGE_INFO_V6. Availability depends
---on the macOS version, hardware and SDK used to build the extension.
---Returns nil and an error message if unsupported, the API fails, or the counter is zero.
---A zero counter may be temporarily unavailable early in the process lifetime; retry later.
---Power is calculated from energy differences and monotonic elapsed time, updating
---when at least one second has elapsed since the previous power sample. Between updates,
---watts retains the latest average; it is nil until two readings establish an interval.
---Lua calls and the debug profiler share the sampling interval. Release builds sample
---only on Lua calls, so call regularly for recent power readings. Reading does not reset
---the cumulative energy counter. A counter reset starts a new power interval.
---@return defold_api.energysaver.cpu_energy_usage|nil usage CPU energy and power, or nil when unavailable.
---@return string|nil error_message Error description on failure; absent on success.
function energysaver.get_cpu_energy_usage() end
