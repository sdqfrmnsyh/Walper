A simple and modern wallpaper manager for Linux. Supports both static (JPG, PNG, WebP, etc.) and live wallpapers (video/GIF) with a clean Qt5 GUI.

<img width="78" height="20" alt="license-MIT-blue" src="https://github.com/user-attachments/assets/81a7493f-9423-4ff0-815a-b695b7e39c66" />
<img width="86" height="20" alt="status-active-brightgreen" src="https://github.com/user-attachments/assets/ae8fbd5a-a57c-4135-a455-423d40bb6c3e" />

## Walper
<img width="500" height="450" alt="walper" src="https://github.com/user-attachments/assets/4d8a6a86-c030-4c33-ae61-17c72e1d25d5" />

## Features

- 🖼️ **Static wallpapers** – apply images instantly using `feh --bg-fill`
- 🎥 **Live wallpapers** – play video/GIF wallpapers via a custom script (`hy.sh`)
- 📂 **Folder browsing** – add a folder and all supported media files appear sorted by name
- 🔄 **Real‑time update** – folder changes (file added/deleted) are reflected immediately
- 🧹 **Stop live wallpaper** – one button to kill running `xwinwrap`/`mpv` processes
- 🗂️ **History** – remembers the last used folder
- 🖱️ **Context menu** – right‑click to apply or delete a wallpaper file
- 🌐 **Localization** – English & Indonesian (more can be added)
- 📦 **Embedded translations** – no extra files needed at runtime
- 💻 **Easy install/uninstall** – CMake build system with `make install` & `make uninstall`

## Dependencies

- **Qt5** – Widgets module
- **feh** – for setting static wallpapers
- **mpv** – for video/GIF live wallpapers
- **xwinwrap** – to embed mpv on the desktop background (live wallpaper)
- **lrelease** (optional, from `qt5-tools` or `qttools5-dev-tools`) – to compile translations

Install dependencies

Arch Linux

```bash
sudo pacman -S qt5-base feh mpv xwinwrap qt5-tools
```

Debian / Ubuntu

```bash
sudo apt install qtbase5-dev qttools5-dev-tools feh mpv xwinwrap
```

Note: xwinwrap may need to be built from source on some distributions.
If you don't need live wallpapers, you can still use Walper for static images (just ignore the script).

Installation

```bash
git clone https://github.com/yourusername/walper.git
cd walper
mkdir build && cd build
cmake .. -DCMAKE_INSTALL_PREFIX=$HOME/.local
make
make install
```

After installation:

· Binary: ~/.local/bin/walper
· Desktop entry: ~/.local/share/applications/walper.desktop
· Live wallpaper script: ~/.config/walper/hy.sh
· History file: ~/.config/walper/history (auto‑created on first run)

Make sure ~/.local/bin is in your PATH. Restart your desktop environment if you want the application menu entry to appear.

Usage

Launch the application:

```bash
walper
```

or from your desktop menu (Walper).

Adding a folder

1. Click Tambah Folder (Add Folder).
2. In the dialog, double‑click a folder to select it quickly.
3. All supported image and video files will appear in the list.

Setting a wallpaper

· Static image – select a .jpg, .png, .webp… and press Terapkan Wallpaper (Apply Wallpaper) or right‑click → Terapkan.
· Live wallpaper – select a video/GIF file and press Terapkan Wallpaper. The default script ~/.config/walper/hy.sh will be executed with the file path as argument.
· To stop a running live wallpaper, click Hentikan Live Wallpaper (Stop Live Wallpaper).

Language

By default Walper uses your system locale:

· If your locale is Indonesian (id_ID), the interface appears in Indonesian.
· Otherwise it falls back to English.

You can force English by running:

```bash
LANGUAGE=en walper
```

Customising the live wallpaper script

Edit ~/.config/walper/hy.sh. The first argument $1 is the full path to the selected video/GIF.
Example script:

```bash
#!/bin/bash
WALLPAPER="$1"
if [ -z "$WALLPAPER" ]; then exit 1; fi

pkill -f "xwinwrap.*mpv" 2>/dev/null
xwinwrap -g 1920x1080 -ni -s -nf -b -un -argb -fdt -- \
    mpv --no-audio --no-osc --no-osd-bar --loop-file --really-quiet "$WALLPAPER" &
```

Make the script executable: chmod +x ~/.config/walper/hy.sh.

Uninstallation

Run from the build directory:

```bash
cd walper/build
make uninstall
```

This removes the binary, desktop entry, and hy.sh.
To also delete the configuration folder (history, etc.):

```bash
rm -rf ~/.config/walper
```

Building from source (without installing)

```bash
cd walper
mkdir build && cd build
cmake ..
make
./walper
```

Contributing

Pull requests are welcome! For major changes, please open an issue first to discuss what you would like to change.

1. Fork the repository
2. Create your feature branch (git checkout -b feature/amazing-feature)
3. Commit your changes (git commit -m 'Add some amazing feature')
4. Push to the branch (git push origin feature/amazing-feature)
5. Open a Pull Request

License

Distributed under the MIT License. See LICENSE for more information.

---

Enjoy your beautiful desktop! 🌄
