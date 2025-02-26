#!/bin/bash

# Configuration
TONY_USER="mjgrouchy"
TONY_HOST="10.0.0.1"
TONY_GIT_REGISTRY="~/git-registry/"
GITHUB_PREFIX="git@github.com:"


# Function to set the override
set_override() {
    echo "Configuring Git to use Tony's registry instead of GitHub..."
    git config --global url."$TONY_USER@$TONY_HOST:$TONY_GIT_REGISTRY".insteadOf "$GITHUB_PREFIX"
    echo "Override set! GitHub submodules will now be pulled from Tony when available."
}

# Function to check the current override
check_override() {
    echo "Checking Git override settings..."
    git config --list | grep url || echo "No override set. Using default submodule URLs."
}

# Function to remove the override
remove_override() {
    echo "Removing Git override..."
    git config --global --unset url."$TONY_USER@$TONY_HOST:$TONY_GIT_REGISTRY".insteadOf
    echo "Override removed. GitHub submodules will now use their original URLs."
}

# Function to sync and update submodules
update_submodules() {
    echo "Syncing and updating submodules..."
    git submodule sync
    git submodule update --init --recursive
    echo "Submodules updated!"
}

# Menu
case "$1" in
    set)
        set_override
        ;;
    check)
        check_override
        ;;
    remove)
        remove_override
        ;;
    update)
        update_submodules
        ;;
    *)
        echo "Usage: $0 {set|check|remove|update}"
        echo "  set     - Use Tony's Git registry instead of GitHub"
        echo "  check   - Check current override settings"
        echo "  remove  - Remove the Git override"
        echo "  update  - Sync and update submodules"
        exit 1
        ;;
esac
