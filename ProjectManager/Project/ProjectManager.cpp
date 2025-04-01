#include "pch.h"
#include "ProjectManager.h"

bool                ProjectManager::mIsLoadProject = false;
ProjectData         ProjectManager::mProjectData = {};
ProjectSetting      ProjectManager::mProjectSetting = {};

void ProjectManager::Create(File::Path _path)
{
    // path가 존재하는지, 폴더인지 확인
    if (fs::exists(_path))
    {
        if (fs::is_directory(_path))
        {
            // 이미 존재하는 프로젝트인지 확인
            File::Path projectFilePath = _path / (_path.filename().string() + _PROJECT_EXT);
            if (fs::exists(projectFilePath))
            {
                MessageBox(nullptr, L"이미 존재하는 프로젝트 경로입니다.", L"프로젝트 생성 실패", MB_OK);
            }
            // 존재하지 않으면 생성
            else
            {
                if (FAILED(CreateProjectFile(_path)))
                {
                    MessageBox(nullptr, _path.c_str(), L"프로젝트 생성 실패", MB_OK);
                }
                else
                {
                    Load(projectFilePath);
                }
            }
        }
    }
}

void ProjectManager::Save()
{
    // 프로젝트가 로드 되어 있을 때만 가능
    if(IsProjectLoad())
    {
        int result = MessageBox(
            GetFocus(),                                 // 부모 창 핸들 (NULL로 하면 독립적 메시지 박스)
            L"현재 프로젝트를 저장하시겠습니까?",			// 메시지 텍스트
            L"저장",		                                // 메시지 박스 제목
            MB_YESNO | MB_ICONWARNING					// 스타일: 예/아니오 버튼 + 질문 아이콘
        );
        if (result == IDYES)
        {

        }
    }
}

void ProjectManager::Load(File::Path _path)
{
    // 경로에 파일이 실존하고, 확장자가 맞을 때만
    if (std::filesystem::exists(_path))
    {
        if (fs::is_regular_file(_path) && _path.extension() == _PROJECT_EXT)
        {
            // 기존 파일 저장 여부 물어보기
            Save();
            if (false == FAILED(LoadProjectFile(_path)))
            {

            }
        }
    }
}

const File::Path& ProjectManager::GetProjectPath()
{
    return mProjectData.ProjectPath;
}

bool ProjectManager::IsProjectLoad()
{
    return mProjectData.ProjectID.empty();
}

HRESULT ProjectManager::CreateProjectFile(const File::Path& _path)
{
    HRESULT hr = File::CreateFileID(&mProjectData.ProjectID);
    if (FAILED(hr))
    {
        return hr;
    }
    File::Path projectFileName = _path.filename().string() + _PROJECT_EXT;
    std::ofstream fout(_path / projectFileName);
    if (false == fout.is_open())
    {
        std::cerr << "Failed to open file: " << _path << std::endl;
        return E_FAIL;
    }
    else
    {
        YAML::Node node;
        node["ProjectID"] = mProjectData.ProjectID;
        fout << node;
        mProjectData.ProjectPath = _path;
        return S_OK;
    }
}

HRESULT ProjectManager::LoadProjectFile(const File::Path& _path)
{
    YAML::Node node = YAML::LoadFile(_path.string());
    if (node.IsNull())
    {
        return E_FAIL;
    }

    if (node["ProjectID"])
    {
        mProjectData.ProjectID = node["ProjectID"].as<std::string>();
        mProjectData.ProjectName = _path.filename();
        mProjectData.ProjectPath = _path.parent_path();
        if (mProjectData.ProjectID.empty())
        {
            return E_FAIL;
        }
        return S_OK;
    }
    return E_FAIL;
}

HRESULT ProjectManager::CreateProjectSettingFile()
{
    return E_NOTIMPL;
}

HRESULT ProjectManager::LoadProjectSettingFile()
{
    return E_NOTIMPL;
}
