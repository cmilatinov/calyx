#pragma once

namespace Calyx::Utils {

    String ToLowercase(const String& str);
    List<String> Split(const String& str, const String& regex);
    bool IsSearchMatch(const String& query, const String& searchTarget);

    UUID GenerateUUID();

}