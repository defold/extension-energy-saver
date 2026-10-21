---@meta
---@diagnostic disable: lowercase-global
---@diagnostic disable: missing-return
--[[
Functions for measuring CPU energy usage with the Energy Saver extension.
The energysaver module is registered automatically; no require() is needed.
--]]

---@class defold_api.energysaver
energysaver = {}

---Returns the current process's cumulative CPU energy usage since launch, in joules.
---The reading covers CPU energy only; GPU, display and other device energy are excluded.
---Supported on macOS using proc_pid_rusage() with RUSAGE_INFO_V6. Availability depends
---on the macOS version, hardware and SDK used to build the extension.
---Returns nil and an error message if unsupported, the API fails, or the counter is zero.
---A zero counter may be temporarily unavailable early in the process lifetime; retry later.
---Reading the counter does not reset it. Subtract two successful readings to measure
---interval energy, then divide by elapsed seconds to obtain average CPU power in watts.
---@return number|nil joules Cumulative CPU energy in joules, or nil when unavailable.
---@return string|nil error_message Error description on failure; absent on success.
function energysaver.get_cpu_energy_usage() end
