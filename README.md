<img width="86" height="20" alt="status-active-brightgreen" src="https://github.com/user-attachments/assets/75f2ce16-447e-45ce-b072-5e81dae49c20" />Walper

A simple and modern wallpaper manager for Linux. Supports both static (JPG, PNG, WebP, etc.) and live wallpapers (video/GIF) with a clean Qt5 GUI.

![Uploading status-active-brightgreen.svg…]<svg xmlns="http://www.w3.org/2000/svg" width="86" height="20" role="img" aria-label="status: active"><title>status: active</title><filter id="blur"><feGaussianBlur stdDeviation="16"/></filter><linearGradient id="s" x2="0" y2="100%"><stop offset="0" stop-color="#bbb" stop-opacity=".1"/><stop offset="1" stop-opacity=".1"/></linearGradient><clipPath id="r"><rect width="86" height="20" rx="3"/></clipPath><g clip-path="url(#r)"><rect width="43" height="20" fill="#555"/><rect x="43" width="43" height="20" fill="#4b0"/><rect width="86" height="20" fill="url(#s)"/></g><g fill="#fff" text-anchor="middle" font-family="Verdana,Geneva,DejaVu Sans,sans-serif" text-rendering="geometricPrecision" font-size="110"><g transform="scale(.1)"><g aria-hidden="true" fill="#010101"><text x="225" y="150" fill-opacity=".8" filter="url(#blur)" textLength="330">status</text><text x="225" y="150" fill-opacity=".3" textLength="330">status</text></g><text x="225" y="140" textLength="330">status</text></g><g transform="scale(.1)"><g aria-hidden="true" fill="#010101"><text x="635" y="150" fill-opacity=".8" filter="url(#blur)" textLength="330">active</text><text x="635" y="150" fill-opacity=".3" textLength="330">active</text></g><text x="635" y="140" textLength="330">active</text></g></g></svg>()
![Uploading license<svg xmlns="http://www.w3.org/2000/svg" width="78" height="20" role="img" aria-label="license: MIT"><title>license: MIT</title><filter id="blur"><feGaussianBlur stdDeviation="16"/></filter><linearGradient id="s" x2="0" y2="100%"><stop offset="0" stop-color="#bbb" stop-opacity=".1"/><stop offset="1" stop-opacity=".1"/></linearGradient><clipPath id="r"><rect width="78" height="20" rx="3"/></clipPath><g clip-path="url(#r)"><rect width="47" height="20" fill="#555"/><rect x="47" width="31" height="20" fill="#007ec6"/><rect width="78" height="20" fill="url(#s)"/></g><g fill="#fff" text-anchor="middle" font-family="Verdana,Geneva,DejaVu Sans,sans-serif" text-rendering="geometricPrecision" font-size="110"><g transform="scale(.1)"><g aria-hidden="true" fill="#010101"><text x="245" y="150" fill-opacity=".8" filter="url(#blur)" textLength="370">license</text><text x="245" y="150" fill-opacity=".3" textLength="370">license</text></g><text x="245" y="140" textLength="370">license</text></g><g transform="scale(.1)"><g aria-hidden="true" fill="#010101"><text x="615" y="150" fill-opacity=".8" filter="url(#blur)" textLength="210">MIT</text><text x="615" y="150" fill-opacity=".3" textLength="210">MIT</text></g><text x="615" y="140" textLength="210">MIT</text></g></g></svg>-MIT-blue.svg…]()


<img width="500" height="450" alt="walper" src="https://github.com/user-attachments/assets/913755b0-944f-4c30-a08e-28cb17c86506" />


Features

• 🖼️ Static wallpapers – apply images instantly using feh --bg-fill
• 🎥 Live wallpapers – play video/GIF wallpapers via a custom script (hy.sh)
· 📂 Folder browsing – add a folder and all supported media files appear sorted by name
· 🔄 Real‑time update – folder changes (file added/deleted) are reflected immediately
· 🧹 Stop live wallpaper – one button to kill running xwinwrap/mpv processes
· 🗂️ History – remembers the last used folder
· 🖱️ Context menu – right‑click to apply or delete a wallpaper file
· 🌐 Localization – English & Indonesian (more can be added)
· 📦 Embedded translations – no extra files needed at runtime
· 💻 Easy install/uninstall – CMake build system with make install & make uninstall

Dependencies

· Qt5 – Widgets module
· feh – for setting static wallpapers
· mpv – for video/GIF live wallpapers
· xwinwrap – to embed mpv on the desktop background (live wallpaper)
· lrelease (optional, from qt5-tools or qttools5-dev-tools) – to compile translations

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
