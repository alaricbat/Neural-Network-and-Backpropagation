@echo off
:: 設定編碼為 UTF-8，避免中文路徑或訊息亂碼
chcp 65001 >nul

echo ===================================================
echo [1/3] 清理並建立 arm64-v8a 編譯目錄...
echo ===================================================
if exist build_android_arm64 (
    rmdir /s /q build_android_arm64
)
mkdir build_android_arm64
cd build_android_arm64

echo ===================================================
echo [2/3] 執行 CMake 配置 (使用 NDK 27)...
echo ===================================================
:: %USERPROFILE% 會自動帶入 C:\Users\您的使用者名稱
cmake .. ^
  -G Ninja ^
  -DCMAKE_TOOLCHAIN_FILE="%USERPROFILE%\AppData\Local\Android\Sdk\ndk\27.0.12077973\build\cmake\android.chain.cmake" ^
  -DANDROID_ABI=arm64-v8a ^
  -DANDROID_PLATFORM=android-24 ^
  -DCMAKE_BUILD_TYPE=Release

if %errorlevel% neq 0 (
    echo [錯誤] CMake 配置失敗，請檢查 NDK 路徑或 Ninja 是否安裝！
    pause
    exit /b %errorlevel%
)

echo ===================================================
echo [3/3] 開始編譯...
echo ===================================================
cmake --build . --config Release

if %errorlevel% neq 0 (
    echo [錯誤] 編譯失敗！
    pause
    exit /b %errorlevel%
)

echo ===================================================
echo [成功] Android arm64-v8a 動態庫編譯完成！
echo ===================================================
pause
