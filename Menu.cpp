#include "Menu.h"
#include "Controls.h"
#include "Hooks.h" 
#include "Interfaces.h"
#include "CRC32.h"
#include <fstream>
#include "XorStr.hpp"
#define WINDOW_WIDTH 690
#define WINDOW_HEIGHT 507
PhantomWindow Options::Menu;
struct Config_t {
	int id;
	std::string name;
};
std::vector<Config_t> configs;
typedef void(__cdecl* MsgFn)(const char* msg, va_list);
void MsgX(const char* msg, ...)
{
	if (msg == nullptr)
		return; //If no string was passed, or it was null then don't do anything
	static MsgFn fn = (MsgFn)GetProcAddress(GetModuleHandle("tier0.dll"), "Msg"); 	char buffer[989];
	va_list list;
	va_start(list, msg);
	vsprintf(buffer, msg, list);
	va_end(list);
	fn(buffer, list); //Calls the function, we got the address above.
}
void save_callback()
{
	int should_save = Options::Menu.ColorsTab.ConfigListBox.GetIndex();
	std::string config_directory = "zer0\\cfg\\";
	config_directory += configs[should_save].name; config_directory += ".xml";
	GUI.SaveWindowState(&Options::Menu, XorStr(config_directory.c_str()));
	auto clr = (Color)Options::Menu.ColorsTab.Menu.GetValue();

	Interfaces::CVar->ConsoleColorPrintf(Color(clr.r(), clr.g(), clr.b(), 255), "[zer0cheat] ");
	std::string uremam = "SAVED CONFIGURATION.     \n";
	MsgX(uremam.c_str());
}
void load_callback()
{
	int should_load = Options::Menu.ColorsTab.ConfigListBox.GetIndex();
	std::string config_directory = "zer0\\cfg\\";
	config_directory += configs[should_load].name; config_directory += ".xml";
	GUI.LoadWindowState(&Options::Menu, XorStr(config_directory.c_str()));
	auto clr = (Color)Options::Menu.ColorsTab.Menu.GetValue();

	Interfaces::CVar->ConsoleColorPrintf(Color(clr.r(), clr.g(), clr.b(), 255), "[zer0cheat] ");
	std::string uremam = "LOADED CONFIGURATION.     \n";
	MsgX(uremam.c_str());
}
void list_configs() {
	configs.clear();
	Options::Menu.ColorsTab.ConfigListBox.ClearItems();
	std::ifstream file_in;
	file_in.open("zer0\\cfg\\zer0cfg.txt");
	if (file_in.fail()) {
		std::ofstream("zer0\\cfg\\zer0cfg.txt");
		file_in.open("zer0\\cfg\\zer0cfg.txt");
	}
	int line_count;
	while (!file_in.eof()) {
		Config_t config;
		file_in >> config.name;
		config.id = line_count;
		configs.push_back(config);
		line_count++;
		Options::Menu.ColorsTab.ConfigListBox.AddItem(config.name);
	}
	file_in.close();
	if (configs.size() > 7) Options::Menu.ColorsTab.ConfigListBox.AddItem(" ");
}
void add_config() {
	std::fstream file;
	file.open("zer0\\cfg\\zer0cfg.txt", std::fstream::app);
	if (file.fail()) {
		std::fstream("zer0\\cfg\\zer0cfg.txt");
		file.open("zer0\\cfg\\zer0cfg.txt", std::fstream::app);
	}
	file << std::endl << Options::Menu.ColorsTab.NewConfigName.getText();
	file.close();
	list_configs();
	int should_add = Options::Menu.ColorsTab.ConfigListBox.GetIndex();
	std::string config_directory = "zer0\\cfg\\";
	config_directory += Options::Menu.ColorsTab.NewConfigName.getText(); config_directory += ".xml";
	GUI.SaveWindowState(&Options::Menu, XorStr(config_directory.c_str()));
	Options::Menu.ColorsTab.NewConfigName.SetText("");
}
void remove_config() {
	int should_remove = Options::Menu.ColorsTab.ConfigListBox.GetIndex();
	std::string config_directory = "zer0\\cfg\\";
	config_directory += configs[should_remove].name; config_directory += ".xml";
	std::remove(config_directory.c_str());
	std::ofstream ofs("zer0\\cfg\\zer0cfg.txt", std::ios::out | std::ios::trunc);
	ofs.close();
	std::fstream file;
	file.open("zer0\\cfg\\zer0cfg.txt", std::fstream::app);
	if (file.fail()) {
		std::fstream("zer0\\cfg\\zer0cfg.txt");
		file.open("zer0\\cfg\\zer0cfg.txt", std::fstream::app);
	}
	for (int i = 0; i < configs.size(); i++) {
		if (i == should_remove) continue;
		Config_t config = configs[i];
		file << std::endl << config.name;
	}
	file.close();
	list_configs();
}

void KnifeApplyCallbk()
{
	static ConVar* Meme = Interfaces::CVar->FindVar("cl_fullupdate");
	Meme->nFlags &= ~FCVAR_CHEAT;
	Interfaces::Engine->ClientCmd_Unrestricted("cl_fullupdate");
}
void UnLoadCallbk()
{
	DoUnload = true;
}
void PhantomWindow::Setup()
{
	SetPosition(350, 50);
	SetSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	SetTitle("");
	RegisterTab(&RageBotTab);
	RegisterTab(&VisualsTab);
	RegisterTab(&MiscTab);
	RegisterTab(&ColorsTab);
	RECT Client = GetClientArea();
	Client.bottom -= 29;
	RageBotTab.Setup();
	VisualsTab.Setup();
	MiscTab.Setup();
	ColorsTab.Setup();
#pragma region Bottom Buttons
#pragma endregion
}
void CRageBotTab::Setup()
{
	SetTitle("A");
#pragma region Aimbot
	AimbotGroup.SetPosition(10, 25);
	AimbotGroup.SetText("aimbot");
	AimbotGroup.SetSize(650, 413);
	AimbotGroup.AddTab(CGroupTab("main", 1));
	AimbotGroup.AddTab(CGroupTab("accuracy", 2));
	RegisterControl(&AimbotGroup);
	AimbotEnable.SetFileId("aim_enable");
	AimbotGroup.PlaceLabledControl(1, "enable", this, &AimbotEnable);
	AimbotAutoFire.SetFileId("aim_autofire");
	AimbotGroup.PlaceLabledControl(1, "auto fire", this, &AimbotAutoFire);
	AimbotSilentAim.SetFileId("aim_silentaim");
	AimbotGroup.PlaceLabledControl(1, "silent aim", this, &AimbotSilentAim);
	AccuracyAutoWall.SetFileId("aim_autowall");
	AimbotGroup.PlaceLabledControl(1, "auto wall", this, &AccuracyAutoWall);
	AccuracyAutoScope.SetFileId("aim_autoscope");
	AimbotGroup.PlaceLabledControl(1, "auto scope", this, &AccuracyAutoScope);
	lag_pred.SetFileId("aim_lagprediction");
	AimbotGroup.PlaceLabledControl(1, "lag predition", this, &lag_pred);


	TargetSelection.SetFileId("target_selection");
	TargetSelection.AddItem("closest to crosshair");
	TargetSelection.AddItem("distance");
	TargetSelection.AddItem("lowest health");
	TargetSelection.AddItem("threat");
	TargetSelection.AddItem("next shot");
	AimbotGroup.PlaceLabledControl(1, "selection", this, &TargetSelection);
	TargetHitbox.SetFileId("target_hitboxpriority");
	TargetHitbox.AddItem("head");
	TargetHitbox.AddItem("neck");
	TargetHitbox.AddItem("chest");
	TargetHitbox.AddItem("pelvis");
	TargetHitbox.AddItem("shin");
	AimbotGroup.PlaceLabledControl(1, "priority", this, &TargetHitbox);
	TargetHitscan.SetFileId("target_hitscan");
	TargetHitscan.AddItem("off");
	TargetHitscan.AddItem("head, body");
	TargetHitscan.AddItem("head, body, chest, pelvis, legs");
	TargetHitscan.AddItem("head, body, chest, pelvis, legs, toes, arms");
	AimbotGroup.PlaceLabledControl(1, "hitscan", this, &TargetHitscan);
	preso.SetFileId("accuracy_pitchresolver");
	preso.AddItem("off");
	preso.AddItem("down");
	preso.AddItem("up");
	preso.AddItem("minimal");
	preso.AddItem("automatic");
	AimbotGroup.PlaceLabledControl(1, "pitch resolver", this, &preso);
	resolver.SetFileId("accuracy_yawresolver");
	resolver.AddItem("pff");
	resolver.AddItem("on");

	AimbotGroup.PlaceLabledControl(1, "yaw resolver", this, &resolver);
	AimbotFov.SetFileId("aim_fieldofview");
	AimbotFov.SetBoundaries(0.f, 180.f);
	AimbotFov.extension = XorStr("�");
	AimbotFov.SetValue(0.f);
	AimbotGroup.PlaceLabledControl(1, "field of view", this, &AimbotFov);
	AccuracyHitchance.SetFileId("accuracy_spreadlimit");
	AccuracyHitchance.SetBoundaries(0, 100);
	AccuracyHitchance.SetValue(20);
	AimbotGroup.PlaceLabledControl(2, "spread limit", this, &AccuracyHitchance);
	AccuracyMinimumDamage.SetFileId("accuracy_minimumdamage");
	AccuracyMinimumDamage.SetBoundaries(0, 120);
	AccuracyMinimumDamage.SetValue(20);
	AimbotGroup.PlaceLabledControl(2, "minimum damage", this, &AccuracyMinimumDamage);
	BaimIfUnderXHealth.SetFileId("accuracy_bodyaimifhpislowerthen");
	BaimIfUnderXHealth.SetBoundaries(0, 100);
	BaimIfUnderXHealth.extension = XorStr("HP");
	BaimIfUnderXHealth.SetValue(0);
	AimbotGroup.PlaceLabledControl(2, "body aim if hp is lower then", this, &BaimIfUnderXHealth);
	baim.SetFileId("target_forcebodyaim");
	baim.SetBoundaries(0.f, 15.f);
	baim.SetValue(4.f);
	AimbotGroup.PlaceLabledControl(2, "force bodyaim if shots", this, &baim);
	OverrideKey.SetFileId("accuracy_overrideyawbind");
	AimbotGroup.PlaceLabledControl(2, "override yaw bind", this, &OverrideKey);
	bigbaim.SetFileId("accuracy_forcebodyaimbind");
	AimbotGroup.PlaceLabledControl(2, "force bodyaim bind", this, &bigbaim);
	QuickStop.SetFileId("accuracy_autostop");
	AimbotGroup.PlaceLabledControl(2, "auto stop", this, &QuickStop);
	EnginePrediction.SetFileId("accuracy_engineprediction");
	AimbotGroup.PlaceLabledControl(2, "engine prediction", this, &EnginePrediction);
	pointscaleyes.SetFileId("accuracy_scaledyawpoint");
	AimbotGroup.PlaceLabledControl(2, "scaled yaw point", this, &pointscaleyes);
	pointscaleval.SetFileId("stfudognigger");
	pointscaleval.SetBoundaries(0, 100);
	pointscaleval.SetValue(20);
	AimbotGroup.PlaceLabledControl(2, "", this, &pointscaleval);
	Multienable.SetFileId("accuracy_multipoint");
	AimbotGroup.PlaceLabledControl(2, "multi point", this, &Multienable);
	Multival2.SetFileId("accuracy_multipoint_head");
	Multival2.SetBoundaries(0, 100);
	Multival2.SetValue(20);
	Multival2.extension = XorStr("%%");
	AimbotGroup.PlaceLabledControl(2, "head", this, &Multival2);
	Multival.SetFileId("accuracy_multipoint_body");
	Multival.SetBoundaries(0, 100);
	Multival.SetValue(20);
	Multival.extension = XorStr("%%");
	AimbotGroup.PlaceLabledControl(2, "body", this, &Multival);
	extrapolation.SetFileId("accuracy_extrapolate");
	AimbotGroup.PlaceLabledControl(2, "extrapolate", this, &extrapolation);
	/*----------------------------------------------------------------------
	--------------------------------------------------*/
#pragma endregion  AntiAim controls get setup in here
}
void CLegitBotTab::Setup()
{	/*
	 _      ______ _____ _____ _______
	| |    |  ____/ ____|_   _|__   __|
	| |    | |__ | |  __  | |    | |
	| |    |  __|| | |_ | | |    | |
	| |____| |___| |__| |_| |_   | |
	|______|______\_____|_____|  |_|

	*/
	SetTitle("B");
	AimbotGroup.SetText("Main");
	AimbotGroup.SetPosition(15, 25);
	AimbotGroup.SetSize(640, 222);
	AimbotGroup.AddTab(CGroupTab("Aimbot", 1));
	AimbotGroup.AddTab(CGroupTab("Triggerbot", 2));
	AimbotGroup.AddTab(CGroupTab("Filters", 3));
	RegisterControl(&AimbotGroup);
	RegisterControl(&Active);
	Active.SetFileId("active");
	AimbotGroup.PlaceLabledControl(1, "Activate Legitbot", this, &Active);
	AimbotEnable.SetFileId("l_aimbot");
	AimbotGroup.PlaceLabledControl(1, "Aimbot Enable", this, &AimbotEnable);
	aimbotfiremode.SetFileId("l_autoaimbot");
	aimbotfiremode.AddItem("On Shot");
	aimbotfiremode.AddItem("Automatic");
	AimbotGroup.PlaceLabledControl(1, "Fire Mode", this, &aimbotfiremode);
	AimbotKeyBind.SetFileId("l_aimkey");
	AimbotGroup.PlaceLabledControl(1, "KeyBind", this, &AimbotKeyBind);
	BackTrack.SetFileId("l_backtrack");
	AimbotGroup.PlaceLabledControl(1, "Backtrack", this, &BackTrack);
	AimbotSmokeCheck.SetFileId("l_smokeaimbot");
	AimbotGroup.PlaceLabledControl(1, "Smoke Check", this, &AimbotSmokeCheck);

	legitresolver.SetFileId("l_B1GresolverTappingSkeet");
	legitresolver.AddItem("Off");
	legitresolver.AddItem("Default");
	legitresolver.AddItem("Alternative");
	AimbotGroup.PlaceLabledControl(1, "Resolver", this, &legitresolver);

	//---- Trigger ---//
	TriggerEnable.SetFileId("t_triggerbotenable");
	AimbotGroup.PlaceLabledControl(2, "Activate Trigger", this, &TriggerEnable);
	triggertype.SetFileId("t_triggerbottyp");
	triggertype.AddItem("None");
	triggertype.AddItem("Automatic");
	AimbotGroup.PlaceLabledControl(2, "Trigger Mode", this, &triggertype);
	TriggerHitChanceAmmount.SetFileId("l_trigHC");
	TriggerHitChanceAmmount.SetBoundaries(0, 100);
	TriggerHitChanceAmmount.extension = XorStr("%%");
	TriggerHitChanceAmmount.SetValue(0);
	AimbotGroup.PlaceLabledControl(2, "Hitchance", this, &TriggerHitChanceAmmount);
	TriggerSmokeCheck.SetFileId("l_trigsmoke");
	AimbotGroup.PlaceLabledControl(2, "Smoke Check", this, &TriggerSmokeCheck);
	TriggerRecoil.SetFileId("l_trigRCS");
	TriggerRecoil.SetBoundaries(0.f, 100.f);
	TriggerRecoil.extension = XorStr("%%");
	TriggerRecoil.SetValue(5.00f);
	AimbotGroup.PlaceLabledControl(2, "Recoil", this, &TriggerRecoil);
	TriggerKeyBind.SetFileId("l_trigkey");
	AimbotGroup.PlaceLabledControl(2, "KeyBind", this, &TriggerKeyBind);
	// ---- Hitboxes ---- //
	TriggerHead.SetFileId("l_trighead");
	AimbotGroup.PlaceLabledControl(3, "Head", this, &TriggerHead);
	TriggerChest.SetFileId("l_trigchest");
	AimbotGroup.PlaceLabledControl(3, "Chest", this, &TriggerChest);
	TriggerStomach.SetFileId("l_trigstomach");
	AimbotGroup.PlaceLabledControl(3, "Stomach", this, &TriggerStomach);
	TriggerArms.SetFileId("l_trigarms");
	AimbotGroup.PlaceLabledControl(3, "Arms", this, &TriggerArms);
	TriggerLegs.SetFileId("l_triglegs");
	AimbotGroup.PlaceLabledControl(3, "Legs", this, &TriggerLegs);

	//--------------------------legitaa--------------------------((
/*	aaenable.SetFileId("AA_anable");
	AimbotGroup.PlaceLabledControl(4, "Legit AA Enable", this, &aaenable);

	aatyp.SetFileId("AA_aatyp");
	aatyp.AddItem("Static");
	aatyp.AddItem("Lowerbody");
	aatyp.AddItem("Freestanding");
	AimbotGroup.PlaceLabledControl(4, "Anti-Aim", this, &aatyp);

	aatyp2.SetFileId("AA_aatyp2");
	aatyp2.AddItem("Default");
	aatyp2.AddItem("Jitter");
	aatyp2.AddItem("Shuffle");
	aatyp2.AddItem("Spin");
	AimbotGroup.PlaceLabledControl(4, "Anti-Aim Type", this, &aatyp2);

	aatyp3.SetFileId("AA_aatyp3");
	aatyp3.SetBoundaries(0, 90);
	aatyp3.SetValue(20);
	AimbotGroup.PlaceLabledControl(4, "Anti-Aim Value", this, &aatyp3);

	aafl.SetFileId("AA_aaFL");
	aafl.SetBoundaries(1, 4);
	aafl.SetValue(1);
	AimbotGroup.PlaceLabledControl(4, "Legit FakeLag", this, &aafl); */
	//----------------------solid kys---------------------//
	weapongroup.SetText("Main");
	weapongroup.SetPosition(15, 256);
	weapongroup.SetSize(640, 183);
	weapongroup.AddTab(CGroupTab("Rifle", 1));
	weapongroup.AddTab(CGroupTab("Pistol", 2));
	weapongroup.AddTab(CGroupTab("Sniper", 3));
	weapongroup.AddTab(CGroupTab("SMG", 4));
	weapongroup.AddTab(CGroupTab("Heavy", 5));
	RegisterControl(&weapongroup);
	RegisterControl(&Active);
	WeaponMainHitbox.SetFileId("l_rhitbox");
	WeaponMainHitbox.AddItem("Head");
	WeaponMainHitbox.AddItem("Neck");
	WeaponMainHitbox.AddItem("Chest");
	WeaponMainHitbox.AddItem("Stomach");
	WeaponMainHitbox.AddItem("Nearest");
	weapongroup.PlaceLabledControl(1, "Hitbox", this, &WeaponMainHitbox);
	WeaponMainSpeed.SetFileId("l_rspeed");
	WeaponMainSpeed.SetBoundaries(0, 75);
	WeaponMainSpeed.SetValue(5);
	weapongroup.PlaceLabledControl(1, "Speed", this, &WeaponMainSpeed);
	WeaponMainRecoil.SetFileId("l_rRecoil");
	WeaponMainRecoil.SetBoundaries(0, 200);
	WeaponMainRecoil.SetValue(165);
	weapongroup.PlaceLabledControl(1, "Recoil", this, &WeaponMainRecoil);
	WeaponMainFoV.SetFileId("l_fov");
	WeaponMainFoV.SetBoundaries(0, 45);
	WeaponMainFoV.SetValue(10);
	weapongroup.PlaceLabledControl(1, "Field Of View", this, &WeaponMainFoV);
	// --- Pistols --- //
	WeaponPistHitbox.SetFileId("l_phitbox");
	WeaponPistHitbox.AddItem("Head");
	WeaponPistHitbox.AddItem("Neck");
	WeaponPistHitbox.AddItem("Chest");
	WeaponPistHitbox.AddItem("Stomach");
	WeaponPistHitbox.AddItem("Nearest");
	weapongroup.PlaceLabledControl(2, "Hitbox", this, &WeaponPistHitbox);
	WeaponPistSpeed.SetFileId("l_pspeed");
	WeaponPistSpeed.SetBoundaries(0, 75);
	WeaponPistSpeed.SetValue(5);
	weapongroup.PlaceLabledControl(2, "Speed", this, &WeaponPistSpeed);
	WeaponPistRecoil.SetFileId("l_pRecoil");
	WeaponPistRecoil.SetBoundaries(0, 200);
	WeaponPistRecoil.SetValue(165);
	weapongroup.PlaceLabledControl(2, "Recoil", this, &WeaponPistRecoil);
	WeaponPistFoV.SetFileId("l_pfov");
	WeaponPistFoV.SetBoundaries(0, 45);
	WeaponPistFoV.SetValue(10);
	weapongroup.PlaceLabledControl(2, "Field Of View", this, &WeaponPistFoV);
	// --- Sniper --- //
	WeaponSnipHitbox.SetFileId("l_shitbox");
	WeaponSnipHitbox.AddItem("Head");
	WeaponSnipHitbox.AddItem("Neck");
	WeaponSnipHitbox.AddItem("Chest");
	WeaponSnipHitbox.AddItem("Stomach");
	WeaponSnipHitbox.AddItem("Nearest");
	weapongroup.PlaceLabledControl(3, "Hitbox", this, &WeaponSnipHitbox);
	WeaponSnipSpeed.SetFileId("l_sspeed");
	WeaponSnipSpeed.SetBoundaries(0, 75);
	WeaponSnipSpeed.SetValue(5);
	weapongroup.PlaceLabledControl(3, "Speed", this, &WeaponSnipSpeed);
	WeaponSnipRecoil.SetFileId("l_sRecoil");
	WeaponSnipRecoil.SetBoundaries(0, 200);
	WeaponSnipRecoil.SetValue(165);
	weapongroup.PlaceLabledControl(3, "Recoil", this, &WeaponSnipRecoil);
	WeaponSnipFoV.SetFileId("l_sfov");
	WeaponSnipFoV.SetBoundaries(0, 45);
	WeaponSnipFoV.SetValue(10);
	weapongroup.PlaceLabledControl(3, "Field Of View", this, &WeaponSnipFoV);
	// --- SMG --- //
	WeaponMpHitbox.SetFileId("l_sniphitbox");
	WeaponMpHitbox.AddItem("Head");
	WeaponMpHitbox.AddItem("Neck");
	WeaponMpHitbox.AddItem("Chest");
	WeaponMpHitbox.AddItem("Stomach");
	WeaponMpHitbox.AddItem("Nearest");
	weapongroup.PlaceLabledControl(4, "Hitbox", this, &WeaponMpHitbox);
	WeaponMpSpeed.SetFileId("l_sspeed");
	WeaponMpSpeed.SetBoundaries(0, 75);
	WeaponMpSpeed.SetValue(5);
	weapongroup.PlaceLabledControl(4, "Speed", this, &WeaponMpSpeed);
	WeaponMpRecoil.SetFileId("l_sRecoil");
	WeaponMpRecoil.SetBoundaries(0, 200);
	WeaponMpRecoil.SetValue(165);
	weapongroup.PlaceLabledControl(4, "Recoil", this, &WeaponMpRecoil);
	WeaponMpFoV.SetFileId("l_sfov");
	WeaponMpFoV.SetBoundaries(0, 45);
	WeaponMpFoV.SetValue(10);
	weapongroup.PlaceLabledControl(4, "Field Of View", this, &WeaponMpFoV);
	// --- MachineGun --- //
	WeaponMGHitbox.SetFileId("l_mghitbox");
	WeaponMGHitbox.AddItem("Head");
	WeaponMGHitbox.AddItem("Neck");
	WeaponMGHitbox.AddItem("Chest");
	WeaponMGHitbox.AddItem("Stomach");
	WeaponMGHitbox.AddItem("Nearest");
	weapongroup.PlaceLabledControl(5, "Hitbox", this, &WeaponMGHitbox);
	WeaponMGSpeed.SetFileId("l_mgspeed");
	WeaponMGSpeed.SetBoundaries(0, 75);
	WeaponMGSpeed.SetValue(5);
	weapongroup.PlaceLabledControl(5, "Speed", this, &WeaponMGSpeed);
	WeaponMGRecoil.SetFileId("l_mgRecoil");
	WeaponMGRecoil.SetBoundaries(0, 200);
	WeaponMGRecoil.SetValue(165);
	weapongroup.PlaceLabledControl(5, "Recoil", this, &WeaponMGRecoil);
	WeaponMGFoV.SetFileId("l_mgfov");
	WeaponMGFoV.SetBoundaries(0, 45);
	WeaponMGFoV.SetValue(10);
	weapongroup.PlaceLabledControl(5, "Field Of View", this, &WeaponMGFoV);
}
void CVisualTab::Setup()
{

	SetTitle("B");
#pragma region Options
	OptionsGroup.SetText("options");
	OptionsGroup.SetPosition(15, 30);
	OptionsGroup.SetSize(200, 390);
	OptionsGroup.AddTab(CGroupTab("main", 1));
	OptionsGroup.AddTab(CGroupTab("filtering", 2));
	RegisterControl(&OptionsGroup);
	RegisterControl(&Active);
	Active.SetFileId("visuals_active");
	OptionsGroup.PlaceLabledControl(1, "activate visuals", this, &Active);
	OptionsBox.SetFileId("visuals_box");
	OptionsBox.AddItem("off");
	OptionsBox.AddItem("default");
	OptionsBox.AddItem("genuine");
	OptionsGroup.PlaceLabledControl(1, "box", this, &OptionsBox);
	OptionsName.SetFileId("visuals_name");
	OptionsGroup.PlaceLabledControl(1, "name", this, &OptionsName);
	OptionsHealth.SetFileId("visuals_health");
	OptionsHealth.AddItem("off");
	OptionsHealth.AddItem("default");
	OptionsHealth.AddItem("lined");
	OptionsGroup.PlaceLabledControl(1, "Health", this, &OptionsHealth);
	OptionsInfo.SetFileId("visuals_information");
	OptionsGroup.PlaceLabledControl(1, "information", this, &OptionsInfo);
	OtherHitmarker.SetFileId("visuals_hitmarker");
	OptionsGroup.PlaceLabledControl(1, "hitmarker", this, &OtherHitmarker);
	OptionsSkeleton.SetFileId("visuals_bonematrix");
	OptionsGroup.PlaceLabledControl(1, "bone matrix", this, &OptionsSkeleton);
	OptionsGlow.SetFileId("visuals_glow_enemy");
	OptionsGroup.PlaceLabledControl(1, "enemy player glow", this, &OptionsGlow);
	GlowZ.SetFileId("visuals_glow_enemy_opacity");
	GlowZ.SetValue(99.f);
	GlowZ.SetBoundaries(0.f, 100.f);
	GlowZ.extension = XorStr("%%");
	OptionsGroup.PlaceLabledControl(1, "enemy player glow opacity", this, &GlowZ);
	OptionsGlow_lcl.SetFileId("visuals_glow_local");
	OptionsGroup.PlaceLabledControl(1, "local player glow", this, &OptionsGlow_lcl);
	Glowz_lcl.SetFileId("visuals_glow_local_opacity");
	Glowz_lcl.SetBoundaries(0.f, 100.f);
	Glowz_lcl.extension = XorStr("%%");
	OptionsGroup.PlaceLabledControl(1, "local player glow opacity", this, &Glowz_lcl);
	Weapons.SetFileId("visuals_weapons");
	Weapons.AddItem("off");
	Weapons.AddItem("text");
	Weapons.AddItem("icon");
	OptionsGroup.PlaceLabledControl(1, "weapons", this, &Weapons);
	Ammo.SetFileId("visuals_ammunition");
	OptionsGroup.PlaceLabledControl(1, "ammunition", this, &Ammo);
	OffscreenESP.SetFileId("visuals_offscreenindicator");
	OptionsGroup.PlaceLabledControl(1, "offscreen indicator", this, &OffscreenESP);
	FiltersAll.SetFileId("filter_everything");
	OptionsGroup.PlaceLabledControl(2, "everything", this, &FiltersAll);
	FiltersPlayers.SetFileId("filter_players");
	FiltersPlayers.SetState(true);
	OptionsGroup.PlaceLabledControl(2, "players", this, &FiltersPlayers);
	FiltersEnemiesOnly.SetFileId("filter_enemiesonly");
	OptionsGroup.PlaceLabledControl(2, "enemies only", this, &FiltersEnemiesOnly);
	FiltersWeapons.SetFileId("filter_weapons");
	OptionsGroup.PlaceLabledControl(2, "weapons", this, &FiltersWeapons);
	FiltersNades.SetFileId("filter_nades");
	OptionsGroup.PlaceLabledControl(2, "nades", this, &FiltersNades);
	FiltersC4.SetFileId("filter_c4");
	OptionsGroup.PlaceLabledControl(2, "c4", this, &FiltersC4);

#pragma endregion Setting up the Options controls
	ChamsGroup.SetText("chams");
	ChamsGroup.SetPosition(225, 30);
	ChamsGroup.SetSize(200, 205);
	RegisterControl(&ChamsGroup);
	ChamsEnemyVis.SetFileId("chams_enemy_visible");
	ChamsGroup.PlaceLabledControl(0, "enemies ( visible )", this, &ChamsEnemyVis);
	ChamsEnemyNoVis.SetFileId("chams_en_novis");
	ChamsGroup.PlaceLabledControl(0, "enemies ( invisible )", this, &ChamsEnemyNoVis);

	ChamsLocal.SetFileId("chams_local");
	ChamsGroup.PlaceLabledControl(0, "local player", this, &ChamsLocal);

	localmaterial.SetFileId("chams_inscope_material");
	localmaterial.AddItem("transparency");
	ChamsGroup.PlaceLabledControl(0, "inscope material", this, &localmaterial);

#pragma region World
	worldgroup.SetText("world");
	worldgroup.SetPosition(225, 245);
	worldgroup.SetSize(200, 175);
	RegisterControl(&worldgroup);
	colmodupdate.SetFileId("world_updatematerials");
	worldgroup.PlaceLabledControl(0, "force update materials", this, &colmodupdate); //you could've just made this a button lol

	asusamount.SetFileId("world_asusprops");
	asusamount.SetBoundaries(1.f, 100.f);
	asusamount.extension = XorStr("%%");
	asusamount.SetValue(50.f);
	worldgroup.PlaceLabledControl(0, "asus alpha", this, &asusamount);
	customskies.SetFileId("world_customskies");
	customskies.AddItem("default");
	customskies.AddItem("night_1");
	customskies.AddItem("night_2");
	customskies.AddItem("black");
	worldgroup.PlaceLabledControl(0, "custom skies", this, &customskies);


#pragma region Other
	OtherGroup.SetText("misc");
	OtherGroup.SetPosition(436, 30);
	OtherGroup.SetSize(210, 390);
	RegisterControl(&OtherGroup);
	AutowallCrosshair.SetFileId("misc_autowall_crosshair");
	OtherGroup.PlaceLabledControl(0, "autowall crosshair", this, &AutowallCrosshair);
	SpreadCross.SetFileId("misc_inaccuracy_circle");
	OtherGroup.PlaceLabledControl(0, "inaccuracy circle", this, &SpreadCross);
	SpreadCrosshair.SetFileId(XorStr("misc_inaccuracy_custom_circle"));
	SpreadCrosshair.AddItem("black");
	SpreadCrosshair.AddItem("custom colour");
	OtherGroup.PlaceLabledControl(0, XorStr("inaccuracy circle type"), this, &SpreadCrosshair);
	SpreadCrossSize.SetValue(45.f);
	logs.SetFileId("misc_eventlog");
	OtherGroup.PlaceLabledControl(0, "event log", this, &logs);
	OtherNoFlash.SetFileId("misc_remove_flash_blindness");
	OtherGroup.PlaceLabledControl(0, "remove flash blindness", this, &OtherNoFlash);
	flashAlpha.SetFileId("misc_remove_flash_blindness_alpha");
	flashAlpha.SetBoundaries(0, 100);
	flashAlpha.extension = XorStr("%%");
	OtherGroup.PlaceLabledControl(0, "remove flash blindness alpha", this, &flashAlpha);
	OtherNoSmoke.SetFileId("misc_remove_smoke_effect");
	OtherNoSmoke.SetState(true);
	OtherNoScope.SetFileId("misc_remove_scope_border");
	OtherGroup.PlaceLabledControl(0, "remove inscope border", this, &OtherNoScope);
	RemoveZoom.SetFileId("misc_remove_inscope_zoom");
	OtherGroup.PlaceLabledControl(0, "remove inscope zoom", this, &RemoveZoom);
	OtherViewmodelFOV.SetFileId("misc_viewmodel_field_of_view");
	OtherViewmodelFOV.SetBoundaries(30.f, 120.f);
	OtherViewmodelFOV.SetValue(90.f);
	OtherGroup.PlaceLabledControl(0, "viewmodel field of view", this, &OtherViewmodelFOV);
	OtherFOV.SetFileId("misc_player_field_of_view");
	OtherFOV.SetBoundaries(0.f, 50.f);
	OtherFOV.SetValue(0.f);
	OtherGroup.PlaceLabledControl(0, "player field of view", this, &OtherFOV);
	BulletTracers.SetFileId("misc_bullet_beam_tracers");
	OtherGroup.PlaceLabledControl(0, "bullet beam tracers", this, &BulletTracers);
	beamtime.SetFileId("misc_bullet_beam_tracers_alive_time");
	beamtime.SetBoundaries(0.1, 10);
	beamtime.extension = XorStr("s");
	beamtime.SetValue(2);
	OtherGroup.PlaceLabledControl(0, "bullet beam tracers lifetime", this, &beamtime);

	manualaa.SetFileId("misc_manual_antiaim_indicator");
	OtherGroup.PlaceLabledControl(0, "manual antiaim indicator", this, &manualaa);
	BackTrackBones.SetFileId(XorStr("others_backtrack_chmas"));
	OtherGroup.PlaceLabledControl(0, XorStr("backtrack chams"), this, &BackTrackBones);
	DebugLagComp.SetFileId(XorStr("others_lagcompensation_misc"));
	OtherGroup.PlaceLabledControl(0, XorStr("render lag compensation"), this, &DebugLagComp);
	lagcomptime.SetFileId("misc_lag_compensation_alive_time");
	lagcomptime.SetBoundaries(0.1, 10);
	lagcomptime.extension = XorStr("s");
	lagcomptime.SetValue(2);
	OtherGroup.PlaceLabledControl(0, "lag compensation debug lifetime", this, &lagcomptime);

	FPSIndicator.SetFileId("misc_extraindicators");
	OtherGroup.PlaceLabledControl(0, "extra indicators", this, &FPSIndicator);



#pragma endregion Setting up the Other controls
}
void CLogoTab::Setup()
{

	SetTitle("C");

}

void CMiscTab::Setup()
{
	/*
	 __  __ _____  _____  _____
	|  \/  |_   _|/ ____|/ ____|
	| \  / | | | | (___ | |
	| |\/| | | |  \___ \| |
	| |  | |_| |_ ____) | |____
	|_|  |_|_____|_____/ \_____|

	*/
	SetTitle("D");
#pragma region Other
	OtherGroup.SetPosition(10, 30);
	OtherGroup.SetSize(320, 410);
	OtherGroup.SetText("others");
	RegisterControl(&OtherGroup);

	OtherSafeMode.SetFileId("others_safemode");
	OtherSafeMode.AddItem("keep valid angles");
	OtherSafeMode.AddItem("keep valid fov & angles");
	OtherSafeMode.AddItem("keep real angles & restrict real to real fov");
	OtherSafeMode.AddItem("unrestricted");
	OtherGroup.PlaceLabledControl(0, "safe mode", this, &OtherSafeMode);

	OtherAutoJump.SetFileId("others_autojump");
	OtherGroup.PlaceLabledControl(0, "automatic jump", this, &OtherAutoJump);

	OtherAutoStrafe.SetFileId("others_automaticstrafer");
	OtherGroup.PlaceLabledControl(0, "automatic strafer", this, &OtherAutoStrafe);

	SniperCrosshair.SetFileId("others_sniper_crosshair");
	OtherGroup.PlaceLabledControl(0, "sniper crosshair", this, &SniperCrosshair);

	OtherThirdperson.SetFileId("others_third_person");
	OtherGroup.PlaceLabledControl(0, "3rd person", this, &OtherThirdperson);
	ThirdPersonKeyBind.SetFileId("others_third_person_bind");
	OtherGroup.PlaceLabledControl(0, "", this, &ThirdPersonKeyBind);

	ThirdPersonVal.SetFileId("others_third_person_range");
	ThirdPersonVal.SetBoundaries(10, 300);
	ThirdPersonVal.SetValue(150);
	OtherGroup.PlaceLabledControl(0, "range", this, &ThirdPersonVal);


	PreserveKillfeed.SetFileId("others_preserve_killfeed");
	OtherGroup.PlaceLabledControl(0, "preserve killfeed", this, &PreserveKillfeed);


	DisablePostProcess.SetFileId("others_disable_post_processing");
	OtherGroup.PlaceLabledControl(0, "disable post processing", this, &DisablePostProcess);

	experimental.SetFileId("others_experimental");
	experimental.SetState(true);



	Radar.SetFileId("others_radar");
	OtherGroup.PlaceLabledControl(0, "draw new radar", this, &Radar);

	RadarX.SetFileId("others_radar_x_axis");
	RadarX.SetBoundaries(0, 1920);
	RadarX.SetValue(0);
	OtherGroup.PlaceLabledControl(0, "x axis", this, &RadarX);

	RadarY.SetFileId("others_radar_y_axis");
	RadarY.SetBoundaries(0, 1080);
	RadarY.SetValue(0);
	OtherGroup.PlaceLabledControl(0, "y axis", this, &RadarY);
	FakelagStand.SetFileId("others_fakelag_standing");
	FakelagStand.SetBoundaries(0, 14);
	FakelagStand.SetValue(1);
	OtherGroup.PlaceLabledControl(0, "fakelag standing", this, &FakelagStand);
	FakelagMove.SetFileId("others_fakelag_moving");
	FakelagMove.SetBoundaries(0, 14);
	FakelagMove.SetValue(1);
	OtherGroup.PlaceLabledControl(0, "fakelag moving", this, &FakelagMove);
	Fakelagjump.SetFileId("others_fakelag_inair");
	Fakelagjump.SetBoundaries(0, 14);
	Fakelagjump.SetValue(1);
	OtherGroup.PlaceLabledControl(0, "fakelag in-air", this, &Fakelagjump);

	fl_spike.SetFileId("others_fakelag_spike");
	fl_spike.AddItem("off");
	fl_spike.AddItem("while shooting");
	fl_spike.AddItem("on ground");
	OtherGroup.PlaceLabledControl(0, "fakelag spike", this, &fl_spike);


	AntiAimGroup.SetPosition(346, 30);
	AntiAimGroup.SetText("anti aimbot");
	AntiAimGroup.SetSize(320, 410);
	AntiAimGroup.AddTab(CGroupTab("main", 1));
	AntiAimGroup.AddTab(CGroupTab("builder", 2));
	AntiAimGroup.AddTab(CGroupTab("binds", 3));
	RegisterControl(&AntiAimGroup);
	AntiAimEnable.SetFileId("antiaimbot_enable");
	AntiAimGroup.PlaceLabledControl(1, "enable", this, &AntiAimEnable);

	AntiAimPitch.SetFileId("antiaimbot_pitch");
	AntiAimPitch.AddItem("off");
	AntiAimPitch.AddItem("down");
	AntiAimPitch.AddItem("up");
	AntiAimPitch.AddItem("jitter");
	AntiAimGroup.PlaceLabledControl(1, "pitch", this, &AntiAimPitch);

	AntiAimYaw.SetFileId("antiaimbot_yaw");
	AntiAimYaw.AddItem("off");
	AntiAimYaw.AddItem("static");
	AntiAimYaw.AddItem("jitter");
	AntiAimYaw.AddItem("180 degree spin");
	AntiAimYaw.AddItem("manual static");
	AntiAimYaw.AddItem("manual jitter");
	AntiAimYaw.AddItem("lowerbody break");
	AntiAimYaw.AddItem("random lowerbody break");
	AntiAimYaw.AddItem("automatic");
	AntiAimGroup.PlaceLabledControl(1, "real standing yaw", this, &AntiAimYaw);

	AntiAimYawrun.SetFileId("antiaimbot_movingyaw");
	AntiAimYawrun.AddItem("off");
	AntiAimYawrun.AddItem("static");
	AntiAimYawrun.AddItem("jitter");
	AntiAimYawrun.AddItem("180 degree spin");
	AntiAimYawrun.AddItem("manual static");
	AntiAimYawrun.AddItem("manual jitter");
	AntiAimYawrun.AddItem("lowerbody break");
	AntiAimYawrun.AddItem("random lowerbody break");
	AntiAimYawrun.AddItem("automatic");
	AntiAimGroup.PlaceLabledControl(1, "real moving yaw", this, &AntiAimYawrun);

	AntiAimYaw3.SetFileId("antiaimbot_inairyaw");
	AntiAimYaw3.AddItem("off");
	AntiAimYaw3.AddItem("static");
	AntiAimYaw3.AddItem("jitter");
	AntiAimYaw3.AddItem("180 degree spin");
	AntiAimYaw3.AddItem("manual static");
	AntiAimYaw3.AddItem("manual jitter");
	AntiAimYaw3.AddItem("lowerbody break");
	AntiAimYaw3.AddItem("random lowerbody break");
	AntiAimYaw3.AddItem("automatic");
	AntiAimGroup.PlaceLabledControl(1, "in-air yaw", this, &AntiAimYaw3);

	FakeYaw.SetFileId("antiaim_fakeyaw");
	FakeYaw.AddItem("off");
	FakeYaw.AddItem("opposite");
	FakeYaw.AddItem("jitter");
	FakeYaw.AddItem("random lowerbody break");
	FakeYaw.AddItem("90 degree spin");
	FakeYaw.AddItem("random");
	FakeYaw.AddItem("inverse break");
	FakeYaw.AddItem("dynamic");
	AntiAimGroup.PlaceLabledControl(1, "fake standing yaw", this, &FakeYaw);

	FakeYaw2.SetFileId("antiaim_fakemovingyaw");
	FakeYaw2.AddItem("off");
	FakeYaw2.AddItem("opposite");
	FakeYaw2.AddItem("jitter");
	FakeYaw2.AddItem("random lowerbody break");
	FakeYaw2.AddItem("random");
	FakeYaw2.AddItem("dynamic");
	AntiAimGroup.PlaceLabledControl(1, "fake moving yaw", this, &FakeYaw2);

	FakeYaw3.SetFileId("fakeaaaa"); // aaaaaaaaaaaaaaaaaaa sp00ky
	FakeYaw3.AddItem("off");
	FakeYaw3.AddItem("opposite");
	FakeYaw3.AddItem("jitter");
	FakeYaw3.AddItem("random lowerbody break");
	AntiAimGroup.PlaceLabledControl(1, "fake in-air yaw", this, &FakeYaw3);


	//-<------------------------------------->-//


	twitchr.SetValue(35);
	twitchr2.SetValue(60);
	randlbyr.SetValue(60);
	lby1.SetValue(60);
	twitchf.SetValue(60);
	randlbyf.SetValue(90);

	//-----
	antilby.SetFileId("antiaim_lowerbodybreak");
	antilby.AddItem("off");
	antilby.AddItem("on");
	antilby.AddItem("no 979");
	AntiAimGroup.PlaceLabledControl(2, "lowerbody break", this, &antilby);


	BreakLBYDelta.SetFileId("antiaim_lowerbodybreak_range");
	BreakLBYDelta.SetBoundaries(35, 180);
	BreakLBYDelta.SetValue(90);
	AntiAimGroup.PlaceLabledControl(2, "lowerbody break range", this, &BreakLBYDelta);

	freestandtype.SetFileId("antiaim_freestanding_type");
	freestandtype.AddItem("default");
	freestandtype.AddItem("jitter");
	freestandtype.AddItem("random");
	freestandtype.AddItem("spin");
	AntiAimGroup.PlaceLabledControl(2, "freestanding type", this, &freestandtype);

	freerange.SetFileId("antiaim_freestanding_range");
	freerange.SetBoundaries(0, 60);
	freerange.SetValue(35);
	AntiAimGroup.PlaceLabledControl(2, "freestanding type range", this, &freerange);

	FakeWalkSpeed.SetFileId("others_fakewalk_speed");
	FakeWalkSpeed.SetBoundaries(1, 100);
	FakeWalkSpeed.SetValue(40);
	FakeWalkSpeed.extension = XorStr("%%");
	AntiAimGroup.PlaceLabledControl(2, "fakewalk speed", this, &FakeWalkSpeed);







	FakelagBreakLC.SetFileId("others_fakelag_breaklagcompensation");
	AntiAimGroup.PlaceLabledControl(2, "break lag compensation", this, &FakelagBreakLC);

	manualleft.SetFileId("others_manual_right_bind");
	AntiAimGroup.PlaceLabledControl(3, "manual right bind", this, &manualleft);
	manualright.SetFileId("others_manual_left_bind");
	AntiAimGroup.PlaceLabledControl(3, "manual left bind", this, &manualright);
	manualback.SetFileId("others_manual_back_bind");
	AntiAimGroup.PlaceLabledControl(3, "manual back bind", this, &manualback);

	fw.SetFileId("others_fakewalk_bind");
	AntiAimGroup.PlaceLabledControl(3, "fakewalk bind", this, &fw);



}

void CColorTab::Setup()
{
	SetTitle("E");
#pragma region Visual Colors
	ColorsGroup.SetText("settings");
	ColorsGroup.SetPosition(16, 30);
	ColorsGroup.AddTab(CGroupTab("visuals", 1));
	ColorsGroup.AddTab(CGroupTab("materials", 2));
	ColorsGroup.SetSize(314, 320);
	RegisterControl(&ColorsGroup);
	/*---------------------- COL ----------------------*/
	/*---------------------- COL ----------------------*/
	/*---------------------- COL ----------------------*/

	NameCol.SetFileId(XorStr("player_espname_color"));
	ColorsGroup.PlaceLabledControl(1, XorStr("name"), this, &NameCol);
	BoxCol.SetFileId(XorStr("player_espbox_color"));
	ColorsGroup.PlaceLabledControl(1, XorStr("box"), this, &BoxCol);
	Skeleton.SetFileId(XorStr("player_bone_matrix_color"));
	ColorsGroup.PlaceLabledControl(1, XorStr("bone matrix"), this, &Skeleton);
	GlowEnemy.SetFileId(XorStr("player_glowenemy_color"));
	ColorsGroup.PlaceLabledControl(1, XorStr("glow enemy"), this, &GlowEnemy);
	GlowLocal.SetFileId(XorStr("player_glowlocal_color"));
	ColorsGroup.PlaceLabledControl(1, XorStr("glow local"), this, &GlowLocal);
	Weapons.SetFileId(XorStr("player_weapons_color"));
	ColorsGroup.PlaceLabledControl(1, XorStr("weapons"), this, &Weapons);
	Ammo.SetFileId(XorStr("player_ammo_color"));
	ColorsGroup.PlaceLabledControl(1, XorStr("ammunition"), this, &Ammo);
	Offscreen.SetFileId(XorStr("player_offscreen_color"));
	ColorsGroup.PlaceLabledControl(1, XorStr("offscreen"), this, &Offscreen);
	ChamsLocal.SetFileId(XorStr("player_chamslocal_color"));
	ColorsGroup.PlaceLabledControl(1, XorStr("chams local"), this, &ChamsLocal);
	ChamsEnemyVis.SetFileId(XorStr("player_chamsEVIS_color"));
	ColorsGroup.PlaceLabledControl(1, XorStr("chams enemy"), this, &ChamsEnemyVis);
	ChamsEnemyNotVis.SetFileId(XorStr("player_chamsENVIS_color"));
	ColorsGroup.PlaceLabledControl(1, XorStr("chams enemy (behind wall)"), this, &ChamsEnemyNotVis);
	GunChamsCol.SetFileId(XorStr("player_chamsBACKTRACK_color"));
	ColorsGroup.PlaceLabledControl(1, XorStr("chams enemy (backtrack)"), this, &GunChamsCol);
	LagComp.SetFileId(XorStr("player_LAGCOMP_color"));
	ColorsGroup.PlaceLabledControl(1, XorStr("lag compensation color"), this, &LagComp);
	//	ChamsTeamVis.SetFileId(XorStr("player_chamsTVIS_color"));
	//	ColorsGroup.PlaceLabledControl(1, XorStr("Chams Team"), this, &ChamsTeamVis);
	//	ChamsTeamNotVis.SetFileId(XorStr("player_chamsTNVIS_color"));
	//	ColorsGroup.PlaceLabledControl(1, XorStr("Chams Team (Behind Wall)"), this, &ChamsTeamNotVis);


	Bullettracer.SetFileId(XorStr("player_beam_color"));
	ColorsGroup.PlaceLabledControl(1, XorStr("bullet beam tracers"), this, &Bullettracer);

	scoped_c.SetFileId(XorStr("scope_c"));
	ColorsGroup.PlaceLabledControl(1, XorStr("local scoped material"), this, &scoped_c);



	Menu.SetFileId(XorStr("menu_color"));
	Menu.SetColor(170, 20, 250, 255);
	ColorsGroup.PlaceLabledControl(2, XorStr("menu color"), this, &Menu);
		
	spreadcrosscol.SetFileId(XorStr("weapon_spreadcross_col"));
	ColorsGroup.PlaceLabledControl(2, XorStr("inaccuracy circle"), this, &spreadcrosscol);

	HandChamsCol.SetFileId(XorStr("handchams_col"));
	ColorsGroup.PlaceLabledControl(2, XorStr("hands"), this, &HandChamsCol);


	WeaponChamsCol.SetFileId(XorStr("weaponchams_col"));
	ColorsGroup.PlaceLabledControl(2, XorStr("weapons"), this, &WeaponChamsCol);


	outl_r.SetValue(55.f);
	outl_g.SetValue(15.f);	
	outl_b.SetValue(210.f);
	inl_r.SetValue(185.f);
	inl_g.SetValue(25.f);
	inl_b.SetValue(230.f);
	inr_r.SetValue(185.f);

	inr_g.SetValue(25.f);

	inr_b.SetValue(230.f);
	outr_r.SetValue(55.f);
	outr_g.SetValue(15.f);
	outr_b.SetValue(210.f);



	ConfigGroup.SetText("configs");
	ConfigGroup.SetPosition(340, 30);
	ConfigGroup.SetSize(310, 320);
	RegisterControl(&ConfigGroup); ConfigListBox.SetHeightInItems(7);
	list_configs();
	ConfigGroup.PlaceLabledControl(0, XorStr(""), this, &ConfigListBox);
	LoadConfig.SetText(XorStr("load"));
	LoadConfig.SetCallback(&load_callback);
	ConfigGroup.PlaceLabledControl(0, "", this, &LoadConfig);
	SaveConfig.SetText(XorStr("save"));
	SaveConfig.SetCallback(&save_callback);
	ConfigGroup.PlaceLabledControl(0, "", this, &SaveConfig);
	RemoveConfig.SetText(XorStr("delete"));
	RemoveConfig.SetCallback(&remove_config);
	ConfigGroup.PlaceLabledControl(0, "", this, &RemoveConfig);
	ConfigGroup.PlaceLabledControl(0, "", this, &NewConfigName);
	AddConfig.SetText(XorStr("add"));
	AddConfig.SetCallback(&add_config);
	ConfigGroup.PlaceLabledControl(0, "", this, &AddConfig);
	/*---------------------- OTHERS ----------------------*/
	/*---------------------- OTHERS ----------------------*/
	/*---------------------- OTHERS ----------------------*/
#pragma endregion
#pragma region GloveModel
#pragma endregion
}
void Options::SetupMenu()
{
	Menu.Setup();
	GUI.RegisterWindow(&Menu);
	GUI.BindWindow(VK_INSERT, &Menu);
}
void Options::DoUIFrame()
{
	if (Menu.VisualsTab.FiltersAll.GetState())
	{
		Menu.VisualsTab.FiltersC4.SetState(true);
		Menu.VisualsTab.FiltersPlayers.SetState(true);
		Menu.VisualsTab.FiltersWeapons.SetState(true);
		Menu.VisualsTab.FiltersNades.SetState(true);
	}
	GUI.Update();
	GUI.Draw();
}