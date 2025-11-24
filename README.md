# TeleCloud

![Toolkit: QT](https://img.shields.io/badge/Toolkit-QT-informational?color=41CD52&logo=qt&logoColor=white)  
![Language: C++ / QT](https://img.shields.io/badge/language-C%2B%2B%20%2F%20QT-orange)  

TeleCloud is a high-performance C++/Qt client that transforms a Telegram Channel into unlimited distributed cloud storage for video assets. It supports metadata ingestion, automated queuing, and visual indexing.

It allows users to archive large video libraries (e.g., security feeds, stream VODs) by mapping local metadata to remote Telegram file IDs, effectively using the messenger as a NoSQL Object Store.

---

## 📸 Screenshot

<img width="323" height="166" alt="TeleCloud Interface" src="https://github.com/user-attachments/assets/7eadae61-089b-4821-9d06-82e7b1ba4e96" />

---

## 🚀 Key Features

*   **Metadata-Driven Ingestion:** Parses standardized JSON sidecar files to organize content by "Channel" (Source) and Date.
*   **Visual Indexing (Contact Sheets):** Automatically detects associated image files (contact sheets/previews) in the metadata. These are uploaded as threaded replies to the video message, allowing users to scan content content visually without downloading the full video.
*   **State Tracking:** Maintains a local SQLite database to track upload progress, file IDs, and message links.
*   **Resiliency:** Handles network interruptions and API limits via the local Telegram Bot API server.

---

## 📦 Build Requirements

- Qt **5.12+**
- C++17 compiler  
- CMake

## 🔧 Runtime Requirements
 
1. Local Telegram API (`tdlight-telegram-bot-api`)  
2. Telegram Bot Token  
3. Target Telegram Channel ID  
4. Video files supported by Telegram's streaming player (H.264 MP4/MKV recommended for direct playback).

---

## 🧪 Testing (Mock Data)

To test the application logic without an existing metadata library:

1.  **Prepare Data:** Place video files (`.mp4`, `.mkv`) in a local folder.
2.  **Generate Metadata:** Run the provided Python script to generate the JSON sidecars expected by the parser.
    ```bash
    # Generates "Clean Schema" metadata for all videos in the folder
    python tools/generate_mock_data.py ./path/to/my_videos
    ```
3.  **Configure:** Update `config/config.ini` so `RECORDINGS_JSON_PATH` points to your video folder.
4.  **Run:** Launch TeleCloud and click **Sync Metadata**. The videos will appear in the queue, organized by the generated mock channel.

---

## ⚙️ Setup Before Building

1. Copy `config/auth.example.ini` → `config/auth.ini`  
   - Replace `BOT_API_TOKEN` with your bot token  
   - Replace `CHANNEL_ID` with your channel ID  

2. Copy `config/config.example.ini` → `config/config.ini`  
   - Set `RECORDINGS_JSON_PATH` to the absolute path where your `.json` metadata files reside.  
   - Optionally adjust `SQLITE_DB_PATH`  

3. During build, config files will be auto-copied next to the executable.

---

## ▶️ How to Use

1. Launch **local Telegram API** (keep default port)  
2. Run **TeleCloud**  
3. Press **Sync Metadata** — parses JSON sidecars and identifies new assets.
4. Press **Fill Queue** — validates files and populates the upload manager.
5. Press **Start** — begins uploading video assets and linking contact sheets.
6. Press **Stop** — gracefully stops after finishing the current file.

---

## ✅ Database

The app automatically initializes a SQLite database (`telecloud.db`) to track upload state and file IDs. You can query the **uploads** table to retrieve the Telegram links for archived assets.

---

## 📝 License

This project and all submodules are licensed under **GNU General Public License v3.0 (GPL-3.0-or-later)**. See the [LICENSE](LICENSE) file for full terms.

---

## 🛠 Dependencies

This project includes **telegrambotlib-qt** as a submodule.
