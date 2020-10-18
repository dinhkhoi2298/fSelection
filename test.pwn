/* fSelection test */
#include <a_samp>
#include <Pawn.Cmd>
#include <fSelection>

const 
	FSELECTION_CLOTHES = 0, 
	FSELECTION_WEAPONS = 1,
	FSELECTION_VEHICLES = 2; 

CMD:clothes(playerid) {
	if(fselection_showing(playerid)) 
		return SendClientMessage(playerid, -1, "SU DUNG ESC DE TAT.");

	for(new skinID = 1; skinID < 312; skinID ++) {
		if(skinID == 74) continue;
		fselection_add_item(playerid, skinID);
	}

	fselection_show(playerid, FSELECTION_CLOTHES, "Clothes");
	return 1;
}

CMD:weapons(playerid) {
	if(fselection_showing(playerid)) 
		return SendClientMessage(playerid, -1, "SU DUNG ESC DE TAT.");

	for(new modelid = 321; modelid < 371; modelid ++) {
        switch(modelid) {
        	case 327..330, 332, 340: continue;
        }
        fselection_add_item(playerid, modelid, .zoom = 2.0);
    }

    fselection_show(playerid, FSELECTION_WEAPONS, "Weapons");
    return 1;
}

CMD:vehicles(playerid) {
	if(fselection_showing(playerid)) 
		return SendClientMessage(playerid, -1, "SU DUNG ESC DE TAT.");

	for(new iVeh = 400; iVeh < 612; iVeh ++) {
        fselection_add_item(playerid, iVeh, -10.0, 0.0, -20);
    }
    fselection_show(playerid, FSELECTION_VEHICLES, "Vehicles");
    return 1;
}

public OnFSelectionResponse(playerid, fselectid, modelid, response) {	
	switch(fselectid) {
		case FSELECTION_CLOTHES: {
			if(response == fS_RESPONSE_CLICKED) {
				SetPlayerSkin(playerid, modelid);		
			}
			else SendClientMessage(playerid, -1, "BRO DA TAT FSELECTION CLOTHES.");
		}
		case FSELECTION_WEAPONS: {
			if(response == fS_RESPONSE_CLICKED) {
				GivePlayerWeapon(playerid, Utils_ModelToWeapon(modelid), 6000);	
			}
			else SendClientMessage(playerid, -1, "BRO DA TAT FSELECTION WEAPONS.");			
		}
		case FSELECTION_VEHICLES: {
			if(response == fS_RESPONSE_CLICKED) {
				static 
					Float:X, Float:Y, Float:Z;

				GetPlayerPos(playerid, X, Y, Z);
				if(400 <= modelid < 612) CreateVehicle(modelid, X, Y, Z, float(90), random(126), random(126), -1);
			}
			else SendClientMessage(playerid, -1, "BRO DA TAT FSELECTION VEHICLES.");
		}
	}
	fselection_hide(playerid);
	return 1;
}

// utils.inc from base-alt-rp (https://github.com/duydang2311/base-alt-rp)
stock Utils_ModelToWeapon(const model)
{
    switch(model) 
    {
        case 331: return 1;
        case 333: return 2;
        case 334: return 3;
        case 335: return 4;
        case 336: return 5;
        case 337: return 6;
        case 338: return 7;
        case 339: return 8;
        case 341: return 9;
        case 321: return 10;
        case 322: return 11;
        case 323: return 12;
        case 324: return 13;
        case 325: return 14;
        case 326: return 15;
        case 342: return 16;
        case 343: return 17;
        case 344: return 18;
        case 346: return 22;
        case 347: return 23;
        case 348: return 24;
        case 349: return 25;
        case 350: return 26;
        case 351: return 27;
        case 352: return 28;
        case 353: return 29;
        case 355: return 30;
        case 356: return 31;
        case 372: return 32;
        case 357: return 33;
        case 358: return 34;
        case 359: return 35;
        case 360: return 36;
        case 361: return 37;
        case 362: return 38;
        case 363: return 39;
        case 364: return 40;
        case 365: return 41;
        case 366: return 42;
        case 367: return 43;
        case 368: return 44;
        case 369: return 45;
        case 371: return 46;
    }
    return -1;
}

