#pragma once

class EditorObject;
class EditorManager;
extern EditorManager& Editor;

 class EditorManager
 {
 public:
     static EditorManager Editor;
 private:
     EditorManager();
     ~EditorManager();
 private:
     void Begin();
     void End();
 public:
     /* InitImGui 직후 호출 */
     void OnGuiStart();
     void DrawGui();

     void PushTool(EditorObject* tool);
 private:
     ImGuiContext* mGuiContext;
     std::vector<EditorObject*> mEditorWindowArray;
 };
