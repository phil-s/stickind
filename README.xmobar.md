I've integrated the modified variant `inds_loop.c` into xmobar to
provide a permanent status display of the indicators.  The output
of the looping variant is written (with per-line output buffering)
to a FIFO, which xmobar then reads from to obtain the current status.

# Xmobar

~/.xmonad/xmonad.xmobar file:
```
Config { --[...]
       , template = "%battery% | %memory% %swap% | %cpu% | %stickind%}{%date% | %hostname%"
       , commands = [ --[...]
                    , Run PipeReader "/path/to/fifo" "stickind"
                    --[...]
                    ]
       --[...]
       }
```

# Xmonad

~/.xmonad/xmonad.hs file:
```
--[...]
main = do
  -- Sticky keys indicator.
  xmproc <- spawnPipe "test -p /path/to/fifo || mkfifo /path/to/fifo; killall -qe /path/to/xkb_inds_loop; stdbuf -oL -eL /path/to/xkb_inds_loop >/path/to/fifo"
  -- Run xmobar
  xmproc <- spawnPipe "killall -qe xmobar; xmobar ~/.xmonad/xmonad.xmobar"
  xmonad $ ewmh defaultConfig {
    --[...]
```
