# Intro

When using sticky keys, no existing method of checking the currently active
modifiers worked for my desktop environment.

Happily https://github.com/marf41/stickind provided all of the key functionality
needed to do this, so I've adapted that code to create a version which which
continually polls the state of the modifier keys, and outputs any changes.

My xmobar configuration then tracks that output, and displays the current state.

# Sticky keys config

Example:
```
# - Enable "sticky" modifier keys (for all modifiers)
# - Enable "latchlock" (lock any modifier by pressing it twice in a row)
# - Disable "twokey" (permanently disable sticky keys by pressing any
#   modifier and another key at the same time)
# - Disable "accessx" (toggle sticky keys support by pressing Shift 5 times)
# - (If needed) Disable "bell"/"feedback" (audio feedback for current state)
xkbset sticky latchlock -twokey -accessx # -bell -feedback

# Disable expiration of "sticky keys" after the specified time in
# seconds. When expiration is enabled, sticky keys are permanently
# disabled after that many seconds. This is quite unhelpful.
xkbset exp 64 =sticky =latchlock =twokey =accessx
```

# Modifier names

- AltGr
- CapsLock
- Ctrl
- Meta
- Shift
- Super

# Example output

- `-`: no modifiers
- `Ctrl-`: Ctrl active for the next keypress
- `Meta-Ctrl-`: Both Meta and Ctrl active for the next keypress
- `- ++ Ctrl-`: Ctrl locked on
- `Shift- ++ Super-Ctrl-`: Shift active for next keypress; Super and Ctrl both locked.

# Xmobar

I've integrated the modified variant `inds_loop.c` into xmobar to provide a
permanent status display of the indicators.  The output of the looping variant
is written (with per-line output buffering) to a FIFO, which xmobar then reads
from to obtain the current status.

~/.xmonad/xmonad.xmobar file:
```
Config { --[...]
       , template = "%battery% | %memory% %swap% | %cpu% | %modifiers%}{%date% | %hostname%"
       , commands = [ --[...]
                    , Run PipeReader "/path/to/fifo" "modifiers"
                    --[...]
                    ]
       --[...]
       }
```

# Xmonad

Crucially, the `stdbuf` utility is used to employ line buffering for
the output, so that we don't need to wait for a large amount of output
before the FIFO can be read.

~/.xmonad/xmonad.hs file:
```
--[...]
main = do
  -- Sticky keys indicator.
  xmproc <- spawnPipe "test -p /path/to/fifo || mkfifo /path/to/fifo; \
                      \killall -qe xkb_inds_loop; \
                      \stdbuf -oL -eL /path/to/xkb_inds_loop >/path/to/fifo"
  -- Run xmobar
  xmproc <- spawnPipe "killall -qe xmobar; \
                      \xmobar ~/.xmonad/xmonad.xmobar"
  xmonad $ ewmh defaultConfig {
    --[...]
  }
```
