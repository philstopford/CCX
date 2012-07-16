// prefs.c

// newprefs now defined in configurations for XCode
// #define newprefs

#ifdef newprefs
#ifndef __APPLE__
// sanity check for newprefs on incompatible arch
#undefine newprefs
#endif
#endif

#ifndef newprefs
#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "resources.h"
#endif

#ifdef __APPLE__ && newprefs
#include "main.h"
#include "prefs.h"
#include "music.h"
#include "soundfx.h"
#include "hiscore.h"
#include "keyselect.h"
#include "level.h"

// CoreFoundation will provide the .plist support
#include <CoreFoundation/CoreFoundation.h>
#include <CoreFoundation/CFPreferences.h>

#include "SDLKeyLookup.h"

// CFPreferences code based on Apple's SDK example
// CFPreferencesCopyAppValue() and CFPreferencesSetAppValue() are the most straightforward way
// for an app to read/write preferences that are per user and per app; they will apply on all 
// machines (on which this user can log in, of course --- for users who are local to a machine,
// the preferences will end up being restricted to that host). These functions also do a search 
// through the various cases; if a preference has been set in a less-specific domain (for 
// instance, "all apps"), its value will be retrieved with this call. This allows globally 
// setting some preference values (which makes more sense for some preferences than others).

// Note that you can read/write any "property list" type in preferences; these are
// CFArray, CFDictionary, CFNumber, CFBoolean, CFData, and CFString.

void LoadPrefsMac() {
	printf("LoadPrefsMac.\n");
	
	CFStringRef appName = CFSTR("Candy Crisis");	
	int number; // used for numeric preference values
	Boolean test;
	
	// music first
	// tested and working
	musicOn = true;
	EnableMusic(true);
    CFStringRef musicKey = CFSTR("Music");
	number = CFPreferencesGetAppIntegerValue(musicKey, appName, &test);
	if (test) {
		// if 0 switch music off
		if (number == 0) {
			musicOn = false;
			EnableMusic(false);
		}
	} else {
		printf("No saved data found. Use defaults.\n");
	}
	CFRelease(musicKey);
	
	// sound next
	// tested and working.
    CFStringRef soundKey = CFSTR("Sound");
	number = CFPreferencesGetAppIntegerValue(soundKey, appName, &test);
	soundOn = true; // default
	if (test) {
		if (number == 0 ) {
			soundOn = false;
		}
	}
	else {
		printf("No saved data found. Use defaults.\n");
	}
	CFRelease(soundKey);
	
	// scores is a struct that effectively is a char[] list and a (long)score list joined together (i.e. 2 CSV in 10 rows).
	// Approach to migrate right now is to create a mutable array
	// Pull names and scores from two separate arrays in the plist. scores are ints. into the mutable array
	// For convenience, we separate names and scores for the .plist. We'll pull in those separate arrays and adjust our
	// struct score based on that data.
	// Tested OK
	
	CFStringRef namesKey = CFSTR("Names");
	CFStringRef scoresKey = CFSTR("Scores");
	if (CFArrayRef namesArray = (CFArrayRef)CFPreferencesCopyAppValue(namesKey, appName)) {
		//		printf("Found names\n");
		if (CFArrayRef scoresArray = (CFArrayRef)CFPreferencesCopyAppValue(scoresKey, appName))
		{
			CFIndex idx;
			int i=0;
			// Each array should only be long enough to hold ten entries right now.
			// We iterate through the names array and the score array to load our struct.
			for (idx = 0; idx < 10; idx++) {
				// Load in the names
				if (!CFStringGetCString((CFStringRef)CFArrayGetValueAtIndex(namesArray, idx), scores[i].name, kNameLength, kCFStringEncodingASCII)) {
					//					printf("No saved data found. Use defaults.\n");
					strcpy(scores[i].name, defaultScores[i].name);
				}
				else {
					//					printf("Found saved data. Using it.\n");
				}
				// Now we load in the scores
				// int should have enough range for the values we are interested in.
				int number;
				if (CFNumberGetValue((CFNumberRef)CFArrayGetValueAtIndex(scoresArray, idx), kCFNumberIntType, &number)) {
					//					printf("Found saved data. Using it.\n");
					scores[i].score = number;
				}
				else {
					//					printf("No saved data found. Use defaults.\n");
					scores[i].score = defaultScores[i].score;
				}
				i++;
			}
			CFRelease(scoresArray);
		}
		CFRelease(namesArray);
	}
	else {
		printf("No saved data found. Use defaults.\n");
		
		for(int i=0; i<10; i++) {
			scores[i].score=defaultScores[i].score;
			strcpy(scores[i].name,defaultScores[i].name);
			printf("Good to here: %d, %s\n", scores[i].score, scores[i].name);
		}
	}
	
	// best is a complex beast of a struct.
	// The first element is a [6][13] int array, grid (sizes are defined in main.h)
	
	for (int row=0; row<6; row++) {
		char combinedstring[13];
		strcpy(combinedstring, "Combo Row ");
		char integerstring[3];
		sprintf(integerstring, "%d", row);
		strcat(combinedstring, integerstring);
		//		printf("%s\n", combinedstring);
		CFStringRef comboGridRow = CFStringCreateWithCString(NULL, combinedstring, kCFStringEncodingASCII);
		if (CFArrayRef comboGridRowArray = (CFArrayRef)CFPreferencesCopyAppValue(comboGridRow, appName)) {
			CFIndex idx;
			int i=0;
			for (idx = 0; idx < 13; idx++) {
				int number;
				CFNumberGetValue((CFNumberRef)CFArrayGetValueAtIndex(comboGridRowArray, idx), kCFNumberIntType, &number);
				//				printf("Found saved grid - loading best.grid[%d][%d]\n", row, i);
				best.grid[row][i] = number;
				i++;
			}
			CFRelease(comboGridRowArray);
		}
		else {
			// we failed to open the row array, so we'll need to load defaults
			for (int i=0; i<13; i++) {
				best.grid[row][i] = defaultBest.grid[row][i];
			}
		}
		CFRelease(comboGridRow);
	}
	// Print the grid for comparison.
	for (int row=0; row<6; row++) {
		//		printf("Row %d : ", row);
		for (int col=0; col<13; col++) {
			//			printf("%d ", best.grid[row][col]);
		}
		//		printf("\n");
	}
	
	// There are then 2 int values to load.
	CFStringRef comboa = CFSTR("Combo a");
	if(CFPreferencesCopyAppValue(comboa, appName)) {
		number = CFPreferencesGetAppIntegerValue(comboa, appName, &test);
		//			printf("Setting best.a to %d\n", number);
		if(test)
			best.a = number;
		else
			best.a = 2;
	}
	else {
		//		printf("Applying default to best.a : 2\n");
		best.a = 2; // default
	}		
	CFRelease(comboa);
	
	CFStringRef combob = CFSTR("Combo b");
	if(CFPreferencesCopyAppValue(combob, appName)) {
		number = CFPreferencesGetAppIntegerValue(combob, appName, &test);
		//			printf("Setting best.b to %d\n", number);
		if (test)
			best.b = number;
		else
			best.b = 2;
	}
	else {
		//			printf("Applying default to best.b : 2\n");
		best.b = 2; // default
	}		
	CFRelease(combob);
	
	// Two boolean values....
	CFStringRef combom = CFSTR("Combo m");
	best.m = true;
	number = CFPreferencesGetAppIntegerValue(combom, appName, &test);
	if (test) {
		if ( number == 0) {
			best.m = false;
		}
	}
	CFRelease(combom);
	
	CFStringRef combog = CFSTR("Combo g");
	best.g = true;
	number = CFPreferencesGetAppIntegerValue(combog, appName, &test);
	if (test) {
		if (number == 0) {
			//			printf("Setting best.g : false\n");
			best.g = false;
		}
	}
	CFRelease(combog);
	
	// an int for the level
	CFStringRef comboLv = CFSTR("Combo Level");
	if(CFPreferencesCopyAppValue(comboLv, appName)) {
		number = CFPreferencesGetAppIntegerValue(comboLv, appName, &test);
		if (test) {
			//			printf("Setting best.lv to : %d\n", (int)number);
			best.lv = number;
		}
		else {
			//			printf("Applying default to best.lv : %d\n", (int)kTutorialLevel);
			best.lv = kTutorialLevel;
		}		
	}
	else {
		//		printf("Applying default to best.lv : %d\n", (int)kTutorialLevel);
		best.lv = kTutorialLevel;
	}
	CFRelease(comboLv);
	
	CFStringRef combox = CFSTR("Combo x");
	if(CFPreferencesCopyAppValue(combox, appName)) {
		number = CFPreferencesGetAppIntegerValue(combox, appName, &test);
		if (test) {
			//			printf("Setting best.x to : %d\n", (int)number);
			best.x = number;
		}
		else {
			//			printf("Applying default to best.x : 1\n");
			best.x = 1;
		}		
	}
	else {
		//		printf("Applying default to best.x : 1\n");
		best.x = 1;
	}
	CFRelease(combox);
	
	CFStringRef combor = CFSTR("Combo r");
	if(CFPreferencesCopyAppValue(combor, appName)) {
		number = CFPreferencesGetAppIntegerValue(combor, appName, &test);
		if (test) {
			//			printf("Setting best.r to : %d\n", (int)number);
			best.r = number;
		}
		else {
			//			printf("Applying default to best.r : %d\n", upRotate);
			best.r = upRotate;
		}		
	}
	else {
		//		printf("Applying default to best.r : %d\n", upRotate);
		best.r = upRotate;
	}
	CFRelease(combor);
	
	CFStringRef comboplayer = CFSTR("Combo player");
	if(CFPreferencesCopyAppValue(comboplayer, appName)) {
		number = CFPreferencesGetAppIntegerValue(comboplayer, appName, &test);
		if (test) {
			//			printf("Setting best.player to : %d\n", (int)number);
			best.player = number;
		}
		else {
			//			printf("Applying default to best.player : 0\n");
			best.player = 0;
		}		
	}
	else {
		//		printf("Applying default to best.player : 0\n");
		best.player = 0;
	}
	CFRelease(comboplayer);
	
	// 1 int value for the score
	CFStringRef comboscore = CFSTR("Combo score");
	if(CFPreferencesCopyAppValue(comboscore, appName)) {
		number = CFPreferencesGetAppIntegerValue(comboscore, appName, &test);
		if (test) {
			//			printf("Setting best.value to : %d\n", (int)number);
			best.value = number;
		}
		else {
			//			printf("Applying default to best.value : %d\n", (40*1) + (50*9));
			best.value = (40*1) + (50*9);
		}		
	}
	else {
		//		printf("Applying default to best.value : %d\n", (40*1) + (50*9));
		best.value = (40*1) + (50*9);
	}
	CFRelease(comboscore);
	
	// and then the char array holding the name of the player.
	CFStringRef comboplayerName = CFSTR("Combo player name");
	if(CFPreferencesCopyAppValue(comboscore, appName)) {
		if(!CFStringGetCString((CFStringRef)CFPreferencesCopyAppValue(comboplayerName, appName), best.name, kNameLength, kCFStringEncodingASCII)) {
			//			printf("Setting best.name to : %s\n", best.name);			
			strcpy(best.name, "Tutorial");
		}
	}
	else {
		//		printf("Applying default to best.name : Tutorial\n");
		strcpy(best.name, "Tutorial");
	}
	CFRelease(comboplayerName);
	
	// end of the enormous best struct.
	
	// playerKeys is a [2][4] array.
	// We'll have two separate arrays - one for player 1, and the other for player 2.
	// Load each separately and map into SDLKeys for use in the main program.
	
	CFStringRef player1Keys = CFSTR("Player1Keys");
	if (CFArrayRef keysArray = (CFArrayRef)CFPreferencesCopyAppValue(player1Keys, appName)) {
		//		printf("Loading keys for player1\n");
		CFIndex idx;
		int i=0;
		for (idx = 0; idx < 4; idx++) {
			// SDLkeys are integers. We store integers and map them to SDLKeys at load time.
			//			printf("In player1 keys loop\n");
			int number;
			if (CFNumberGetValue((CFNumberRef)CFArrayGetValueAtIndex(keysArray, idx), kCFNumberIntType, &number)) {
				//				printf("Player1Key%d is: %d\n", i, number);
				playerKeys[0][i] = ASCII_SDLKey(number);
			}
			else {
				//				printf("We blew up in the get value step, defaulting\n");
				playerKeys[0][i] = defaultPlayerKeys[0][i];
			}
			i++;
		}
		CFRelease(keysArray);
	}
	else {
		// defaults
		for (int i=0; i<4; i++) {
			//			printf("No saved data found.\nUsing defaults : player1key%d is %d\n", i, defaultPlayerKeys[0][i]);
			playerKeys[0][i] = defaultPlayerKeys[0][i];
		}
	}
	CFRelease(player1Keys);
	
	CFStringRef player2Keys = CFSTR("Player2Keys");
	if (CFArrayRef keysArray = (CFArrayRef)CFPreferencesCopyAppValue(player2Keys, appName)) {
		//		printf("Loading keys for player2\n");
		CFIndex idx;
		int i=0;
		for (idx = 0; idx < 4; idx++) {
			// SDLkeys are integers. We store integers and map them to SDLKeys at load time.
			//			printf("In player2 keys loop\n");
			int number;
			if (CFNumberGetValue((CFNumberRef)CFArrayGetValueAtIndex(keysArray, idx), kCFNumberIntType, &number)) {
				//				printf("Player2Key%d is: %d\n", i, number);
				playerKeys[1][i] = ASCII_SDLKey(number);
			} else {
				//				printf("We blew up in the get value step, defaulting\n");
				playerKeys[1][i] = defaultPlayerKeys[1][i];
			}			
			i++;
		}
		CFRelease(keysArray);
	}
	else {
		// defaults
		for (int i=0; i<4; i++) {
			//			printf("No saved data found.\nUsing defaults : player2key%d is %d\n", i, defaultPlayerKeys[1][i]);
			playerKeys[1][i] = defaultPlayerKeys[1][i];
		}
	}	
	CFRelease(player2Keys);
	
	
	CFRelease(appName);
	
}

void SavePrefsMac() {
	CFStringRef appName = CFSTR("Candy Crisis");	
	
	// Music first
	// tested OK
	int musicpref = 1;
	if (!musicOn) {
		musicpref = 0;
	}
	CFNumberRef musicRef = CFNumberCreate(NULL, kCFNumberIntType, &musicpref);
	CFPreferencesSetAppValue(CFSTR("Music"), musicRef, appName);
	CFRelease(musicRef);
	
	// Sound
	// tested OK
	int sfx = 1;
	if (!soundOn) {
		sfx = 0;
	}
	CFNumberRef soundRef = CFNumberCreate(NULL, kCFNumberIntType, &sfx);
	CFPreferencesSetAppValue(CFSTR("Sound"), soundRef, appName);
	CFRelease(soundRef);
	
	// Scores. We need to dump our struct containing the hiscore information back to our separate arrays.
	// We have 10 fields, so make the arrays the appropriate size
	// Tested OK.
	CFMutableArrayRef scoresArray = CFArrayCreateMutable(NULL, 10, NULL);
	CFMutableArrayRef namesArray = CFArrayCreateMutable(NULL, 10, NULL);
	
	for (int i = 0; i < 10; i++) {
		// convert our struct data into something we can use
		int scoretemp;
		scoretemp=(int)scores[i].score;
		const char* nametemp = scores[i].name;
		//		printf("score being written is %d\n", scoretemp);
		//		printf("name being written is %s\n", nametemp);
		// now perform second round of conversion to allow data to be used with the arrays
		// printfs for debugging purposes.
		CFStringRef savename = CFStringCreateWithCString(NULL, nametemp, kCFStringEncodingASCII);
		CFNumberRef savescore = CFNumberCreate(NULL, kCFNumberIntType, &scoretemp);
		// now load data into the arrays by appending.
		CFArrayAppendValue(namesArray, savename);
		CFArrayAppendValue(scoresArray, savescore);
		//		if (savename)
		//			CFRelease(savename);
		//		if (savescore)
		//			CFRelease(savescore);
	}
	// Write out to the preference fields
	CFPreferencesSetAppValue(CFSTR("Names"), namesArray, appName);
	//	printf("Good to here: set names pref\n");
	CFPreferencesSetAppValue(CFSTR("Scores"), scoresArray, appName);
	//	printf("Good to here: set scores pref\n");
	// Write to disk
	(void)CFPreferencesAppSynchronize(appName);
	//	printf("Prefs dump");
	// release resources no longer needed.
	CFRelease(scoresArray);
	CFRelease(namesArray);
	
	// Keys get saved next.
	// Saves numeric values. Would be nicer to save SDL_blah values, but for now, this will do.
	CFMutableArrayRef player1keysArray = CFArrayCreateMutable(NULL, 4, NULL);
	CFMutableArrayRef player2keysArray = CFArrayCreateMutable(NULL, 4, NULL);
	CFIndex idx;
	int i=0;
	for (idx = 0; idx < 4; idx++) {
		char key1string[10];
		sprintf(key1string, "%d", playerKeys[0][i]);
		int keyvalue = atoi(key1string);
		//		printf("Player1Key%d is going to be stored as: %d\n", i, keyvalue);
		CFNumberRef savekey = CFNumberCreate(NULL, kCFNumberIntType, &keyvalue);
		//		printf("savekey created\n");
		CFArraySetValueAtIndex(player1keysArray, idx, savekey);
		//		printf("value set\n");
		char key2string[10];
		sprintf(key2string, "%d", playerKeys[1][i]);
		keyvalue = atoi(key2string);
		//		printf("Player2Key%d is going to be stored as: %d\n", i, keyvalue);
		savekey = CFNumberCreate(NULL, kCFNumberIntType, &keyvalue);
		//		printf("savekey created\n");
		CFArraySetValueAtIndex(player2keysArray, idx, savekey);
		//		printf("value set\n");
		i++;
	}
	CFPreferencesSetAppValue(CFSTR("Player1Keys"), player1keysArray, appName);
	CFPreferencesSetAppValue(CFSTR("Player2Keys"), player2keysArray, appName);
	(void)CFPreferencesAppSynchronize(appName);
	CFRelease(player1keysArray);
	CFRelease(player2keysArray);
	
	// Now for the best struct....
	
	// start with our 6 rows of grid data.....
	// Tested and working!
	
	for (int row=0; row<6; row++) {
		char combinedstring[13];
		strcpy(combinedstring, "Combo Row ");
		char integerstring[3];
		sprintf(integerstring, "%d", row);
		strcat(combinedstring, integerstring);
		//		printf("%s\n", combinedstring);
		CFStringRef comboGridRow = CFStringCreateWithCString(NULL, combinedstring, kCFStringEncodingASCII);
		CFMutableArrayRef comboGridRowArray = CFArrayCreateMutable(NULL, 13, NULL);
		for (int i=0; i < 13; i++) {
			// convert our struct data into something we can use
			int gridtemp;
			gridtemp=(int)best.grid[row][i];
			// now perform second round of conversion to allow data to be used with the arrays
			// printfs for debugging purposes.
			CFNumberRef savegrid = CFNumberCreate(NULL, kCFNumberIntType, &gridtemp);
			//			printf("Good to here: savegrid is %d\n", gridtemp);
			// now load data into the arrays by appending.
			CFArrayAppendValue(comboGridRowArray, savegrid);
			//			printf("Good to here: appended %s to %s\n", savegrid, comboGridRowArray);
			// release resources we no longer need.
			//			printf("About to release resources\n");
			CFRelease(savegrid);
			//			printf("Resources released\n");
		}
		// Write out to the preference fields
		CFPreferencesSetAppValue(comboGridRow, comboGridRowArray, appName);
		//		printf("Good to here: set combo row %d pref\n", row);
		CFRelease(comboGridRowArray);
		CFRelease(comboGridRow);
	}
	// Print the grid for comparison.
	for (int row=0; row<6; row++) {
		printf("Row %d : ", row);
		for (int col=0; col<13; col++) {
			printf("%d ", best.grid[row][col]);
		}
		printf("\n");
	}	
	// End of combo Grid block.
	
	// best. struct numerical values
	int combotemp;
	combotemp=(int)best.a;
	CFNumberRef comboval = CFNumberCreate(NULL, kCFNumberIntType, &combotemp);
	CFPreferencesSetAppValue(CFSTR("Combo a"), comboval, appName);
	combotemp=(int)best.b;
	comboval = CFNumberCreate(NULL, kCFNumberIntType, &combotemp);
	CFPreferencesSetAppValue(CFSTR("Combo b"), comboval, appName);
	combotemp=(int)best.lv;
	comboval = CFNumberCreate(NULL, kCFNumberIntType, &combotemp);
	CFPreferencesSetAppValue(CFSTR("Combo Lv"), comboval, appName);
	combotemp=(int)best.x;
	comboval = CFNumberCreate(NULL, kCFNumberIntType, &combotemp);
	CFPreferencesSetAppValue(CFSTR("Combo x"), comboval, appName);
	CFRelease(comboval);
	combotemp=(int)best.r;
	comboval = CFNumberCreate(NULL, kCFNumberIntType, &combotemp);
	CFPreferencesSetAppValue(CFSTR("Combo r"), comboval, appName);
	combotemp=(int)best.player;
	comboval = CFNumberCreate(NULL, kCFNumberIntType, &combotemp);
	CFPreferencesSetAppValue(CFSTR("Combo player"), comboval, appName);
	combotemp=(int)best.value;
	comboval = CFNumberCreate(NULL, kCFNumberIntType, &combotemp);
	CFPreferencesSetAppValue(CFSTR("Combo score"), comboval, appName);
	combotemp=(int)best.m;
	comboval = CFNumberCreate(NULL, kCFNumberIntType, &combotemp);
	CFPreferencesSetAppValue(CFSTR("Combo m"), comboval, appName);
	combotemp=(int)best.g;
	comboval = CFNumberCreate(NULL, kCFNumberIntType, &combotemp);
	CFPreferencesSetAppValue(CFSTR("Combo g"), comboval, appName);
	
	// combo name is a string
	const char* nametemp = best.name;
	CFStringRef comboname = CFStringCreateWithCString(NULL, nametemp, kCFStringEncodingASCII);
	CFPreferencesSetAppValue(CFSTR("Combo player name"), comboname, appName);
	CFRelease(comboname);
	CFRelease(comboval);
	
    // Without an explicit synchronize, the saved values actually do not get written out.
    // If you are writing multiple preferences, you might want to sync only after the last one.
    // A preference panel might want to synchronize when the user hits "OK".
    // In some cases you might not want to sync at all until the app quits.
    // The AppKit automatically synchronizes on app termination, so Cocoa apps don't need to do this.
	
	(void)CFPreferencesAppSynchronize(appName);
	CFRelease(appName);
}

#endif

// Old preferences.txt code below here.
// NOTE THAT NONE OF THIS CODE IS ENDIAN-SAVVY.
// PREFERENCES FILES WILL NOT TRANSFER BETWEEN PLATFORMS.
#ifndef newprefs

#define kPrefsMaxSize 65536

PrefList prefList[] = {
	{ 'mod ', &musicOn,                    sizeof( MBoolean       ) },
	{ 'sfx ', &soundOn,                    sizeof( MBoolean       ) },
	/*		{ 'size', registeredKey,               sizeof( registeredKey  ) },*/
	{ 'keys', playerKeys,                  sizeof( playerKeys     ) },
	{ 'high', scores,                      sizeof( scores         ) },
	{ 'cmbx', &best,                       sizeof( best           ) }/*,
																	  { 'user', registeredName,              sizeof( registeredName ) }*/
};

#define kPrefListSize (sizeof(prefList)/sizeof(prefList[0]))
#endif

// Unless we call LoadPrefsMac, this will load the preferences from a file in the System Folder:Preferences.

void LoadPrefs( void )
{	
#ifdef newprefs && __APPLE__
	LoadPrefsMac();
#else
	FILE *F;
	int fileSize, count, digitsLeft;
	unsigned char info, *infoAt, *dataAt, *fileData;
	
	F = fopen( QuickResourceName( "Preferences", 0, ".txt" ), "r" );
	
	if( F != NULL )
	{
		fileData = (unsigned char*) calloc( 1, kPrefsMaxSize );
		if( fileData != NULL )
		{
			fileSize = fread( fileData, 1, kPrefsMaxSize, F );
			if( fileSize >= 0 )
			{
				for( count=0; count<kPrefListSize; count++ )
				{
					infoAt = FindPrefsLine( fileData, fileSize, prefList[count].itemName, prefList[count].size );
					if( infoAt )
					{
						dataAt = (unsigned char*) prefList[count].itemPointer;
						digitsLeft = prefList[count].size;
						
						while( digitsLeft-- )
						{
							info  = ((*infoAt >= 'A')? (*infoAt - 'A' + 0xA): (*infoAt - '0')) << 4;
							infoAt++;
							info |= ((*infoAt >= 'A')? (*infoAt - 'A' + 0xA): (*infoAt - '0'));
							infoAt++;
							
							*dataAt++ = info;
						}
					}
				}
			}
			
			free( fileData );
		}
		
		fclose( F );
	}
#endif
}

// Finds a specific line in the prefs.
#ifndef newprefs
unsigned char* FindPrefsLine( unsigned char *prefsText, long prefsLength, long searchCode, long dataQuantity )
{
	unsigned char *prefsAt, *check, *endCheck;
	
	for( prefsAt = prefsText; prefsAt < (prefsText+prefsLength-3); prefsAt++ )
	{
		if( (prefsAt[0] == ((searchCode >> 24) & 0xFF)) &&
		   (prefsAt[1] == ((searchCode >> 16) & 0xFF)) &&
		   (prefsAt[2] == ((searchCode >>  8) & 0xFF)) &&
		   (prefsAt[3] == ((searchCode      ) & 0xFF))    ) 
		{
			prefsAt += 6;
			
			// perform sizing check
			
			dataQuantity *= 2; // hexadecimal bytes are 2 chars
			
			if( ((prefsAt + dataQuantity) - prefsText) > prefsLength ) return NULL; // prefs block ended too early
			
			check = prefsAt;
			endCheck = check + dataQuantity;
			while( check < endCheck )
			{
				if( (*check < '0' || *check > '9') && (*check < 'A' || *check > 'F') )
				{
					return NULL; // incorrect size, too short
				}
				
				check++;
			}
			
			if( (*endCheck >= '0' && *endCheck <= '9') || (*endCheck >= 'A' && *endCheck <= 'F') )
			{
				return NULL; // incorrect size, too long
			}
			
			return prefsAt;
		}
	}
	
	return NULL;
}
#endif

// Unless we call SavePrefsMac(), this saves out preferences into a file.

void SavePrefs( void )
{
#ifdef newprefs && __APPLE__
	SavePrefsMac();
#else
	FILE *F;
	short count, size;
	unsigned char* dataAt;
	
	F = fopen( QuickResourceName( "Preferences", 0, ".txt" ), "w" );
	
	if( F != NULL )
	{
		for( count=0; count<kPrefListSize; count++ )
		{
			fprintf( F, "%c%c%c%c: ", (prefList[count].itemName >> 24) & 0xFF, 
					(prefList[count].itemName >> 16) & 0xFF,
					(prefList[count].itemName >>  8) & 0xFF,
					(prefList[count].itemName      ) & 0xFF   );
			
			dataAt = (unsigned char*) prefList[count].itemPointer;
			for( size=0; size<prefList[count].size; size++ )
			{
				fprintf( F, "%02X", *dataAt );
				dataAt++;
			}
			
			fputc( '\n', F );
		}
	}
	
	fclose( F );
#endif
}
