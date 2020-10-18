/*
	* fSelection-pp (* Unlimited item)

    Callback: 
		OnFSelectionResponse(playerid, fselectid, modelid, response)

    Functions: 
        fselection_add_item(playerid, modelid, Float:rotx, Float:roty, Float:rotz, Float:zoom)
		fselection_show(playerid, fselectid, header[], page)
		fselection_showing(playerid)
		fselection_hide(playerid)
*/

#if defined _fSelection_included
	#endinput
#endif
#define _fSelection_included

#include <a_samp>
#include <PawnPlus>

const 
	fS_ITEM_PER_PAGE = 6,
	fS_MAX_HEADER_LEN = 16,
	INVALID_FSELECTION_ID = 0xFFFF;

const 
	fS_RESPONSE_ESC = 0,
	fS_RESPONSE_CLICKED = 1;

// setting sound
const
	fSS_NEXT_PAGE = 1145,
	fSS_PREV_PAGE = 1145,
	fSS_CLICK = 1145,
	fSS_SHOW = 1130,
	fSS_HIDE = 1136;

// Custom postion
const 
	Float:fS_add_pos_X = 25.0,
	Float:fS_add_pos_Y = 5.0;

enum _:E_FSELECTION {
	fs_show,
	fs_cur_page,
	Task:fs_task,
	fs_fselection_id,
	PlayerText:fs_page_td,
	PlayerText:fs_header_td,
	PlayerText:fs_item_td[fS_ITEM_PER_PAGE],
}

enum _:E_FSELECTION_DATA {
	fs_item_modelid,
	Float:fs_item_rotx,
	Float:fs_item_roty,
	Float:fs_item_rotz,
	Float:fs_item_zoom,
}

enum _:E_FSELECTION_INFO {
	fs_modelid,
	fs_response,
}

static 
	Text:fS_button_td[2],
	fSelection[MAX_PLAYERS][E_FSELECTION],
	List:fS_item_lists[MAX_PLAYERS];

forward OnFSelectionResponse(playerid, fselectid, modelid, response);

stock fselection_add_item(const playerid, const modelid, Float:rotx = 0.0, Float:roty = 0.0, Float:rotz = 0.0, Float:zoom = 1.0) {
	if(modelid == 0) 
		return 0;

	new 
		Info[E_FSELECTION_DATA];
	
	Info[fs_item_modelid] = modelid;	
	Info[fs_item_rotx] = rotx;
	Info[fs_item_roty] = roty;
	Info[fs_item_rotz] = rotz;
	Info[fs_item_zoom] = zoom;

	return list_add_arr(fS_item_lists[playerid], Info);
}

stock fselection_show_index(const playerid, const index, const modelid, Float:rotx = 0.0, Float:roty = 0.0, Float:rotz = 0.0, Float:zoom = 1.0, const bool:show = false) {
	if(fSelection[playerid][fs_item_td][index] != PlayerText:INVALID_TEXT_DRAW) {
		PlayerTextDrawDestroy(playerid, fSelection[playerid][fs_item_td][index]);
		fSelection[playerid][fs_item_td][index] = PlayerText:INVALID_TEXT_DRAW;
	}

    new 
        Float:X, Float:Y, Float:yDefault = 150.644409, Float:xDefault = 470.0;
    
    X = (!index) ? xDefault : index < 2 ? (xDefault + index * 56.55) : index < 4 ? ((X = xDefault) + (index - 2) * 56.55) : (xDefault) + (index - floatround(fS_ITEM_PER_PAGE / 1.5)) * 56.55;
    Y = index < (fS_ITEM_PER_PAGE / 3) ? (yDefault) : index < floatround(fS_ITEM_PER_PAGE / 1.5) ? yDefault + 59.55 : yDefault + 59.55 * 2;	

	fSelection[playerid][fs_item_td][index] = CreatePlayerTextDraw(playerid, X + fS_add_pos_X, Y + fS_add_pos_Y, #);
	PlayerTextDrawSetPreviewRot(playerid, fSelection[playerid][fs_item_td][index], rotx, roty, rotz, zoom);
	PlayerTextDrawTextSize(playerid, fSelection[playerid][fs_item_td][index], 56.000000, 59.000000);
	PlayerTextDrawBackgroundColor(playerid, fSelection[playerid][fs_item_td][index], 168430280);
	PlayerTextDrawSetPreviewModel(playerid, fSelection[playerid][fs_item_td][index], modelid);
	PlayerTextDrawSetSelectable(playerid, fSelection[playerid][fs_item_td][index], true);
	PlayerTextDrawSetShadow(playerid, fSelection[playerid][fs_item_td][index], 0);
	PlayerTextDrawColor(playerid, fSelection[playerid][fs_item_td][index], -1);
	PlayerTextDrawFont(playerid, fSelection[playerid][fs_item_td][index], 5);
	if(show) 
		PlayerTextDrawShow(playerid, fSelection[playerid][fs_item_td][index]);
	
	return 1;
}

stock fselection_destroy_index(const playerid, const index) {
	if(fSelection[playerid][fs_item_td][index] != PlayerText:INVALID_TEXT_DRAW) {
		PlayerTextDrawDestroy(playerid, fSelection[playerid][fs_item_td][index]);
		fSelection[playerid][fs_item_td][index] = PlayerText:INVALID_TEXT_DRAW;
	}		
}

stock fselection_show(const playerid, const fselectid, const header[] = "", const page = 0, bool:reshow = false) {
	if(fselectid != (-0xF)) if(fselectid < 0) return print(("FSELECTION: XAY RA LOI."));

	new 
		str[12], index = 0, i = page * 6, Info[E_FSELECTION_DATA];
	
	for(; i < fS_ITEM_PER_PAGE * (page + 1); i++) {
		if(i < list_size(fS_item_lists[playerid])) list_get_arr_safe(fS_item_lists[playerid], i, Info);
		else Info[fs_item_modelid] = 0;

		if(Info[fs_item_modelid] != 0) fselection_show_index(playerid, index, Info[fs_item_modelid], Info[fs_item_rotx], Info[fs_item_roty], Info[fs_item_rotz], Info[fs_item_zoom], true);
		else fselection_destroy_index(playerid, index);

		++index;
	}
	if(reshow == false) {
		PlayerTextDrawSetString(playerid, fSelection[playerid][fs_header_td], header);
		PlayerTextDrawShow(playerid, fSelection[playerid][fs_header_td]);
		fSelection[playerid][fs_fselection_id] = fselectid;
		PlayerPlaySound(playerid, fSS_SHOW, 0, 0, 0);
		fSelection[playerid][fs_show] = true;
	}
	SelectTextDraw(playerid, -1);
	fSelection[playerid][fs_cur_page] = page;
	TextDrawShowForPlayer(playerid, Text:GetSVarInt("TD.fSelection::BG"));
	for(i = 0; i < 2; i ++) {
		TextDrawShowForPlayer(playerid, fS_button_td[i]);	
	}
	format(str, 12, "%d/%d", 1 + page, fselection_get_max_page(playerid));
	PlayerTextDrawSetString(playerid, fSelection[playerid][fs_page_td], str);
	PlayerTextDrawShow(playerid, fSelection[playerid][fs_page_td]);
	return 1;
}

stock Task:async_fselection_show(const playerid, const header[]) {
	if(task_valid(fSelection[playerid][fs_task]))
		task_delete(fSelection[playerid][fs_task]);

	fSelection[playerid][fs_task] = task_new();

	fselection_show(playerid, (-0xF), header, 0, false);
	return fSelection[playerid][fs_task];
}


stock fselection_hide(const playerid) {
	CancelSelectTextDraw(playerid);
	list_clear_deep(fS_item_lists[playerid]);
	fSelection[playerid][fs_show] = false;
	for(new i = 0; i < fS_ITEM_PER_PAGE; i++) {
		if(fSelection[playerid][fs_item_td][i] != PlayerText:INVALID_TEXT_DRAW) {
			PlayerTextDrawDestroy(playerid, fSelection[playerid][fs_item_td][i]);
			fSelection[playerid][fs_item_td][i] = PlayerText:INVALID_TEXT_DRAW;
		}
		if(i < 2) TextDrawHideForPlayer(playerid, fS_button_td[i]);	
	}
	PlayerTextDrawHide(playerid, fSelection[playerid][fs_page_td]);
	PlayerTextDrawHide(playerid, fSelection[playerid][fs_header_td]);		
	TextDrawHideForPlayer(playerid, Text:GetSVarInt("TD.fSelection::BG"));
	PlayerPlaySound(playerid, fSS_HIDE, 0, 0, 0);
}

stock fselection_get_max_page(const playerid) {
	new 
		const j = fS_ITEM_PER_PAGE, i = list_size(fS_item_lists[playerid]);

	return floatround(i % j == 0 ? i / j : i / j + 1);
}

stock fselection_showing(const playerid) {
	return fSelection[playerid][fs_show];
}

#include <YSI_Coding\y_hooks>
hook OnGameModeInit() {
	new 
		Text:fselection_td_bg = TextDrawCreate(466.70 + fS_add_pos_X, 130.770355 + fS_add_pos_Y, "LD_SPAC:white");
	
	TextDrawTextSize(fselection_td_bg, 119.000000, 215.000000);
	TextDrawColor(fselection_td_bg, 168430280);
	TextDrawAlignment(fselection_td_bg, 1);
	TextDrawSetShadow(fselection_td_bg, 0);
	TextDrawFont(fselection_td_bg, 4);
	SetSVarInt("TD.fSelection::BG", _:fselection_td_bg);
	for(new i = 0; i < 2; i ++) {
		fS_button_td[i] = TextDrawCreate(541.436096 + i * 24.094116 + fS_add_pos_X, 334.518280 + fS_add_pos_Y, !i ? "LD_BEAT:left" : "LD_BEAT:right");
		TextDrawTextSize(fS_button_td[i], 7.000000, 9.000000);
		TextDrawSetSelectable(fS_button_td[i], true);
		TextDrawColor(fS_button_td[i], -2139062017);
		TextDrawAlignment(fS_button_td[i], 1);
		TextDrawSetShadow(fS_button_td[i], 0);
		TextDrawFont(fS_button_td[i], 4);	
	}
}

hook OnPlayerConnect(playerid) {
	fS_item_lists[playerid] = list_new();
	
	for(new i = 0; i < fS_ITEM_PER_PAGE; i ++) {
		fSelection[playerid][fs_item_td][i] = PlayerText:INVALID_TEXT_DRAW;
	}
	fSelection[playerid][fs_cur_page] = 0;
	fSelection[playerid][fs_header_td] = CreatePlayerTextDraw(playerid, 525.700695 + fS_add_pos_X, 133.588836 + fS_add_pos_Y, #);
	PlayerTextDrawLetterSize(playerid, fSelection[playerid][fs_header_td], 0.324000, 1.487998);
	PlayerTextDrawBackgroundColor(playerid, fSelection[playerid][fs_header_td], 255);
	PlayerTextDrawSetProportional(playerid, fSelection[playerid][fs_header_td], 1);
	PlayerTextDrawAlignment(playerid, fSelection[playerid][fs_header_td], 2);
	PlayerTextDrawSetShadow(playerid, fSelection[playerid][fs_header_td], 0);
	PlayerTextDrawColor(playerid, fSelection[playerid][fs_header_td], -1);
	PlayerTextDrawFont(playerid, fSelection[playerid][fs_header_td], 1);
	
	fSelection[playerid][fs_page_td] = CreatePlayerTextDraw(playerid, 557.000244 + fS_add_pos_X, 334.440826 + fS_add_pos_Y, #);
	PlayerTextDrawLetterSize(playerid, fSelection[playerid][fs_page_td], 0.118333, 0.849184);
	PlayerTextDrawAlignment(playerid, fSelection[playerid][fs_page_td], 2);
	PlayerTextDrawColor(playerid, fSelection[playerid][fs_page_td], -1);
	PlayerTextDrawSetShadow(playerid, fSelection[playerid][fs_page_td], 0);
	PlayerTextDrawBackgroundColor(playerid, fSelection[playerid][fs_page_td], 255);
	PlayerTextDrawFont(playerid, fSelection[playerid][fs_page_td], 2);
	PlayerTextDrawSetProportional(playerid, fSelection[playerid][fs_page_td], 1);	
	fSelection[playerid][fs_show] = 0;
	return 1;
}

hook OnPlayerDisconnect(playerid) {
	list_delete_deep(fS_item_lists[playerid]);
	if(task_valid(fSelection[playerid][fs_task])) task_delete(fSelection[playerid][fs_task]);

	for(new i = 0; i < fS_ITEM_PER_PAGE; i++) {
		if(fSelection[playerid][fs_item_td][i] != PlayerText:INVALID_TEXT_DRAW) {
			PlayerTextDrawDestroy(playerid, fSelection[playerid][fs_item_td][i]);
			fSelection[playerid][fs_item_td][i] = PlayerText:INVALID_TEXT_DRAW;
		}			
	}	
	fSelection[playerid][fs_fselection_id] = INVALID_FSELECTION_ID;
	PlayerTextDrawDestroy(playerid, fSelection[playerid][fs_page_td]);
	PlayerTextDrawDestroy(playerid, fSelection[playerid][fs_header_td]);
	return 1;
}

hook OnPlayerClickTD(playerid, Text:clickedid) {
	if(fSelection[playerid][fs_show]) {
		if(clickedid == Text: 0xFFFF) {
			fselection_hide(playerid);
			if((-0xF) != fSelection[playerid][fs_fselection_id]) {
				CallLocalFunction("OnFSelectionResponse", "dddd", playerid, fSelection[playerid][fs_fselection_id], 0, fS_RESPONSE_ESC);
			}
			else {
				new 
					Info[E_FSELECTION_INFO];

				Info[fs_modelid] = 0;
				Info[fs_response] = fS_RESPONSE_ESC;
				task_set_result_arr(fSelection[playerid][fs_task], Info);
				fSelection[playerid][fs_task] = Task:0;
			}
		}
		else {
			for(new i = 0, maxpage, page, curpage = fSelection[playerid][fs_cur_page]; i < 2; i ++) {
				if(clickedid == fS_button_td[i]) {
					maxpage = fselection_get_max_page(playerid);
					page = (!i) ? (--curpage <= -1 ? maxpage - 1 : curpage) : (++curpage >= maxpage ? 0 : curpage);
					PlayerPlaySound(playerid, (!i) ? fSS_PREV_PAGE : fSS_NEXT_PAGE, 0, 0, 0);
					fselection_show(playerid, fSelection[playerid][fs_fselection_id], .page = page, .reshow = true);
					return ~1; // break
				}
			}
		}
	}
	return 1;
}

hook OnPlayerClickPlayerTD(playerid, PlayerText:playertextid) {
	if(fSelection[playerid][fs_show]) {
		for(new i = 0, index; i < fS_ITEM_PER_PAGE; i ++) {
			if(playertextid == fSelection[playerid][fs_item_td][i]) {		
				index = fS_ITEM_PER_PAGE * fSelection[playerid][fs_cur_page] + i;
				PlayerPlaySound(playerid, fSS_CLICK, 0, 0, 0);
				if(index < list_size(fS_item_lists[playerid])) {
					if((-0xF) != fSelection[playerid][fs_fselection_id]) {
						CallLocalFunction("OnFSelectionResponse", "dddd", playerid, fSelection[playerid][fs_fselection_id], list_get(fS_item_lists[playerid], index, fs_item_modelid), fS_RESPONSE_CLICKED);
					}
					else {
						new 
							Info[E_FSELECTION_INFO];

						Info[fs_response] = fS_RESPONSE_CLICKED;
						Info[fs_modelid] = list_get(fS_item_lists[playerid], index, fs_item_modelid);
						task_set_result_arr(fSelection[playerid][fs_task], Info);
						fSelection[playerid][fs_task] = Task:0;
					}		
				}	
				fselection_hide(playerid);
				return ~1;
			}
		}
	}
	return 1;
}

