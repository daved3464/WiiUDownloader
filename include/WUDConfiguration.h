#pragma once

#include <string>

enum class Theme {
    System = 0,
    Light,
    Dark
};


class Configuration {
public:
    Configuration();
    Theme getTheme() const;    
    void setTheme(Theme theme);    
    bool getIsFirstTimeOpen() const;    
    void setIsFirstTimeOpen(bool isFirstTimeOpen);    
    void save() const;

private:
    std::string m_configDir;
    Theme m_theme;
    bool m_isFirstTimeOpen;
};