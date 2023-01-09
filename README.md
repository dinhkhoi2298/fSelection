# fSelection

[![sampctl](https://img.shields.io/badge/sampctl-fSelection-2f2f2f.svg?style=for-the-badge)](https://github.com/roesanne26/fSelection)

![](https://media.discordapp.net/attachments/759415958937796649/766296419204071434/unknown.png)![](https://media.discordapp.net/attachments/759415958937796649/766296582576144384/unknown.png)
<!--
Short description of your library, why it's useful, some examples, pictures or
videos. Link to your forum release thread too.

Remember: You can use "forumfmt" to convert this readme to forum BBCode!

What the sections below should be used for:

`## Installation`: Leave this section un-edited unless you have some specific
additional installation procedure.

`## Testing`: Whether your library is tested with a simple `main()` and `print`,
unit-tested, or demonstrated via prompting the player to connect, you should
include some basic information for users to try out your code in some way.

And finally, maintaining your version number`:

* Follow [Semantic Versioning](https://semver.org/)
* When you release a new version, update `VERSION` and `git tag` it
* Versioning is important for sampctl to use the version control features

Happy Pawning!
-->

## Installation

Simply install to your project:

```bash
sampctl package install roesanne26/fSelection
```

Include in your code and begin using the library:

```pawn
#include <fSelection>
```

### fSelection-pp.inc
- Unlimited item

(require https://github.com/IllidanS4/PawnPlus)
## Usage

## Defines

```pawn
#define FSELECTION_AUTO_HIDE  // Hide the fSelection menu after model selection. (Useful for people moving from mSelection)
#include <fSelection>
```

Make sure its defined before the include!


### Functions
```pawn
fselection_add_item(playerid, modelid, Float:rotx, Float:roty, Float:rotz, Float:zoom);
fselection_show(playerid, fselectid, const header[], page);
fselection_showing(playerid);
fselection_hide(playerid);

// fSelection-pp.inc
async_fselection_show(playerid, const header[]);
```
#### Example #1
```pawn
// 
#include <fSelection>

const FSELECTION_CLOTHES = 0;

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

public OnFSelectionResponse(playerid, fselectid, modelid, response) {	
	if(fselectid == FSELECTION_CLOTHES && response) {
		SetPlayerSkin(playerid, modelid);	
	}
	return 1;
}
```
#### Example #2
Using `async_fselection_show`
```pawn
/* fSelection-pp test */
#include <fSelection-pp>

CMD:clothes(playerid) {
    if(fselection_showing(playerid)) 
        return SendClientMessage(playerid, -1, "SU DUNG ESC DE TAT.");

    new 
        str[64], fSelectionInfo[E_FSELECTION_INFO];

    task_yield(1);
    for(new i = 0; i < 311; i ++) {
        if(i == 74) 
            continue; // bad id

        fselection_add_item(playerid, i);
    }
    task_await_arr(async_fselection_show(playerid, "Clothes"), fSelectionInfo);
    if(fSelectionInfo[fs_response] == fS_RESPONSE_CLICKED) {
        SetPlayerSkin(playerid, fSelectionInfo[fs_modelid]);
        format(str, 64, "Bro da thay trang phuc %d", fSelectionInfo[fs_modelid]);
    }
    else strcat(str, "Bro da tat fSelection", 64);

    SendClientMessage(playerid, -1, str);
    return 1;
}

```

## Testing

To test, simply run the package:

```bash
sampctl package run
```
## Credits
jav - rfogkid26    
