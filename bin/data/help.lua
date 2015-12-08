
function help(topic)

if topic == "keys" then
print([[
Visual keys
-----------

MOD -> CMD (Mac) or CTRL (Win & Linux)

MOD + t: toggle whether to show or hide the editor
MOD + r & MOD + 0: switch to REPL (console), if enabled
MOD + 1 to MOD + 9: switch to editor 1 - 9

Editor
------

ARROWS: move cursor
ARROWS + SHIFT: select text

MOD + e: trigger an executeScript event
MOD + b: blow up the cursor
MOD + a: select all text
MOD + a + SHIFT: clear all text
MOD + c: copy selected text
MOD + v: paste text

MOD + s: save file, shows save as dialog if no filename has been set
MOD + d: save as dialog, saves in current path (default: home folder)
MOD + o: open a file via file browser, starts in current path
MOD + -: decrease current editor alpha
MOD + =: increase current editor alpha

MOD + z: undo
MOD + y: redo

ESC: clear current selection

MOD + f: toggle fullscreen
MOD + l: toggle line wrapping
MOD + n: toggle line numbers
MOD + k: toggle auto focus zoom

REPL (Read-Eval-Print-Loop)
---------------------------

MOD + a + SHIFT: clear console
MOD + c + SHIFT: clear console history
UP & DOWN: step through command history (on prompt line)
RETURN: eval current command

File Dialog
-----------

ARROWS: select file to open
RETURN: open/save file
ESC: exit file dialog
]])

	elseif topic == "console" then
print([[
Visual console (or REPL) --------------

If you press MOD + 0 or MOD + R
]])

	else
print([[
Visual documentation
--------------------

More help topics:
help("keys") for keyboard commands for controlling Visual
help("console") for ore help on the console
]])
end


end
