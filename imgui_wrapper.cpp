#include "imgui_wrapper.h"
#include "imgui/imgui.h"

IMGUIWrapper::IMGUIWrapper() {
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
}

IMGUIWrapper::~IMGUIWrapper() {
	ImGui::DestroyContext();
}