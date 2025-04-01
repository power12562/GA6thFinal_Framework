#include "pch.h"
#include "EditorBase.h"
#include "EditorManager.h"

/*
2025.03.13 -  
Begin의 if문 안에 End를 넣으니까 같은 Tab으로 Docking시도 시 Missing End() 예외가 발생하며 터짐.
이유는 ImGui 공식 문서에 있었다...
ImGui 공식 문서:
When using Docking, it is expected that all dockable windows are submitted each frame and properly Begin/End even if you don't show their contents.
해석: 
도킹을 사용할 때는 도킹 가능한 모든 창이 각 프레임에 제출되고, 창이 내용을 표시하지 않더라도 적절하게 시작/종료되는 것이 예상됩니다.

결론: Docking을 사용 시 Begin상관 없이 End를 호출해줘야 한다.
*/
void EditorTool::OnDrawGui()
{
    if (GetActive())
    {
        OnPreFrame();

        ImGui::Begin(GetLabel().c_str(), nullptr, mFlag);

        OnFrame();

        ImGui::End();

        OnPostFrame();
    }
}