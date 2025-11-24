import os
import json
import uuid
import time
import argparse
from pathlib import Path

# Configuration for the "Mock" Channel
MOCK_CHANNEL = {
    "type": "GenericStreamSource",
    "name": "Test_Streamer_01",
    "displayName": "Test Streamer (Gaming)",
    "description": "Backup of VODs from local storage",
    "url": "https://twitch.tv/test_streamer",
    "preview": "",
    "priority": 10
}

def generate_metadata(video_path):
    """
    Generates a JSON sidecar file compatible with TeleCloud
    using the modern 'Channel' schema.
    """
    path_obj = Path(video_path).resolve()

    # Basic file stats
    stat = path_obj.stat()
    size_bytes = stat.st_size
    created_time_ms = int(stat.st_mtime * 1000)

    # Generate the JSON structure matching DbManager::readMetadata
    metadata = {
        "id": str(uuid.uuid4()),
        "channel": MOCK_CHANNEL, # The new clean schema key
        "startDate": created_time_ms,
        "status": "FINISHED",
        "progress": 100,
        "sizeInByte": size_bytes,
        "singleFile": True,
        "pinned": False,
        "associatedFiles": [str(path_obj)],
        "absoluteFile": str(path_obj),
        "postProcessedFile": str(path_obj),
        "selectedResolution": 1080,
        "lastSizeUpdate": int(time.time() * 1000)
    }

    # Write to .json file next to the video
    json_path = path_obj.with_suffix('.json')

    with open(json_path, 'w', encoding='utf-8') as f:
        json.dump(metadata, f, indent=2)

    print(f"[+] Generated metadata: {json_path}")

def scan_and_generate(directory):
    target_dir = Path(directory)
    if not target_dir.exists():
        print(f"Error: Directory '{directory}' not found.")
        return

    video_extensions = {'.mp4', '.mkv', '.avi', '.mov', '.flv'}
    count = 0

    print(f"Scanning {target_dir} for video files...")

    for root, _, files in os.walk(target_dir):
        for file in files:
            if Path(file).suffix.lower() in video_extensions:
                full_path = Path(root) / file
                generate_metadata(full_path)
                count += 1

    print(f"Done. Generated {count} metadata files.")

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Generate TeleDrive mock metadata for video files.")
    parser.add_argument("directory", nargs="?", default=".", help="Directory to scan (default: current dir)")
    args = parser.parse_args()

    scan_and_generate(args.directory)
