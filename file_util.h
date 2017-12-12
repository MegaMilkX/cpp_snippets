#ifndef FILE_UTIL_H
#define FILE_UTIL_H

#include <string>

#include <windows.h>

inline long long filetime_to_posix(FILETIME ft)
{
  LARGE_INTEGER date, adjust;
  date.HighPart = ft.dwHighDateTime;
  date.LowPart = ft.dwLowDateTime;

  // 100-nanoseconds = milliseconds * 10000
  adjust.QuadPart = 11644473600000 * 10000;

  // removes the diff between 1970 and 1601
  date.QuadPart -= adjust.QuadPart;

  return date.QuadPart / 10000000;
}

inline std::string find_last_modified_file(const std::string& filename)
{
  std::string result;

  WIN32_FIND_DATA find_data;
  HANDLE h;
  long long time = 0;
  
  h = FindFirstFileA(filename.c_str(), &find_data);
  if (h == NULL)
  {
    return result;
  }

  if (filetime_to_posix(find_data.ftLastWriteTime) > time)
  {
    result = std::string(find_data.cFileName);
  }
  
  while (FindNextFileA(h, &find_data))
  {
    if (filetime_to_posix(find_data.ftLastWriteTime) > time)
    {
      result = std::string(find_data.cFileName);
    }
  }

  return result;
}

inline HMODULE this_module_handle()
{
  HMODULE h = NULL;
  GetModuleHandleExW(
    GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
    reinterpret_cast<LPCWSTR>(&this_module_handle),
    &h
  );
  return h;
}

#endif
