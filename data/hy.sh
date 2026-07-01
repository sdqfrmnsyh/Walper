#!/bin/bash
# hy.sh - script untuk live wallpaper
WALLPAPER="$1"
if [ -z "$WALLPAPER" ]; then exit 1; fi
# Hentikan proses sebelumnya
pkill -f "xwinwrap.*mpv" 2>/dev/null
# Jalankan wallpaper
xwinwrap -fs -ni -b -nf -un -ov -fdt -- mpv -wid %WID --loop-file=inf --no-audio --no-config --profile=fast --vo=gpu --gpu-context=x11 --hwdec=no --interpolation=no --scale=bilinear --cscale=bilinear --vd-lavc-threads=1 --cache=no --no-sub --no-osc --no-osd-bar --really-quiet "$WALLPAPER" &
