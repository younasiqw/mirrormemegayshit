#include <array>
#include <string>
#include <deque>
#include <algorithm>
#include "Entities.h"
#include "CommonIncludes.h"
#include "Entities.h"
#include "Vector.h"
#include <map>
#include "Interfaces.h"
#include "Hacks.h"
#include "Hooks.h"

class ResolverSetup
{
public:
	bool didhit;
	void FSN(IClientEntity* pEntity, ClientFrameStage_t stage); //used to get the fresh THINGS and the resolve
	void CM(IClientEntity* pEntity);
	void preso(IClientEntity * pEntity);
	
	//cause this is slower than FSN so we are going to get older info and not updated at all ez to compare between etc.
	std::map<int, float>badangle;

	void Resolve(IClientEntity * pEntity, int CurrentTarget);
	//resolve

	float newsimtime;
	float storedsimtime;
	bool lbyupdated;
	float storedlbyFGE;
	float storedanglesFGE;
	float storedsimtimeFGE;
	float movinglbyFGE;
	bool didhitHS;

	static ResolverSetup GetInst()
	{
		static ResolverSetup instance;
		return instance;
	}
};
namespace Globals
{

	extern CUserCmd* UserCmd;
	extern IClientEntity* Target;
	extern int Shots;
	extern int fired;
	extern int hit;

	extern bool change;
	extern int TargetID;
	extern bool didhitHS;
	extern int missedshots;
	extern std::map<int, QAngle> storedshit;
	extern bool Up2date;
}