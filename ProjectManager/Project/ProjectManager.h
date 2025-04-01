#pragma once

/*
프로젝트 매니저는 특정 프로젝트의 ID를 식별하고, 모든 프로젝트 세팅을 로드해준다.
*/

struct ProjectData;
struct ProjectSetting;

#define _PROJECT_EXT ".PROJECT"
#define _SETTING_EXT ".SETTING"
#define _ASSET_FOLDER "Assets"
#define _SETTING_FOLDER "Settings"

class ProjectManager
{
public:
    /* 새 프로젝트를 만듭니다. */
    static void Create(File::Path _path);
    /* 프로젝트를 저장합니다. */
    static void Save();
    /* 프로젝트를 로드합니다. */
    static void Load(File::Path _path);

    /* 프로젝트 경로를 반환합니다. */
    static const File::Path& GetProjectPath();
    /* 프로젝트가 로드되어있는지 확인합니다. */
    static bool IsProjectLoad();
private:
    /* 프로젝트가 로드 되었는가 */
    static bool             mIsLoadProject;
    /* 로드된 프로젝트 정보들 */
    static ProjectData      mProjectData;
    /* 프로젝트 세팅 정보 */
    static ProjectSetting   mProjectSetting;
private:
    static HRESULT CreateProjectFile(const File::Path& _path);
    static HRESULT LoadProjectFile(const File::Path& _path);
    static HRESULT CreateProjectSettingFile();
    static HRESULT LoadProjectSettingFile();
};

/* 프로젝트ID와 루트경로 */
struct ProjectData
{
    /* 프로젝트 고유 ID */
    File::ID    ProjectID;
    /* 프로젝트 폴더 경로 */
    File::Path  ProjectPath;
    /* 프로젝트 이름 */
    File::Path  ProjectName;
};

struct ProjectSetting
{

};