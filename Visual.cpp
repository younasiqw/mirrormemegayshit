#include "Visuals.h"
#include "Interfaces.h"
#include "RenderManager.h"
#include "Autowall.h"
#include <chrono>
void CVisuals::Init()
{
}
void CVisuals::Move(CUserCmd *pCmd, bool &bSendPacket) {}


void CVisuals::Draw()
{
	if (!Interfaces::Engine->IsConnected() || !Interfaces::Engine->IsInGame())
		return;
	IClientEntity* pLocal = (IClientEntity*)Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());
	if (Options::Menu.VisualsTab.OtherNoScope.GetState() && pLocal->IsAlive() && pLocal->IsScoped())
		NoScopeCrosshair();
	if (Options::Menu.VisualsTab.FPSIndicator.GetState() && pLocal->IsAlive())
	{
		DrawFPS();
		DrawPing();
	}
	if (Options::Menu.VisualsTab.AutowallCrosshair.GetState())
	{
		AutowallCrosshair();
	}

}


void CVisuals::NoScopeCrosshair()
{
	RECT View = Render::GetViewport();
	int MidX = View.right / 2;
	int MidY = View.bottom / 2;
	IClientEntity* pLocal = hackManager.pLocal();
	C_BaseCombatWeapon* pWeapon = (C_BaseCombatWeapon*)Interfaces::EntList->GetClientEntityFromHandle(pLocal->GetActiveWeaponHandle());
	if (GameUtils::IsSniper(pWeapon))
	{
		Render::Line(MidX - 1000, MidY, MidX + 1000, MidY, Color(0, 0, 0, 255));
		Render::Line(MidX, MidY - 1000, MidX, MidY + 1000, Color(0, 0, 0, 255));
	}
}
void CVisuals::DrawCrosshair()
{
	RECT View = Render::GetViewport();
	int MidX = View.right / 2;
	int MidY = View.bottom / 2;
	Render::Line(MidX - 10, MidY, MidX + 10, MidY, Color(0, 255, 0, 255));
	Render::Line(MidX, MidY - 10, MidX, MidY + 10, Color(0, 255, 0, 255));
}
void CVisuals::DrawRecoilCrosshair()
{
	IClientEntity *pLocal = hackManager.pLocal();
	Vector ViewAngles;
	Interfaces::Engine->GetViewAngles(ViewAngles);
	ViewAngles += pLocal->localPlayerExclusive()->GetAimPunchAngle() * 2.f;
	Vector fowardVec;
	AngleVectors(ViewAngles, &fowardVec);
	fowardVec *= 10000;
	Vector start = pLocal->GetOrigin() + pLocal->GetViewOffset();
	Vector end = start + fowardVec, endScreen;
	if (Render::TransformScreen(end, endScreen) && pLocal->IsAlive())
	{
		Render::Line(endScreen.x - 10, endScreen.y, endScreen.x + 10, endScreen.y, Color(0, 255, 0, 255));
		Render::Line(endScreen.x, endScreen.y - 10, endScreen.x, endScreen.y + 10, Color(0, 255, 0, 255));
	}
}
void CVisuals::SpreadCrosshair()
{
	IClientEntity *pLocal = hackManager.pLocal();
	C_BaseCombatWeapon* pWeapon = (C_BaseCombatWeapon*)Interfaces::EntList->GetClientEntityFromHandle(pLocal->GetActiveWeaponHandle());
	IClientEntity* WeaponEnt = Interfaces::EntList->GetClientEntityFromHandle(pLocal->GetActiveWeaponHandle());
	if (!hackManager.pLocal()->IsAlive())
		return;
	if (!GameUtils::IsBallisticWeapon(pWeapon))
		return;
	if (pWeapon == nullptr)
		return;
	int xs;
	int ys;
	Interfaces::Engine->GetScreenSize(xs, ys);
	xs /= 2; ys /= 2;
	float inaccuracy = pWeapon->GetInaccuracy() * 1000;
	char buffer4[64];
	sprintf_s(buffer4, "%.00f", inaccuracy);
	Render::DrawFilledCircle(Vector2D(xs, ys), Color(20, 20, 20, 124), inaccuracy, 60);
}
void CVisuals::AutowallCrosshair() 
{
	IClientEntity *pLocal = hackManager.pLocal();
	RECT View = Render::GetViewport();
	int MidX = View.right / 2;
	int MidY = View.bottom / 2;
	Vector ViewAngles;
	Interfaces::Engine->GetViewAngles(ViewAngles);
	ViewAngles += pLocal->localPlayerExclusive()->GetAimPunchAngle() * 2.f;
	Vector fowardVec;
	AngleVectors(ViewAngles, &fowardVec);
	fowardVec *= 10000;
	Vector start = pLocal->GetOrigin() + pLocal->GetViewOffset();
	Vector end = start + fowardVec, endScreen;
	int xs, ys;
	Interfaces::Engine->GetScreenSize(xs, ys);
	xs /= 2; ys /= 2;
	if (Render::TransformScreen(end, endScreen) && pLocal->IsAlive())
	{
		float damage = 0.f;
		Render::OutlinedRect(xs - 2, ys - 2, 5, 5, Color(27, 27, 27), trace_autowallable(damage) ? Color(0, 250, 0) : Color(250, 0, 0));
	}
}

void CVisuals::DrawPing()
{
	RECT scrn = Render::GetViewport();

	float ping = 0;

	INetChannelInfo *nci = Interfaces::Engine->GetNetChannelInfo();
	if (nci)
	{
		ping = nci->GetAvgLatency(FLOW_INCOMING) + nci->GetAvgLatency(FLOW_OUTGOING);
	}

	int Red = 255 - ((ping * 100) * 2.55);
	int Green = (ping * 100) * 2.55;

	Render::Text(9, scrn.bottom - 110, Color(Red, Green, 0, 255), Render::Fonts::LBY, "PING:");
}

int GetFPS567() {
	static int iFps, iLastFps;
	static float flLastTickCount, flTickCount;
	flTickCount = clock() * 0.001f;
	iFps++;
	if ((flTickCount - flLastTickCount) >= 1.0f) {
		flLastTickCount = flTickCount;
		iLastFps = iFps;
		iFps = 0;
	}
	return iLastFps;
}

void CVisuals::DrawFPS()
{
	RECT scrn = Render::GetViewport();

	float fps = GetFPS567();

	int Red = 255 - (fps * 2.55);
	int Green = fps * 2.55;

	Render::textT(9, scrn.bottom - 90, Render::Fonts::LBY, Color(0, 250, 30, 255), "FPS: %i", GetFPS567());
}