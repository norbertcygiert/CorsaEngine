#include "Camera.h"
#include "imgui/imgui.h"
namespace ig = ImGui;

DirectX::XMMATRIX Camera::getMatrix() const noexcept {
	using namespace DirectX;
	const auto pos = XMVector3Transform(
		XMVectorSet(0.f, 0.f, -r, 0.f),
		XMMatrixRotationRollPitchYaw(phi, -theta, 0.f)
	);

	return 
		XMMatrixLookAtLH(pos, XMVectorZero(), XMVectorSet(0.f, 1.f, 0.f, 0.f)) //Camera pointing up is the origin
		* 
		XMMatrixRotationRollPitchYaw(pitch, -yaw, roll);
}

void Camera::resetCamera() noexcept {
	r = 20.f;
	theta = .0f;
	phi = .0f;
	pitch = .0f;
	yaw = .0f;
	roll = .0f;
}

void Camera::spawnControlWindow() noexcept{
	if (ig::Begin("Camera Settings")) {
		ig::Text("Position");
		ig::SliderFloat("Radius", &r, .1f, 80.f, "%.1f"); //Setting the radius of the camera to 0.0f causes a crash
		ig::SliderAngle("Theta", &theta, -180.f, 180.f);
		ig::SliderAngle("Phi", &phi, -90.f, 90.f);
		ig::Text("Orientation");
		ig::SliderAngle("Roll", &roll, -180.f, 180.f);
		ig::SliderAngle("Pitch", &pitch, -180.f, 180.f);
		ig::SliderAngle("Yaw", &yaw, -180.f, 180.f);
		if (ig::Button("Reset Camera")) {
			resetCamera();
		}
	}
	ig::End();
}