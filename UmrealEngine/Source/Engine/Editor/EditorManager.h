#pragma once

class EditorTool;
class EditorManager;
extern EditorManager& Editor;

 class EditorManager
 {
     friend class EditorTool;
 public:
     static EditorManager Editor;
 private:
     EditorManager();
     ~EditorManager();
 public:
     static void InitImGuiThemeStyle();
 private:
     void Begin();
     void End();
 public:
     /* InitImGui 직후 호출 */
     void OnGuiStart();
     void DrawGui();
 public:
     // 나중에 자동으로 하나씩 전부 들어가게 만들자
     void PushTool(EditorTool* tool);

     template <typename T>
     T* GetTool(const std::string& name)
     {
         auto itr = mEditorToolArray.find(name);
         if (itr == mEditorToolArray.end())
             return nullptr;
         return dynamic_cast<T*>(itr->second);
     }

     inline const auto& GetRefToolArray() 
     { 
         return mEditorToolArray;
     }
 private:
     ImGuiContext* mGuiContext;

     /* 에디터 툴 리스트 (map을 써야 나중에 정렬이 가능) */
     std::map<std::string, EditorTool*> mEditorToolArray;

     //////////////////////////////////////////////
     // 아래부턴 DockSpace용 변수와 메소드
     //////////////////////////////////////////////
 private:
     /* DockSpace식별용 Label값 */
     std::string mDockingWindowLabel = "MainDockSpaceWindow";

     /* DockSpace가 화면 전체를 차지하는지 여부 */
     bool mIsFullSpace = true;

     /* DockSpace가 Padding을 할지 여부 */
     bool mIsPadding = false;

     /* DockSpace 플래그 값 */
     ImGuiDockNodeFlags mDockNodeFlags;

     /* DockWindow 플래그 값 */
     ImGuiWindowFlags mDockWindowFlag = ImGuiWindowFlags_None;

     /* 메인 도킹영역 ID값 */
     ImGuiID mDockSpaceMainID;

     /* 도킹 영역에 대한 ID값 */
     ImGuiID mDockSpaceAreaID[(INT)eDockSpaceArea::SIZE];

     /* (미구현) 초기 도킹 세팅 저장 값 */
     const char* mDockAreaInitalData;
 private:
     /* 최초로 에디터를 킬 경우 초기 툴의 DockSpace 공간 지정 */
     void InitDockSpaceArea();
     /* DockSpace를 현재의 Flag, Style 기반으로 등록 */
     void SubmitDockSpace();
     /* DockWindow Begin */
     void BeginDock();
     /* DockWindow End */
     void EndDock();
     /* DockWindow Flag 업데이트 (SubmitDockSpace 이전에 호출해야 함) */
     void UpdateWindowFlag();
     /* DockSytle Push (SubmitDockSpace 이전에 호출해야 함) */
     void PushDockStyle();
     /* DockSytle Pop (SubmitDockSpace 이후에 호출해야 함) */
     void PopDockStyle();
 public:
     /* DockSpace 초기 창 위치 복원 */
     inline void ResetDockBuild() { ImGui::LoadIniSettingsFromMemory(mDockAreaInitalData); }
     /* DockSpace가 윈도우 전체를 차지하도록 설정함. */
     inline void SetDockFullSpace(bool isFullSpace) { mIsFullSpace = isFullSpace; }
     inline auto IsDockFullSpace() { return mIsFullSpace; }
     /* DockSpace가 윈도우 전체를 차지하도록 설정함. */
     inline void SetDockPadding(bool isPadding) { mIsPadding = isPadding; }
     inline auto IsDockPadding() { return mIsPadding; }
     /* Dock에 대한 플래그 설정 */
     inline void SetDockFlag(ImGuiDockNodeFlags flag) { mDockNodeFlags = flag; }
     /* 메인 DockSpace의 ID 반환 */
     inline ImGuiID GetDockSpaceID() { return mDockSpaceMainID; }
     /* 메인 DockSpace의 특정 영역 ID 반환 */
     inline ImGuiID GetDockSpaceAreaID(eDockSpaceArea area) { return mDockSpaceAreaID[(INT)area]; }
 };
