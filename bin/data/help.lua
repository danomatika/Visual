
function visual.help()
print([[

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

ESC: clear current selection

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
end
