#pragma once

class SettingManager
{
public:
    static bool CheckExistsForder();
    template <typename SETTING>
    static void Save(File::Path _FileName, SETTING& _settingData)
    {
        if (ProjectManager::IsProjectLoad())
        {
            File::Path path = ProjectManager::GetProjectPath() / _SETTING_FOLDER;
            path += _FileName.string();
            path += _SETTING_EXT;
            rfl::yaml::save(path.string(), _settingData);
        }
    }
    template <typename SETTING>
    static void Load()
    {

    }
};

