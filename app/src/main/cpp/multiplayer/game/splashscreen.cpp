#include "../main.h"
#include "RW/RenderWare.h"
#include "game.h"
#include "../gui/gui.h"
#include "../util/CJavaWrapper.h"
#include "java_systems/HUD.h"
#include "SnapShots.h"
#include "ObjectEditor.h"
#include "game/Shadow/Shadows.h"

extern CGUI* pGUI;

RwTexture* splashTexture = nullptr;

void LoadSplashTexture()
{
	Log("Loading hud bg texture..");
	splashTexture = CUtil::LoadTextureFromDB("samp", "hud_bg");

	// Khởi tạo an toàn cho các con trỏ texture tránh crash
	CVehicleNeon::pNeonTex = nullptr;
	CGUI::m_pIconTruckTex = nullptr;
	CGUI::m_pPassengerButtonTex = nullptr;
	CShadows::gpShadowHeadLightsTexLong = nullptr;
}

void ImGui_ImplRenderWare_RenderDrawData(ImDrawData* draw_data);
void ImGui_ImplRenderWare_NewFrame();

void go_new_sprite(ObjectProperties &obj, RwTexture* tex) {
	if(tex)
		RwTextureDestroy(tex);

	tex = CSnapShots::CreateObjectSnapShot(obj.id, &obj.rotation, &obj.position);
}

bool testSpriteRender = false;
ObjectProperties testSpriteTmpObject{};

void RenderSpriteCoords()
{
	CObjectEditor::bIsToggle = true;
    ObjectProperties& tmp_obj = testSpriteTmpObject;
	static ObjectProperties previousProps = tmp_obj;

	static RwTexture* temp =  CSnapShots::CreateObjectSnapShot(tmp_obj.id, &tmp_obj.rotation, &tmp_obj.position);

    ImGuiStyle& style = ImGui::GetStyle();
    style.GrabMinSize = 80.0f;

	auto size = ImVec2(600, 500);
	ImGui::SetNextWindowPos( CGUI::GetPercent(40.f, 70.f, size) );
	ImGui::SetNextWindowSize(size);

	ImGui::Begin("Object Properties");

	ImGui::InputInt("ID", &tmp_obj.id);

	ImGui::Text("Position");
	ImGui::InputFloat("X##Position", &tmp_obj.position.x, 0.1f);
	ImGui::InputFloat("Y##Position", &tmp_obj.position.y, 0.1f);
    ImGui::InputFloat("Z##Position", &tmp_obj.position.z, 0.1f);

	ImGui::Text("Rotation");
	ImGui::InputFloat("X##Rotation", &tmp_obj.rotation.x, 1.f);
	ImGui::InputFloat("Y##Rotation", &tmp_obj.rotation.y, 1.f);
	ImGui::InputFloat("Z##Rotation", &tmp_obj.rotation.z, 1.f);

	ImGui::End();

	// Bổ sung kiểm tra temp và temp->raster trước khi vẽ
	if (temp && temp->raster) {
		ImGui::GetBackgroundDrawList()->AddImage((ImTextureID)temp->raster,
					CGUI::GetCenterScreen(ImVec2(500, 500)),
					CGUI::GetCenterScreen(ImVec2(500, 500)) + ImVec2(500, 500)
		);
	}

	if (memcmp(&previousProps, &tmp_obj, sizeof(ObjectProperties)) != 0) {
		go_new_sprite(tmp_obj, temp);
		previousProps = tmp_obj;
	}
}

void RenderBackgroundHud()
{
    if(testSpriteRender)
	    RenderSpriteCoords();

	if (CHUD::bIsShow)
	{
		// Kiểm tra an toàn cả splashTexture và raster
		if (splashTexture && splashTexture->raster)
		{
			ImGui::GetBackgroundDrawList()->AddImage((ImTextureID)splashTexture->raster,
													 ImVec2(CHUD::radarBgPos1.x, CHUD::radarBgPos1.y),
													 ImVec2(CHUD::radarBgPos2.x, CHUD::radarBgPos2.y));
		}
	}
}
