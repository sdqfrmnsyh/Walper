#!/bin/bash
set -e

echo "=========================================="
echo "        Walper Installer"
echo "=========================================="
echo ""

# Cek dependensi dasar
command -v cmake >/dev/null 2>&1 || {
    echo "Error: cmake not found. Please install cmake first."
    echo "  Arch: sudo pacman -S cmake"
    echo "  Debian/Ubuntu: sudo apt install cmake"
    exit 1
}

command -v make >/dev/null 2>&1 || {
    echo "Error: make not found. Please install build tools."
    echo "  Arch: sudo pacman -S base-devel"
    echo "  Debian/Ubuntu: sudo apt install build-essential"
    exit 1
}

# Cek Qt5 (cari qmake versi 5)
if ! command -v qmake &>/dev/null && ! command -v qmake-qt5 &>/dev/null; then
    echo "Error: Qt5 development tools not found."
    echo "  Arch: sudo pacman -S qt5-base"
    echo "  Debian/Ubuntu: sudo apt install qtbase5-dev"
    exit 1
fi

# Dapatkan direktori proyek (dimana script ini berada)
PROJECT_DIR="$(cd "$(dirname "$0")" && pwd)"
BUILD_DIR="$PROJECT_DIR/build"

echo "Project Directory : $PROJECT_DIR"
echo "Build Directory : $BUILD_DIR"
echo "Install prefix   : $HOME/.local"
echo ""

# Konfirmasi jika build sudah ada
if [ -d "$BUILD_DIR" ]; then
    read -p "Build directory already exists. Delete and rebuild? (y/N)" -n 1 -r
    echo
    if [[ $REPLY =~ ^[Yy]$ ]]; then
        echo "Cleaning build directory..."
        rm -rf "$BUILD_DIR"
    else
        echo "Using the existing build directory..."
    fi
fi

# Buat dan masuk ke build directory
mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"

# Konfigurasi CMake
echo ""
echo "[1/3] Configuring CMake..."
cmake .. -DCMAKE_INSTALL_PREFIX="$HOME/.local"

# Build
echo ""
echo "[2/3] Building Walper..."
make -j$(nproc)

# Install
echo ""
echo "[3/3] Installing Walper..."
make install

# Verifikasi
echo ""
echo "=========================================="
echo "  Installation complete!"
echo "=========================================="
echo ""
echo "  Binary    : ~/.local/bin/walper"
echo "  Desktop   : ~/.local/share/applications/walper.desktop"
echo "  Config    : ~/.config/walper/"
echo "  Script    : ~/.config/walper/hy.sh"
echo ""
echo "  Ensure ~/.local/bin is in your PATH."
echo "  Run 'walper' from the terminal or search for 'Walper' in the application menu."
echo ""
echo "  To uninstall, run the following from this directory:"
echo "    cd $BUILD_DIR && make uninstall"
echo ""
