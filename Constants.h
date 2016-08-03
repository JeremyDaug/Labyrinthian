#ifndef __CONSTANTS_H
#define __CONSTANTS_H

#include <string>

// string constants for names
const std::string STR      = "strength";
const std::string DEX      = "dexterity";
const std::string CON      = "constitution";
const std::string INT      = "intelligence";
const std::string ITU      = "intuition";
const std::string WIL      = "willpower";
const std::string HP       = "healthPoints";
const std::string SP       = "staminaPoints";
const std::string MP       = "mentalPoints";
const std::string WP       = "willPoints";
const std::string PS       = "physicalSaturation";
const std::string MS       = "mentalSaturation";
const std::string DR       = "damageResistance";
const std::string DODGE    = "dodge";
const std::string NONE     = "";

// spare string constants to be nice to myself.
const std::string TRUESTR  = "true";
const std::string FALSESTR = "false";

// race type flags
const long HUMANOID        = 0x0001;
const long UNDEAD          = 0x0002;
const long ROBOT           = 0x0004;
const long ABBERANT        = 0x0008;
const long PURPLEELEPHANT  = 0x0010;

// Ability Flags
const long AOEFLAG         = 0x0001;
const long DOTFLAG         = 0x0002;
const long TEMPFLAG        = 0x0004;
const long ATTACKFLAG      = 0x0008;
const long MAGICFLAG       = 0x0010;
const long TECHFLAG        = 0x0020;
const long PHYSICALFLAG    = 0x0040;
const long MENTALFLAG      = 0x0080;
const long STACKABLEFLAG   = 0x0100;
const long PERCENTFLAG     = 0x0200;
const long FLATFLAG        = 0x0400;
const long ALLOWANCE       = 0x0800;
const long ACTIONFLAG      = 0x1000;
const long LIMITEDFLAG     = 0x2000;
const long COSTFLAG        = 0x4000;

// Item Flags
const long WEAPONFLAG      = 0x0001;
const long ARMORFLAG       = 0x0002;
const long SHIELDFLAG      = 0x0004;
const long RINGFLAG        = 0x0008;
const long BOOTSFLAG       = 0x0010;
const long GLOVESFLAG      = 0x0020;
const long CAPEFLAG        = 0x0040;
const long EYESFLAG        = 0x0080;
const long BELTFLAG        = 0x0100;
const long PACKFLAG        = 0x0200;
const long NECKFLAG        = 0x0400;
const long HELMETFLAG      = 0x0800;
const long USEITEM         = 0x1000;
const long CONSUMABLE      = 0x2000;

// Weapon Flags
const long LIGHTWEAPON     = 0x0001;
const long HEAVYWEAPON     = 0x0002;
const long RANGEDWEAPON    = 0x0004;
const long FIREARM         = 0x0008;
const long SLASHINGWEAPON  = 0x0010;
const long BLUDGEONWEAPON  = 0x0020;
const long PIERCINGWEAPON  = 0x0040;
const long MAGICWEAPON     = 0x0080;
const long TECHWEAPON      = 0x0100;

// Armor Flags
const long FLEXIBLEARMOR   = 0x0001;
const long HARDARMOR       = 0x0002;
const long SHIELDINGARMOR  = 0x0004;

#endif