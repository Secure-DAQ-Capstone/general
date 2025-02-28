#!/bin/bash

# Configuration
TONY_USER="mjgrouchy"
TONY_HOST="10.0.0.1"
TONY_GIT_REGISTRY="$HOME/git-registry/"
GITHUB_PREFIX="git@github.com:"

# Submodules to be mirrored
SUBMODULES=(
    "gpearcey NMEA2000_socketCAN git@github.com:gpearcey/NMEA2000_socketCAN.git"
    "ttlappalainen NMEA2000 git@github.com:ttlappalainen/NMEA2000.git"
)

# Function to create bare registries for submodules
create_bare_repos() {
    echo "Creating bare repositories for submodules..."
    for entry in "${SUBMODULES[@]}"; do
        IFS=' ' read -r username name url <<< "$entry"
        REPO_PATH="$TONY_GIT_REGISTRY$username/$name.git"
        
        if [ -d "$REPO_PATH" ]; then
            if [ -z "$(ls -A "$REPO_PATH")" ]; then
                echo "Repository $name exists but is empty. Recreating..."
                rm -rf "$REPO_PATH"
            else
                echo "Repository $name already exists and is not empty. Skipping..."
                continue
            fi
        fi
        
        echo "Creating bare repository for $name..."
        mkdir -p "$REPO_PATH"
        git clone --mirror "$url" "$REPO_PATH"
    done
    echo "Bare repositories created successfully!"
}

# Execute the function
create_bare_repos
