Candy Crisis OSX-x86
====================

Notes:
------
There is more information in the _ReadMe_ file, which was written by the original author.
main.cpp used to have a huge comment block full of historical information. This was migrated to 'Original_main-cpp_info'
Version History is the release history from the original project.
ChangeLog contains the current log of activity on the OSX-x86 project.

What this is:
-------------

Candy Crisis has, until now, only been available as a PowerPC executable for OS X (the only x86 version released was for Windows).
To play it on an Intel Mac required Rosetta, which was unfortunate for those of us wanting to play it whilst on the road - batteries went flat very quickly and machines were too hot to place on laps.
Candy Crisis OSX-x86 changes this. The starting point was the Win32 SDL sources, along with a patch cc-mac-linux.patch that was posted on the candycrisis sourceforge project.

From there, it was a matter of getting SDL, and the various libraries compiled, and then bringing the show into XCode. From there, the Shareware code and registration stuff was ripped out, so you won't need any registration code. The file-handling was changed to work with the new .app bundle's resource system (localisation ahoy!) and OS X preference support was added (prefs now stored in each user's preferences folder in a .plist).

The work is continuing and this readme will be updated as the program nears completion. Your effort in testing this project is appreciated.

GPL applies. Copyrights are diverse, with the original coder being John Stiles. The conversion noted above and in the ChangeLog was performed by Philip Stopford.

---

            Candy Crisis
                                                        by John Stiles

- about the game...

Recently, a mad scientist at a candy factory unveiled his greatest creation: a batch of living candy, brought to life through the miracles of genetic mutation. Unfortunately, his formula had a fatal flaw--the living candy uncontrollably multiplies by the thousands! Within minutes, the plant began to overflow with the genetically altered candy. Luckily, a quick-thinking researcher discovered how to fend off the rapidly multiplying creatures. The mutated candy became extremely reactive when grouped together--and when four of the same color touched, they instantly vaporized!  

Candy Crisis is an exciting combination of pure action and puzzle gaming. The game features twelve unique stages, complete with its own character and individual technique. In One Player mode, these dynamic computer opponents can react to your style of gameplay and quickly learn to adjust their tactics to match yours. In Two Player mode, you can pair off against a friend, and even specify a handicap so that both players are evenly matched. And if you want to practice, then  Solitaire Crisis mode allows you to play without having to compete against an opponent. A tutorial is included with the game as well, so even puzzle novices can become Candy Crisis champions in no time.  

- how to play...

When you begin the game, candies start falling from the sky, two at a time. Use the arrow keys to maneuver them left and right. Pressing the up arrow will cause the candies to rotate around each other, and pressing the down arrow will cause the candy to drop faster. 

Candies of identical color will stick together when they are next to each other. When four candies of the same color are touching, they will vaporize! 

The object of the game is to keep the candy from overflowing the top of your board. To achieve this goal, you need to continually vaporize candy! In One Player mode, you compete against the computer for survival--your board is on the left side of the screen, and the computer's is on the right. In Solitaire Crisis, you don't have an opponent; you only need to keep the candy from stacking up to the top of the screen.

For more gameplay details, please read the Candy Crisis in-game tutorial. 

- how to register...

Candy Crisis is free! Here is a valid registration code that you can use:
	
				User Name:	 Everybody
				Code:            WPTV-CRHV

Thanks for playing Candy Crisis!
