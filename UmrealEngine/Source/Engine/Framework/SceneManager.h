#pragma once

class Scene;
class SceneManager;
extern SceneManager& _SceneManager;

class SceneManager
{
public:
    static SceneManager _SceneManager;
public:
    SceneManager();
    ~SceneManager();
public:
    void ClientUpdate();
    void ClientRender();
private:
    /* 씬 교체 등 */
    void UpdateScene();

    /* 라이프 사이클 */
    void FixedUpdate();
    void Update();
    void Render();
private:
    Scene* mCurrScene = nullptr;
    Scene* mNextScene = nullptr;

    std::vector<Scene*> mSceneArray;
};

