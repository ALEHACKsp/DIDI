#include "GUI.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"

#include <array>
#include <ctime>
#include <iomanip>
#include <Pdh.h>
#include <sstream>
#include <vector>
#include <Windows.h>

#include "Config.h"
#include "Hooks.h"


GUI::GUI() noexcept
{
    ImGui::CreateContext();
    ImGui_ImplWin32_Init(FindWindowW(L"Valve001", nullptr));

    ImGui::StyleColorsClassic();
    ImGuiStyle& style = ImGui::GetStyle();

    style.ScrollbarSize = 13.0f;
    style.WindowTitleAlign = { 0.5f, 0.5f };

    ImGuiIO& io = ImGui::GetIO();

    auto fonts = io.Fonts;

    fonts->AddFontFromFileTTF(
        "c:/windows/fonts/simhei.ttf",
        13.0f,
        NULL,
        fonts->GetGlyphRangesChineseFull()
    );

    io.IniFilename = nullptr;
    io.LogFilename = nullptr;
    io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;

}

void GUI::render() noexcept
{
    if (!open)
        return;
 
    ImGui::Begin(u8"ҩˮ�� INS����ʾ/����", nullptr,  ImGuiWindowFlags_NoResize  | ImGuiWindowFlags_NoCollapse);

    ImGuiColorEditFlags misc_flags = 0 | 0 | ImGuiColorEditFlags_AlphaPreview | 0;

    
    ImGui::Checkbox(u8"����͸��", &config->espEnable);

    ImGui::Checkbox(u8"����͸��3D", &config->boxEnable);
    ImGui::Text("  "); ImGui::SameLine();
    ImGui::BeginGroup();

    ImGui::Checkbox(u8"��ʾ����", &config->BoxTeammate->enable);
    ImGui::SameLine();
    ImGui::ColorEdit4("box_teammate_visiable", config->BoxTeammate->visiableColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | misc_flags);
    ImGui::SameLine();
    ImGui::ColorEdit4("box_teammate_invisiable", config->BoxTeammate->invisiableColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | misc_flags);

    ImGui::Checkbox(u8"��ʾ����", &config->BoxEnemy->enable);
    ImGui::SameLine();
    ImGui::ColorEdit4("box_enemy_visiable", config->BoxEnemy->visiableColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | misc_flags);
    ImGui::SameLine();
    ImGui::ColorEdit4("box_enemy_invisiable", config->BoxEnemy->invisiableColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | misc_flags);

    ImGui::EndGroup();


    ImGui::Checkbox(u8"��ʾѪ��", &config->healthBarEnable);
    ImGui::Text("  "); ImGui::SameLine();
    ImGui::BeginGroup();

    ImGui::Checkbox(u8"����Ѫ��", &config->HealthBarTeammate->enable);
    ImGui::SameLine();
    ImGui::ColorEdit4("healthbar_teammate_visiable", config->HealthBarTeammate->visiableColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | misc_flags);
    ImGui::SameLine();
    ImGui::ColorEdit4("healthbar_teammate_invisiable", config->HealthBarTeammate->invisiableColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | misc_flags);

    ImGui::Checkbox(u8"����Ѫ��", &config->HealthBarEnemy->enable);
    ImGui::SameLine();
    ImGui::ColorEdit4("healthbar_enemy_visiable", config->HealthBarEnemy->visiableColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | misc_flags);
    ImGui::SameLine();
    ImGui::ColorEdit4("healthbar_enemy_invisiable", config->HealthBarEnemy->invisiableColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | misc_flags);

    ImGui::EndGroup();



    ImGui::Checkbox(u8"��ʾѪ��", &config->healthValueEnable);
    ImGui::Text("  "); ImGui::SameLine();
    ImGui::BeginGroup();

    ImGui::Checkbox(u8"����Ѫ��", &config->HealthValueTeammate->enable);
    ImGui::SameLine();
    ImGui::ColorEdit4("healthvalue_teammate_visiable", config->HealthValueTeammate->visiableColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | misc_flags);
    ImGui::SameLine();
    ImGui::ColorEdit4("healthvalue_teammate_invisiable", config->HealthValueTeammate->invisiableColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | misc_flags);

    ImGui::Checkbox(u8"����Ѫ��", &config->HealthValueEnemy->enable);
    ImGui::SameLine();
    ImGui::ColorEdit4("healthvalue_enemy_visiable", config->HealthValueEnemy->visiableColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | misc_flags);
    ImGui::SameLine();
    ImGui::ColorEdit4("healthvalue_enemy_invisiable", config->HealthValueEnemy->invisiableColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | misc_flags);

    ImGui::EndGroup();




    ImGui::Checkbox(u8"��ʾ����", &config->lineEnable);
    ImGui::Text("  "); ImGui::SameLine();
    ImGui::BeginGroup();

    ImGui::Checkbox(u8"��������", &config->LineTeammate->enable);
    ImGui::SameLine();
    ImGui::ColorEdit4("line_teammate_visiable", config->LineTeammate->visiableColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | misc_flags);
    ImGui::SameLine();
    ImGui::ColorEdit4("line_teammate_invisiable", config->LineTeammate->invisiableColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | misc_flags);

    ImGui::Checkbox(u8"��������", &config->LineEnemy->enable);
    ImGui::SameLine();
    ImGui::ColorEdit4("line_enemy_visiable", config->LineEnemy->visiableColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | misc_flags);
    ImGui::SameLine();
    ImGui::ColorEdit4("line_enemy_invisiable", config->LineEnemy->invisiableColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | misc_flags);

    ImGui::EndGroup();

    


    ImGui::NewLine();

    ImGui::Checkbox(u8"��������", &config->aimbotConfig->enable);

    const char* aimTypeItems[] = { u8"���", u8"����", u8"ALT" };
    ImGui::Combo(u8"���鷽ʽ", &config->aimbotConfig->aimType, aimTypeItems, IM_ARRAYSIZE(aimTypeItems));

    const char* aimItems[] = {u8"�ͽ�ԭ��", u8"ͷ��", u8"�ز�", u8"�ɲ�"};
    ImGui::Combo(u8"��׼λ��", &config->aimbotConfig->aimPlace, aimItems, IM_ARRAYSIZE(aimItems));

    ImGui::InputFloat(u8"��׼��Χ", &config->aimbotConfig->aimAngleRange, 0.1f);

    ImGui::InputFloat(u8"��ǹ�ٶ�", &config->aimbotConfig->smooth, 0.1f);
 

    if (config->aimbotConfig->aimAngleRange < 0.f)
        config->aimbotConfig->aimAngleRange = 0.f;

    if (config->aimbotConfig->aimAngleRange > 360.f)
        config->aimbotConfig->aimAngleRange = 360.f;

    if (config->aimbotConfig->smooth < 1.f)
        config->aimbotConfig->smooth = 1.f;

    if (config->aimbotConfig->smooth > 50.f)
        config->aimbotConfig->smooth = 50.f;



    ImGui::Checkbox(u8"����ѹǹ", &config->aimbotConfig->antPunch);

    ImGui::SliderFloat(u8"ѹǹX��", &config->aimbotConfig->antPunshX, 1.0f, 3.0f);

    ImGui::SliderFloat(u8"ѹǹY��", &config->aimbotConfig->antPunshY, 1.0f, 3.0f);
 
    ImGui::Checkbox(u8"��������", &config->aimbotConfig->ignoreSmoke);
    ImGui::Checkbox(u8"��������", &config->aimbotConfig->ignoreFlash);
    ImGui::Checkbox(u8"��׼����", &config->aimbotConfig->aimTeammate);

    const char* styleItems[] = { u8"�ݼ�", u8"��ͨ", u8"��̬" };
    if (ImGui::Combo(u8"��������", &config->aimbotConfig->style, styleItems, IM_ARRAYSIZE(styleItems))) {
        switch (config->aimbotConfig->style)
        {
        case 0:
            config->boxEnable = false;
            config->healthBarEnable = false;
            config->healthValueEnable = false;
            config->lineEnable = false;

            config->aimbotConfig->smooth = 5.f;
            config->aimbotConfig->aimAngleRange = 5.f;
            config->aimbotConfig->aimPlace = NEAST;
            break;
        case 1:
            config->boxEnable = true;
            config->BoxEnemy->enable = true;

            config->healthBarEnable = true;
            config->HealthBarEnemy->enable = true;

            config->healthValueEnable = true;
            config->HealthValueEnemy->enable = true;

            config->lineEnable = true;
            config->LineEnemy->enable = true;

            config->aimbotConfig->smooth = 2.f;
            config->aimbotConfig->aimAngleRange = 35.f;
            config->aimbotConfig->aimPlace = CHEAST;
            break;

        case 2:
            config->boxEnable = true;
            config->BoxEnemy->enable = true;

            config->healthBarEnable = true;
            config->HealthBarEnemy->enable = true;

            config->healthValueEnable = true;
            config->HealthValueEnemy->enable = true;

            config->lineEnable = true;
            config->LineEnemy->enable = true;

            config->aimbotConfig->smooth = 1.f;
            config->aimbotConfig->aimAngleRange = 360.f;
            config->aimbotConfig->aimPlace = HEAD;
            break;
        default:
            break;
        }
    }

    ImGui::NewLine();

    if (ImGui::Button(u8"             �ر�             "))
    {
        hooks->restore();
    }
    
    ImGui::End();
}
