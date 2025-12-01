#!/bin/bash

# This script bumps the project version in the CMakeLists.txt file
# and the github workflow files.

set -e

case "$1" in
    "-h"|"--help")
        echo ""
        echo "Bumps the project version."
        echo ""
        echo "Usage:"
        echo "  ./bump_version.sh <major|minor|patch>"
        echo ""
        echo "Examples:"
        echo "  ./bump_version.sh major  # 1.0.0 -> 2.0.0"
        echo "  ./bump_version.sh minor  # 1.0.0 -> 1.1.0"
        echo "  ./bump_version.sh patch  # 1.0.0 -> 1.0.1"
        exit 0
        ;;
    "major"|"minor"|"patch")
        BUMP_TYPE="$1"
        ;;
    *)
        echo "Error: Invalid argument '$1'"
        echo "Use './bump_version.sh -h' for help"
        exit 1
        ;;
esac

# Extract current version from CMakeLists.txt
CURRENT_VERSION=$(grep -m 1 'set(PROJECT_VERSION' CMakeLists.txt | grep -oE '[0-9]+\.[0-9]+\.[0-9]+')

if [ -z "$CURRENT_VERSION" ]; then
    echo "Error: Could not find PROJECT_VERSION in CMakeLists.txt"
    exit 1
fi

# Split version into components
IFS='.' read -r MAJOR MINOR PATCH <<< "$CURRENT_VERSION"

# Increment appropriate component
case "$BUMP_TYPE" in
    "major")
        MAJOR=$((MAJOR + 1))
        ;;
    "minor")
        MINOR=$((MINOR + 1))
        ;;
    "patch")
        PATCH=$((PATCH + 1))
        ;;
esac

NEW_VERSION="$MAJOR.$MINOR.$PATCH"

echo "Bumping version: $CURRENT_VERSION -> $NEW_VERSION"

# Update CMakeLists.txt
sed -i '' "s/set(PROJECT_VERSION $CURRENT_VERSION)/set(PROJECT_VERSION $NEW_VERSION)/" CMakeLists.txt
echo "✓ Updated CMakeLists.txt"

# Update branch-build.yml if it exists
if [ -f ".github/workflows/branch-build.yml" ]; then
    sed -i '' "s/PROJECT_VERSION: $CURRENT_VERSION/PROJECT_VERSION: $NEW_VERSION/" .github/workflows/branch-build.yml
    echo "✓ Updated .github/workflows/branch-build.yml"
fi

# Update release.yml if it exists
if [ -f ".github/workflows/release.yml" ]; then
    sed -i '' "s/PROJECT_VERSION: $CURRENT_VERSION/PROJECT_VERSION: $NEW_VERSION/" .github/workflows/release.yml
    echo "✓ Updated .github/workflows/release.yml"
fi

echo "Version bump complete: $NEW_VERSION"